import imp, weakref, ctypes, traceback, os, thread

from Sardana import *
from Params import *

NOT_IMPL = """Macro not implemented. 
Inform your computing contact person in case you need this macro implemented."""  

class MacroFactory:

    def __init__(self, manager):
        self.manager = weakref.ref(manager)

    def __getattr__(self, name):
        return self.manager().getMacroClass(name)


class AbortException(Exception):
    pass


class Macro(Logger):
    """ The Macro base class"""
    
    # States
    Init     = PyTango.DevState.INIT
    Running  = PyTango.DevState.RUNNING
    Pause    = PyTango.DevState.STANDBY
    Fault    = PyTango.DevState.FAULT
    Finished = PyTango.DevState.ON
    Abort    = PyTango.DevState.ALARM

    All      = PoolObjParamType.All
    
    param_def = []
    result_def = []
    hints = {}
    
    def __init__(self, manager, parent_macro, log_parent, environment, *args, **opts):
        self.name = self.__class__.name
        self.manager = manager
        self.in_pars = args
        self.out_pars = []
        self.parent_macro = parent_macro
        self.child_macro = None
        self.environment = environment
        
        log_parent = log_parent or parent_macro or manager
        Logger.__init__(self, 'Macro-' + self.name, log_parent)

        arg_str = ''
        in_pars_str = self.manager.strMacroParamValues(self.in_pars)
        for arg in in_pars_str:
            arg_str += '%s%s' % (arg_str and ', ', arg)
        self.desc = 'Macro "%s(%s)"' % (self.name, arg_str)

        event_name = '%s state' % self.desc
        self.state = EventGenerator(event_name)
        self.state.fireEvent(Macro.Init)

        event_name = '%s result' % self.desc
        self.result = EventGenerator(event_name)

        self.macro = MacroFactory(manager)
        self.thread = None

        self.objs = []
        
        self.obj_loghandlers = {}
        for obj in args:
            # isiterable
            if type(obj) not in [type(list()), type(tuple())]:
                obj = [obj]
            for sub_obj in obj:
                self.addObj(sub_obj)
        
        if 'no_exec' in opts and opts['no_exec']:
            return
        self.prepare(*args, **opts)
        self.start()
        if not ('no_wait' in opts and opts['no_wait']):
            self.waitFinished()
        

    def __del__(self):
        self.debug("deleting")

    def getManager(self):
        return self.manager
    
    def prepare(self, *args, **opts):
        pass

    def prepareOutput(self,out):
        if isinstance(out,list):
            for i in xrange(len(out)):
                out[i] = str(out[i])
        else:
            out = [str(out)]
        return out

    def on_abort(self):
        pass
    
    def threadFunct(self):
        self.thread_id = threading._get_ident()

        self.state.fireEvent(Macro.Running)
        
        try:
            self.debug("Preparing to 'run'")
            out_pars = self.run(*self.in_pars)
            out_pars = self.prepareOutput(out_pars)
            self.debug("Finished 'run'")
            self.state.lock()
            self.out_pars = out_pars
            self.state.unlock()
        except Exception, err:
            if not isinstance(err, AbortException):
                exc_info = traceback.format_exc()
                self.error("Error: %s", exc_info)
            
            self.state.fireEvent(Macro.Abort)

            for obj in self.objs:
                try:
                    obj.abort(wait_ready=False)
                except AttributeError:
                    pass

            for obj in self.objs:
                try:
                    obj.waitReady()
                except AttributeError:
                    pass
                
            self.on_abort()

            if self.parent_macro is not None:
                raise
            
        self.syncLog()
        
        self.state.fireEvent(Macro.Finished)
        
        for obj in self.objs:
            self.returnObj(obj)

        print "Finishing thread",threading.currentThread().getName()
        del self.thread
        
    def start(self, timeout=None, stack=None):
        self.state.lock()
        try:
            if self.thread is not None:
                raise RuntimeError, 'Macro %s already started' % self.name

            self.debug("Starting %s", self.desc)
            self.thread = threading.Thread(target=self.threadFunct)
            
            self.thread.start()
            self.state.waitEvent(Macro.Init, equal=False,
                                 timeout=timeout, stack=stack)
            out_pars = self.out_pars
        finally:
            self.state.unlock()
                
        return out_pars
        
    def getStateObj(self):
        return self.state
    
    def waitFinished(self, timeout=None):
        self.state.waitEvent(val=Macro.Finished, timeout=timeout)

    def abort(self, timeout=None, stack=None):
        self.state.lock()
        try:
            if self.state.read() == Macro.Running:
                self.debug("Aborting %s", self.desc)
                ret = 0
                while ret != 1:
                    time.sleep(0.1)
                    exc = AbortException
                    fn = ctypes.pythonapi.PyThreadState_SetAsyncExc
                    ret = fn(self.thread_id, ctypes.py_object(exc))
                self.state.waitEvent(Macro.Abort, timeout=timeout, stack=stack)
        finally:
            self.state.unlock()

    def addObj(self, obj):
        if obj is None or obj in self.objs:
            return
        
        self.objs.append(obj)

    def returnObj(self, obj):
        self.objs.remove(obj)
        
    def getObj(self, name, type_class=All, pool=All):
        obj = self.getManager().getObj(name, type_class, pool)
        self.addObj(obj)
        return obj
    
    def findObjs(self, re_pattern, type_class=All, pool=All):
        objs = self.getManager().findObjs(re_pattern, type_class, pool)
        for obj in objs:
            self.addObj(obj)
        return objs

    def getAllEnv(self,cache=True):
        """returns a dictionary of property names and values"""
        return self.environment.getAllEnv(cache)
        
    def getEnv(self,prop_name,cache=True):
        """returns a string containing the value of the given property"""
        return self.environment.getEnv(prop_name,cache)

    def setEnv(self,prop_name,prop_value):
        self.environment.setEnv(prop_name, prop_value)
        
    def getAllEnvDescription(self,cache=True):
        return self.environment.getAllEnvDescription(cache)
        
    def execMacro(self, name, *pars, **opts):
        self.debug("Executing macro: %s%s%s", name, pars, opts)
        manager = self.getManager()
        macro_class = manager.getMacroClass(name)
        macro, ret = manager.prepareMacro(macro_class, pars, opts, self)
        macro.start()
        macro.waitFinished()

    def getDateString(self,time_format='%a %b %d %H:%M:%S %Y'):
        return time.strftime(time_format)
    
    def outputDate(self,time_format='%a %b %d %H:%M:%S %Y'):
        self.output(self.getDateString(time_format=time_format))

    @classmethod
    def hasResult(cls):
        return len(cls.result_def) > 0
    
    def getResult(self):
        return self.out_pars
    
    def getMacroLib(self,module_name):
        return self.manager.getMacroLib(module_name)
    
    def getMacro(self,name):
        return self.manager.getMacro(name)
    
    def getMotion(self, elems, motion_source=None, read_only=False, cache=True):
        return self.manager.getMotion(elems, motion_source=motion_source,
                                      read_only=read_only, cache=cache)
    
