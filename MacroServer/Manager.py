import os, threading, inspect
from Log import *
from Macro import *
from Params import *

class MacroLib:
    """Object representing a python module containning macro classes.
       Public members:
           f_path - complete (absolute) path and filename
           f_name - filename (including file extension)
           path - complete (absolute) path
           name - module name (without file extension)
           macro_list - list<MacroData>
    """
    
    def __init__(self,f_path):
        self.f_path = f_path
        self.f_name = f_path[f_path.rfind('/')+1:]
        self.path = f_path[:f_path.rfind('/')]
        self.name = self.f_name[:self.f_name.rfind('.py')]
        self.macro_list = []
    
    def addMacro(self,macro_data):
        self.macro_list.append(macro_data)
    
class MacroData:
    """Object representing a python macro class. 
       Public members:
           name - class name
           klass - python class object
           lib - MacroLib object representing the module where the macro is.
    """
            
    def __init__(self,lib,name,klass):
        self.name = name
        self.klass = klass
        self.lib = lib
    
    def getMacroCode(self):
        """Returns a tuple (sourcelines, firstline) corresponding to the 
        definition of the macro class. sourcelines is a list of source code 
        lines. firstline is the line number of the first source code line.
        """
        try:
            return inspect.getsourcelines(self.klass)
        except e:
            return None


