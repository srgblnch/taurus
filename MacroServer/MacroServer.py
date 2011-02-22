import os, sys
import threading

from Door         import *
from Manager      import *

import utils
from PoolTimer import PoolTimer
from Attribute import WorkingState

class TypeListAttribute(PyTango.SpectrumAttr):
    pass
    
class MacroServer(PyTango.Device_3Impl, Logger):

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
        self.set_state(PyTango.DevState.ON)
        self.set_change_event('State', True, False)
        self.set_change_event('Status', True, False)
        
        dev_class = self.get_device_class()
        self.get_device_properties(dev_class)
        self.door_list = {}

        self.polling_timer = PoolTimer(2000)
        self.polling_timer.start()
        
        self.pool_state_dict = {}
        for name in self.PoolNames:
            self.debug("Creating pool %s", name)
            pool = Pool(name, self)
            self.pool_state_dict[pool] = None
            pool.subscribeEvent(self.poolStateChanged,pool)

        self.MacroDirs = self.MacroPath.split(':')

        pools = self.pool_state_dict.keys()

        self.manager = Manager('Manager', pools, parent=self, mac_path=self.MacroDirs)
        
        self.set_change_event('TypeList', True, False)
        self.type_list = self.manager.getTypeListObj()

        self.set_change_event('MacroList', True, False)
        self.macro_list = self.manager.getMacroListObj()

    def delete_device(self):
        self.debug("Start deleting device")
        self.set_state(PyTango.DevState.UNKNOWN)
        self.push_change_event('State')
        for type_name in self.type_list.read():
            if type_name[-1] == '*':
                self.delTypeAttribute(type_name[:-1])

    def dyn_attr(self):
        self.type_list.subscribeEvent(self.typeListChanged)
        self.macro_list.subscribeEvent(self.macroListChanged)
        
    def registerDoor(self, door):
        self.door_list[door.get_name()] = door

    def unregisterDoor(self, door):
        del self.door_list[door.get_name()]
        
    def GetDoor(self, client_id):
        return 'door/instance/%s' % client_id

    def GetMacroInfo(self, macro_name):
        """GetMacroInfo(string macro_name):
        
           Returns a list of string representing macro information.
           
           Params:
               - macro_name: a string with the macro name
           Returns:
               - list[0]: macro description (documentation)
                 list[1]: macro hints, if any
                 list[2]: number of parameters = N
                 list[3...3+4*N]: parameter info: name, type, desc, default value
                 list[4+4*N]: number of results = M
                 list[5+4*N...5+4*N+4*M]: result info: name, type, desc, default value
        """
                
        macro_class = self.manager.getMacroClass(macro_name)
        info = [macro_class.__doc__ or 'Undocumented macro',str(macro_class.hints)]
        par_def = macro_class.param_def
        info += self.getMacroParamInfo(par_def)
        result_def = macro_class.result_def
        info += self.getMacroResultInfo(result_def)
        return info

    def getMacroParamInfo(self, par_def):
        info = [str(len(par_def))]
        for name, type_class, def_val, desc in par_def:
            repeat = isinstance(type_class, ParamRepeat)
            info.append(name)
            type_name = (repeat and 'ParamRepeat') or type_class
            info.append(type_name)
            info.append(desc)
            if repeat:
                rep = type_class
                opts = sep = ''
                for opt, val in rep.items():
                    opts += '%s%s=%s' % (sep, opt, val)
                    sep = ', '
                info.append(opts)
                info += self.getMacroParamInfo(rep.param_def)
            else:
                info.append(str(def_val))
        return info
    
    def getMacroResultInfo(self,result_def):
        info = [str(len(result_def))]
        for name, type_class, def_val, desc in result_def:
            repeat = isinstance(type_class, ParamRepeat)
            info.append(name)
            type_name = (repeat and 'ParamRepeat') or type_class
            info.append(type_name)
            info.append(desc)
            if repeat:
                rep = type_class
                opts = sep = ''
                for opt, val in rep.items():
                    opts += '%s%s=%s' % (sep, opt, val)
                    sep = ', '
                info.append(opts)
                info += self.getMacroParamInfo(rep.param_def)
            else:
                info.append(str(def_val))
        return info
        
    
    def read_DoorList(self, attr):
        door_list = self.door_list.keys()
        return attr.set_value(door_list)
    
    def typeListChanged(self, data, type_data):
        all_types, old_types, new_types = type_data

        for type_name in old_types:
            if type_name[-1] == '*':
                self.delTypeAttribute(type_name[:-1])

        for type_name in new_types:
            if type_name[-1] == '*':
                self.addTypeAttribute(type_name[:-1])

        self.push_change_event('TypeList', all_types)

    def read_TypeList(self, attr):
        attr.set_value(self.type_list.read())

    def addTypeAttribute(self, type_name):
        type_obj = self.manager.getType(type_name)
        attr_name = type_name + 'List'
        attr_data = (type_name, attr_name)
        
        self.debug("Adding dynamic attribute %s", attr_name)
        attr = TypeListAttribute(attr_name, PyTango.DevString,
                                 PyTango.READ, 512)
        self.add_attribute(attr, MacroServer.read_typeItemList)
        self.set_change_event(attr_name, True, False)
        type_obj.subscribeItemListEvent(self.typeItemListChanged, attr_data)

    def delTypeAttribute(self, type_name):
        type_obj = self.manager.getType(type_name)
        attr_name = type_name + 'List'
        attr_data = (type_name, attr_name)
        
        self.debug("Deleting dynamic attribute %s", attr_name)
        type_obj.unsubscribeItemListEvent(self.typeItemListChanged, attr_data)
        self.remove_attribute(attr_name)

    def macroListChanged(self, data, macro_data):
        all_macros, old_macros, new_macros = macro_data
        self.push_change_event('MacroList', all_macros)
        
    def read_MacroList(self, attr):
        attr.set_value(self.macro_list.read())

    def typeItemListChanged(self, type_data, item_data):
        type_name, attr_name = type_data
        all_items, old_items, new_items = item_data
        self.push_change_event(attr_name, sorted(all_items))
        
    def read_typeItemList(self, attr):
        attr_name = attr.get_name()
        type_name = attr_name.split('List')[0]
        type_obj = self.manager.getType(type_name)
        item_list = type_obj.getItemList()
        attr.set_value(item_list)
    
    def poolStateChanged(self,pool,pool_state_info):
                
        self.pool_state_dict[pool] = pool_state_info
        
        p_state, p_last_state, p_work_state, p_last_work_state = pool_state_info
        
        curr_status = ''
        
        any_pool_offline = False
        any_pool_online = False
        
        for pool,s_info in self.pool_state_dict.items():
            
            pool_tg_st = s_info[0]
            pool_wk_st = s_info[2]
            if pool_wk_st == WorkingState.ONLINE:
                any_pool_online = True
            else:
                any_pool_offline = True
                self.polling_timer.addPool(pool)
                
            if pool.dev_name != pool.alias:
                p_name = pool.alias + " (" + pool.dev_name + ")"
            else:
                p_name = pool.alias
            
            if pool_wk_st == WorkingState.ONLINE:
                curr_status += "Connected to Pool %s (in %s state)\n" % (p_name,str(pool_tg_st))
            elif pool_wk_st == WorkingState.CRASHED:
                curr_status += "Connection to Pool %s was lost (Server crashed or network failed)\n" % p_name
            elif pool_wk_st == WorkingState.OFFLINE:
                curr_status += "NOT connect to Pool %s. Pool is offline\n" % p_name
            elif pool_wk_st == WorkingState.SHUTDOWN:
                curr_status += "Disconnected from Pool %s. Pool as been shutdown\n" % p_name
        
        old_state = self.get_state()
        old_status = self.get_status()
        
        self.set_status(curr_status)
        
        if any_pool_offline:
            self.set_state(PyTango.DevState.FAULT)
        else:
            self.set_state(PyTango.DevState.ON)
            
        new_state = self.get_state()
        
        if new_state != old_state:
            self.debug("Changed state from %s to %s" % (old_state, new_state))
            self.push_change_event('State')
            
        if curr_status != old_status:
            self.push_change_event('Status')
    
    def always_executed_hook(self):
        #print "in always_executed_hook"
        pass
    
