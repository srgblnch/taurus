import os, sys, threading, weakref, time, string, copy
import IPython
from utils import TermColors, TermTangoColors
from Attribute import *
import PyTango
from Log import Logger


OkStates = [PyTango.DevState.INIT, PyTango.DevState.RUNNING, PyTango.DevState.STANDBY, PyTango.DevState.ON, None]
ErrorStates = [PyTango.DevState.FAULT, PyTango.DevState.ALARM, PyTango.DevState.UNKNOWN]


class ArrayStream:
    def __init__(self, array=[]):
        self.array = array

    def __getattr__(self, name):
        return getattr(self.array, name)

    def read(self):
        val = self.array[0]
        del self.array[0]
        return val


class AbstractParam:
    
    def __init__(self, name=None, desc=None):
        self.name     = name
        self.desc     = desc
        
    def getParam(self,idx=0):
        return self

    def getParamStr(self):
        return "<"+self.name+">"

    def getParamDescr(self):
        str = self.name + " (" + self.type + ") - " + self.desc
        if self.defvalue != 'None':
            str = str + "\nDefault value: " + self.defvalue 
        return str

    def getParamCount(self):
        return 1

    def formatParamValue(self,value):
        return value

class Param(AbstractParam):
    
    def __init__(self, name=None, type_name=None, desc=None, defvalue=None,
                 from_array=None):
        AbstractParam.__init__(self,name=name,desc=desc)
        self.type     = type_name
        self.defvalue = defvalue

        if from_array is not None:
            for key in ['name', 'type', 'desc', 'defvalue']:
                setattr(self, key, from_array.read())
    
    def formatParamValue(self,value):
        ret = value
        if self.type == "File":
            f = open(ret,'r')
            ret = f.read()
            f.close()
        return ret
    
          
class ParamRepeat(AbstractParam):
    def __init__(self, name=None, desc=None, opts=None, param_def=None,
                 from_array=None):
        AbstractParam.__init__(self,name=name,desc=desc)
        self.opts      = opts
        self.param_def = param_def

        if from_array is not None:
            self.name = from_array.read()
            type_name = from_array.read() 
            if type_name != 'ParamRepeat':
                msg = 'Expecting "ParamRepeat" type, got ' + type_name
                raise ValueError, msg
            self.desc = from_array.read()
            opt_str = from_array.read()
            opt_list = opt_str.split(', ')
            opts = {}
            for opt_str in opt_list:
                name, val = opt_str.split('=')
                if val == repr(None):
                    val = None
                else:
                    try:    val = int(val)
                    except: pass
                opts[name] = val
            self.opts = opts
            self.param_def = ParamList(from_array=from_array)

    def getParam(self,idx=0):
        idx = idx % len(self.param_def.pars)
        return self.param_def.getParam(idx)
        
    def getParamStr(self):
        return "[" + self.param_def.getParamStr() + " ]"

    def getParamDescr(self):
        return self.param_def.getParamDescr()
        
    def getParamCount(self):
        return self.self.param_def.getParamCount()
    
    def formatParamValue(self,value):
        return self.param_def.formatParamValue(value)

 
