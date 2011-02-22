import PyTango
import pool
from pool import MotorController
import array
import sys


class NSC200:
    MOTOR_MOVING = 80
    MOTOR_ON = 81
    MOTOR_OFF = 64
    ERRORS = {
         0: 'No errors',
         1: 'Driver fault (open load)',
         2: 'Driver fault (thermanl shut down)',
         3: 'Driver fault (short)',
         6: 'Invalid command',
         7: 'Parameter out of range',
         8: 'No motor connected',
         10: 'Brown-out',
         38: 'Command parameter missing', 
         24: 'Positive hardware limit detected',
         25: 'Negative hardware limit detected',
         26: 'Positive sofware limit detected',
         27: 'Negative software limit detected',
         210: 'Max velocity exceeded',
         211: 'Max acceleration exceeded',
         213: 'Motor not enabled',
         214: 'Switch to invalid axis',
         220: 'Homing aborted',
         226: 'Parameter change not allowed during motion'
    }
    
    @staticmethod
    def getErrorMessage(errorcode):
        if errorcode in NSC200.ERRORS:
            return NSC200.ERRORS[errorcode]
        else:
            return "Unknown error"
    
    @staticmethod
    def getLimitPositive(register):
        val = register >> 9
        val = val & 1
        return val
    
    @staticmethod
    def getLimitNegative(register):
        val = register >> 8
        val = val & 1
        return val
    
class CB:    
    def push_event(self,event):
        print "[NSC200Controller]---------------------------------------------------------------------- Received event"

