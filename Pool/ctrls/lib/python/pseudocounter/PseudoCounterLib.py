""" The standard pseudo counter controller library for the device pool """ 

from pool import PseudoCounterController
from pool import PoolUtil

try:
    import scipy
    __SCIPY_AVAILABLE__ = True
except:
    __SCIPY_AVAILABLE__ = False
    
class PseudoCounterProxy(PseudoCounterController):
    """ A pseudo counter controller whose pseudo counter value is the 
        physical counter/timer it is connected to"""
    class_prop = {}
    counter_roles = ("Counter",)
        
    def calc(self,counter_values):
        return counter_values[0]
    

class Current(PseudoCounterController):
    """ A simple pseudo counter which receives to counter values (I and I0) 
        and returns I/I0"""

    counter_roles = ("I","I0")

    def calc(self,counter_values):
        if counter_values[1] is 0:
            return counter_values[0]
        return float(counter_values[0]/counter_values[1])

    
class PseudoAdlCounterController(PseudoCounterController):
    """ A simple pseudo counter that returns the last value read from channel 0 
        of the Adlink 2005 card."""

    def __init__(self):
        PseudoCounterController.__init__(self)
        util = PoolUtil()
        self.channel = util.get_device(self.inst_name,"WS/Adlink2005/input1")
        self.channel.command_inout("Start")

    def calc(self,counter_values):
        return self.channel.read_attribute("C00_LastValue").value

class PseudoEMCounterController(PseudoCounterController):
    """ A simple pseudo counter which returns the position of the electrometer 
        range and frequency \"motors\"."""

    class_prop = {'em_motor':{'Description':'The motor device the counter uses.'
                             ,'Type' : 'PyTango.DevString'} }

    def calc(self,counter_values):
        util = PoolUtil()
        motor = util.get_motor(self.inst_name,self.em_motor)
        return motor.read_attribute("Position").value
    

class PseudoCoMotorWrapper(PseudoCounterController):
    """ A pseudo counter controller whose pseudo counter value is the motor 
        position to which it is connected to"""
    
    class_prop = { 'Motor' : { 'Description' : 'the motor tango device name (or alias)','Type' : 'PyTango.DevString'} }
    
    def calc(self,counter_values):
        mot = PoolUtil().get_motor(self.inst_name,self.Motor)
        return mot.read_attribute("position").value
    

class PseudoFunctionCounterController(PseudoCounterController):
    """ A simple pseudo counter that returns a function of the last values of a counter using
     a dummy motor. It resets the data list when the dummy motor is at position zero.
     Typical for being used within a mesh."""

     # IF YOU NEED TO CREATE THE CONTROLLER WITH JIVE:
     # TEST THE POOL DEVICE AND ISSUE THE CreateController command with:
     # PseudoCounter,file_name,class_name,ctrl_name,counter_to_read,pseudo_counter_name,dummy_motor,motor_value,function,function_value

    MEAN = "MEAN"
    STD = "STD"
    
    class_prop = {'dummy_motor':{'Description':'The dummy motor device the reset counter values.'
                              ,'Type' : 'PyTango.DevString'},
                  'function':{'Description':'The function to use (MEAN|STD).'
                              ,'Type' : 'PyTango.DevString'}
                  }
    counter_roles = ("Counter",)

    def __init__(self,inst,props):
        PseudoCounterController.__init__(self,inst,props)
        
        if not __SCIPY_AVAILABLE__:
            raise RuntimeException("scipy python package not availabe.")
        
        util = PoolUtil()
        self.dummy_motor_dev = util.get_device(self.inst_name,self.dummy_motor)
        self.dict_values = {}


    def calc(self,counter_values):
        position =  self.dummy_motor_dev.read_attribute("Position").value
        if position == 0:
            self.dict_values = {}

        self.dict_values[position] = counter_values[0]

        if self.function.upper() == self.MEAN:
            return scipy.mean(self.dict_values.values())
        elif self.function.upper() == self.STD:
            return scipy.std(self.dict_values.values())
        else:
            return 0
