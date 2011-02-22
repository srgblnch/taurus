from utils import CaselessDict

class Moveable:
    """ An item that can 'move'. In order to move it you need to provide a list
    of values (normally interpreted as motor positions).
    Therefore this Moveable can represent a single motor, a group of motors"""
    def __init__(self):
        pass
    
    def startMove(self, new_pos, timeout=None):
        """ startMove(list<double> new_pos, double timeout=None) -> None 
        
        Calling this method will trigger a movement off all components of
        the movement."""
        pass

    def waitMove(self, timeout=None):
        """ waitMove(double timeout=None) -> None
        
        Waits for the movement to end. If no movement is active the method
        returns immediately."""
        pass

    def move(self, new_pos, timeout=None):
        """ move(list<double> new_pos, timeout=None) -> None 
        
        Triggers a movement and waits for it to end.
        
        """
        self.startMove(new_pos, timeout=timeout)
        self.waitMove(timeout=timeout)

    def abort(self, wait_ready=True, timeout=None):
        pass
    
    def readPosition(self):
        pass
    
    def getMoveableSource(self):
        return None
    
    def getSize(self):
        return 0
    
    def getIndex(self, name):
        pass
    
class MoveableSource:
    """ A container of Moveable items. """
    
    def __init__(self):
        pass
    
    def getMoveable(self, names):
        """ getMoveable(list<string> names) -> Moveable 

        Returns a Moveable object that handles all the moveable items given in 
        names.
        """
        pass

