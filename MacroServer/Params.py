from Log import *
from Event import *
        
class TypeNames:
    def addType(self, name):
        setattr(self, name, name)

Type = TypeNames()

class ParamType(Logger):

    # Capabilities
    ItemList        = 'ItemList'
    ItemListEvents    = 'ItemListEvents'
    
    capabilities    = []
    
    
    def __init__(self, name, manager):
        Logger.__init__(self, name + 'Type', manager)
        self.__class__.name = name
        self.manager = weakref.ref(manager)

    def __del__(self):
        self.debug("deleting")

    def getName(self):
        return self.name
    
    def hasCapability(self, cap):
        return cap in self.capabilities
    
    def getManager(self):
        return self.manager()

    def fromStr(self, str):
        return self.type_class(str)

    def toStr(self, obj):
        return str(obj)
    
    def getItemList(self,cache=False):
        return []

    def subscribeItemListEvent(self, cb, data=None):
        pass
        
    def unsubscribeItemListEvent(self, cb, data=None):
        pass

class AttrParamType(ParamType):
    
    type_class = str
    capabilities = [ParamType.ItemList]
    
    def __init__(self, name, manager):
        ParamType.__init__(self, name, manager)
  
                  
class PoolObjParamType(ParamType, ListEventGenerator):

    FullNameDelim    = '/'

    All            = 'All'
    
    capabilities = [ParamType.ItemList, ParamType.ItemListEvents]

    def __init__(self, name, manager):
        ParamType.__init__(self, name, manager)
        ListEventGenerator.__init__(self, '%s Type' % name)
        
        # pool_dict: keys -> <Pool> ; value -> list<string>> with name of objects
        self.pool_dict = {}
        
        # obj_dict: keys -> <string> with name of object ; value -> <Pool>
        self.obj_dict = {}
        
        self.item_list_cache = []
        
        if not self.hasCapability(ParamType.ItemListEvents):
            return
        
        for pool in self.getPoolList():
            self.subscribePoolObjListEvent(self.poolObjListChanged, pool)

    def getPoolList(self):
        return self.getManager().getPoolList()

    def getOnlinePoolList(self):
        return self.getManager().getOnlinePoolList()
        
    def getFullObjName(self, pool_name, obj_name):
        return '%s%s%s' % (pool_name, self.FullNameDelim, obj_name)

    def isFullObjName(self, obj_name):
        return obj_name.count(self.FullNameDelim) > 0

    def splitFullObjName(self, name):
        """splitFullObjName(full_obj_name) -> [pool name, obj_name]
           
           Splits the given full object name into the name of the Pool tango 
           device and the object name.
           
           Params:
               full_obj_name - string representing obj: <pool tango device name>'/'obj_name
           Retuns:
               A list with the pool name and object name strings.
        """ 
        if not self.isFullObjName(name):
            return [None, name]
        tokens = name.split(self.FullNameDelim)
        pool_name = tokens[:-1]
        obj_name = tokens[-1]
        return [pool_name, obj_name]

    def fromStr(self, str):
        """ fromStr(full_obj_name) -> PoolObj
            
            Returns the Pool object from the given name.
            
            Params:
              full_obj_name - string representing obj: <pool tango device name>'/'obj_name
            Returns:
                The PoolObj represented by the given string
            Throws:
                AttributeError in case the given name is not of the same type as 
                the type this ParamType object is representing.
        """
        
        if str not in self.getItemList(cache=True):
            print "List of objects:",self.getItemList(cache=True)
            raise AttributeError, 'Object "%s" not found' % str
        pool = self.obj_dict[str]
        pool_name, obj_name = self.splitFullObjName(str)
        return self.getPoolObj(pool, obj_name)

    def toStr(self, obj):
        """ toStr(PoolObj) -> string
            
            Returns the string representing the given object.
            The string contains the <pool tango device name>'/'obj_name
            
            Params:
                obj - The PoolObj for which a string has to be calculated.
            Returns:
                A string representing the given object
            Throws:
                AttributeError in case the given object is not of the same
                type as the ParamType this object is representing
        """
        name = self.getObjName(obj)
        obj_list = self.getItemList(cache=True)
        if name in obj_list:
            return name
        for pool in self.pool_list:
            name = self.getFullObjName(pool.name, name)
            if name in obj_list:
                return name
        raise AttribueError, 'Object "%s" not found' % obj
    
    def poolObjListChanged(self, pool, obj_data):
        all_obj, old_obj, new_obj = obj_data
        if old_obj:
            self.debug("Deleting: %s", old_obj)
        if new_obj:
            self.debug("Adding:   %s", new_obj)
        
        self.pool_dict[pool] = all_obj
        self.obj_dict = self.buildFullObjDict(self.pool_dict)
        obj_list = sorted(self.obj_dict.keys())
        self.fireEvent(obj_list)
        
    def getItemList(self,cache=False):
        if self.hasCapability(ParamType.ItemListEvents):
            self.item_list_cache = self.read()
            return self.item_list_cache
        
        if cache:
            return self.item_list_cache
        
        pool_dict = {}
        for pool in self.getOnlinePoolList():
            pool_dict[pool] = self.getPoolObjList(pool)

        self.pool_dict = pool_dict
        self.obj_dict = self.buildFullObjDict(pool_dict)
        self.item_list_cache = sorted(self.obj_dict.keys())
        return self.item_list_cache

    def buildFullObjDict(self, pool_dict):
        obj_dict = {}
        for pool, pool_obj_list in pool_dict.items():
            for name in pool_obj_list:
                if name not in obj_dict.keys():
                    obj_dict[name] = pool
                    continue
                other_pool = obj_dict[name]
                del obj_dict[name]
                other_name = self.getFullObjName(other_pool.name, name)
                obj_dict[other_name] = other_pool
                this_name = self.getFullObjName(pool.name, name)
                obj_dict[this_name] = pool
        return obj_dict

    def getObjName(self, obj):
        return obj.getName()

    def getPoolObjList(self, pool):
        obj_list = pool.getHardwareListObj(self.getName())
        return obj_list.read()

    def getPoolObj(self, pool, name):
        return pool.getHardwareObj(self.getName(), name)
    
    def subscribeItemListEvent(self, cb, data=None):
        self.subscribeEvent(cb, data)
    
    def unsubscribeItemListEvent(self, cb, data=None):
        self.unsubscribeEvent(cb, data)
    
    def subscribePoolObjListEvent(self, cb, pool):
        obj_list = pool.getHardwareListObj(self.getName())
        obj_list.subscribeEvent(cb, pool)

    def unsubscribePoolObjListEvent(self, cb, pool):
        obj_list = pool.getHardwareListObj(self.getName())
        obj_list.unsubscribeEvent(cb, pool)


