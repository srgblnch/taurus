import weakref, threading, time
from Object import *

class BoundMethodWeakref:
    def __init__(self, bound_method, del_cb=None):
        cb = (del_cb and self.deleted)
        self.func_ref = weakref.ref(bound_method.im_func, cb) 
        self.obj_ref = weakref.ref(bound_method.im_self, cb)
        if cb:
            self.del_cb = CallableRef(del_cb)
        self.already_deleted = 0

    def deleted(self, obj):
        if not self.already_deleted:
            del_cb=self.del_cb()
            if del_cb is not None:
                del_cb(self)
                self.already_deleted = 1
        
    def __call__(self):
        obj = self.obj_ref()
        if obj is not None:
            func = self.func_ref()
            if func is not None:
                return func.__get__(obj)

    def __hash__(self):
        return id(self)

    def __cmp__(self, other):
        if other.__class__ == self.__class__:
            ret = cmp((self.func_ref, self.obj_ref),
                      (other.func_ref, other.obj_ref))
            return ret
        return 1

    def __repr__(self):
        obj, func = self.obj_ref(), self.func_ref()
        return 'BoundMethodWeakRef of %s.%s' % (obj, func)

    
def CallableRef(object, del_cb=None):
    if hasattr(object, 'im_self'):
        if object.im_self is not None:
            return BoundMethodWeakref(object, del_cb)
    return weakref.ref(object, del_cb)


class EventStack:

    def __init__(self, history=True):
        self.unread_stack = []
        self.read_stack = []
        self.history = history

    def isEmpty(self):
        return len(self.unread_stack) == 0

    def push(self, event):
        self.unread_stack.append(event)

    def getNext(self):
        if self.isEmpty():
            return None
        event = self.unread_stack[0]
        del self.unread_stack[0]
        if self.history:
            self.read_stack.append(event)
        
        return event

    def getAllUnread(self):
        unread = self.unread_stack
        self.unread_stack = []
        return unread
    
    def getAllRead(self):
        read = self.read_stack
        self.read_stack = []
        return read
    

class EventGenerator(Object):
    """Base class capable of firing events"""

    WaitTimeout = 0.1
    
    def __init__(self, name, events_active=True):
        self.call__init__(Object)
        self.event_name = name
        self.cb_list = []
        self.last_val = None
        self.first_event_val = None
        self.events_active = events_active
        self.cond = threading.Condition()
        self.wait_list = []
        
    def lock(self):
        self.cond.acquire()

    def unlock(self):
        self.cond.release()

    def subscribeEvent(self, cb, data=None):
        if not self.events_active:
            raise RuntimeError, ('%s does not have ' \
                                 'events/polling active' % self.event_name)
        
        cb_ref = CallableRef(cb, self.unsubscribeDeletedEvent)

        self.lock()
        try:
            if (cb_ref, data) in self.cb_list:
                raise RuntimeError, ('Callback %s(%s) already reg. on %s' %
                                    (cb, data, self.event_name))
            self.cb_list.append((cb_ref, data))
            cb(data, self.first_event_val)
        finally:
            self.unlock()

        
    def unsubscribeDeletedEvent(self, cb_ref):
        self.lock()
        try:
            aux_list = list(self.cb_list)
            for i in xrange(len(aux_list) - 1, -1, -1):
                pair = self.cb_list[i]
                if pair[0] is cb_ref:
                    del self.cb_list[i]
        finally:
            self.unlock()
                
    def unsubscribeEvent(self, cb, data=None):
        cb_ref = CallableRef(cb)
        self.lock()
        try:
            if (cb_ref, data) in self.cb_list:
                self.cb_list.remove((cb_ref, data))
            else:
                self.debug("Trying to unsubscribe: %s is not a listener of %s" % (str(cb_ref),self.event_name))
        finally:
            self.unlock()
    
    def isSubscribed(self, cb, data=None):
        cb_ref = CallableRef(cb, self.unsubscribeDeletedEvent)
        return (cb_ref, data) in self.cb_list
         
    def setEventsActive(self, events_active):
        self.events_active = events_active

    def getEventsActive(self):
        return self.events_active
    
    def fireEvent(self, val, event_val=None):
        self.lock()
        try:
            self.last_val = val
            self.cond.notifyAll()
            if event_val is None:
                event_val = val
            self.first_event_val = event_val
            for stack in self.wait_list:
                stack.push(event_val)
            for cb_ref, data in self.cb_list:
                cb = cb_ref()
                if cb is not None:
                    cb(data, event_val)
        finally:
            self.unlock()
            
    def waitEvent(self, val=None, equal=True, any=False, timeout=None,
                  stack=None):
        if not self.events_active:
            raise RuntimeError, ('%s does not have ' \
                                 'events/polling active' % self.event_name)
        self.lock()
        try:
            t0 = time.time()
            timeout_expired = False
            stack = stack or EventStack(history=False)
            while True:
                curr_val = self.last_val
                avail = not stack.isEmpty()
                if avail:
                    curr_val = stack.getNext()
                if any:
                    block = not avail
                else:
                    block = ((equal and (val != curr_val)) or \
                             (not equal and (val == curr_val)))
                if timeout:
                    timeout_expired = (time.time() - t0 > timeout)
                if not block or timeout_expired:
                    break
                self.wait_list.append(stack)
                self.cond.wait(self.WaitTimeout)
                self.wait_list.remove(stack)
            val = curr_val
        finally:
            self.unlock()

        return val

    def read(self):
        self.lock()
        try:
            val = self.last_val
        finally:
            self.unlock()
        return val
    

class ConfigEventGenerator(EventGenerator):
    """Manage configuration events"""

    def fireEvent(self, val, event_val=None):
        EventGenerator.fireEvent(self, val, event_val)
        

class ListEventGenerator(EventGenerator):
    """Manage list events, detecting changes in the list"""

    def __init__(self, name, events_active=True):
        self.call__init__(EventGenerator, name, events_active)
        self.last_val = []
        self.first_event_val = [], [], []
        
    def fireEvent(self, val):
        # if attribute is not alive and last time was also not alive then
        # skip the event propagation
        if val is None:
            if len(self.last_val) == 0:
                return
            val = []
            
        self.lock()
        try:
            val = list(val)
            last_val = self.last_val
            rm  = [x for x in last_val if x not in val]
            add  = [x for x in val if x not in last_val]
            event_val = val, rm, add
            EventGenerator.fireEvent(self, val, event_val)
            self.first_event_val = val, [], val
        finally:
            self.unlock()
