import os, sys, string, re, thread
import utils
from Attribute import *
from Motion import *

class ControllerClassProp(Object):
    
    def __init__(self,name,type,desc,dftv):
        self.name = name
        self.type = type
        self.desc = desc
        self.dftv = dftv
        
class ControllerClassPropInstance(ControllerClassProp):
    
    def __init__(self,name,type,desc,value):
        self.name = name
        self.type = type
        self.desc = desc
        self.value = value

class ControllerClass(Object):
    
    def __init__(self, pool, type, name, file):
        self.pool = weakref.ref(pool)
        self.type = type
        self.name = name
        self.file = file
        self.filename = self.file[self.file.rfind('/')+1:]
        self.gender = None
        self.model = None
        self.organization = None
        self.props = {}
        self.buildInfo()
        
    def getName(self):
        return self.name

    def getClassName(self):
        return self.name
    
    def getType(self):
        return self.type

    def getLib(self):
        return self.filename
    
    def getGender(self):
        return self.gender
    
    def getModel(self):
        return self.model
    
    def getOrganization(self):
        return self.organization

    def getPool(self):
        return self.pool()
        
    def buildInfo(self):
        info = self.getPool().getControllerInfo([self.type,self.filename,self.name])
        self.descr = info[0]
        self.gender = info[1]
        self.model = info[2]
        self.organization = info[3]
        
        self.nb_prop = int(info[4])
        for i in xrange(self.nb_prop):
            idx = 5 + 4*i
            p = ControllerClassProp(info[idx],info[idx+1],info[idx+2],info[idx+3])
            self.props[p.name] = p
        
        i = 5 + 4 * self.nb_prop
        
        if self.type == "PseudoMotor":
            self.mot_roles = []
            self.nb_mot_roles = int(info[i])
            i = i + 1
            for m in xrange(self.nb_mot_roles):
                self.mot_roles.append(info[i])
                i = i + 1 
                
            self.pm_roles = []
            self.nb_pm_roles = int(info[i])
            i = i + 1
            for pm in xrange(self.nb_pm_roles):
                self.pm_roles.append(info[i])
                i = i + 1
            
        elif self.type == "PseudoCounter":
            self.counter_roles = []
            self.nb_counter_roles = int(info[i])
            i = i + 1
            for c in xrange(self.nb_counter_roles):
                self.counter_roles.append(info[i])
                i = i + 1 
    
class ControllerClassList(Logger):

    def __init__(self, pool):
        self.call__init__(Logger, 'ControllerClassList', pool)
        self.attr = pool.createAttr('ControllerClassList', DictListAttribute,
                                    dict_keys=['Type', 'Class', 'File'])
        self.cache = None
        self.pool = weakref.ref(pool)
        
    def __del__(self):
        self.debug("deleting")
        
    def getPool(self):
        return self.pool()
            
    def read(self,cache=False):
        if cache and not self.cache is None:
            return self.cache
        ctrl_classes = self.attr.read()
        self.cache = {}
        pool = self.getPool()
        for ctrl_class in ctrl_classes:
            key = ctrl_class['Class']
            type = ctrl_class['Type']
            file = ctrl_class['File']
            self.debug("Creating class %s" % key)
            self.cache[key] = ControllerClass(pool,type,key,file)
        return self.cache.keys()
    
    def getObj(self,name):
        return self.cache.get(name)
    
    def getObjs(self,cache=False):
        self.read(cache)
        return self.cache.values()
    
    def getControllerClass(self,name,cache=False):
        if not cache or self.cache is None:
            self.read(cache)
        return self.getObj(name)
    
class HardwareObjInfo(Object):
    
    BaseReStr = '(?P<name>\S+)\s+\((?P<dev_name>[^\)]+)\)'

    ReprStr   = '%(name)s (%(dev_name)s)'
    
    def __init__(self, name=None, dev_name=None, info_dict=None,
                 base_re=BaseReStr, info_re=None, from_str=None,
                 repr_str=ReprStr):
        self.call__init__(Object)
        
        self.name = name
        self.dev_name = dev_name
        self.info_dict = info_dict
        
        if from_str is not None:
            re_str = base_re
            if info_re is not None:
                re_str += '\s+' + info_re
            re_obj = re.compile(re_str)
            m = re_obj.match(from_str)
            
            if not m: 
                print "String '" + from_str + "' could not match " + re_str
                
            self.info_dict = m.groupdict()

        self.repr_str = repr_str
        if self.info_dict is not None:
            self.__dict__.update(self.info_dict)

    def copyInfoTo(self, obj):
        obj.updateAttrDict(self)
        
    def __repr__(self):
        return self.repr_str % self.__dict__
    
    def __str__(self):
        return repr(self)

    