class ParamList(AbstractParam):
    
    def __init__(self, from_array):
        AbstractParam.__init__(self,name="ParamList",desc="List of parameters")
    
        self.pars = []
        self.has_param_repeat = False
        
        npars = int(from_array.read())
        for i in range(npars):
            if from_array[1] == 'ParamRepeat':
                par = ParamRepeat(from_array=from_array)
                self.has_param_repeat = True
            else:
                par = Param(from_array=from_array)
            self.pars.append(par)
    
    def getParam(self,idx=0):
        par_nb = len(self.pars)
        if idx < par_nb:
            par = idx
            return self.pars[idx].getParam(0)
        else:
            for i in range(par_nb):
                local_idx = idx - i
                local_nb = self.pars[i].getParamCount()
                if local_idx < local_nb:
                    return self.pars[i].getParam(local_idx)        

    def getParamStr(self):
        str = ""
        for par in self.pars:
            str = str + " " + par.getParamStr()
        return str

    def getParamDescr(self):
        str = ""
        for par in self.pars:
            str = str + "\n" + par.getParamDescr()
        return str

    def getParamCount(self):
        nb = 0
        for par in self.pars:
            local_nb = par.getParamCount()
            if local_nb == sys.maxint: 
                return sys.maxint
            nb += local_nb
        return nb
    
    def formatParamValue(self,value):
        
        # for now we are not able to handle ParamRepeat with special value format
        if self.has_param_repeat:
            return value
        
        for i,v in enumerate(value):
            value[i] = self.pars[i].formatParamValue(v)
        return value
    
    
class MacroInfo:
    def __init__(self, name, pars=None, results=None, desc=None, from_array=None):
        self.name = name
        self.desc = desc
        self.pars = pars
        self.results = results
        
        if from_array is not None:
            from_array = ArrayStream(from_array)
            self.desc = from_array.read()
            self.hints = eval(from_array.read())
            self.pars = ParamList(from_array=from_array)
            self.results = ParamList(from_array=from_array)
    
    def hasParams(self):
        return self.pars and len(self.pars.pars) > 0
    
    def getParam(self,idx=0):
        return self.pars.getParam(idx)

    def getParamStr(self):
        return self.pars.getParamStr()
    
    def getParamDescr(self):
        return self.pars.getParamDescr()

    def hasResult(self):
        return self.results and len(self.results.pars) > 0
    
    def getResult(self,idx=0):
        return self.results.getParam(idx)

    def getResultStr(self):
        return self.results.getParamStr()
    
    def getResultDescr(self):
        return self.results.getParamDescr()

    def formatParamValue(self,value):
        return self.pars.formatParamValue(value)
        
    def formatResult(self,res):
        if not self.hasResult():
            raise Exception("Macro %s does not return any result" % self.name)
        rtype = self.getResult()
        if rtype.type == "File":
            try:
                os.makedirs("./tmp")
            except:
                pass
            fname = "./tmp" + res[0][res[0].rfind("/"):]
            f = open(fname, "w")
            f.write(res[1])
            f.close()
            res.insert(0,fname)
            return res
        if len(res) > 0:
            if rtype.type == "Float":
                return float(res[0])
            elif rtype.type == "Integer":
                return int(res[0])
            elif rtype.type == "String" or rtype.type == "User" or rtype.type == "Filename":
                return res[0]
            else:
                raise Exception("Unknown return type for macro %s" % self.name)
        else:
            return None


class Macro:

    Ready    = PyTango.DevState.ON
    Init     = PyTango.DevState.INIT
    Running  = PyTango.DevState.RUNNING
    Pause    = PyTango.DevState.STANDBY
    Fault    = PyTango.DevState.FAULT
    Finished = PyTango.DevState.ON
    Abort    = PyTango.DevState.ALARM

    def __init__(self, door, name, info, pars=""):
        self.door = door
        self.name = name
        self.pars = pars.split()
        self.info = info
        self.result_lock = threading.Lock()
        self.cond = threading.Condition()
        self.res = None
        self.macro = None
        
        self.state = door.state
        self.state.subscribeEvent(self.stateChanged)
        
    def __del__(self):
        pass

    def stateChanged(self, data, state):
        pass
    
    def waitState(self, state, equal=True):
        self.state.waitEvent(state, equal=equal)
        return self.state.read()
       
    def start(self):
        self.state.lock()
        self.pars = self.info.formatParamValue(self.pars)
        self.door.RunMacro([self.name,] + self.pars)
        self.waitState(Macro.Ready, equal=False)
        self.state.unlock()
    
    def waitFinished(self):
        self.waitState(Macro.Finished)

    def abort(self):
        self.state.lock()
        try:
            self.door.Abort()
            self.waitState(Macro.Ready)
        finally:
            self.state.unlock()

    def setResult(self,res):
        self.result_lock.acquire()
        try:
            self.res = self.info.formatResult(res)
        finally:
            self.result_lock.release()

    def getResult(self):
        res = None
        self.result_lock.acquire()
        try:
            res = self.res
        finally:
            self.result_lock.release()
        return res