class MacroServerClass(PyTango.PyDeviceClass):

    class_name = 'MacroServer'

    class_property_list = {
    }

    device_property_list = {
        'PoolNames':	[PyTango.DevVarStringArray, 'Pool device names', ['']],
        'MaxDoors':	[PyTango.DevLong,   'Maximum number of doors', [1]],
        'MacroPath':	[PyTango.DevString, 'Path to macro directory', ['']],
    }
    
    cmd_list = {
        'GetDoor':	[[PyTango.DevString, 'Client ID string'],
                         [PyTango.DevString, 'Door device name']],
        'GetMacroInfo': [[PyTango.DevString, 'Macro name'],
                         [PyTango.DevVarStringArray, 'Macro description']],
    }

    attr_list = {
        'DoorList':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 32]],
        'MacroList':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 1024]],
        'TypeList':	[[PyTango.DevString, PyTango.SPECTRUM,
                          PyTango.READ, 256]],
    }

    def dyn_attr(self, dev_list):
        for dev in dev_list:
            dev.dyn_attr()

            
def main(argv):

    pyu = PyTango.PyUtil(sys.argv)
    
    import os, signal
    def handler(signo, stack_frame):
        pass
    signal.signal(signal.SIGUSR1, handler)
    os.kill(os.getpid(), signal.SIGUSR1)
    
    pyu.add_TgClass(MacroServerClass, MacroServer, 'MacroServer')
    pyu.add_TgClass(DoorClass, Door, 'Door')

    u = PyTango.Util.instance()
    u.set_serial_model(PyTango.SerialModel.BY_PROCESS)
    u.server_init()

    u.server_run()    

if __name__ == '__main__':
    try:
        main(sys.argv)
    except PyTango.DevFailed,e:
        utils.print_DevFailed(e)
    except:
        import traceback
        traceback.print_exc()
    