class HardwareObj(EventGenerator, Logger):

    Created = 'Created'
    Deleted = 'Deleted'

    info_class = HardwareObjInfo

    def __init__(self, pool, info=None, from_str=None, parent=None):
        self.pool = weakref.ref(pool)
        if info is None:
            info = self.info_class(from_str=from_str)
        self.setInfo(info)
        
        self.call__init__(Logger, self.name, parent)
        self.call__init__(EventGenerator, str(self))
        
    def __del__(self):
        self.debug("deleting")

    def setInfo(self, info):
        self.info = info
        info.copyInfoTo(self)
        
    def getName(self):
        return self.name

    def getPool(self):
        return self.pool()

    def getInfoClass(self):
        return self.info_class
    
    def create(self):
        self.fireEvent(self.Created)
    
    def deleted(self):
        self.fireEvent(self.Deleted)
        
    def __repr__(self):
        return repr(self.info)

    def __str__(self):
        return repr(self)

    
class HardwareObjList(Logger, ListEventGenerator):

    def __init__(self, pool, name, obj_class, attr):
        self.pool = weakref.ref(pool)
        self.type = name
        self.list_name = '%sList' % self.type
        self.call__init__(Logger, self.list_name, pool)

        self.obj_class  = obj_class
        self.info_class = obj_class.info_class

        self.obj_dict = utils.CaselessDict()
        self.nonexist_dict = utils.CaselessDict()

        event_name = 'Pool %s %s list' % (pool.getName(), type)
        self.call__init__(ListEventGenerator, event_name)
        
        self.attr = attr
        self.connect()

    def __del__(self):
        self.debug("deleting")

    def getPool(self):
        return self.pool()
    
    def getRawListNames(self, raw_list):
        names = []
        for s in raw_list:
            info = self.info_class(from_str=s)
            names.append(info.name)
        return names
    
    def listChanged(self, data, event_val):
        self.debug("List has changed")
        all, rm, add = event_val
        pool = self.getPool()
        for s in add:
            info = self.info_class(from_str=s)
            name = info.name
            obj = self.obj_class(pool, info, parent=self)
            self.obj_dict[name] = obj

        rm = self.getRawListNames(rm)
            
        self.__removeObjs(rm)
        
        name_list = sorted(self.obj_dict.keys())
        self.fireEvent(name_list)

    def __removeObjs(self,raw_names):
        for name in raw_names:
            obj = self.getObj(name)
            del self.obj_dict[obj.name]
            obj.deleted()
        
    def getObj(self, name):
        return self.obj_dict[name]
        
    def connect(self):
        self.attr.connect()
        if self.attr.getEventsActive():
            if not self.attr.isSubscribed(self.listChanged):
                self.attr.subscribeEvent(self.listChanged)

    def disconnect(self):
        self.debug("Disconnecting")
        if self.attr.isSubscribed(self.listChanged):
            self.attr.unsubscribeEvent(self.listChanged)
            #self.__removeObjs(self.obj_dict.keys())
            #self.listChanged(None,([],[],[]))
            self.attr.disconnect()        

class ControllerInfo(HardwareObjInfo):

    BaseReStr = '(?P<name>\S+)\s+' \
                  '\((?P<module>[^\.]+)\.(?P<klass>[^/]+)/(?P<inst>[^\)]+)\)'

    InfoReStr = '- (?P<type>\S+)\s+(?P<lang>\S+)\s+' \
                  '(?P<ctrl_key>\S+)\s+\((?P<file>[^\)]+)\)'

    ReprStr   = '%(name)s (%(module)s.%(klass)s/%(inst)s) - ' \
                '%(type)s %(lang)s %(ctrl_key)s (%(file)s)'

    def __init__(self, name=None, from_str=None, **info_dict):
        self.call__init__(HardwareObjInfo, name=None, info_dict=info_dict,
                          from_str=from_str, repr_str=self.ReprStr,
                          base_re=self.BaseReStr, info_re=self.InfoReStr)
                                 
 
class Controller(HardwareObj):

    info_class = ControllerInfo
    
    def __init__(self, pool, info=None, from_str=None, parent=None):
        self.type = self.name = None
        self.call__init__(HardwareObj, pool, info=info, from_str=from_str,
                          parent=parent)

        extra_info = self.getPool().getControllerInfo([self.type,self.file,self.klass,self.name])
        extra_info = self.formatInfo(self.type, extra_info)
        self.__dict__.update(extra_info)
        
        self.dev_dict = {}

    def addDevice(self, dev):
        self.dev_dict[dev.ctrl_idx] = dev
        dev.ctrl = weakref.ref(self)
        
    def delDevice(self, dev):
        del self.dev_dict[dev.ctrl_idx]
        dev.ctrl = None
        
    def __str__(self):
        return '%s controller %s' % (self.type, self.name)

    def __getattr__(self, attr):
        return getattr(self.dev_dict, attr)

    @staticmethod
    def formatInfo(type, info):
        ret = {}
        ret['descr'] = info[0]
        ret['gender'] = info[1]
        ret['model'] = info[2]
        ret['organization'] = info[3]
        
        nb_prop = int(info[4])
        ret['nb_prop'] = nb_prop

        ret['props'] = {}
        for i in xrange(nb_prop):
            idx = 5 + 4*i
            p = ControllerClassPropInstance(info[idx],info[idx+1],info[idx+2],info[idx+3])
            ret['props'][p.name] = p
        
        i = 5 + 4 * nb_prop
        
        if type == "PseudoMotor":
            ret['mot_roles'] = []
            nb_mot_roles = int(info[i])
            ret['nb_mot_roles'] = nb_mot_roles 
            i = i + 1
            for m in xrange(nb_mot_roles):
                ret['mot_roles'].append(info[i])
                i = i + 1 
                
            ret['pm_roles'] = []
            nb_pm_roles = int(info[i])
            ret['nb_pm_roles'] = nb_pm_roles
            i = i + 1
            for pm in xrange(nb_pm_roles):
                ret['pm_roles'].append(info[i])
                i = i + 1
            
        elif type == "PseudoCounter":
            ret['counter_roles'] = []
            nb_counter_roles = int(info[i])
            ret['nb_counter_roles'] = nb_counter_roles
            i = i + 1
            for c in xrange(nb_counter_roles):
                ret['counter_roles'].append(info[i])
                i = i + 1 
        return ret

