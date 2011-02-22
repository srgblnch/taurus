import string, re, time, thread
import PyTango
from Event import *
from Log import *
from utils import Enumeration

WorkingState = Enumeration("WorkingState",
                           ['ONLINE', 
                            'OFFLINE', 
                            'SHUTDOWN', 
                            'CRASHED'])

#class WorkingState:
#    """Enumerating class designed to represent possible 'working states' 
#    (not TANGO states) of a Tango device.
#    Possible states: 
#      ONLINE - means the pool process is running.
#      OFFLINE - means the pool process is NOT running and has never been detected 
#                as running in the lifetime of the application.
#      SHUTDOWN - means the pool process is NOT running.
#      CRASHED - means the pool process is NOT running and it was not shutdown 
#                properly (i.e. the device is still reported as exported in the
#                database)
#    """
#    
#    ONLINE, OFFLINE, SHUTDOWN, CRASHED = range(4)
    
class EventCallback:
    def __init__(self, callback_object):
        self.cbo = weakref.ref(callback_object)
    
    def push_event(self, event):
        cbo = self.cbo()
        if cbo is not None:
            cbo.push_event(event)


class ChangeEventCallback(EventCallback):
    pass


class ConfigEventCallback(EventCallback):

    def push_event(self, event):
       cbo = self.cbo()
       if cbo is not None:
          cbo.push_config_event(event)


class AttributeBase(Logger):
    """Generic class for reading, writing, and managing events on attributes"""
    
    def __init__(self, obj, name, no_check=False):
        self.call__init__(Logger, name + 'Attr', obj)
        self.obj_ref = weakref.ref(obj)
        self.name = name
        self.subscription_id = None
        self.conf_subscription_id = None
        if not no_check:
            self.connect()
        
    def __del__(self):
        dev = self.__checkDev()
        if not dev:
            return
        if self.subscription_id:
            dev.unsubscribe_event(self.subscription_id)
        if self.conf_subscription_id:
            dev.unsubscribe_event(self.conf_subscription_id)
    
    def __checkDev(self):
        obj = self.obj_ref()
        if obj is None:
            return
        dev = obj.checkDev()
        return dev
    
    def __subscribeChangeEvent(self):
        dev = self.__checkDev()
        if not dev:
            return
        
        if self.subscription_id is None:
            try:
                id = dev.subscribe_event(self.name,PyTango.EventType.CHANGE,
                                         self, [])
                self.subscription_id = id
            except PyTango.DevFailed, traceback:
                for error in traceback:
                    if error['reason'] == 'API_DSFailedRegisteringEvent' or \
                       error['reason'] == 'API_BadConfigurationProperty':
                        break
                else:
                    raise
    
    def __unsubscribeChangeEvent(self):
        dev = self.__checkDev()
        if not dev:
            return

        if self.subscription_id:
            try:
                self.debug("unsubscribing change event...")
                id = dev.unsubscribe_event(self.subscription_id)
                self.subscription_id = None
                self.debug("Done")
            except PyTango.DevFailed, traceback:
                raise

    def __subscribeConfigEvent(self):
        dev = self.__checkDev()
        if not dev:
            return
       
        if self.conf_subscription_id is None:
            try:
                id = dev.subscribe_event(self.name,
                                         PyTango.EventType.ATTR_CONF_EVENT,
                                         ConfigEventCallback(self), [])
                self.conf_subscription_id = id
            except PyTango.DevFailed, traceback:
                for error in traceback:
                    if error['reason'] == 'API_DSFailedRegisteringEvent' or \
                       error['reason'] == 'API_BadConfigurationProperty':
                        break
                else:
                    raise        
            
    def __unsubscribeConfigEvent(self):
        dev = self.__checkDev()
        if not dev:
            return

        if not self.conf_subscription_id:
            try:
                id = dev.unsubscribe_event(self.conf_subscription_id)
                self.conf_subscription_id = None
            except PyTango.DevFailed, traceback:
                raise        

    def connect(self):
        self.__subscribeChangeEvent()
        self.__subscribeConfigEvent()
        

    def disconnect(self):
        self.__unsubscribeChangeEvent()
        self.__unsubscribeConfigEvent()
        
    def rawRead(self):
        dev = self.__checkDev()
        
        if not dev:
            return
        
        return dev.read_attribute(self.name).value

    def rawWrite(self, val):
        dev = self.__checkDev()
        
        if not dev:
            return
        
        return dev.write_attribute(self.name,val)
    
    def decode(self, val):
        return val

    def encode(self, val):
        return val
    
    def read(self):
        return self.decode(self.rawRead())

    def write(self, val):
        return self.rawWrite(self.encode(val))
    
    #
    # Change event API
    #
    
    def push_event(self, event):
        if not event.err:
            val = event.attr_value.value
            val = self.decode(val)
        # empty result should not be an eror
        elif event.errors[0]['reason'] == 'API_EmptyDeviceAttribute':
            val = []
            event.err = False
        else:
            val = None
        
        try:
            self.fireEvent(val)
        except Exception, err:
            print err

    def fireEvent(self, val):
        pass
    
    #
    # Attribute configuration API
    #

    def getConfig(self):
        dev = self.__checkDev()
        return dev.get_attribute_config_ex(self.name)
    
    def push_config_event(self, event):
        if not event.err:
            conf = event.attr_conf
        else:
            conf = None
        try:
          self.fireConfigEvent(conf)
        except Exception, err:
          print err
        
    def fireConfigEvent(self, conf):
        pass
    
    