class Motion(Moveable):
    """ A motion object """
    
    def __init__(self, elements, moveable_srcs, allow_repeat=False, 
                 allow_unknown=False, read_only=False):
        self.read_only = read_only
        
        if len(elements) == 0:
            raise Exception("Motion with no elements")
        
        first_elem = elements[0]
        
        if isinstance(first_elem,str):
            self.init_by_names(elements, moveable_srcs, allow_repeat, allow_unknown)
        elif isinstance(first_elem,Moveable):
            self.init_by_movables(elements, moveable_srcs, allow_repeat, allow_unknown)
        else:
            raise Exception("Invalid parameter 'elements'")
    
    def init_by_movables(self, elements, moveable_srcs, allow_repeat, allow_unknown):
        # TODO: Optimize this. Dont call init_by_names. It its possible to do it
        # manually with some performance gain
        names = [ elem.name for elem in elements]
        self.init_by_names(names, moveable_srcs, allow_repeat, allow_unknown)
    
    def init_by_names(self, names, moveable_srcs, allow_repeat, allow_unknown):

        ms_elem_names = self.getElemNamesByMoveableSource(names, moveable_srcs, 
                                                         allow_repeat=allow_repeat, 
                                                         allow_unknown=allow_unknown)
        
        # map<MoveableSource, Moveable>
        ms_moveables = {}
        for moveable_source, ms_names in ms_elem_names.items():
            ms_moveables[moveable_source] = moveable_source.getMoveable(ms_names)

        # list<Moveable>
        moveable_list = ms_moveables.values()
        
        # list<tuple(int moveable_index, int position_index)>
        pos_to_moveable = len(names)*[None,]
        
        for i, name in enumerate(names):
            moveable_index, position_index = -1, -1
            for j, moveable in enumerate(moveable_list):
                position_index = moveable.getIndex(name)
                if position_index >= 0:
                    pos_to_moveable[i] = (j, position_index)
                    break
        
        # list<string> moveable item names
        self.names = names
        
        # list<Moveable>
        self.moveable_list = moveable_list
        
        # list<tuple(int moveable_index, int position_index)>
        # the list index itself is the position index for this motion
        # the tuple for a given index gives the corresponding moveable index and
        # the position index inside the moveable object 
        self.pos_to_moveable = pos_to_moveable
        
        # map<MoveableSource, Moveable>
        self.ms_moveables = ms_moveables
    
        # a buffer for positions for performance reasons
        #list<list<double>> index of outer list is the moveable index. The 
        #contents of the list are the ordered positions for that moveable 
        pos_buff = []
        total_count = 0
        for m in moveable_list:
            count = m.getSize()
            total_count += count
            pos_buff.append(m.getSize()*[0.0,])
        self.pos_buff = pos_buff
        
        assert(len(self.pos_to_moveable) == len(self.names))
        assert(total_count == len(self.names))
    
    def getElemNamesByMoveableSource(self, names, moveable_sources, 
                                    allow_repeat,
                                    allow_unknown):
        """ getElemNamesByMoveableSource(list<string>names, 
                                        list<MoveableSource> moveable_sources, 
                                        bool allow_repeat, bool allow_unknown)
        
        Organizes the elements by moveable source. The result is a dictionary
        with key being the MoveableSource and data a list of the names that 
        belong to the that motion source.
        """ 

        ms_elems = {}
        
        for name in names:
            moveable = None
            
            for moveable_source in moveable_sources:
                 moveable = moveable_source.getMoveable([name])
                 if not moveable is None:
                     if not ms_elems.has_key(moveable_source):
                         ms_elems[moveable_source] = []
                     moveable_source_moveables = ms_elems.get(moveable_source)
                     present = name in moveable_source_moveables
                     if not present or (present and allow_repeat):
                         moveable_source_moveables.append(name)
                         break
                     elif present and not allow_repeat:
                         raise Exception("Moveable item %s appears more than once" % name)
            if moveable is None and not allow_unknown:
                 raise Exception("Moveable item %s not found" % name)
        return ms_elems
        
    def startMove(self, pos_list, timeout=None):
        if self.read_only:
            raise Exception("Trying to move read only motion")
        
        if len(pos_list) != len(self.names):
            raise Exception("Invalid number of position values")
        
        buff = self.pos_buff
        for i, pair in enumerate(self.pos_to_moveable):
            pos = pos_list[i]
            buff[pair[0]][pair[1]] = pos 

        for i, moveable in enumerate(self.moveable_list):
            moveable.startMove(buff[i], timeout=timeout)

    def waitMove(self, timeout=None):
        for moveable in self.moveable_list:
            moveable.waitMove(timeout=timeout)

    def move(self, new_pos, timeout=None):
        self.startMove(new_pos, timeout=timeout)
        self.waitMove(timeout=timeout)

    def readPosition(self):
        moveable_pos_list = [ m.readPosition() for m in self.moveable_list ]
        pos = [ moveable_pos_list[pair[0]][pair[1]] for pair in self.pos_to_moveable ]
        return pos
    
    def abort(self, wait_ready=True, timeout=None):
        for moveable in self.moveable_list:
            moveable.abort(wait_ready=wait_ready, timeout=timeout)
    
    def read(self):
        pass

#-------------------------------------------------------------------------------
# Small test framework
#-------------------------------------------------------------------------------

class PoolMoveableTest(Moveable):
    def __init__(self, name, elems, moveable_src, mov_items = None):
        self.name = name
        self.elem_names = elems
        self.moveable_src = moveable_src
        self.pos = len(elems)*[0.0,]
        
    def startMove(self, new_pos, timeout=None):
        #print "[MG %s] startMove(%s)" % (self.name, ", ".join([str(x) for x in new_pos]))
        self.pos = new_pos 
        pass

    def waitMove(self, timeout=None):
        #print "[MG %s] waitMove()" % (self.name)
        pass

    def abort(self, wait_ready=True, timeout=None):
        #print "[MG %s] abort()" % (self.name)
        pass
    
    def readPosition(self):
        #print "[MG %s] read() = %s" % (self.name,str(self.pos))
        return self.pos
    
    def getMoveableSource(self):
        return self.moveable_src
    
    def getSize(self):
        return len(self.elem_names)
    
    def getIndex(self, name):
        try:
            return self.elem_names.index(name)
        except:
            return -1
       