class HardwareDevice(HardwareObj):

    def __init__(self, pool, info=None, from_str=None, parent=None, ctrl=None):
        self.ctrl = ctrl and weakref.ref(ctrl)
        self.call__init__(HardwareObj, pool, info=info, from_str=from_str,
                          parent=parent)

    def waitReady(self, timeout=None):
        pass

    def abort(self, wait_ready=True, timeout=None):
        if wait_ready:
            self.waitReady(timeout=timeout)

    def getCtrl(self):
        return self.ctrl and self.ctrl()
    
    
class HardwareTangoObj(HardwareObj, TangoDevice):

    Ready   = PyTango.DevState.ON

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False):
        self.call__init__(HardwareObj, pool, info=info, from_str=from_str,
                          parent=parent)
        self.call__init__(TangoDevice, self.dev_name, no_check=no_check,
                          parent=parent)
        if not no_check:
            self.create()
            
    def create(self):
        self.connect()
        self.createAttr()
        HardwareObj.create(self)

    def deleted(self):
        HardwareObj.deleted(self)
        self.deleteAttr()
        self.disconnect()

    def createAttr(self):
        pass

    def deleteAttr(self):
        pass
    
    def waitReady(self, timeout=None):
        return self.state.waitEvent(self.Ready, timeout=timeout)
                            

class HardwareTangoDevice(HardwareTangoObj, HardwareDevice):

    Ready   = PyTango.DevState.ON

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False, ctrl=None):
        self.call__init__(HardwareDevice, pool, info=info, from_str=from_str,
                          parent=parent, ctrl=ctrl)
        self.call__init__(HardwareTangoObj, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)
            

class HardwareObjGroupInfo(HardwareObjInfo):

    InfoReStr = '(?P<type>\S+)\s+list:\s+(?P<obj_name_list>.+)\s+\((?P<obj_name_list2>.+)\)'

    def __init__(self, name=None, dev_name=None, from_str=None):
        self.type = self.obj_name_list = None
        self.call__init__(HardwareObjInfo, name=name, dev_name=dev_name,
                          info_re=self.InfoReStr, from_str=from_str)
        self.obj_name_list = self.obj_name_list.split(', ')


class HardwareObjGroup(HardwareTangoObj):

    info_class = HardwareObjGroupInfo
    
    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False):

        self.obj_list = None
        self.call__init__(HardwareTangoObj, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)

    def createAttr(self):
        self.debug("Adding %s(s) %s", self.type, self.obj_name_list)
        self.obj_list = map(self.getPoolObj, self.obj_name_list)
        
    def deleteAttr(self):
        self.obj_list = None
        
    def getPoolObj(self, name):
        pool = self.getPool()
        return pool.getHardwareObj(self.type, name)

    def getObjList(self):
        return self.obj_list
    

class MotorInfo(HardwareObjInfo):

    InfoReStr = '(?P<type>\S+)(\s+Motor list:\s+(?P<obj_name_list>.+))?'

    ReprStr   = '%(type)s %(name): Ctrl %(ctrl_name)s, Idx %(ctrl_idx)s'
    
    def __init__(self, name=None, dev_name=None, type=None, from_str=None):
        self.type = self.ctrl_name = self.ctrl_idx = None
        self.call__init__(HardwareObjInfo, name, dev_name, from_str=from_str,
                          info_re=self.InfoReStr)
        
        self.name = self.name
        if self.dev_name:
            self.dev_name = self.dev_name
            expchan, ctrl, idx = self.dev_name.split('/')
            self.ctrl_name = ctrl
            self.ctrl_idx = int(idx)
        
        if self.obj_name_list:
            self.obj_name_list = self.obj_name_list.split(', ')

