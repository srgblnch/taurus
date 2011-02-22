import logging, weakref
from Object import *

class Logger(Object):

    root_inited	= False

    Error	= logging.ERROR
    Warning	= logging.WARNING
    Output	= 15
    Result  = 16
    Info	= logging.INFO
    Debug	= logging.DEBUG
    
    BlockStart	= '<BLOCKSTART>'
    BlockReturn = '<BLOCKRETURN>'

    def __init__(self, name='', parent=None):
        self.call__init__(Object)
        
        if not Logger.root_inited:
            self.initRoot()
            Logger.root_inited = True
            
        self.log_name = name
        if parent is not None:
            self.full_name = '%s.%s' % (parent.full_name, name)
        else:
            self.full_name = name

        self.log_obj = logging.getLogger(self.full_name)
        self.log_handlers = []

        self.parent = None
        self.children = []
        if parent is not None:
            self.parent = weakref.ref(parent)
            parent.addChild(self)

    def __del__(self):
        parent = self.getParent()
        if parent is not None:
            parent.delChild(self)

    def initRoot(self):
        logging.basicConfig(level=logging.DEBUG,
                            format='%(threadName)-10s %(levelname)-8s %(asctime)s %(name)s: %(message)s')
        
    def getRootLog(self):
        return logging.getLogger()
    
    def getParent(self):
        if self.parent is None:
            return None
        return self.parent()

    def getChildren(self):
        children = []
        for ref in self.children:
            child = ref()
            if child is not None:
                children.append(child)
        return children
        
    def addChild(self, child):
        ref = weakref.ref(child)
        if not ref in self.children:
            self.children.append(ref)
        
    def delChild(self, child):
        ref = weakref.ref(child)
        if ref in self.children:
            self.children.remove(ref)

    def __eq__(self, other):
        return self is other

    def addLogHandler(self, handler):
        self.log_obj.addHandler(handler)
        self.log_handlers.append(handler)

    def copyLogHandlers(self, other):
        for handler in other.log_handlers:
            self.addLogHandler(handler)
            
    def debug(self, msg, *args, **kw):
        self.log_obj.debug(msg, *args, **kw)

    def output(self, msg, *args, **kw):
        self.log_obj.log(Logger.Output, msg, *args, **kw)
        
    def info(self, msg, *args, **kw):
        self.log_obj.info(msg, *args, **kw)

    def warning(self, msg, *args, **kw):
        self.log_obj.warning(msg, *args, **kw)

    def error(self, msg, *args, **kw):
        self.log_obj.error(msg, *args, **kw)

    def flushOutput(self):
        self.syncLog()

    def setLogBlockStart(self):
        self.output(Logger.BlockStart)

    def goLogBlockStart(self):
        self.output(Logger.BlockReturn)

    def syncLog(self):
        logger = self
        synced = []
        while logger is not None:
            for handler in logger.log_handlers:
                if handler in synced:
                    continue
                try:
                    sync = getattr(handler, 'sync')
                except:
                    continue
                sync()
                synced.append(handler)
            logger = logger.getParent()
            

class LogFilter(logging.Filter):

    def __init__(self, level):
        self.filter_level = level
        logging.Filter.__init__(self)

    def filter(self, record):
        ok = (record.levelno == self.filter_level)
        return ok