class PoolMSTest(MoveableSource):

    def __init__(self, initial_elems):
        MoveableSource.__init__(self)
        self.elem_names = initial_elems
        self.moveable_inc = 0
        
        self.motors = CaselessDict()
        for elem_name in initial_elems:
            self.motors[elem_name] = PoolMoveableTest(elem_name, [elem_name], self)
        self.motor_group = None
        
    def getMoveable(self,names):
        if len(names) == 1:
            return self.motors.get(names[0])
        l = [name for name in self.elem_names if name in names]
        self.motor_group = PoolMoveableTest("moveable %d" % self.moveable_inc, l, self)
        self.moveable_inc += 1
        return self.motor_group

def test():
    ms1 = PoolMSTest(["m1","m2","m3","m4"])
    ms2 = PoolMSTest(["m5","m6","m7","m8"])
    ms3 = PoolMSTest(["m9","m10","m11","m12"])
    
    motion1 = Motion(["m1"], [ms1, ms2, ms3])
    
    # some tests
    for p in [[5.0], [20.0], [0.0], [-14.5], [0.0]]:
        motion1.move(p)
        assert(ms1.motors["m1"].pos[0] == p[0])
        assert(motion1.readPosition() == p)

    motion2 = Motion(["m1","m2","m3"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0, 0.0], [-14.5, 3.32, 232], [0.0, 0.0, 0.0]]:
        motion2.move(p)
        assert(ms1.motor_group.pos == p)
        assert(motion2.readPosition() == p)
    
    motion3 = Motion(["m3","m1","m2"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0, 0.0], [-14.5, 3.32, 232], [0.0, 0.0, 0.0]]:
        motion3.move(p)
        assert(ms1.motor_group.pos[0] == p[1])
        assert(ms1.motor_group.pos[1] == p[2])
        assert(ms1.motor_group.pos[2] == p[0])
        assert(motion3.readPosition() == p)

    motion4 = Motion(["m2","m6"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0], [-14.5, 3.32], [0.0, 0.0]]:
        motion4.move(p)
        assert(ms1.motors["m2"].pos[0] == p[0])
        assert(ms2.motors["m6"].pos[0] == p[1])
        assert(motion4.readPosition() == p)

    motion5 = Motion(["m3","m1","m7"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0, 0.0], [-14.5, 3.32, 232], [0.0, 0.0, 0.0]]:
        motion5.move(p)
        assert(ms1.motor_group.pos[1] == p[0])
        assert(ms1.motor_group.pos[0] == p[1])
        assert(ms2.motors["m7"].pos[0] == p[2])
        assert(motion5.readPosition() == p)
    
    motion6 = Motion(["m4", "m9", "m10", "m2"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0, 0.0, 21.0], [-14.5, 3.32, 232, 45.4], [0.0, 0.0, 0.0, 0.0]]:
        motion6.move(p)
        assert(ms1.motor_group.pos[1] == p[0])
        assert(ms1.motor_group.pos[0] == p[3])
        assert(ms3.motor_group.pos[0] == p[1])
        assert(ms3.motor_group.pos[1] == p[2])
        assert(motion6.readPosition() == p)
    
    motion7 = Motion(["m4", "m9", "m7", "m2"], [ms1, ms2, ms3])
    for p in [[5.0, 20.0, 0.0, 21.0], [-14.5, 3.32, 232, 45.4], [0.0, 0.0, 0.0, 0.0]]:
        motion7.move(p)
        assert(ms1.motor_group.pos[1] == p[0])
        assert(ms1.motor_group.pos[0] == p[3])
        assert(ms3.motors["m9"].pos[0] == p[1])
        assert(ms2.motors["m7"].pos[0] == p[2])
        assert(motion7.readPosition() == p)
    
    try:
        m = Motion(["m1", "m2"], [ms1, ms2, ms3], read_only=True)
        m.startMove([0.5, 20.4])
    except Exception, e:
        assert(e.message == "Trying to move read only motion")
    
    try:
        m = Motion(["m1", "m1"], [ms1, ms2, ms3])
    except Exception, e:
        assert(e.message == "Moveable item m1 appears more than once")
    
    try:
        m = Motion(["m1", "m999"], [ms1, ms2, ms3])
    except Exception, e:
        assert(e.message == "Moveable item m999 not found")
    
if __name__ == "__main__":
    test()    
    
    