class Motor(HardwareTangoDevice, Moveable):

    Moving  = PyTango.DevState.MOVING

    info_class = MotorInfo
    
    def __init__(self, pool, info=None, from_str=None, parent=None,
                 slope_error=0, no_check=False):
        
        self.got_pos = False
        self.final_pos = None
        self.slope_error = slope_error

        self.sign   = 1

        self.call__init__(HardwareTangoDevice, pool, info=info,
                          from_str=from_str, parent=parent, no_check=no_check)
        self.call__init__(Moveable)
        
    def createAttr(self):
        
        attrs = self.dev.attribute_list_query_ex()
        
        for attr in attrs:
            name_lower = attr.name.lower()
            if name_lower == 'state' or name_lower == 'status':
                continue
            setattr(self,name_lower,Attribute(self,name_lower))
        
        self.position.subscribeEvent(self.positionChanged)
        
    def deleteAttr(self):
        for attr_name in dir(self):
            attr = getattr(self,attr_name)
            if isinstance(attr,Attribute):
                setattr(self,attr_name,None)  
        
    def stateChanged(self, data, state):
        self.debug("state is %s", state)
            
        if state == self.Ready and self.final_pos is not None:
            error = abs(self.getUserPos() - self.final_pos)
            if error > self.slope_error:
                self.warning("Warning: %s did not reach final position",
                             self.name)
            self.final_pos = None

    def positionChanged(self, data, pos):
        """To be called when a change event occurs on the position"""
        msg = 'moved to'
        if not self.got_pos:
            msg = 'is at'
            self.got_pos = True
        self.debug("%s %s", msg, pos)

    def offsetChanged(self, data, offset):
        """To be called when a change event occurs on the offset"""
        self.debug("offset is now %s", offset)

    def getControllerName(self):
        return self.info.ctrl_name

    def getPosObj(self):
        return self.position
    
    def getDialPosObj(self):
        """Returns the dial position 'Attribute' object. If the object does not 
        contain a dial position (PseudoMotor) it returns the user position 
        object"""
        try:
            ret = getattr(self,'dialposition')
            # this obj inherits the "__getattr__ of PyTango.DeviceProxy (see 
            # TangoDevice Class for details). Therefore the above call will 
            # return a ClientRequestHandler if the atrib does not exist
            if isinstance(ret, Attribute):
                return ret
            else:
                return self.getPosObj()
        except:
            return self.getPosObj()
    
    def getOffsetObj(self):
        """Returns the offset 'Attribute' object or None if it does not exist""" 
        ret = getattr(self,'offset',None)
        if isinstance(ret, Attribute):
            return ret
        return None
    
    def getUserPos(self):
        return self.position.read()

    def getDialPos(self):
        return self.getDialPosObj().read()

    def getOffset(self):
        offset = self.getOffsetObj()
        if offset:
            return offset.read()
        else:
            return 0.0

    def user2Dial(self, user):
        try:
            u = float(user)
            return (u - self.getOffset()) / self.sign
        except:
            return user

    def dial2User(self, dial):
        try:
            d = float(dial)
            return self.sign * d + self.getOffset()
        except:
            return dial

    def getUserLimits(self):
        return self.getPosObj().getLimits()
    
    def setUserLimits(self,low,high):
        self.getPosObj().setLimits(low,high)

    def setDialLimits(self,low,high):
        self.getDialPosObj().setLimits(dial2User(low),dial2User(high))
    
    def getDialLimits(self):
        return [self.user2Dial(x) for x in self.getUserLimits()]
    
    def definePosition(self, new_pos):
        self.dev.DefinePosition(new_pos)
    
    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    # Moveable interface
    #
        
    def startMove(self, new_pos, timeout=None):
        new_pos = new_pos[0]
        self.state.lock()
        try:
            try:
                self.getPosObj().write(new_pos)
                self.final_pos = new_pos
                self.state.waitEvent(self.Moving, timeout=timeout)
            except PyTango.DevFailed, err_traceback:
                for err in err_traceback:
                    if err['reason'] == 'API_AttrNotAllowed':
                        raise RuntimeError, '%s is already moving' % self
                    else:
                        raise
        finally:
            self.state.unlock()
        
    def waitMove(self, timeout=None):
        return self.state.waitEvent(self.Moving, equal=False, timeout=timeout)

    def move(self, new_pos, timeout=None):
        self.startMove(new_pos, timeout=timeout)
        self.waitMove(timeout=timeout)

    def abort(self, wait_ready=True, timeout=None):
        self.state.lock()
        try:
            if self.state.read() == self.Moving:
                self.debug("Aborting")
                self.checkDev().Abort()
            HardwareDevice.abort(self, wait_ready, timeout)
        finally:
            self.state.unlock()

    def readPosition(self):
        return [self.getUserPos(self)]
    
    def getMoveableSource(self):
        return self.getPool()

    def getSize(self):
        return 1
    
    def getIndex(self, name):
        if name.lower() == self.name.lower():
            return 0
        return -1
    #
    # End of Moveable interface
    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    
    def getAttrObj(self,attr_name):
        """Returns 'Attribute' object or None if it does not exist""" 
        ret = getattr(self,attr_name.lower(),None)
        if isinstance(ret, Attribute):
            return ret
        return None        

    def getAttrValue(self,attr_name):
        """Returns 'Attribute' object value or None if it does not exist"""
        ret = self.getAttrObj(attr_name)
        if ret is None:
            return None
        return ret.read() 

    def getAttrValues(self,attr_name_list):
        ret = {}
        for attr_value in self.dev.read_attributes(attr_name_list):
            ret[attr_value.name] = attr_value.value
        return ret
    
    #
    # String like motor API
    #

    def getStrUserLimits(self):
        return [self.getPosObj().getMinStrValue(),self.getPosObj().getMaxStrValue()]

    def getStrDialLimits(self):
        return [self.getDialPosObj().getStrForValue(x) for x in self.getDialLimits()]

    def getStrUserPos(self):
        return self.getPosObj().getStrValue()

    def getStrDialPos(self):
        return self.getDialPosObj().getStrForValue(self.getDialPos())

