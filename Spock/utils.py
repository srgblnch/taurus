import os, sys, imp, shutil, time, threading, types
import string, getopt
import PyTango

from Log import Logger

def print_DevFailed(df):
    """Alternative print of PyTango.DevFailed. The original PyTango.Except.print_exception
    seems not to work at shutdown so this ensures a good print in that case"""
    
    try:    
        desc = df.message['desc'].rstrip('\n').replace('\n',"              \n")
    
        print "   Severety = %s" % df.message['severity']
        print "     Reason = %s" % df.message['reason']
        print "Description = %s" % desc
        print "     Origin = %s" % df.message['origin']
    except:
        print "Exception = %s" % str(df)

        
class Enumeration:
    def __init__(self, name, enumList):
        self.__doc__ = name
        lookup = { }
        reverseLookup = { }
        uniqueNames = [ ]
        self._uniqueValues = uniqueValues = [ ]
        self._uniqueId = 0
        for x in enumList:
            if type(x) == types.TupleType:
                x, i = x
                if type(x) != types.StringType:
                    raise EnumException, "enum name is not a string: " + x
                if type(i) != types.IntType:
                    raise EnumException, "enum value is not an integer: " + i
                if x in uniqueNames:
                    raise EnumException, "enum name is not unique: " + x
                if i in uniqueValues:
                    raise EnumException, "enum value is not unique for " + x
                uniqueNames.append(x)
                uniqueValues.append(i)
                lookup[x] = i
                reverseLookup[i] = x
        for x in enumList:
            if type(x) != types.TupleType:
                if type(x) != types.StringType:
                    raise EnumException, "enum name is not a string: " + x
                if x in uniqueNames:
                    raise EnumException, "enum name is not unique: " + x
                uniqueNames.append(x)
                i = self.generateUniqueId()
                uniqueValues.append(i)
                lookup[x] = i
                reverseLookup[i] = x
        self.lookup = lookup
        self.reverseLookup = reverseLookup
    def generateUniqueId(self):
        while self._uniqueId in self._uniqueValues:
            self._uniqueId += 1
        n = self._uniqueId
        self._uniqueId += 1
        return n
    def __getattr__(self, attr):
        if not self.lookup.has_key(attr):
            raise AttributeError
        return self.lookup[attr]
    def whatis(self, value):
        return self.reverseLookup[value]


class Timer(Logger):

    def __init__(self, interval, function, parent, *args, **kwargs):
        """Create Timer Object"""
        self.call__init__(Logger, 'Timer on ' + function.__name__, parent)
        self.__lock = threading.Lock()
        self.__interval = interval
        self.__function = function
        self.__args = args
        self.__kwargs = kwargs
        self.__loop = False
        self.__alive = False
        self.__start_nb = 0
        self.__thread = None
        
    def start(self):
        """ Start Timer Object """
        self.__lock.acquire()
        if not self.__alive:
            self.debug("Timer::start()")
            self.__loop = True
            self.__alive = True
            self.__start_nb += 1
            thread_name = "TimerLoop %d" % self.__start_nb
            self.__thread = threading.Thread(target=self.__run, name = thread_name)
            self.__thread.setDaemon(True)
            self.__thread.start()
        self.__lock.release()
    
    def stop(self):
        """ Stop Timer Object """
        self.debug("Timer::stop()")
        self.__lock.acquire()
        self.__loop = False
        self.__lock.release()
    
    def __run(self):
        """ Private Thread Function """
        self.debug("Timer thread starting")
        while self.__loop:
            self.__function(*self.__args, **self.__kwargs)
            time.sleep(self.__interval)
        self.__alive = False
        self.debug("Timer thread ending")

class CaselessDict(dict):
    def __init__(self, other=None):
        if other:
            # Doesn't do keyword args
            if isinstance(other, dict):
                for k,v in other.items():
                    dict.__setitem__(self, k.lower(), v)
            else:
                for k,v in other:
                    dict.__setitem__(self, k.lower(), v)

    def __getitem__(self, key):
        return dict.__getitem__(self, key.lower())

    def __setitem__(self, key, value):
        dict.__setitem__(self, key.lower(), value)

    def __contains__(self, key):
        return dict.__contains__(self, key.lower())

    def has_key(self, key):
        return dict.has_key(self, key.lower())

    def get(self, key, def_val=None):
        return dict.get(self, key.lower(), def_val)

    def setdefault(self, key, def_val=None):
        return dict.setdefault(self, key.lower(), def_val)

    def update(self, other):
        for k,v in other.items():
            dict.__setitem__(self, k.lower(), v)

    def fromkeys(self, iterable, value=None):
        d = CaselessDict()
        for k in iterable:
            dict.__setitem__(d, k.lower(), value)
        return d

    def pop(self, key, def_val=None):
        return dict.pop(self, key.lower(), def_val)
    
    