class CommitData:
    def __init__(self):
        pass
    
    def commit(self,descr):
        pass
    
    
class CommitFile(CommitData):
    def __init__(self,cmd,local_filename,remote_filename):
        self.magic_cmd = cmd
        self.local_filename = local_filename
        self.remote_filename = remote_filename
    
    def commit(self,descr):
        fname = self.remote_filename[self.remote_filename.rfind('/')+1:]
        print "Commiting " + fname + "...",
        try:
            cmd = "%s %s %s %s %s" % (self.magic_cmd, self.remote_filename, 
                                      os.getlogin(), descr, self.local_filename)
            ip = IPython.ipapi.get()
            ip.magic(cmd)
        except:
            print '[ ' + TermColors.Red + 'FAIL' + TermColors.Normal + ' ]'
        print '[ ' + TermColors.Green + 'DONE' + TermColors.Normal + ' ]'
        return True
    

class Door(TangoDevice):

    console_editors = ('vi','vim','nano','joe','pico','emacs')

    def __init__(self, dev_name, macro_serv):
        TangoDevice.__init__(self, dev_name)
        self.macro_serv = weakref.ref(macro_serv)
        self.log_attr = {}
        
        self.log_start = {'Error'   : TermColors.Red,
                          'Info'    : TermColors.LightBlue, 
                          'Warning' : TermColors.Brown, 
                          'Output'  : TermColors.Normal, 
                          'Debug'   : TermColors.DarkGray, 
                          'Result'  : TermColors.LightGreen }
        self.log_stop = { 'Error'   : TermColors.Normal, 
                          'Info'    : TermColors.Normal, 
                          'Warning' : TermColors.Normal, 
                          'Output'  : TermColors.Normal, 
                          'Debug'   : TermColors.Normal, 
                          'Result'  : TermColors.Normal}
        
        self.last_state = None
        
        self.macro = None
        self.last_macro_name = ''
        
        self.ignore_logs = True 
        
        for log_name in ['Error', 'Warning', 'Info', 'Output', 'Debug', 'Result']:
            attr = Attribute(self, log_name)
            if log_name == 'Result':
                attr.subscribeEvent(self.resultAvailable, log_name)
            else:
                attr.subscribeEvent(self.logAvailable, log_name)
            self.log_attr[log_name] = attr
        
        self.ignore_logs = False
            
        self.block_lines = 0
        self.debug = False
        self.pending_commits = {}
        
    def runMacro(self, name, parameter_s=''):
        macro_serv = self.macro_serv()
        info = macro_serv.getMacroInfo(name)
        self.macro = Macro(self, name, info, parameter_s)
        self.last_macro_name = name
        macro = self.macro
        self.first_line = True
        macro.start()
        try:
            macro.waitFinished()
        except KeyboardInterrupt:
            print '\nCtrl-C received: Aborting ...'
            self.block_lines = 0
            macro.abort()

        ret = self.__getMacroResult(macro)
        self.macro = None
        return ret
    
    def __getMacroResult(self, macro):
        ret = None
        if macro.info.hasResult():
            ret = macro.getResult()
            if macro.info.getResult().type == "File":
                
                commit_cmd = macro.info.hints['commit_cmd']
                
                if commit_cmd == None:
                    return ret
                
                local_f_name = ret[0]
                remote_f_name = ret[1]
                line_nb = ret[3]
                commit = CommitFile(commit_cmd, local_f_name, remote_f_name)
                self.pending_commits.update( { remote_f_name : commit } )
                ip = IPython.ipapi.get()
                editor = ip.options.editor or 'vi'
                
                cmd = 'edit -x -n %s %s' % (line_nb, local_f_name)
                if not editor in self.console_editors:
                    cmd = 'bg _ip.magic("' + cmd + '")'
                ip.magic(cmd)
                # The return value of the macro was saved in a file and opened
                # with edit so we don't return anything to avoid big outputs
                # to the console  
                ret = None
        return ret
        
    def resultAvailable(self, log_name, result):
        """Method invoked by the arrival of a change event on the Result attribute"""
        
        if self.ignore_logs or self.macro is None: 
            return
        
        self.macro.setResult(result)
        
    def logAvailable(self, log_name, output):

        if self.ignore_logs or (log_name == 'Debug' and not self.debug): 
            return
        
        if output is None:
            output = []
        
        sys.stdout.write(self.log_start[log_name])
        sys.stdout.flush()
        for line in output:
            if not self.debug:
                if line == Logger.BlockStart:
                    self.block_lines = 0
                    continue
                if line == Logger.BlockReturn:
                    for i in xrange(self.block_lines):
                        sys.stdout.write('\x1b[2K' + ('\x1b[1A' + '\x1b[2K'))
                        sys.stdout.flush()
                        
                    self.block_lines = 0
                    continue
            print line.decode('unicode-escape')
            self.block_lines += 1
        sys.stdout.write(self.log_stop[log_name])
        sys.stdout.flush()
        
    def setDebug(self, state):
        self.debug = state
    
    def commit(self,descr):
        good_commits = []
        for k in self.pending_commits:
            v = self.pending_commits[k]
            ret = v.commit(descr)
            if ret is True:
                os.remove(v.local_filename)
                good_commits.append(k)
        for e in good_commits:
            self.pending_commits.pop(e)
    
    def stateChanged(self, data, state):
        update_prompt = False
        if not self.last_state is None:
            if self.last_state in OkStates and state in ErrorStates:
                update_prompt = True
            if state in OkStates and self.last_state in ErrorStates:
                update_prompt = True
        self.last_state = state
        
        if update_prompt:
            ms = self.macro_serv()
            ms_state = ms.state.read()
            ms.updatePrompt(ms_state, state)
        
        TangoDevice.stateChanged(self,data,state)