class MotorGroup(HardwareObjGroup, Moveable):

    Moving  = PyTango.DevState.MOVING

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False):

        self.got_pos = False
        self.final_pos = None
        self.call__init__(HardwareObjGroup, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)
        self.call__init__(Moveable)
        self.debug("MG %s CREATED" % self.name)

    def getPosObj(self):
        return self.position

    def getUserPos(self):
        return self.position.read()
    
    def createAttr(self):
        HardwareObjGroup.createAttr(self)
        attrs = self.dev.attribute_list_query_ex()
        
        for attr in attrs:
            name_lower = attr.name.lower()
            if name_lower == 'state' or name_lower == 'status':
                continue
            setattr(self,name_lower,Attribute(self,name_lower))
        
        self.position.subscribeEvent(self.positionChanged)
        
    def deleteAttr(self):
        HardwareObjGroup.deleteAttr(self)
        for attr_name in dir(self):
            attr = getattr(self,attr_name)
            if isinstance(attr,Attribute):
                setattr(self,attr_name,None)
                
    def positionChanged(self, data, pos):
        """To be called when a change event occurs on the position"""
        msg = 'moved to'
        if not self.got_pos:
            msg = 'is at'
            self.got_pos = True
        self.debug("%s %s", msg, pos)                       
    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    # Moveable interface
    #
        
    def startMove(self, new_pos, timeout=None):
        self.state.lock()
        try:
            try:
                self.getPosObj().write(new_pos)
                self.final_pos = new_pos
                self.state.waitEvent(self.Moving, timeout=timeout)
            except PyTango.DevFailed, err_traceback:
                for err in err_traceback:
                    if err['reason'] == 'API_AttrNotAllowed':
                        raise RuntimeError, '%s is already moving' % self
                    else:
                        raise
        finally:
            self.state.unlock()
        
    def waitMove(self, timeout=None):
        return self.state.waitEvent(self.Moving, equal=False, timeout=timeout)

    def move(self, new_pos, timeout=None):
        self.startMove(new_pos, timeout=timeout)
        self.waitMove(timeout=timeout)

    def abort(self, wait_ready=True, timeout=None):
        self.state.lock()
        try:
            if self.state.read() == self.Moving:
                self.debug("Aborting")
                self.checkDev().Abort()
            HardwareDevice.abort(self, wait_ready, timeout)
        finally:
            self.state.unlock()

    def getMoveableSource(self):
        return self.getPool()

    def getSize(self):
        return len(self.obj_name_list)
    
    def getIndex(self, name):
        name = name.lower()
        for i,n in enumerate(self.obj_name_list):
            if n.lower() == name:
                return i
        return -1    
    #
    # End of Moveable interface
    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    
class CountingObj(HardwareTangoObj):

    Counting = PyTango.DevState.MOVING

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False):

        self.call__init__(HardwareTangoObj, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)

    def startCount(self, timeout=None):
        self.state.lock()
        try:
            try:
                self.checkDev().Start()
                self.state.waitEvent(self.Counting, timeout=timeout)
            except PyTango.DevFailed, err_traceback:
                raise
        finally:
            self.state.unlock()


    def waitCount(self, timeout=None):
        self.state.waitEvent(self.Counting, equal=False, timeout=timeout)

    def count(self, timeout=None):
        self.startCount(timeout=timeout)
        self.waitCount(timeout=timeout)
        
    def stopCount(self, timeout=None):
        self.state.lock()
        try:
            if self.state.read() == self.Counting:
                self.debug("Stopping")
                self.checkDev().Stop()
                self.waitCount(timeout=timeout)
        finally:
            self.state.unlock()
            
    def abort(self, wait_ready=True, timeout=None):
        self.stopCount(timeout=timeout)
        HardwareDevice.abort(self, wait_ready, timeout)

    
