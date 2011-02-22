import PyTango
import time
import sys

class Attribute:
    def __init__(self, dev,attr_name):
        try:
            dev_alias = dev.alias()
            self.attr = dev_alias + "/" + attr_name
        except PyTango.DevFailed:
            self.attr = dev.dev_name() + "/" + attr_name
        try:
            self.attr_t = PyTango.AttributeProxy(dev,attr_name)
            self.attr_t.set_transparency_reconnection(True)
            self.attr_ok = True
        except PyTango.DevFailed:
            print "ERROR:", "Attribute %s can not be initialized" % self.attr
            self.attr_ok = False

    class AttributeCallback:
        def __init__(self, motor):
            self.motor = motor
        def push_event(self, event):
            if not event.err:
                print event.attr_name, event.attr_value.value
                self.motor.state = self.motor.translate_state(
                                                 int(event.attr_value.value))
            else:
                self.motor.state = self.motor.translate_state(exception = 1)
                print event.errors

    def subscribe(self, motor):
        try:
            cb = self.AttributeCallback(motor)
            self.attr_t.subscribe_event(PyTango.EventType.CHANGE, cb, [])
	except PyTango.DevFailed:
	    print "ERROR:", "Can't subscribe to attribute %s" % self.attr
            exctype, value = sys.exc_info()[:2]
            print value[0]["desc"]
        
    def write(self, value):    
        if self.attr_ok:
            val = PyTango.AttributeValue()
            val.value = value
            try:
                self.attr_t.write(val)
                return 1
            except PyTango.DevFailed:
                print "ERROR:", "Attribute %s can not be written" % self.attr
                raise
        else:
            print "ERROR:", "Attribute %s is not initialized" % self.attr
            
        
    def read(self):
        if self.attr_ok:
            try:
                return self.attr_t.read().value
            except PyTango.DevFailed:
                print "ERROR:", "Attribute %s can not be read" % self.attr
                raise
        else:
            print "ERROR:", "Attribute %s is not initialized" % self.attr
        return 0    

    def set_lm(self, min_value, max_value):
        if self.attr_ok:
            try:
                conf = self.attr_t.get_config()
                conf.min_value = str(min_value)
                conf.max_value = str(max_value)
                self.attr_t.set_config(conf)
                return 0
            except PyTango.DevFailed:
                print "ERROR:", "Attribute %s limits can not be modified" % self.attr
                raise
        else:
            print "ERROR:", "Attribute %s is not initialized" % self.attr
        return 0    

    def get_lm(self):
        if self.attr_ok:
            try:
                conf = self.attr_t.get_config()
                return float(conf.min_value), float(conf.max_value)
            except PyTango.DevFailed:
                print "ERROR:", "Attribute %s limits can not be modified" % self.attr
                raise
        else:
            print "ERROR:", "Attribute %s is not initialized" % self.attr
        return 0    


class Pool:
    def __init__(self, pool):
        self.pool = pool
        self.importtangodevice()
        if (self.device_ok == True):
            self.motorlist_attr = Attribute(self.device,"MotorList")
            self.controllerlist_attr = Attribute(self.device,"ControllerList")

    def importtangodevice(self):
        try:
            self.device = PyTango.DeviceProxy(self.pool)
            self.device_ok = True
        except PyTango.DevFailed:
            print "ERROR:", "Pool %s can not be initialized" % self.pool
            self.device_ok = False

    def getmotorlist(self):
        def extractmne(inval):
            return inval.split("-")[1].split()[0]
        return map(extractmne, self.motorlist_attr.read())

    def getcontrollerids(self):
        def extractid(inval):
            return int(inval.split("-")[0])
        return map(extractid, self.controllerlist_attr.read())

    def initcontroller(self, id):
        return self.device.InitController(id)

class PoolMotor:
    def __init__(self,mne):
        try:
            self.mne = mne
            self.importtangodevice()
            self.position_attr = Attribute(self.device,"position")
            self.state_attr = Attribute(self.device,"state")
            self.state = 0
            self.state_attr.subscribe(self)
        except PyTango.DevFailed:
            print "Exception received while trying to create attribute",mne

    def importtangodevice(self):
        try:
            self.device = PyTango.DeviceProxy(self.mne)
            self.device_ok = True
        except PyTango.DevFailed:
            print "ERROR:", "Motor %s can not be initialized" % self.mne
            self.device_ok = False
            
    def move(self, pos):
        # Move the motor
        self.state = 1 # Set my state to moving
        try:
            self.position_attr.write(pos)
        except PyTango.DevFailed:
            exctype, value = sys.exc_info()[:2]
            print value[0]["desc"]
            raise
            
    def move_state(self):
        """ Returns 1 if the motor is still moving """
        try:
            return self.translate_state(self.device.state())
        except PyTango.DevFailed:
            print "WARNING:", "Motor %s could not read the state" % self.mne
            return self.translate_state(exception = 1)

    def translate_state(self, tangostate = 0, exception = 0):
        """ Returns 1 if the motor is still moving """
        if exception:
            return 3
        
        if tangostate != PyTango.DevState.MOVING:
            if tangostate == PyTango.DevState.ALARM:
                return 2
            else:
                return 0
        else:
            return 1 

    def wait_a(self):    
        counter, start = 0, time.time()
        while 1:
            counter = counter + 1
            state = self.move_state()
            if state == 1:
                time.sleep(0.001)
            else:
                break
                        
        # print "Waited %i times in %f sec" % (counter, (time.time()-start)/counter)
        if state != 0:
            print self.device.status()

    def wait_e(self):    
        counter, start = 0, time.time()
        while 1:
            counter = counter + 1
            if self.state == 1:
                time.sleep(0.0001)
            else:
                break
                        
        # print "Waited %i times in %f sec" % (counter, (time.time()-start)/counter)
        if self.state != 0:
            print self.device.status()

    def set_lm(self, llow, lup):
        self.position_attr.set_lm(llow, lup)

    def get_lm(self):
        return self.position_attr.get_lm()

    def set_dial(self, pos):
        return self.device.SetPosition(pos)

    def read(self):
        return self.position_attr.read()
    
    def abort(self):
        # Abort the movement
        self.device.abort()
        # Wait until the motor has really aborted
        time.sleep(1.0)

    def mv(self, pos):
        try:
            self.move(pos)
            self.wait_e()
        except PyTango.DevFailed:
            pass
