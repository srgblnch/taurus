from Macro import *

# Basic types

class Integer(ParamType):
    type_class = int

class Float(ParamType):
    type_class = float

class String(ParamType):
    type_class = str

class User(ParamType):
    type_class = str

class Filename(ParamType):
    type_class = str

class File(ParamType):
    
    type_class = str
    
    def __init__(self, name, manager):
         
        ParamType.__init__(self, name, manager)
        self.filename = None
        #self.data is supposed to be a array.array object
        self.data = None
    
    def set(self,filename,data):
        self.filename = filename
        self.data = data
        
class Macro(ParamType):
    type_class = str
    #capabilities = [ParamType.ItemList]
    
    #def __init__(self, name, manager):
    #    ParamType.__init__(self, name, manager)
    
    #def getItemList(self):
    #    return self.getManager().macro_dict.keys()

class MacroLib(ParamType):
    type_class = str
    
# Hardware types

class MotorParam(AttrParamType):
    """ Class designed to represent a motor parameter name. Usual values
    are acceleration,deceleration,velocity,backlash,steps_per_unit,etc"""
    
    def __init__(self, name, manager):
        AttrParamType.__init__(self, name, manager)
        self.attr_item_list = ["Acceleration","Backlash","Base_rate","Step_per_unit",
                "Deceleration","Velocity","Offset"]
        self.non_attr_item_list = ["Controller"]
    
    def getItemList(self):
        return self.non_attr_item_list + self.attr_item_list
    
    def getAttrItemList(self):
        return self.attr_item_list
    
    def getNonAttrItemList(self):
        return self.non_attr_item_list

class Motor(PoolObjParamType):
    """ Class designed to represend a generic movement parameter. Could in fact
    be a Motor, PseudoMotor or even a MotorGroup object 
    """
    pass

class MotorGroup(PoolObjParamType):
    pass

class ExpChannel(PoolObjParamType):
    """ Class designed to represend a generic experiment channel parameter. 
    Could in fact be a Counter/Timer, 0D, 1D or 2D channel or a PseudoCounter 
    """
    pass

class MeasurementGroup(PoolObjParamType):
    """ Class designed to represend a generic experiment."""    
    pass

class ComChannel(PoolObjParamType):
    """ Class designed to represend a generic communication channel."""
    pass

class Controller(PoolObjParamType):
    """ Class designed to represent a generic controller."""
    pass

class ControllerClass(PoolObjParamType):
    capabilities = [ParamType.ItemList]
    
    def __init__(self, name, manager):
        PoolObjParamType.__init__(self, name, manager)

    def getPoolObjList(self, pool):
        obj_list = pool.getCtrlClassListObj()
        return obj_list.read()