class _MacroServer(TangoDevice):

    def __init__(self, macro_serv_name, door_name=None):
        
        self.last_state = None
        self.ipython_ready = False
        
        TangoDevice.__init__(self, macro_serv_name)
        
        self.in_prompt1 = TermTangoColors
        self.in_prompt2 = TermTangoColors
                
        self.type_dict = {}
        self.macro_dict = {}
        self.door_list = ListAttribute(self, 'DoorList')
        door_list = self.door_list.read()

        if len(door_list) == 0:
            raise RuntimeError("No doors available in the MacroServer. Please create a door before continuing.")
            
        if door_name is None:
            door_name = door_list[0]
        elif door_name not in door_list:
            raise RuntimeError('Door %s not defined in %s' % (door_name, macro_serv_name))
        
        self.door = Door(door_name, self)

        self.type_list = ListAttribute(self, 'TypeList')
        self.type_list.subscribeEvent(self.typeListChanged)

        self.macro_list = ListAttribute(self, 'MacroList')
        self.macro_list.subscribeEvent(self.macroListChanged)

    def typeListChanged(self, data, event_val):
        all_types, old_types, new_types = event_val
        
        for type in old_types:
            self.delType(type)
        
        for type in new_types:
            self.addType(type)
            
    def macroListChanged(self, data, event_val):
        all_macros, old_macros, new_macros = event_val

        for macro in old_macros:
            self.delMacro(macro)
        
        for macro in all_macros:
            self.addMacro(macro,cache=False)

        if new_macros:
            print "\n%d new macro(s) available\n" % len(new_macros)
        
    def getDoor(self):
        return self.door
    
    def getMacroInfo(self, name, cache=True):
        
        info = None
        if cache == True:
            info = self.macro_dict.get(name)
        
        if not info:
            info_lst = self.GetMacroInfo(name)
            info = MacroInfo(name, from_array=info_lst)
            self.macro_dict[name] = info
            
        return info
    
    def getTypeElements(self, type_name):
        return self.type_dict.get(type_name)
    
    def addMacro(self, name, cache=True):
        def macro_fn(self, parameter_s='', name=name):
            macro_serv = MacroServer()
            door = macro_serv.getDoor()
            res = door.runMacro(name, parameter_s)
            
            if not res is None:
                return res
        
        # build documentation for macro
        macro_info = self.getMacroInfo(name,cache)
        
        doc = "Syntax:"
        doc += "\n\t" + name + " " + macro_info.getParamStr()
        if macro_info.hasResult():
            doc += " -> " + macro_info.getResultStr()
        doc += "\n\n"
        doc += macro_info.desc
        if macro_info.hasParams():
            doc += "\n\nParameters:"
            doc += macro_info.getParamDescr().replace('\n','\n\t')
        if macro_info.hasResult():
            doc += "\n\nResult:"
            doc += macro_info.getResultDescr().replace('\n','\n\t')
        
        macro_fn.__doc__ = doc.decode('unicode-escape')
        
        # register magic command
        ip = IPython.ipapi.get()
        ip.expose_magic(name, macro_fn)
        
        # enable macro param completion
        ip.set_hook('complete_command', macroCompleter, str_key = name)

        # register also when the command as is typed with the magic prefix '%'
        name = str('%') + name
        ip.set_hook('complete_command', macroCompleter, str_key = name)

    def delMacro(self, name):
        delattr(IPython.Shell.InteractiveShell, 'magic_' + name)
        
        if self.macro_dict.has_key(name):
            del macro_dict[name]
        
    def getMacroList(self):
        return self.macro_list.read()

    def delType(self, name):
        if self.type_dict.has_key(name):
            del self.type_dict[name]
        
        attr_list_name = name + "List"
        try:
            delattr(self,attr_list_name)
        except:
            pass
        
    def addType(self, name):
        
        # if we don't have the type already add it
        if self.type_dict.has_key(name):
            return
        
        self.type_dict[name] = []
        
        # if it is a complex type there should be an attribute with
        # the list of available elements for this type
        if not name.endswith('*'):
            return
        
        name = name[:len(name)-1]
        attr_list_name = name + "List"
        
        dev = self.checkDev()
        attrs = dev.attribute_list_query()
        new_attr = False
        for attr in attrs:
            if cmp(attr.name.upper(), attr_list_name.upper()) == 0:
                new_attr = True
                break
        
        if new_attr:
            attr_lst = ListAttribute(self, attr_list_name)
            setattr(self, attr_list_name, attr_lst)
            obj_attr = getattr(self, attr_list_name)
            try:
                obj_attr.subscribeEvent(self.typeListElementChanged, name)
            except Exception, e:
                print "Exception trying to register events for",attr.name
                print e
                        
    def typeListElementChanged(self, data, event_val):
        all_typeListElements = event_val[0]
        type_name = data
        self.type_dict[type_name] = all_typeListElements

    def spock_input_prompt(self, ip, cont):
        """ spock_input_prompt(IPApi ip, bool cont) -> string
            
            Gets the current IPython prompt. Intended to be called by the 
            IPython hook 'generate_prompt'.

            Params:
                ip: the IPython API object
                cont: prompt continuation or not
            Return:
                A string representing the prompt.
        """
        
        ms_state = self.state.read()
        door_state = self.door.state.read()
        return self.spock_input_prompt_for_state(ip, cont, ms_state, door_state)
        
    def spock_input_prompt_for_state(self, ip, cont, ms_state, door_state):
        """ spock_input_prompt_for_state(IPApi ip, bool cont, PyTango.DevState ms_state, PyTango.DevState door_state) -> string

            Gets the current IPython prompt acording to the given parameters.
            
            Params:
                ip: the IPython API object
                cont: prompt continuation or not
                ms_state: macro server state
                door_state: door state
            Return:
                A string representing the prompt.
        """
        
        count = str(len(ip.user_ns["_ih"]))
        profile = ip.user_ns['PROFILE'].upper()
        door = self.getDoor().alias
        ms = self.alias
        
        pwd = os.getcwd()
        if cont:
            return "%s%s: %s" % ( \
                TermColors.Green,
                '.'*(6 + len(count) + len(profile) + len(door) + len(ms)),
                TermColors.Normal)
        else:
            return "%s%s%s.%s%s: %s" % ( \
                ip.options.separate_in,
                TermColors.LightRed,
                count,
                TermColors.Green,
                profile,
                TermColors.Normal)
                
    def spock_output_prompt(self, ip):
        count = str(len(ip.user_ns["_ih"]))
        door = self.getDoor().alias
        macro = self.getDoor().last_macro_name
        if not macro is None and len(macro) > 0:
            macro = "." + macro

        return "%sOut %s%s[%s%s%s]: %s" % ( \
            TermColors.Purple,
            door,
            macro,
            TermColors.LightRed,
            count,
            TermColors.Purple,
            TermColors.Normal)
        
    def stateChanged(self, data, state):
        update_prompt = False
        if not self.last_state is None:
            if self.last_state in OkStates and state in ErrorStates:
                update_prompt = True
            if state in OkStates and self.last_state in ErrorStates:
                update_prompt = True
        self.last_state = state
    
        if update_prompt:
            self.updatePrompt(state,self.door.state.read())
            
        TangoDevice.stateChanged(self,data,state)
    
    def updatePrompt(self,ms_state,door_state):
        # If before IPython initialization don't try to update the prompt
        if not self.ipython_ready:
            return

        ip = IPython.ipapi.get()
        prompt = self.spock_input_prompt_for_state(ip,False,ms_state,door_state)
        sys.stdout.write('\x1b[s\x1b[F' + prompt + '\x1b[u')
        sys.stdout.flush()
        
