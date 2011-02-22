import threading
from utils import Timer, CaselessDict
from Attribute import WorkingState
from Log import Logger

class PoolTimer(Logger):
    """ PoolTimer timer manages a list of Pools that have to be polled in
    the same period """
    
    def __init__(self, period):
        self.call__init__(Logger, 'PoolTimer')
        self.pool_dict = CaselessDict()
        self.timer = Timer(period/1000, self.pingPools, self)
        self.lock = threading.RLock()
        
    def start(self):
        """ Starts the timer """
        self.timer.start()
    
    def stop(self):
        """ Stop the timer"""
        self.timer.stop()
    
    def containsPool(self, pool):
        self.lock.acquire()
        try:
            return self.pool_dict.has_key(pool.getName())
        finally:
            self.lock.release()
            
    def getPoolCount(self):
        return len(self.pool_dict)
    
    def addPool(self,pool):

        self.lock.acquire()
        try:
            if not self.pool_dict.has_key(pool.getName()):
                self.pool_dict[pool.getName()] = pool
#                if len(self.pool_dict) == 1:
#                    self.start()
        finally:
            self.lock.release()
    
    def removePool(self,pool):

        self.lock.acquire()
        try:
            if self.pool_dict.has_key(pool.getName()):
                del self.pool_dict[pool.getName()]
#                if len(self.pool_dict) == 0:
#                    self.stop()
        finally:
            self.lock.release()
        
    def pingPools(self):
        """ Timer event"""
        
        excepts = []
        self.lock.acquire()
        try:
            alive = []
            for pool_name, pool in self.pool_dict.iteritems():
                try:
                    self.debug("Trying online for %s" % pool_name)
                    pool.try_Online()
                    if pool.working_state == WorkingState.ONLINE:
                        self.debug("%s is now online" % pool_name)
                        alive.append(pool)
                except Exception, e:
                    self.debug("%s is still offline" % pool_name)
                    excepts.append(e)
            for p in alive:
                self.removePool(p)
        finally:
            self.lock.release()
            if len(excepts) > 0:
                raise Exception(excepts)
          