class Manager(Logger):

    All = Macro.All
    
    def __init__(self, name, pool_list, mac_path=['macros'], parent=None,
                 param_decoder=ParamDecoder):
        Logger.__init__(self, name, parent)
        
        self.name = name
        self.pool_list = pool_list

        self.type_manager = TypeManager(self)
        
        # dict<<string>,<MacroLib>>
        # key   - module name (without path and without extension
        # value - MacroLib object representing the module 
        self.macro_lib_dict = {}
        
        # dict<<string>,<MacroData>>
        # key   - macro name
        # value - MacroData object representing the macro
        self.macro_dict = {}
        self.macro_list = ListEventGenerator('%s macro list' % name)
        
        self.env = {}
        self.mac_path = mac_path or []

        self.param_decoder = param_decoder

        self.loadTypeFile('Types')
        
        macro_modules = self.getExistingMacroModules(self.mac_path)
        
        for macro_module in macro_modules:
            self.loadMacroFile(macro_module)
        
    def __del__(self):
        self.debug("deleting Manager")

    def getName(self):
        return self.name
    
    def getPoolList(self):
        return self.pool_list
    
    def getOnlinePoolList(self):
        return [p for p in self.getPoolList() if p.working_state == WorkingState.ONLINE]
    
    def getPool(self, name=All):
        if name == Macro.All:
            pool_list = self.getPoolList()
        else:
            pool_list = [x for x in pool_list if x.getName() == name]
        if pool_list:
            return pool_list[0]
        return None

    def loadTypeFile(self, filename, path=None):
        self.type_manager.loadTypefile(filename, path)

    def getTypeListObj(self):
        return self.type_manager.getTypeListObj()
    
    def getType(self, type_name):
        return self.type_manager.getTypeInst(type_name)

    def getMacroPath(self):
        return self.mac_path
    
    def getExistingMacroModules(self, path=None):
        path = path or self.getMacroPath()
        ret = []
        for p in path:
            try:
                elems = os.listdir(p)
                for f in os.listdir(p):
                    if f.endswith('.py'):
                        ret.append(f[:len(f)-3])
            except:
                self.debug("'%s' is not a valid path" % p)
        return ret     
    
    def loadMacroFile(self, module_name, path=None):
        """loadMacroFile(string module_name, string path = None)
        
           Loads the macro code for the given module.
           
           Params:
               - module_name: a string with the name of the module
               - path: a list of strings containing absolute path to search for
               the modules. Optional parameter. Defaults to None. If None, the
               current MacroPath is used as search path.
           Returns:
                a string representing the absolute filename of the loaded module
        """

        path = path or self.getMacroPath()
        
        try:
            self.debug("Trying to find %s macro module" % module_name)
            file, pathname, desc = imp.find_module(module_name, path)
            self.debug("Loading macro file %s", pathname)
            m = imp.load_module(module_name, file, pathname, desc)
        finally:
            if file is not None:
                file.close()
        
        macro_lib = self.macro_lib_dict.get(module_name) or MacroLib(pathname)
        
        lib_contains_macros = False
        for name in dir(m):
            klass = getattr(m, name)
            klass_type = type(Manager)
            if (type(klass) == klass_type and klass is not Macro and \
                issubclass(klass, Macro)):
                lib_contains_macros = True
                self.addMacro(macro_lib, name, klass)

        if lib_contains_macros == True:
            self.macro_lib_dict[macro_lib.name] = macro_lib
            macro_list = self.macro_dict.keys()
            self.macro_list.fireEvent(sorted(macro_list))
            
        return pathname
    
    def reloadMacroFile(self, module_name, path=None):
        """reloadMacroFile(string module_name, string path = None)
        
           Reloads the macro code for the given module. If the module is not
           already loaded, it will load it. Otherwise a reload is done.
           
           Params:
               - module_name: a string with the name of the module
               - path: a list of strings containing absolute path to search for
               the modules. Optional parameter. Defaults to None. If None, the
               current MacroPath is used as search path.
           Returns:
                a string representing the absolute filename of the reloaded module
        """
        
        if self.macro_lib_dict.get(module_name) == None:
            return self.loadMacroFile(module_name)
        
        path = path or self.getMacroPath()
        self.debug("Trying to load %s (%s)" % (module_name, path))
        file, pathname, desc = imp.find_module(module_name, path)
        self.debug("Loading macro file %s", pathname)
        
        try:
            m = imp.load_module(module_name, file, pathname, desc)
        finally:
            file.close()
        
        macro_lib = MacroLib(pathname)
        
        lib_contains_macros = False
        for name in dir(m):
            klass = getattr(m, name)
            klass_type = type(Manager)
            if (type(klass) == klass_type and klass is not Macro and \
                issubclass(klass, Macro)):
                lib_contains_macros = True
                self.addMacro(macro_lib, name, klass)

        if lib_contains_macros == True:
            self.macro_lib_dict[macro_lib.name] = macro_lib 
        
        macro_list = self.macro_dict.keys()
        self.macro_list.fireEvent(sorted(macro_list))
                        
        return pathname
    
    def getMacroData(self,macro_name):
        return self.macro_dict.get(macro_name)
    
    def getMacroLib(self,module_name):
        return self.macro_lib_dict.get(module_name)
    
    def addMacro(self, lib, name, klass, fire_event=False):
        action = ((name in self.macro_dict) and "Updating") or "Adding"
        self.debug("%s macro %s", action, name)
        self.macro_dict[name] = MacroData(lib,name,klass)
        lib.addMacro(self.macro_dict[name])
        klass.name = name
        if fire_event == True:
            macro_list = self.macro_dict.keys()
            self.macro_list.fireEvent(sorted(macro_list))
        
    def getMacroListObj(self):
        return self.macro_list

    def getMacroClass(self, macro_name):
        return self.macro_dict[macro_name].klass

    def decodeMacroParams(self, par_str_list):
        if len(par_str_list) == 0:
            raise RuntimeError, 'Macro name not specified'
        macro_name = par_str_list[0]
        macro_class = self.getMacroClass(macro_name)
        decoder = self.param_decoder
        par_list = decoder(self.type_manager, macro_class, par_str_list)
        return macro_class, par_list
    
    def strMacroParamValues(self,par_list):
        """strMacroParamValues(list<string> par_list) -> list<string> 
        
           Creates a short string representantion of the parameter values list.
           Params:
               - par_list: list of strings representing the parameter values.
           Return:
               a list containning an abreviated version of the par_list argument. 
        """
        ret = []
        for p in par_list:
            param_str = str(p)
            if len(param_str)>9:
                param_str = param_str[:9] + "..."
            ret.append(param_str)
        return ret
    
    def prepareMacro(self, macro_class, par_list, opts={}, parent_macro=None,
                     log_parent=None, environment=None):
        macro_opts = dict(opts)
        macro_opts.update({'no_exec': True})
        par_list_str = self.strMacroParamValues(par_list)
        self.debug("Preparing macro %s%s", macro_class.name, tuple(par_list_str))
        macro = macro_class(self, parent_macro, log_parent, environment,
                            *par_list, **macro_opts)
        ret = macro.prepare(*par_list, **opts)
        return macro, ret

    def getObj(self, name, type_class=All, pool=All):
        objs = self.findObjs(name, type_class, pool)
        if len(objs) == 0:
            return None
        if len(objs) > 1:
            raise AttributeError, 'More than one object "%s" found' % name
        return objs[0]

    def findObjs(self, re_pattern, type_class=All, pool=All):
        if type_class == Macro.All:
            type_name_list = self.type_manager.getTypeListObj.read()
        else:
            type_name_list = [type_class]
        if pool != Macro.All:
            raise RuntimeError, 'Pool object search not implemented yet'
        obj_list = []
        re_obj = re.compile(re_pattern)
        for type_name in type_name_list:
            type_inst = self.type_manager.getTypeInst(type_name)
            if not type_inst.hasCapability(ParamType.ItemList):
                continue
            for name in type_inst.getItemList():
                m = re_obj.match(name)
                if m is not None:
                    obj = type_inst.fromStr(name)
                    obj_list.append(obj)
        return obj_list
    
    def getMotion(self, elems, motion_source=None, read_only=False, cache=True):
        if not motion_source:
            motion_source = self.getPoolList()
        return Motion(elems, motion_source)