class Table:

    DefTermWidth = 80

    PrettyOpts   = {'col_sep': ' |', 'col_head_sep': '-', 'border': '='}
    
    def __init__(self, elem_list, elem_fmt=['%*s'], term_width=None,
                 row_head_str=None, row_head_fmt='%-*s', row_head_width=None,
                 col_head_str=None, col_head_fmt='%*s',  col_head_width=None,
                 col_sep=' ', row_sep=' ', col_head_sep=None, border=None):

        self.nr_col = len(elem_list)
        self.nr_row = len(elem_list[0])
        self.elem_list = elem_list
        self.elem_fmt  = elem_fmt
        if len(elem_fmt) == 1:
            elem_fmt *= self.nr_row
        
        self.term_width = term_width or Table.DefTermWidth
        self.col_sep      = col_sep
        self.row_sep      = row_sep
        self.col_head_sep = col_head_sep
        self.border       = border
        
        max_len_fn = lambda x: reduce(max, map(len, x))
        
        self.row_head_str = row_head_str
        self.row_head_fmt = row_head_fmt
        if row_head_str is not None and len(row_head_str) != self.nr_row:
            msg = 'RowHeadStr nr (%d) and RowNr (%d) mistmatch' % \
                  len(row_head_str), self.nr_row
            raise ValueError, msg
        if row_head_width is None:
            if row_head_str is not None:
                row_head_width = max_len_fn(row_head_str)
            else:
                row_head_width = 0
        self.row_head_width = row_head_width
            
        self.col_head_str = col_head_str
        self.col_head_fmt = col_head_fmt
        if col_head_str is not None and len(col_head_str) != self.nr_col:
            msg = 'ColHeadStr nr (%d) and ColNr (%d) mistmatch' % \
                  len(col_head_str), self.nr_col
            raise ValueError, msg
        if col_head_width is None:
            if col_head_str is not None:
                col_head_width = reduce(max, map(max_len_fn, col_head_str))
            else:
                col_head_width = 10
        self.col_head_width = col_head_width
        if col_head_str is not None:
            self.col_head_lines = len(col_head_str[0])
        else:
            self.col_head_lines = 0
        
    def updateElem(self, elem_list):
        new_col, new_row = len(elem_list), len(elem_list[0])
        if new_col != self.nr_col or new_row != self.nr_row:
            raise 'Invalid new elem list size %dx%d, was %dx%d' % \
                  (new_col, new_row, self.nr_col, self.nr_row)
        self.elem_list = elem_list
        
    def genOutput(self, term_width=None):
        if term_width is None:
            term_width = self.term_width

        
        rhw, chw = self.row_head_width, self.col_head_width
        chl = self.col_head_lines
        lcs = len(self.col_sep)
        width = term_width - chw   # At least one disp column!
        if rhw > 0:
            width -= rhw + lcs
        disp_cols = width / (chw + lcs) + 1
        tot_width = chw + (disp_cols - 1) * (chw + lcs)
        tot_rows = chl + self.nr_row
        if rhw > 0:
            tot_width += rhw + lcs
            
        output = []

        if self.row_head_str is not None:
            row_head = []
            fmt = self.row_head_fmt
            for head in [''] * chl + self.row_head_str:
                head = fmt % (rhw, head)
                row_head.append(head + self.col_sep)
        else:
            row_head = [''] * tot_rows

        for i in xrange(0, self.nr_col, disp_cols):
            if i > 0:
                nr_sep = tot_width / len(self.row_sep)
                output.append(self.row_sep * nr_sep)

            row_end = min(i + disp_cols, self.nr_col)
            line = list(row_head)
            for j in xrange(i, row_end):
                elem = self.elem_list[j]
                if chl:
                    col_head = self.col_head_str[j]
                    if j > i:
                        for k in xrange(tot_rows):
                            line[k] += self.col_sep
                    fmt = self.col_head_fmt
                    for k in xrange(chl):
                        line[k] += fmt % (chw, col_head[k])
                        
                for k in xrange(self.nr_row):
                    fmt = self.elem_fmt[k]
                    line[chl+k] += fmt % (chw, elem[k])

            max_width = reduce(max, map(len, line))
            if self.border is not None:
                nr_border = max_width / len(self.border)
                output.append(self.border * nr_border)
            for l in line[:chl]:
                output.append(l)
            if self.col_head_sep is not None:
                nr_sep = max_width / len(self.col_head_sep)
                output.append(self.col_head_sep * nr_sep)
            for l in line[chl:]:
                output.append(l)
            if self.border is not None:
                output.append(self.border * nr_border)
                    
    
        return output

class List:
    
    def __init__(self, header, border='-'):
        self.col_nb = len(header)
        self.rows = [header,self.col_nb*[border]]
        self.col_lens = self.col_nb*[0,]
        self.border = '-'
        self.line = ''
        
    def recalc_col_sizes(self):
        for i,row in enumerate(self.rows):
            for j,cell in enumerate(row):
                cell = str(cell)
                row[j] = cell
                n = len(cell)
                if n >= self.col_lens[j]:
                    new_len = n + 1
                    self.col_lens[j] = new_len
                    self.rows[1][j] = n*self.border
        self.line = ''
        for n in self.col_lens:
            self.line += '%c%ds' % ('%',n)
    
    def appendRow(self,row):    
        row = row[:self.col_nb]
        self.rows.append(row)
    
    def putRow(self,row,idx):
        row = row[:self.col_nb]
        self.rows[idx] = row

    def genOutput(self):
        self.recalc_col_sizes()
        ret = []
        for row in self.rows:
            ret.append(self.line % tuple(row))
        return ret