class ExpChannelInfo(HardwareObjInfo):

    InfoReStr = '(?P<type>\S+)'

    ReprStr   = '%(type)s %(name): Ctrl %(ctrl_name)s, Idx %(ctrl_idx)s'
    
    def __init__(self, name=None, dev_name=None, type=None, from_str=None):
        self.type = self.ctrl_name = self.ctrl_idx = None
        self.call__init__(HardwareObjInfo, name, dev_name, from_str=from_str,
                          info_re=self.InfoReStr)
        
        self.name = self.name
        if self.dev_name:
            self.dev_name = self.dev_name
            expchan, ctrl, idx = self.dev_name.split('/')
            self.ctrl_name = ctrl
            self.ctrl_idx = int(idx)
            
         
class ExpChannelBase(CountingObj, HardwareDevice):

    Counting = PyTango.DevState.MOVING

    info_class = ExpChannelInfo

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False, ctrl=None):
        self.call__init__(CountingObj, pool, info=info,
                          from_str=from_str, parent=parent, no_check=no_check)
        self.call__init__(HardwareDevice, pool, info=info,
                          from_str=from_str, parent=parent, ctrl=ctrl)

    def createAttr(self):
        self.value = Attribute(self, 'Value')
#        self.value.subscribeEvent(self.valueChanged)

    def deleteAttr(self):
        self.value = None

    def valueChanged(self, data, value):
        self.debug("Count = %s", value)

    def getCount(self):
        return self.value.read()


class CounterTimer(ExpChannelBase):
    pass


class ZeroDExpChannel(ExpChannelBase):
    pass


class PseudoCounterInfo(HardwareObjInfo):

    InfoReStr = '(?P<type>\S+)'

    ReprStr   = '%(type)s %(name): Ctrl %(ctrl_name)s, Idx %(ctrl_idx)s'
    
    def __init__(self, name=None, dev_name=None, type=None, from_str=None):
        self.type = self.ctrl_name = self.ctrl_idx = None
        self.call__init__(HardwareObjInfo, name, dev_name, from_str=from_str,
                          info_re=self.InfoReStr)
        
        self.name = self.name
        if self.dev_name:
            self.dev_name = self.dev_name
            expchan, ctrl, idx = self.dev_name.split('/')
            self.ctrl_name = ctrl
            self.ctrl_idx = int(idx)

            
class PseudoCounter(ExpChannelBase):
    
    info_class = PseudoCounterInfo


class ExpChannelFactory:

    info_class = ExpChannelInfo

    def __call__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False):
        aux_info = info or self.info_class(from_str=from_str)
        classes = globals()
        klass = classes[aux_info.type]
        return klass(pool, info=info, from_str=from_str, parent=parent,
                     no_check=no_check)


ExpChannel = ExpChannelFactory()


class MeasurementGroup(HardwareObjGroup, CountingObj):

    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False, ctrl=None):

        self.call__init__(HardwareObjGroup, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)
        self.call__init__(CountingObj, pool, info=info, from_str=from_str,
                          parent=parent, no_check=no_check)

    def createAttr(self):

        HardwareObjGroup.createAttr(self)
        
    
        self.integ_time    = Attribute(self, 'Integration_time')
        self.integ_count   = Attribute(self, 'Integration_count')
        self.timer_name    = Attribute(self, 'Timer')
        self.timer_name.subscribeEvent(self.timerChanged)
        self.channel_names = ListAttribute(self, 'Channels')
        self.channel_names.subscribeEvent(self.channelsChanged)
        
    def deleteAttr(self):
        self.integ_time    = None
        self.integ_count   = None
        self.timer_name    = None
        self.channel_names = None
        HardwareObjGroup.deleteAttr(self)

    def getChanList(self):
        return self.getObjList()

    def getTimer(self):
        return self.timer
    
    def getChannelList(self):
        return self.channel_list
    
    def getChannelAttrList(self):
        return self.ch_attr_list
    
    def timerChanged(self, data, timer_name):
        if timer_name == None or timer_name == "Not Initialized":
            self.timer = None
        else:
            self.timer = [x for x in self.getChanList()
                          if x.name == timer_name][0]
        
        self.debug("Timer is %s", self.timer)
        
    def channelsChanged(self, data, channels_data):
        all_channels, old_channels, new_channels = channels_data
        self.channel_list = [x for x in self.getChanList()
                               if x.getName() in all_channels]
        self.ch_attr_list = [Attribute(self,x.getName()+"_value") for x in self.getChanList()
                               if x.getName() in all_channels]
        self.debug("Channels are %s", self.channel_list)
        
    def setTimer(self, timer_name):
        timer_name_l = timer_name.lower()
        for x in self.getChanList():
            if x.getName().lower() == timer_name_l:
                self.timer_name.write(timer_name)
                return
        raise "Measurement group does not contain a channel named %s" % timer_name
        
    def setIntegrationTime(self, ctime):
        if self.integ_time.read() != ctime:
            self.integ_time.write(ctime)