class TypeManager(Logger):

    def __init__(self, manager):
        self.manager = weakref.ref(manager)
        Logger.__init__(self, 'TypeManager', manager)
        self.type_list = ListEventGenerator('TypeList')
        
        self.class_dict = {}
        self.inst_dict = {}
        self.type_lock = threading.RLock()

    def __del__(self):
        self.debug("deleting")
        
    def getManager(self):
        return self.manager()
    
    def loadTypefile(self, filename, path=None):
        file, pathname, desc = imp.find_module(filename, path)
        self.debug("Loading type file %s", pathname)
        try:
            m = imp.load_module(filename, file, pathname, desc)
        finally:
            file.close()
        for name in dir(m):
            klass = getattr(m, name)
            klass_type = type(Manager)
            base_klasses = [ParamType, PoolObjParamType, AttrParamType]
            if (type(klass) == klass_type and klass not in base_klasses and \
                issubclass(klass, ParamType)):
                inst = klass(name, self.getManager())
                self.addType(name, klass, inst)
            
    def addType(self, type_name, type_class, type_obj):
        action = (((type_name in self.class_dict) and "Updating") \
                  or "Adding")
        self.debug("%s type %s", action, type_name)
        self.class_dict[type_name] = type_class
        self.inst_dict[type_class] = type_obj
        type_list = list(self.type_list.read())
        exp_type_name = type_name
        if type_obj.hasCapability(ParamType.ItemList):
            exp_type_name += '*'
        type_list.append(exp_type_name)
        self.type_list.fireEvent(type_list)

        global Type
        Type.addType(type_name)
        
    def getTypeListObj(self):
        return self.type_list

    def getTypeClass(self, type_name):
        return self.class_dict[type_name]
    
    def getTypeInst(self, type_name):
        type_class = self.getTypeClass(type_name)
        return self.inst_dict[type_class]

    def __getattr__(self, name):
        if name in self.class_dict:
            return self.class_dict[name]
        return getattr(self.class_dict, name)
       