TheMacroServer = None

def InitMacroServer(dev_name, door_name=None):
    global TheMacroServer
    TheMacroServer = _MacroServer(dev_name, door_name)

    ip = IPython.ipapi.get()
    for cmd in commands:
        fn = globals()[cmd]
        ip.expose_magic(cmd, fn)

def MacroServer():
    global TheMacroServer
    if TheMacroServer is None:
        raise 'MacroServer not set: call \"MacroServer\" first'
    return TheMacroServer

def macroCompleter(self, event):
    """Method called by the IPython autocompleter. It will determine possible
       values for macro arguments.
    """
    
    ms = MacroServer()
    macro_name = event.command.lstrip('%')
    
    # calculate parameter index
    param_idx = len(event.line.split()) - 1
    if not event.line.endswith(' '): param_idx -= 1
    
    # get macro info
    info = ms.getMacroInfo(macro_name)
    
    # if macro doesn't have parameters return
    if param_idx < 0 or not info.hasParams() : return
    
    # get the parameter info
    param = info.getParam(param_idx)
    
    # return the existing elements for the given parameter type
    return ms.getTypeElements(param.type) 

def status(self, parameter_s=''): 
    ms = MacroServer()
    door = ms.getDoor()
    if door.working_state == WorkingState.ONLINE:
        door_state = door.state.read()
        print "Connected to Door %s (in %s%s%s state)" % (door.alias,TermTangoColors[door_state],door_state,TermColors.Normal)
        if door_state != PyTango.DevState.ON:
            status = door.get_status().splitlines()
            for line in status:
                print "\t"+line
    elif door.working_state == WorkingState.OFFLINE:
        print "NOT connected to Door %s. Door is offline" % door.alias
    elif door.working_state == WorkingState.SHUTDOWN:
        print "Disconnected from Door %s. Door as been shutdown" % door.alias
    elif door.working_state == WorkingState.CRASHED:
        print "Connection to Door %s was lost (Server crashed or network failed)" % door.alias
    
    if ms.working_state == WorkingState.ONLINE:
        ms_state = ms.state.read()
        print "Connected to Macro Server %s (in %s%s%s state)" % (door.alias,TermTangoColors[ms_state],ms_state,TermColors.Normal)
        status = ms.status().splitlines()
        for line in status:
            print "\t"+line
    elif ms.working_state == WorkingState.OFFLINE:
        print "NOT connected to Macro Server %s. Door is offline" % ms.alias
    elif ms.working_state == WorkingState.SHUTDOWN:
        print "Disconnected from Macro Server %s. Macro Server as been shutdown" % ms.alias
    elif ms.working_state == WorkingState.CRASHED:
        print "Connection to Macro Server %s was lost (Server crashed or network failed)" % ms.alias
        
