import time,sys
import PyTango
from pool import CounterTimerController

class Channel:
    
    def __init__(self,idx):
        self.idx = idx            # 1 based index
        self.value = 0.0
        self.is_started = False
        self.active = False
        
class DummyCounterTimerController(CounterTimerController):
    "This class is the Tango Sardana CounterTimer controller for tests"

    gender = "Dummy"
    model  = "Dummy"    
    organization = "CELLS - ALBA"
    image = "dummy_ct_ctrl.png"
    logo = "ALBA_logo.png"

    MaxDevice = 1024
    
    StoppedMode = 0
    TimerMode = 1
    MonitorMode = 2
    CounterMode = 3

    Log = True

    def __init__(self,inst,props):
        CounterTimerController.__init__(self,inst,props)
        self.log_pref = "[DummyCounterTimerController %s]" % inst
        self.h('Constructor')

        self.mode = self.StoppedMode
        self.master_channel = None
        self.master_stop_at = None
        self.channels = [ Channel(i+1) for i in xrange(self.MaxDevice) ]
        self.start_time = None
        self.read_channels = {}
        self.counting_channels = {}
        
        self.f()
        
    def h(self,msg,idx = None):
        if not self.Log: return
        if idx is None:
            idx = ''
        else:
            idx = str(idx)
        msg = "%s %s(%s)..." % (self.log_pref,msg,idx)
        print msg, ; sys.stdin.flush()

    def f(self):
        if not self.Log: return
        print "[DONE]"
    
    def AddDevice(self,ind):
        self.h("AddDevice",ind)

        idx = ind - 1
        self.channels[ind].active = True
        
        self.f()
        
    def DeleteDevice(self,ind):
        self.h("DeleteDevice",ind)

        self.channels[ind].active = False
        
        self.f()

    def StateOne(self,ind):
        self.h("StateOne",ind)
        
        try:
            sta = None
            status = None
            channel = self.channels[ind-1]
            m = self.mode
            if not m is self.StoppedMode:
                now = time.time()
                elapsed_time = now - self.start_time
                self._checkState(elapsed_time)
    
            if self.mode == self.StoppedMode and not channel.is_started:
                sta = PyTango.DevState.ON
                status = "Stopped"
            else:
                sta = PyTango.DevState.MOVING
                status = "Acquiring"
        except Exception,e:
            print e
            raise e
        
        self.f()
        return (sta,status)
        
    def _setChannelValue(self, channel, elapsed_time):
        channel.value = elapsed_time * channel.idx

    def _checkState(self, elapsed_time):
        m = self.mode
        if m == self.TimerMode:
            if elapsed_time >= self.master_stop_at:
                self._finished(elapsed_time)
        elif m == self.MonitorMode:
            self._setChannelValue(self.master_channel, elapsed_time)
            if self.master_channel.value >= self.master_stop_at:
                self._finished(elapsed_time)

    def _finished(self, elapsed_time):
        m = self.mode
        if m == self.TimerMode:
            self.master_channel.value = elapsed_time
        
        if not self.master_channel is None:
            self.master_channel.is_started = False

        for channel in self.counting_channels.values():
            if m is self.TimerMode and channel.idx == self.master_channel.idx:
                continue
            # the counting formula (very simple so far...)
            self._setChannelValue(channel, elapsed_time)
            channel.is_started = False
        
        self.start_time = None
        self.master_channel = None
        self.master_stop_at = None
        self.mode = self.StoppedMode
                
    def PreReadAll(self):
        self.h("PreReadAll")

        self.read_channels = {}
        
        self.f()

    def PreReadOne(self,ind):
        self.h("PreReadOne",ind)

        channel = self.channels[ind-1]
        self.read_channels[ind] = channel
        
        self.f()

    def ReadAll(self):
        self.h("ReadAll")

        # if in acquisition then calculate the values to return
        if not self.mode is self.StoppedMode:
            now = time.time()
            elapsed_time = now - self.start_time
            for channel in self.read_channels.values():
                if channel.is_started:
                    self._setChannelValue(channel, elapsed_time)
        
        self.f()

    def ReadOne(self,ind):
        self.h("ReadOne",ind)
        
        v = self.read_channels[ind].value
        
        self.f()
        return v

    def PreStartAllCT(self):
        self.h("PreStartAllCT")

        # if it is first pass in PreStartAllCT or if this ctrl will not have the
        # master...
        if self.master_channel is None: 
            # Clean up data from last acquisition
            for channel in self.counting_channels.values():
                channel.value = 0.0
                channel.is_started = False
            
            self.counting_channels = {}
        
        # if the timer/monitor was not loaded this means this controller is
        # just a simple counter without master channel
        if self.mode == self.StoppedMode:
            self.mode = self.CounterMode
        
        self.f()
    
    def PreStartOneCT(self,ind):
        self.h("PreStartOneCT",ind)
        
        idx = ind - 1
        channel = self.channels[idx]
        channel.value = 0.0
        self.counting_channels[ind] = channel
        
        self.f()
        return True
        
    def StartOneCT(self,ind):
        self.h("StartOneCT",ind)
        
        self.counting_channels[ind].is_started = True
        
        self.f()
            
    def StartAllCT(self):
        self.h("StartAllCT")
       
        self.start_time = time.time()
        
        self.f()
                 
    def LoadOne(self,ind,value):
        self.h("LoadOne",ind)
        idx = ind - 1 
        self.master_channel = self.channels[idx]
        if value > 0:
            self.mode = self.TimerMode
            self.master_stop_at = value
        else:
            self.mode = self.MonitorMode
            self.master_stop_at = -value
            
        self.f()

    def AbortOne(self,ind):
        self.h("AbortOne",ind)

        if not self.mode is self.StoppedMode:
            now = time.time()
            elapsed_time = now - self.start_time
            self._finished(elapsed_time)

        self.f()
        
    def __del__(self):
        self.h('Destructor')
        self.f()
            