def make_color_table(in_class,use_name=False):
    """Build a set of color attributes in a class.

    Helper function for building the *TermColors classes."""
    
    color_templates = (
        ("Black"       , "0;30"),
        ("Red"         , "0;31"),
        ("Green"       , "0;32"),
        ("Brown"       , "0;33"),
        ("Blue"        , "0;34"),
        ("Purple"      , "0;35"),
        ("Cyan"        , "0;36"),
        ("LightGray"   , "0;37"),
        ("DarkGray"    , "1;30"),
        ("LightRed"    , "1;31"),
        ("LightGreen"  , "1;32"),
        ("Yellow"      , "1;33"),
        ("LightBlue"   , "1;34"),
        ("LightPurple" , "1;35"),
        ("LightCyan"   , "1;36"),
        ("White"       , "1;37"),  )

    for name,value in color_templates:
        if use_name:
            setattr(in_class,name,in_class._base % name)
        else:
            setattr(in_class,name,in_class._base % value)

class TermColors:
    """Color escape sequences.

    This class defines the escape sequences for all the standard (ANSI?) 
    colors in terminals. Also defines a NoColor escape which is just the null
    string, suitable for defining 'dummy' color schemes in terminals which get
    confused by color escapes.

    This class should be used as a mixin for building color schemes.
    
    Basicaly this class is just a copy of IPython.ColorANSI.TermColors class"""
    
    NoColor = ''  # for color schemes in color-less terminals.
    Normal = '\033[0m'   # Reset normal coloring
    _base  = '\033[%sm'  # Template for all other colors

class HTMLColors:
    
    NoColor = ''
    Normal = '</font>'
    _base = '<font color=%s>'

# Build the actual color table as a set of class attributes:
make_color_table(TermColors)
make_color_table(HTMLColors,True)

TermTangoColors = { PyTango.DevState.ON      : TermColors.Green,
                    PyTango.DevState.ALARM   : TermColors.Brown,
                    PyTango.DevState.FAULT   : TermColors.Red,
                    PyTango.DevState.UNKNOWN : TermColors.LightGray,
                    None                     : TermColors.DarkGray}

HTMLTangoColors = { PyTango.DevState.ON      : HTMLColors.Green,
                    PyTango.DevState.ALARM   : HTMLColors.Brown,
                    PyTango.DevState.FAULT   : HTMLColors.Red,
                    PyTango.DevState.UNKNOWN : HTMLColors.LightGray,
                    None                     : HTMLColors.DarkGray}

class circbuf:
    """A circular buffer of Python values.

    Examples::
        >>> cb = circbuf(3)
        >>> cb.is_empty()
        1
        >>> cb.put('first')
        >>> cb.is_empty()
        0
        >>> cb.put('second')
        >>> cb.put('third')
        >>> cb.is_full()
        1
        >>> cb.put('fourth')
        >>> cb.get()
        'second'
        >>> cb.get()
        'third'
        >>> cb.get()
        'fourth'
        >>> cb.is_empty()
        1

    inv::
        # there can be from 0 to max items inclusive on the buffer
        0 <= self.len <= len(self.buf)

        # g is a valid index into buf
        0 <= self.g < len(self.buf)

        # p is also a valid index into buf
        0 <= self.p < len(self.buf)

        # there are len items between get and put
        (self.p - self.g) % len(self.buf) == self.len % len(self.buf)
    """
    def __init__(self, leng):
        """Construct an empty circular buffer.

        pre::
            leng > 0
        post[self]::
            self.is_empty() and len(self.buf) == leng
        """

        self.buf = leng*[None]
        self.len = self.g = self.p = 0

    def is_empty(self):
        """Returns true only if circbuf has no items.

        post[]::
            __return__ == (self.len == 0)
        """
        return self.len == 0

    def is_full(self):
        """Returns true only if circbuf has no space.

        post[]::
            __return__ == (self.len == len(self.buf))
        """
        return self.len == len(self.buf)

    def get(self):
        """Retrieves an item from a non-empty circular buffer.

        pre::
            not self.is_empty()
        post[self.len, self.g]::
            self.len == __old__.self.len - 1
            __return__ == self.buf[__old__.self.g]
        """
        result = self.buf[self.g]
        self.g = (self.g + 1) % len(self.buf)
        self.len -= 1
        return result

    def put(self, item):
        """Puts an item onto a circular buffer.

        post[self.len, self.p, self.g, self.buf]::
            # if the circbuf was full on entry, then an entry
            # was bumped
            implies(__old__.self.len == len(self.buf),
                    self.len == __old__.self.len,
                    self.len == __old__.self.len + 1 and \
                      self.g == __old__.self.g)

            # item is now in the buffer
            self.buf[__old__.self.p] == item
            # but no other part of the buffer has changed
            self.buf[:__old__.self.p] == __old__.self.buf[:__old__.self.p]
            self.buf[__old__.self.p+1:] == __old__.self.buf[__old__.self.p+1:]
            __return__ is None
        """
        self.buf[self.p] = item
        self.p = (self.p + 1) % len(self.buf)
        if self.len == len(self.buf):
            self.g = self.p
        else:
            self.len += 1

def _test():
    import doctest, circbuf
    return doctest.testmod(circbuf)

if __name__ == '__main__':
    _test()