class Attribute(EventGenerator, AttributeBase):
    """This class provides cached attributes by using events"""
   
    def __init__(self, obj, name, no_check=False):
        event_name = 'Attribute %s' % name
        self.config_event_generator = ConfigEventGenerator(event_name)
        self.call__init__(EventGenerator, event_name)
        self.call__init__(AttributeBase, obj, name, no_check)
        
    def connect(self):
        AttributeBase.connect(self)
        
        if self.subscription_id is None:
            self.setEventsActive(0)
            
        if self.conf_subscription_id is None:
            self.config_event_generator.setEventsActive(0)
            

    def read(self, cached=True):
        if not cached or not self.events_active:
            self.last_val = AttributeBase.read(self)
        else:
            self.waitEvent(val=None, equal=False)
        return EventGenerator.read(self)
    
    def fireEvent(self,  val):
        EventGenerator.fireEvent(self, val)
    
    #
    # Attribute configuration API
    #

    def subscribeConfigEvent(self, cb, data=None):
        self.config_event_generator.subscribeEvent(cb, data)
        
    def unsubscribeDeletedConfigEvent(self, cb_ref):
        self.config_event_generator.unsubscribeDeletedEvent(cb_ref)
    
    def unsubscribeConfigEvent(self, cb, data=None):
        self.config_event_generator.unsubscribeEvent(cb, data)
    
    def setConfigEventsActive(self, events_active):
        self.config_event_generator.setEventsActive(events_active)
        
    def getConfigEventsActive(self):
        return self.config_event_generator.getEventsActive()

    def fireConfigEvent(self, conf):
        self.config_event_generator.fireEvent(conf)
    
    def waitConfigEvent(self, val=None, equal=True, any=False, timeout=None,
                  stack=None):
        self.config_event_generator.waitConfigEvent(val,equal,any,timeout,stack)
    
    def getConfig(self, cached=True):
        if not cached or not self.config_event_generator.events_active:
            self.config_event_generator.last_val = AttributeBase.getConfig(self)
        else:
            self.config_event_generator.waitEvent(val=None, equal=False)
        return self.config_event_generator.read()
    
    def getType(self, cached=True):
        return self.getConfig(cached=cached).data_type

    def getMaxDimX(self, cached=True):
        return self.getConfig(cached=cached).max_dim_x

    def getMaxDimY(self, cached=True):
        return self.getConfig(cached=cached).max_dim_y
        
    def getDescription(self, cached=True):
        return self.__ue( self.getConfig(cached=cached).description )
        
    def getLabel(self, cached=True):
        return self.__ue( self.getConfig(cached=cached).label )
        
    def getUnit(self, cached=True):
        return self.__ue( self.getConfig(cached=cached).unit )
    
    def getStandardUnit(self, cached=True):
        return self.__ue( self.getConfig(cached=cached).standard_unit )
        
    def getDisplayUnit(self, cached=True):
        return self.__ue( self.getConfig(cached=cached).display_unit )

    def getFormat(self, cached=True):
        return self.getConfig(cached=cached).format
        
    def getMinValue(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).min_value )

    def getMaxValue(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).max_value )
        
    def getLimits(self, cached=True):
        return [self.getMinValue(cached=cached),self.getMaxValue(cached=cached)]

    def getMinAlarm(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).min_alarm )

    def getMaxAlarm(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).max_alarm )
        
    def getAlarms(self, cached=True):
        return [self.getMinAlarm(cached=cached),self.getMaxAlarm(cached=cached)]
    
    def getMinWarning(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).min_warning )

    def getMaxWarning(self, cached=True):
        return self.__tryGetFloat( self.getConfig(cached=cached).max_warning )
        
    def getWarnings(self, cached=True):
        return [self.getMinWarning(cached=cached),self.getMaxWarning(cached=cached)]
    
    def setLimits(self,low, high):
        config = self.getConfig()
        config.min_value = str(low)
        config.max_value = str(high)
        self.__checkDev().set_attribute_config_ex(config)


    def __tryGetFloat(self,v):
        """Returns the float of the given object or the object if it is not 
           possible to decode the float value from it"""
           
        try:
            return float(v)
        except:
            return v

    
    def __ue(self,v):
        """Returns the 'unicode-escape' version of the given string"""
         
        if v is None: 
            return v
        return unicode(str(v),'unicode-escape')
    
    #
    # Output API
    #
    def __str__(self):
        return self.__strof__(self.read())

    def __strof__(self,value,with_label=True,with_unit=True,with_nan='-'):
        c = self.getConfig()
        v = self.getStrForValue(value,with_unit,with_nan)
        if with_label:
            ret = "%15s = %s" % (c.label,v)
        else:
            ret = v
        return ret
    
    def getStrForValue(self,value,with_unit=True,with_nan='-'):
        c = self.getConfig()
        strv = str(value).lower()
        if strv == 'nan' or strv == 'none' or strv == 'not specified':
            v = c.format % 1
            v = len(v)*with_nan
        else:
            v = c.format % value
        
        if with_unit:
            u = c.unit
            if u.lower() != 'no unit':
                v += " " + u
        return v

    def getStrValue(self):
        return self.getStrForValue(self.read())
    
    def getMinStrValue(self):
        return self.getStrForValue(self.getMinValue())

    def getMaxStrValue(self):
        return self.getStrForValue(self.getMaxValue())
    
    def getFullStr(self):
        c = self.getConfig()
        ret = c.label + ":\n"
        ret += "High    %s\n" % self.__strof__(self.getMaxValue(),with_label=False)
        ret += "Current %s\n" % self.__strof__(self.read(),with_label=False)
        ret += "Low     %s" % self.__strof__(self.getMinValue(),with_label=False)
        return ret
        
        