class ParamRepeat:
    # opts: min, max
    def __init__(self, *param_def, **opts):
        self.param_def = param_def
        self.opts = {'min': 1, 'max': None}
        self.opts.update(opts)

    def items(self):
        return self.opts.items()
    
    def __getattr__(self, name):
        return self.opts[name]

    
class ParamDecoder:

    def __init__(self, type_manager, macro_class, param_str_list):
        self.type_manager = type_manager
        self.macro_class = macro_class
        self.param_str_list = param_str_list
        self.param_list = None
        self.decode()

    def decode(self):
        dec_token, obj_list = self.decodeNormal(self.param_str_list[1:],
                                                self.macro_class.param_def)
        self.param_list = obj_list
    
    def decodeNormal(self, str_list, def_list):
        str_len = len(str_list)
        obj_list = []
        str_idx = 0
        for par_def in def_list:
            name, type_class, def_val, desc = par_def
            if str_idx == str_len:
                if def_val is None:
                    if not isinstance(type_class, ParamRepeat):
                        raise RuntimeError, 'Param %s not specified' % name
                    elif isinstance(type_class, ParamRepeat):
                        min = par_def[1].opts['min']
                        if min > 0:
                            raise RuntimeError, 'Param %s demands at least %d values' % (name, min)
                new_obj_list = [def_val]
            else:
                if isinstance(type_class, ParamRepeat):
                    data = self.decodeRepeat(str_list[str_idx:], par_def)
                    dec_token, new_obj_list = data
                else:
                    type_name = type_class
                    type_class = self.type_manager.getTypeClass(type_name)
                    par_type = self.type_manager.getTypeInst(type_name)
                    par_str = str_list[str_idx]
                    val = par_type.fromStr(par_str)
                    if val is None:
                        msg = 'Could not create %s param "%s" from "%s"' % \
                              (par_type.name, par_name, par_str)
                        raise RuntimeError, msg
                    dec_token = 1
                    new_obj_list = [val]
                str_idx += dec_token
            obj_list += new_obj_list
        return str_idx, obj_list

    def decodeRepeat(self, str_list, par_def):
        name, rep_data, def_val, desc = par_def
        min_rep = rep_data.min
        max_rep = rep_data.max
        dec_token = 0
        obj_list = []
        rep_nr = 0
        while dec_token < len(str_list):
            if max_rep is not None and rep_nr == max_rep:
                break
            new_token, new_obj_list = self.decodeNormal(str_list[dec_token:],
                                                        rep_data.param_def)
            dec_token += new_token
            if len(new_obj_list) == 1:
                new_obj_list = new_obj_list[0]
            obj_list.append(new_obj_list)
            rep_nr += 1
        if rep_nr < min_rep:
            msg = 'Found %d repetitions of param %s, min is %d' % \
                  (rep_nr, name, min_rep)
            raise RuntimeError, msg
        return dec_token, obj_list
        
    def getParamList(self):
        return self.param_list

    def __getattr__(self, name):
        return getattr(self.param_list, name)