class NSC200Controller(MotorController):
    """This class is the Tango Sardana motor controller for the 
       Newport NewStep handheld motion controller NSC200"""

    ctrl_features = ['Home_speed','Home_acceleration']

    class_prop = {'SerialCh':{'Type':'PyTango.DevString','Description':'Communication channel name for the serial line'},
                  'SwitchBox':{'Type':'PyTango.DevBoolean','Description':'Using SwitchBox','DefaultValue':False},
                  'ControllerNumber':{'Type':'PyTango.DevLong','Description':'ControllerNumber','DefaultValue':1}}
	

    gender = "Motor"
    model = "NSC200"
    organization = "Newport"
    image = "NSC200.png"
    logo = "Newport.png"

    MaxDevice = 1
    
    def __init__(self,inst,props):
        
        MotorController.__init__(self,inst,props)
            
        self.serial = None
        self.serial_state_event_id = -1

        if self.SwitchBox:
            self.MaxDevice = 8

    def getComChannel(self):
        if self.serial is None:
            self.serial = pool.PoolUtil().get_com_channel(self.inst_name ,self.SerialCh)
            #self.serial = PyTango.DeviceProxy(self.SerialCh)
            self.cb = CB()
            self.serial_state_event_id = self.serial.subscribe_event("State",PyTango.EventType.CHANGE,self.cb,[])
            self.open()
        return self.serial

    def AddDevice(self,axis):
        #Check Axis
        print "[NSC200Controller]",self.inst_name,": In AddDevice method for axis",axis
        if axis > 1 and not self.SwitchBox:
            PyTango.Except.throw_exception("NSC200_AddDevice", "Without using a Switchbox only axis 1 is allowed", "AddDevice()")
        
        if self.SwitchBox:
            self._setCommand("MX", axis)

    def DeleteDevice(self,axis):
        pass
        
    
    def StateOne(self,axis):
        print "[NSC200Controller]",self.inst_name,": In StateOne method for axis",axis

        #try:
        if self.SwitchBox:
            self._setCommand("MX", axis)
            
        status = int(self._queryCommand("TS"))
        
        if status == NSC200.MOTOR_OFF:
            status = PyTango.DevState.OFF
        elif status == NSC200.MOTOR_ON:
            status = PyTango.DevState.ON
        elif status == NSC200.MOTOR_MOVING:
            status = PyTango.DevState.MOVING
        register = int(self._queryCommand("PH"))
        lower = NSC200.getLimitNegative(register) 
        upper = NSC200.getLimitPositive(register) 

        switchstate = 0
        if int(lower) == 1 and int(upper) == 1:
            switchstate = 6
        elif int(lower) == 1:
            switchstate = 4
        elif int(upper) == 1:
            switchstate = 2
        state = (int(status), switchstate)
        return state
        
        #except:
        #    print "Unexpected error:", sys.exc_info()[0]
        #    print "NSC200Controller/. Error getting State"
        #    PyTango.Except.throw_exception("NSC200Controller/_StateOne", "Error reading state, axis not available", "StateOne()")
            

    def PreReadAll(self):
        print "[NSC200Controller]",self.inst_name,": In PreReadAll method"

    def PreReadOne(self,axis):
        print "[NSC200Controller]",self.inst_name,": In PreReadOne method for axis",axis

    def ReadAll(self):
        print "[NSC200Controller]",self.inst_name,": In ReadAll method"

    def ReadOne(self,axis):
        print "[NSC200Controller]",self.inst_name,": In ReadOne method for axis",axis
        try:
            if self.SwitchBox:
                self._setCommand("MX", axis)
            return float(self._queryCommand("TP"))
        except:
            PyTango.Except.throw_exception("NSC200Controller_ReadOne", "Error reading position, axis not available", "ReadOne()")

    def PreStartAll(self):
        print "[NSC200Controller]",self.inst_name,": In PreStartAll method"

    def PreStartOne(self,axis,pos):
        print "[NSC200Controller]",self.inst_name,": In PreStartOne method for axis",axis," with pos",pos
        return True

    def StartOne(self,axis,pos):
        print "[NSC200Controller]",self.inst_name,": In StartOne method for axis",axis," with pos",pos
        #try:
        if self.SwitchBox:
            self._setCommand("MX", axis)
        print "[NSC200Controller] StartOne - Getting status" 
        status = int(self._queryCommand("TS"))
        print "[NSC200Controller] StartOne - DONE Getting status"
        if status == NSC200.MOTOR_OFF:
            print "[NSC200Controller] StartOne - motor is off - make it on"
            self._setCommand("MO","")
            print "[NSC200Controller] StartOne - motor is off - DONE making it on"
        print "[NSC200Controller] StartOne - sending position"
        self._setCommand("PA", pos)
        print "[NSC200Controller] StartOne - DONE sending position"
        #except:
        #    print "NSC200Controller Error StartOne"
        #    print "Unexpected error:", sys.exc_info()[0]
        #    PyTango.Except.throw_exception("NSC200Controller_StartOne", "Error setting position, axis not available", "StartOne()")
            
    def StartAll(self):
        print "[NSC200Controller]",self.inst_name,": In StartAll method"

    def SetPar(self,axis,name,value):
        print "[NSC200Controller]",self.inst_name,": In SetPar method for axis",axis," name=",name," value=",value
        #try:
        name = name.lower()
        if self.SwitchBox:
            self._setCommand("MX", axis)
        if name == "velocity":
            self._setCommand("VA", value)
        elif name == "base_rate":
            self._setCommand("VA", value)
        elif name == "acceleration" or name == "deceleration":
            self._setCommand("AC", value)
        elif name == "backlash":
            self._setCommand("BA", value)
        #except:
        #    print "Unexpected error:", sys.exc_info()[0]
        #    PyTango.Except.throw_exception("NSC200Controller_SetPar", "Error setting %s" % name , "SetPar()")
        

    def GetPar(self,axis,name):
        print "[NSC200Controller]",self.inst_name,": In GetPar method for axis",axis," name=",name
        #try:
        name = name.lower()
        if self.SwitchBox:
            self._setCommand("MX", axis)
        if name == "velocity":
            return float(self._queryCommand("VA").replace(',','.'))
        elif name == "base_rate":
            return float(self._queryCommand("VA").replace(',','.'))
        elif name == "acceleration" or name == "deceleration":
            return float(self._queryCommand("AC").replace(',','.'))
        elif name == "backlash":
            return float(self._queryCommand("BA").replace(',','.'))
        return 0
        #except:
        #    print "Unexpected error:", sys.exc_info()[0]
        #    PyTango.Except.throw_exception("NSC200Controller_GetPar", "Error getting " + name , "GetPar()")

    def GetExtraAttributePar(self,axis,name):
        pass
    def SetExtraAttributePar(self,axis,name,value):
        pass

    def AbortOne(self,axis):
        print "[NSC200Controller]",self.inst_name,": In Abort for axis",axis
        if self.SwitchBox:
            self._setCommand("MX", axis)
        self._setCommand("ST", "")

    def StopOne(self,axis):
        print "[NSC200Controller]",self.inst_name,": In StopOne for axis",axis
        if self.SwitchBox:
            self._setCommand("MX", axis)
        self._setCommand("ST", "")

    def DefinePosition(self, axis, position):
        print "[NSC200Controller]",self.inst_name,": In DefinePosition for axis",axis," pos=",position
        self.iPAP.setPosition(axis, int(position))

    def __del__(self):
        print "[NSC200Controller]",self.inst_name,": Exiting...",
        if self.serial_state_event_id != -1:
            self.getComChannel().unsubscribe_event(self.serial_state_event_id)
        print "[DONE]"
    
    def _queryCommand(self, cmd):
        cmd = str(self.ControllerNumber) + cmd + "?"
        res = self._sendCommand(cmd)

#       self._checkError()
        if res.find(cmd) != -1:
            res = res.lstrip()
            res = res.lstrip(cmd)
            return  res
        else:
           PyTango.Except.throw_exception("NSC200_queryCommand", "Error getting command response, " + cmd, "_queryCommand()")
        
    
    def _setCommand(self, cmd, val):
        cmd = str(self.ControllerNumber) + cmd + str(val).replace(".",",")
        self._sendSetCommand(cmd)
        self._checkError()
   
    def _checkError(self):
        cmd = str(self.ControllerNumber) + "TE?"
        res = self._sendCommand(cmd)
        if res.find(cmd) != -1:
            res = res.lstrip()
            res = res.lstrip(cmd)
            if int(res) > 0:
                PyTango.Except.throw_exception("NSC200",NSC200.getErrorMessage(int(res)) , "_command()")
            

    def _sendCommand(self, cmd):
        cmdarray = array.array('B', cmd)
        res = self.getComChannel().writeread(cmdarray)
        s = array.array('B', res).tostring()
        return s
    
    def _sendSetCommand(self, cmd):
        cmdarray = array.array('B', cmd)
        self.getComChannel().writeread(cmdarray)  

       
