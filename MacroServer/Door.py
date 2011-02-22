import PyTango
import PyTangoExt

from Macro import *
from DataRecorder import *

class Door(PyTango.Device_3Impl, Logger):

    def __init__(self, klass, name):
        PyTango.Device_3Impl.__init__(self, klass, name)
        try:
            self.alias = PyTango.Database().get_alias(name)
            if self.alias.lower() == 'nada':
                self.alias = name
        except:
            self.alias = name
        Logger.__init__(self, self.alias)        
        self.init_device()
        
    def __del__(self):
        self.debug("deleting device")
        Logger.__del__(self)
        
    def init_device(self):
        self.cmd = []
        self.macro = None
        self.mutex = threading.RLock()
        self.env_listeners = []
        
        door_class = self.get_device_class()
        self.get_device_properties(door_class)
        
        pyu = PyTango.PyUtil.instance()
        macserver_list = pyu.get_device_list_by_class('MacroServer')
        macserver_name = self.MacroServerName
        for mac_serv in macserver_list:
            if mac_serv.get_name() == macserver_name:
                self.macro_server = mac_serv
                break
        else:
            raise RuntimeError, ('MacroServer device %s not found' % \
                                 macserver_name)
        self.macro_server.registerDoor(self)
        Logger.__init__(self, self.get_name(), self.macro_server)
        
        self.manager = self.macro_server.manager
        self.set_state(PyTango.DevState.ON)
        self.set_change_event('State', True, False)

        # init events on the 'Result' attribute
        self.result = ['invalid', 'result']
        self.set_change_event('Result', True, False)

        self.loadDataConfig()

        self.handler_dict = {}
        for level in ['Error', 'Warning', 'Info', 'Output', 'Debug']:
            level_val = getattr(Logger, level)
            handler_class = AttributeLogHandler
            if level == 'Output':
                handler_class = AttributeBufferedLogHandler
            handler = handler_class(self, level)
            filter = LogFilter(level=level_val)
            handler.addFilter(filter)
            self.addLogHandler(handler)
            format = None
            if level == 'Debug':
                format = logging.Formatter('%(name)s: %(message)s')
                handler.setFormatter(format)
            self.handler_dict[level] = handler, filter, format
            
    def delete_device(self):
        self.debug("Start deleting device")
        m = self.getMacro()
        if m is not None:
            m.abort()
        for handler, filter, format in self.handler_dict.values():
            handler.finish()
        self.set_state(PyTango.DevState.UNKNOWN)
        self.push_change_event('State')
        self.macro_server.unregisterDoor(self)

    def lock(self):
        self.mutex.acquire()

    def unlock(self):
        self.mutex.release()
    
    def getMacro(self):
        self.lock()
        m = self.macro
        self.unlock()
        return m

    def setMacro(self, m):
        self.lock()
        try:
            if self.macro is not None:
                state = self.macro.getStateObj()
                state.unsubscribeEvent(self.macroStateChanged)
            if m is not None:
                state = m.getStateObj()
                state.subscribeEvent(self.macroStateChanged)
            self.macro = m
        finally:
            self.unlock()
        
    def RunMacro(self, par_str_list):
        ret = []
        try:
            manager = self.manager
            macro_class, args = manager.decodeMacroParams(par_str_list)
            
            macro, ret = manager.prepareMacro(macro_class, args,
                                              log_parent=self, environment=self)
            self.set_door_state = True
            self.setMacro(macro)
            
            ret = self.filterStateEvent(Macro.start)
        except:
            self.set_state(Macro.Abort)
            self.push_change_event('State')
            err = traceback.format_exc()
            par_str = string.join(par_str_list)
            self.error("Error starting \"%s\":\n%s", par_str, err)
            self.set_state(Macro.Finished)
            self.push_change_event('State')
        
        return ret or []
    
    def macroStateChanged(self, data, state):
        state_dict = {Macro.Finished: 'FINISHED', Macro.Abort: 'ABORTED'}
        state_name = (state in state_dict and state_dict[state]) or state
        self.debug("Macro state changed: %s", state_name)
        if state == Macro.Finished and self.macro.hasResult():
            self.result = self.macro.getResult()
            self.push_change_event('Result', self.result)
        if self.set_door_state:
            self.set_state(state)
            self.debug("pushing %s", state_name)
            self.push_change_event('State')
        if state == Macro.Finished:
            self.setMacro(None)        

    def filterStateEvent(self, macro_fn, args=None, opts=None):
        m = self.getMacro()
        if m is None:
            return

        state = m.getStateObj()
        state.lock()
        try:
            self.set_door_state = False
            args = args or []
            opts = dict(opts or {})
            stack = EventStack()
            opts['stack'] = stack
            ret = macro_fn(m, *args, **opts)
            all_events = stack.getAllRead() + stack.getAllUnread()
            for event in all_events:
                self.set_state(event)
                self.debug("pushing filtered %s", event)
                self.push_change_event('State')
            self.set_door_state = True
        finally:
            state.unlock()

        return ret
    
    def Abort(self):
        self.filterStateEvent(Macro.abort)
    
    def read_Command(self, attr):
        attr.set_value(self.cmd)

    def write_Command(self, attr):
        self.cmd = attr.value

    def read_Result(self, attr):
        attr.set_value(self.result)

    def readLogAttr(self, attr):
        name = attr.get_name()
        handler_data = self.handler_dict[name]
        handler = handler_data[0]
        handler.read(attr)

    read_Error = read_Warning = read_Info = \
                 read_Output = read_Debug = readLogAttr

    def addEnvChangeListener(self,listener):
        if not listener in self.env_listeners:
            self.env_listeners.append(listener)
    
    def removeEnvChangeListener(self,listener):
        if listener in self.env_listeners:
            self.env_listeners.remove(listener)
            
    def setEnv(self,prop_name,prop_value):
        if type(prop_value) != list:
            db_prop_value = [prop_value]
        else:
            db_prop_value = prop_value
        setattr(self,prop_name,prop_value)
        db = PyTango.Database() 
        db.put_device_property(self.get_name(),{prop_name:db_prop_value})
        
        for env_listener in self.env_listeners:
            env_listener.env_changed(prop_name,prop_value)
    
    def getEnv(self,prop_name,cache=True):
        try:
            return getattr(self,prop_name)
        except:
            return None
    
    def getEnvDescription(self,prop_name,cache=True):
        door_class = self.get_device_class()
        content = door_class.device_property_list.get(prop_name).copy()
        content[2] = str(getEnv(prop_name))
        return content

    def getAllEnvDescription(self,cache=True):
        door_class = self.get_device_class()
        device_property_list = door_class.device_property_list.copy()
        for prop_name in device_property_list:
            content = device_property_list.get(prop_name)
            content[2] = str(self.getEnv(prop_name))
        return device_property_list
    
    def getAllEnv(self,cache=True):
        door_class = self.get_device_class()
        props = door_class.device_property_list
        ret = {}
        for prop_name in props:
            try:
                ret[prop_name] = getattr(self,prop_name)
            except:
                ret[prop_name] = None
        return ret

    def loadDataConfig(self):

       self.data_handler    = DataHandler()
       
       filer = FileRecorder(format=DataFormats.SPEC )
       filer.setFileName( self.DataFile )
       filer.setSaveMode( SaveModes.RECORD )
       self.addEnvChangeListener( filer )
    
       dumbr = DumbRecorder( )
       dumbr.setSaveMode( SaveModes.BLOCK )
       self.addEnvChangeListener( dumbr )
       
       termr = LoggerRecorder( )
       termr.setSaveMode( SaveModes.RECORD )
       self.addEnvChangeListener( termr )
    
       shmr = ShmRecorder( )
       shmr.setSaveMode( SaveModes.RECORD )
       shmr.setSize( 1024, 3 )
       shmr.setProgram( self.ShmProgram )
       shmr.setShmID( self.ShmID )
       self.addEnvChangeListener( shmr )
    
       self.data_handler.addRecorder( filer )
       self.data_handler.addRecorder( dumbr )
       self.data_handler.addRecorder( shmr )
        
    
class DoorClass(PyTango.PyDeviceClass):

    class_name = 'Door'

    class_property_list = {
    }

    device_property_list = {
        'MacroServerName':	[PyTango.DevString, 'Macro server device name',['']],
        'ActiveMntgrp':     [PyTango.DevString, 'Active measurement group',['']],
        'ActiveDiffrac':     [PyTango.DevString, 'Active diffractometer controller',['']],
        'DataFile':         [PyTango.DevString, 'File where data is stored',['/tmp/spock.dat']],
        'ShmProgram':       [PyTango.DevString, 'Shared memory program name',['']],
        'ShmID':            [PyTango.DevString, 'Shared memory ID',['']]
    }
    
    cmd_list = {
        'RunMacro':	[[PyTango.DevVarStringArray, 'Macro name + arguments'],
                         [PyTango.DevVarStringArray, 'Macro start results']],
        'Abort':	[[PyTango.DevVoid, ''],
                         [PyTango.DevVoid, '']],
    }

    attr_list = {
        'Command':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ_WRITE, 16]],
        'Result':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
        'Error':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
        'Warning':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
        'Info':		[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
        'Output':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
        'Debug':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 512]],
    }