class ListAttribute(ListEventGenerator, AttributeBase):
    """This class provides cached list events by using events"""
   
    def __init__(self, obj, name, no_check=False):
        event_name = 'Attribute %s' % name
        self.call__init__(ListEventGenerator, event_name)
        self.call__init__(AttributeBase, obj, name, no_check)

    def connect(self):
        AttributeBase.connect(self)
        
        if self.subscription_id is None:
            self.setEventsActive(0)
                    
    def read(self, cached=True):
        if not cached or not self.events_active:
            self.last_val = AttributeBase.read(self)
        else:
            self.waitEvent(val=None, equal=False)
        return EventGenerator.read(self)

    def fireEvent(self, val):
        ListEventGenerator.fireEvent(self, val)
    

class MultiFieldListAttribute(ListAttribute):
    """Class decoding multi-field list attributed """
    def __init__(self, *args, **pars):
        self.delimiter = ' - '
        if 'delimiter' in pars.keys():
            self.delimiter = pars['delimiter']
            pars = dict(pars)
            del pars['delimiter']
        self.call__init__(ListAttribute, *args, **pars)

    def decode(self, val):
        list_val = []
        for line in val:
            fields = line.split(self.delimiter)
            list_val.append(fields)
        return list_val


class DictListAttribute(MultiFieldListAttribute):

    def __init__(self, obj, name, dict_keys, **pars):
        self.call__init__(MultiFieldListAttribute, obj, name, **pars)

        self.re_objs = []
        for key in dict_keys:
            re_str = '%s:[ \t]+(?P<%s>.+)' % (key, key)
            self.re_objs.append([key, re.compile(re_str)])
        
    def decode(self, val):
        dict_list = []
        list_val = MultiFieldListAttribute.decode(self, val)
        for line in list_val:
            dict_val = {}
            for field, (key, re_obj) in zip(line, self.re_objs):
                m = re_obj.match(field)
                dict_val[key] = m.group(key)
            dict_list.append(dict_val)
        return dict_list
            