class ComChannelInfo(HardwareObjInfo):

    def __init__(self, name=None, dev_name=None, from_str=None):
        self.ctrl_name = self.ctrl_idx = None
        HardwareObjInfo.__init__(self, name, dev_name, from_str=from_str)
        if self.dev_name:
            self.dev_name = self.dev_name
            mot, ctrl, idx = self.dev_name.split('/')
            self.ctrl_name = ctrl
            self.ctrl_idx = int(idx)


class ComChannel(HardwareTangoDevice):
    info_class = ComChannelInfo
    
    def __init__(self, pool, info=None, from_str=None, parent=None,
                 no_check=False, ctrl=None):
        self.call__init__(HardwareTangoDevice, pool, info=info,
                          from_str=from_str, parent=parent, no_check=no_check)


HardwareObjNames   = ['Controller', 'ComChannel', 'Motor', 
                      'ExpChannel', 'MotorGroup', 'MeasurementGroup']
HardwareObjClasses = [globals()[x] for x in HardwareObjNames]


class Pool(TangoDevice, MoveableSource):
    """ Class encapsulating device Pool functionality.
    
        Acessible members:
            ctrl_class_list - ControllerClassList object
            obj_dict - dict<string, HardwareObjList>, were key is the object 
                       name which is the attribute name without the suffix 'List'
    """ 
    
    def __init__(self, dev_name, parent=None, no_check=False):
        """Pool(string dev_name, Logger parent = None, bool no_check = False) -> Pool
        
           Constructor.
        
           Params:
               dev_name: string with complete tango device name of the pool.
               parent: parent logger if any. Defaults to 'None'.
               no_check: if set to True it avoids checking Pool attributes. Defaults to False.  
        """
        
        self.call__init__(TangoDevice, dev_name, no_check=True,
                          parent=parent)
        self.call__init__(MoveableSource)
        
        if no_check:
            return

        self.ctrl_class_list = ControllerClassList(self)
        self.obj_dict = {}
        for name, obj_class in zip(HardwareObjNames, HardwareObjClasses):
            attr_name = name + 'List'
            attr = self.createAttr(attr_name, ListAttribute)
            obj_list = HardwareObjList(self, name, obj_class, attr)
            self.obj_dict[name] = obj_list
            obj_list.subscribeEvent(self.objListChanged, name)
        self.try_Online()
        
    def go_Online(self):
        """Connects all attributes of the Pool"""
        
        self.debug("Going ONLINE")
        self.connect()
        self.debug("Connected")
        
        for name,obj_list in self.obj_dict.items():
            obj_list.connect()
        
    def go_Offline(self):
        """Disconnects all attributes of the Pool"""
        
        self.debug("Going OFFLINE")
        for name,obj_list in self.obj_dict.items():
            obj_list.disconnect()
            
        self.disconnect()
        self.debug("Disconnected")
        
    def try_Online(self):
        """Checks the working state of the Pool. Atempts to connect if it is ONLINE""" 

        self.debug("Trying to go ONLINE")
        self.__updateWorkingState()
        self.debug("Working state: %s" % WorkingState.whatis(self.working_state))
        
        if self.working_state == WorkingState.ONLINE:
            self.go_Online()
        
    def __del__(self):
        self.debug("deleting")
    
    def __updateWorkingState(self):
        try:
            res = self.checkDev().ping()
            self.working_state = WorkingState.ONLINE
        except PyTango.ConnectionFailed, cf:
            if self.working_state is WorkingState.ONLINE:
                self.working_state = WorkingState.CRASHED
        except PyTango.CommunicationFailed, cf:
            if not self.working_state is WorkingState.OFFLINE:
                self.working_state = WorkingState.SHUTDOWN
        except PyTango.DevFailed,df:
            for error in df:
                if error['reason'] == 'API_CantConnectToDevice':
                    if self.working_state is WorkingState.ONLINE:
                        self.working_state = WorkingState.CRASHED
                    break
            else:
                raise

    def getName(self):
        """getName() -> string
           Returns a string with the pool complete tango device name.
           Return:
             a string with the pool complete tango device name."""
        return self.dev_name
    
    def createAttr(self, name, attr_class=Attribute, **pars):
        """createAttr(string name, class attr_class, **pars) -> Attribute
           
           Creates an attribute object.
           
           Params:
               name: The attribute name. Must be a valid attribute inside the pool.
               attr_class: the class object to be used. Defaults to 'Attribute'
               **pars: parameters to pass to the Attribute constructor
           Return:
               An Attribute object representing the attribute
        """
        return apply(attr_class, (self, name), pars)

    def getCtrlClassListObj(self):
        """getCtrlClassListObj() -> ControllerClassList
           Returns the ControllerClassList object for this pool.
           Return:
               the ControllerClassList object for this pool.
        """
        return self.ctrl_class_list

    def getCtrlClassByName(self,class_name,cache=True):
        """getCtrlClassByName(string class_name, boolean cache = True) -> ControllerClass
        
           Returns the controller class corresponding to the given class name or None
           if not found.
           
           Params:
               class_name: the class name
               cache: If True use the cache value. If False ask the Pool tango device for an updated
                      list of controller classes. Defaults to True
           Return:
               A ControllerClass object or None.
        """
        res = self.ctrl_class_list.getControllerClass(class_name,cache)
        return res
    
    def getHardwareListObj(self, type):
        if type == 'ControllerClass':
            return self.getCtrlClassListObj()
        return self.obj_dict[type]

    def getHardwareObj(self, name):
        for obj_list in self.obj_dict:
            obj = obj_list.getObj(name)
            if obj:
                return obj
        return self.getHardwareObj('ControllerClass',name)

    def getHardwareObj(self, type, name):
        obj_list = self.getHardwareListObj(type)
        return obj_list.getObj(name)
    
    def objListChanged(self, class_name, event_data):
        self.debug("List of %s changed", class_name)
        all_names, rm_names, add_names = event_data
        ctrl_list = self.getHardwareListObj('Controller')

        obj_type = class_name
        obj_list = self.getHardwareListObj(obj_type)
        
        for name in rm_names:
            obj = obj_list.getObj(name)
            self.debug("Deleting %s", obj)
            if isinstance(obj, HardwareDevice):
                ctrl = ctrl_list.getObj(obj.ctrl_name)
                ctrl.delDevice(obj)
        for name in add_names:
            obj = obj_list.getObj(name)
            self.debug("Adding %s", obj)
            if isinstance(obj, HardwareDevice):
                ctrl = ctrl_list.getObj(obj.ctrl_name)
                ctrl.addDevice(obj)
        if not all_names:
            self.debug("No %s in the pool", class_name)
    
    def stateChanged(self, data, tango_state):
        self.debug("Tango state changed from %s to %s" % (self.tango_state, tango_state))
        
        # if pool is shutting down...
        if self.working_state == WorkingState.ONLINE:
            if tango_state is None or tango_state == PyTango.DevState.UNKNOWN:
                self.go_Offline()
        
        TangoDevice.stateChanged(self, data, tango_state)
    
    def createMotorGroup(self, name, elements):
        params = [name,] + [str(x) for x in elements]
        self.debug('trying to create motor group: %s',str(params))
        self.dev.createMotorGroup(params)
        mg_list = self.getHardwareListObj('MotorGroup')
        self.debug("--> Waitting for MG to be created...")
        mg_list.waitEvent(any=True)
        self.debug("<-- Received event in MotorGroupList")
    
    def createMeasurementGroup(self, name, elements):
        params = [name,] + [str(x) for x in elements]
        self.debug('trying to create measurement group: ',params)
        self.dev.createMeasurementGroup(params)

    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    # MoveableSource interface
    #    
    
    def getMoveable(self, names):
        """ getMoveable(list<string> names) -> Moveable 

        Returns a moveable object that handles all the moveable items given in 
        names."""        
        # if simple motor just return it (if the pool has it)
        if len(names) == 1:
            name = names[0]
            return self.getHardwareObj('Motor',name) or getHardwareObj('MotorGroup',name)
        
        # find a motor group that contains elements
        moveable = self.__findMotGrpWithElems(names)
        
        # if none exists create one
        if moveable is None:
            name = "_mg_macserv_" + str(os.getpid()) + "_" + str(thread.get_ident()) 
            self.createMotorGroup(name, names)
            moveable = self.getHardwareObj('MotorGroup', name)
        return moveable
             
    def __findMotGrpWithElems(self, names):
        mg_list = self.getHardwareListObj('MotorGroup')
        len_names = len(names)
        for mg in mg_list.obj_dict.values():
            if len_names == len(mg.obj_name_list):
                found = True
                for name in names:
                    if not name in mg.obj_name_list:
                        found = False
                        break;
                if found:
                    return mg
        return None
    #
    # End of MoveableSource interface
    #-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    
def usage():
    prog_name = os.path.basename(sys.argv[0])
    print 'Usage: %s <pool_dev_name> [motor_name] [slope_error]' % prog_name
    sys.exit(1)
    

def main(argv):
    try:
        pool_dev_name = argv[1]
    except:
        usage()

    try:
        motor_name = argv[2]
    except:
        motor_name = None
        
    try:
        slope_error = float(argv[3])
    except:
        slope_error = 0

    p = Pool(pool_dev_name)
    for c in p.getCtrlClassListObj().read():
        print "Type: %s, Class: %s,\n  File: %s" % \
              (c['Type'], c['Class'], c['File'])

    if motor_name:
        m1 = p.getHardwareObj('Motor', motor_name)
        m1.slope_error = slope_error

        try:
            for pos in range(100, 300, 100):
                print "Moving %s to %s" % (m1, pos)
                m1.move(pos)
        except KeyboardInterrupt:
            print "Ctrl-C received: Aborting"
            m1.abort()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass

    
if __name__ == '__main__':
    main(sys.argv)
    