def lsdef(self, parameter_s=''):
    """List available macros"""

    macro_serv = MacroServer()
    for macro in macro_serv.getMacroList():
        print macro

def commit(self, parameter_s=''):
    """Commits the changes made to the files locally into the server(s)"""
    
    ms = MacroServer()
    cmts = ms.getDoor().pending_commits
    if len(cmts) == 0:
        print "There are no pending commits"
        return

    params = parameter_s.split()
    if len(params) == 0:
        print "Usage: commit <comment>"
        return
    ms.getDoor().commit(parameter_s)

def pending_commits(self, parameter_s=''):
    """Commits the changes made to the files locally into the server(s)"""
    
    cmts = MacroServer().getDoor().pending_commits
    if len(cmts) == 0:
        print "There are no pending commits"
        return
    
    print "Pending commits are:"
    for c in cmts.keys():
        print c 
    
def bench(self, parameter_s=''):
    """Measure the execution time of a macro"""
    
    params = parameter_s.split()
    if len(params) == 0:
        print "Usage: bench macro [par ...]"
        return
    
    name = params[0]
    parameter_s = string.join(params[1:])
    
    t0 = time.time()
    door = MacroServer().getDoor()
    door.runMacro(name, parameter_s)
    t = time.time()
    print "Execution time: %.3f sec" % (t - t0)