class TangoDevice(EventGenerator, Logger):
    
    Offs = [WorkingState.OFFLINE, WorkingState.SHUTDOWN, WorkingState.CRASHED]
    
    def __init__(self, dev_name, no_check=False, parent=None):
        
        self.working_state = WorkingState.OFFLINE
        self.tango_state = PyTango.DevState.UNKNOWN
        
        try:
            self.alias = PyTango.Database().get_alias(dev_name)
            if self.alias.lower() == 'nada':
                self.alias = dev_name
        except:
            self.alias = dev_name
        
        self.call__init__(Logger, self.alias, parent)
        
        event_name = "%s working state" % dev_name
        self.call__init__(EventGenerator, event_name)
        
        self.dev_name = dev_name
        self.dev = None
        self.state = None
        
        if not no_check:
            self.connect()
            
        self.first_event_val = (self.tango_state,PyTango.DevState.UNKNOWN,
                                self.working_state,WorkingState.OFFLINE)
            
    def checkDev(self):
        if self.dev is None and self.dev_name:
            self.dev = PyTango.DeviceProxy(self.dev_name)
        return self.dev

    def connect(self):
        if self.state is None:
            self.state = Attribute(self, 'State')
        if self.state.getEventsActive():
            if not self.state.isSubscribed(self.stateChanged):
                self.state.subscribeEvent(self.stateChanged)
            
    def disconnect(self):
        if self.state is not None:
            if self.state.getEventsActive():
                self.state.unsubscribeEvent(self.stateChanged)
            #self.state = None
            #self.dev = None

    def stateChanged(self, data, tango_state):
        #self.debug("State changed to %s (was %s)" % (tango_state,self.tango_state))
        
        last_tango_state = self.tango_state
        self.tango_state = tango_state
        
        last_working_state = self.working_state
        
        # if device was running ...
        if last_working_state == WorkingState.ONLINE:
            # but is not running now unexpectly
            if tango_state is None:
                self.working_state = WorkingState.CRASHED
            # and now was shutdown properly
            elif tango_state == PyTango.DevState.UNKNOWN:
                self.working_state = WorkingState.SHUTDOWN
        
        # if the device was never online before, was shutdown or crashed ...     
        else:
            # this does not make sence: receiving a shutdown
            if (tango_state is None and last_working_state == WorkingState.OFFLINE) or \
               (tango_state == PyTango.DevState.UNKNOWN):
                print "This should never happen"
            # and it came back online
            elif not tango_state is None:
                self.working_state = WorkingState.ONLINE
        
        state_info = (self.tango_state, last_tango_state,
                      self.working_state, last_working_state)
        
        self.fireEvent(state_info)
        #self.debug("State changed DONE")


    def __getattr__(self, name):
        return getattr(self.dev, name)

    def __nonzero__(self):
        return True
    
        
class AttributeLogHandler(Object, logging.Handler):

    def __init__(self, dev, attr_name, level=logging.NOTSET):
        self.call__init__(Object)
        self.call__init__(logging.Handler, level)

        dev.set_change_event(attr_name, True, False)
        attr_list = dev.get_device_attr()
        attr = attr_list.get_attr_by_name(attr_name)
        attr.set_value([])
        self.dev = weakref.ref(dev)
        self.attr_name = attr_name
        self.waiting_output = []

        self.cond = threading.Condition()
        self.running = True
        self.cond.acquire()
        try:
            self.thread = threading.Thread(target=self.threadFunct)
            self.thread.start()
            self.cond.wait()
        finally:
            self.cond.release()

    def finish(self):
        self.cond.acquire()
        try:
            self.running = False
            self.cond.notifyAll()
        finally:
            self.cond.release()
        
    def threadFunct(self):
        while True:
            self.cond.acquire()
            try:
                self.sending_output = []
                self.cond.notifyAll()
                if len(self.waiting_output) == 0:
                    self.cond.wait()
                self.sending_output = self.waiting_output
                self.waiting_output = []
                stop = not self.running
            finally:
                self.cond.release()
            if stop:
                return
            if len(self.sending_output) == 0:
                continue
            dev = self.dev()
            if dev:
                dev.push_change_event(self.attr_name, self.sending_output)
            del dev
                
    def emit(self, record):
        output = self.getRecordMessage(record)
        self.sendText(output)

    def getRecordMessage(self, record):
        return record.getMessage().split('\n')
    
    def sendText(self, output):
        self.cond.acquire()
        try:
            self.waiting_output += output
            self.cond.notifyAll()
        finally:
            self.cond.release()

    def read(self, attr):
        self.cond.acquire()
        try:
            output = self.waiting_output
        finally:
            self.cond.release()
        attr.set_value(output)

    def sync(self):
        empty = False
        while not empty:
            self.cond.acquire()
            try:
                empty = len(self.waiting_output + self.sending_output) == 0
                if not empty:
                    self.cond.wait()
            finally:
                self.cond.release()


class AttributeBufferedLogHandler(AttributeLogHandler):

    def __init__(self, dev, attr_name, level=logging.NOTSET):
        self.call__init__(AttributeLogHandler, dev, attr_name, level)
        self.output_buffer = []
        
    def emit(self, record):
        output = self.getRecordMessage(record)
        self.output_buffer += output
        
    def sync(self):
        output = self.output_buffer
        self.output_buffer = []
        self.sendText(output)
        AttributeLogHandler.sync(self)
        