def debug(self, parameter_s=''):
    """Activate/Deactivate macro server debug output"""

    params = parameter_s.split()
    try:
        par = string.lower(params[0])
        valid_states = ['off', 'on']
        state = valid_states.index(par)
        door = MacroServer().getDoor()
        door.setDebug(state)
    except:
        print "Usage: debug on|off"

    
commands = ['lsdef', 'commit','pending_commits', 'bench', 'debug', 'status']

def macroserver(self, parameter_s=''):
    """Specifies the macro server"""

    macro_server = parameter_s
    if not macro_server:
        print "Usage: macroserver <macro_server_dev_name>"
        return
    InitMacroServer(macro_server)

def spock_input_prompt_hook(self, cont):
    return MacroServer().spock_input_prompt(self.api, cont)

def spock_output_prompt_hook(self):
    return MacroServer().spock_output_prompt(self.api)

def spock_late_startup_hook(self):
    try:
        MacroServer().ipython_ready = True
    except:
        pass

ip = IPython.ipapi.get()
ip.expose_magic('macroserver', macroserver)
ip.set_hook('generate_prompt', spock_input_prompt_hook)
ip.set_hook('generate_output_prompt', spock_output_prompt_hook)
ip.set_hook('late_startup_hook', spock_late_startup_hook)