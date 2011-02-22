""" Pseudo counter system file"""

class PseudoCounterController:
    """The abstract class for the Python Pseudo Counter.
       Every Pseudo Counter implementation must be a subclass of this class.
       Current procedure for a correct implementation of a Pseudo Counter class:
        - mandatory: define the class level attributes counter_roles, 
                     and class_prop (if any).
        - mandatory: write calc(self,counter_values) method"""

    counter_roles = ()
    class_prop = {}
    
    def __init__(self, inst, inst_prop):
        self.inst_name = inst;
        
        for class_prop_key in self.class_prop:
            #get the value from the instance 
            value = inst_prop.get(class_prop_key)
            
            #if the value does't exist get it from the instance dict
            if value == None:
                value = self.class_prop.get(class_prop_key).get('DefaultValue')
                
                #if the value does't exist in the instance dict -> serious error
                if value == None:
                    raise Exception("property %s must be defined at instance "
                                    "level." % class_prop_key)
            
            #add the attribute
            setattr(self, class_prop_key, value)

    @classmethod
    def get_MaxDevice(cls):
        return cls.get_counter_nb()
                
    @classmethod
    def get_counter_nb(cls):
        """Returns the number of physical counters that an instance of a Pseudo
           Counter controller class must have."""
        return len(cls.counter_roles)
    
    @classmethod
    def get_counter_role(cls, idx):
        """Given a physical counter number, it will return it's description."""
        return cls.counter_roles[idx]
    
    @classmethod
    def get_properties(cls):
        """Returns all properties defined for this class"""
        return cls.class_prop
    
    @classmethod
    def get_property_descr(cls, name):
        """Returns the description of the given property"""
        return cls.class_prop[name]
    
    @classmethod
    def get_class_info(cls):
        """Returns a list with the complete information about the pseudo motor 
           class in the following format:
             ret[0] - class description
             ret[1] - number of physical counters = M
             ret[2...M+1] - physical counter roles
             ret[M+2] - number of properties = P
             ret[M+3...M+(P*4)+2] - properties description
           
           Each property description consists of four strings:
             ret[i] - name
             ret[i+1] - type
             ret[i+2] - description
             ret[i+3] - default value
           
           The default value is None if not defined. When existing, the data type of the default conforms to the corresponding type:
           /------------------------------|--------------------------------------------------------------------------------\
           |        Tango type            |                                 Python type                                    |
           |------------------------------|--------------------------------------------------------------------------------|
           | PyTango.DevBoolean           | bool  (types.BooleanType)                                                      |
           | PyTango.DevLong              | long  (types.LongType)                                                         |
           | PyTango.DevDouble            | float (types.FloatType)                                                        |
           | PyTango.DevString            | str   (types.StringType)                                                       |
           | PyTango.DevVarBooleanArray   | tuple or list (types.TupleType or types.ListType) of bool (types.BooleanType)  |
           | PyTango.DevVarLongArray      | tuple or list (types.TupleType or types.ListType) of long (types.LongType)     |
           | PyTango.DevVarDoubleArray    | tuple or list (types.TupleType or types.ListType) of float (types.FloatType)   |
           | PyTango.DevVarStringArray    | tuple or list (types.TupleType or types.ListType) of string (types.StringType) |
           \------------------------------|--------------------------------------------------------------------------------/
        """
        lst = cls.get_pseudo_class_info()
        
        lst.append(len(cls.class_prop))
        
        for key in cls.class_prop:
            value_dict = cls.class_prop.get(key)
            lst.append(key)
            lst.append(value_dict.get('Type'))
            lst.append(value_dict.get('Description'))
            lst.append(value_dict.get('DefaultValue'))
        
        return lst
    
    @classmethod
    def get_pseudo_class_info(cls):
        """Returns a list with the information (except properties information) 
           about the pseudo motor class in the following format:
             ret[0] - class description
             ret[1] - number of physical counters = M
             ret[2...M+1] - physical counter roles
         """        
        lst = [cls.__doc__, len(cls.counter_roles)]
        
        for counter_role in cls.counter_roles:
            lst.append(counter_role)
        
        return lst
    
    def get_info(self):
        """Returns a list with the complete information about the pseudo motor 
           class in the following format:
             ret[0] - class description
             ret[1] - number of physical motors = M
             ret[2...M+2] - physical motor roles
             ret[M+3] - number of properties = P
             ret[M+4...M+(P*4)+3] - properties description
             
             @see get_class_info() for more information on the format of the 
                  properties
         """
        lst = self.get_pseudo_class_info()
        
        lst.append(len(self.class_prop))
        
        for key in self.class_prop:
            value_dict = self.class_prop.get(key)
            lst.append(key)
            lst.append(value_dict.get('Type'))
            lst.append(value_dict.get('Description'))
            lst.append(getattr(self, key))
        
        return lst
    
    def calc(self, counter_values):
        """Calculate pseuco counter value given the counter values"""
        raise Exception("cal must be redefined")
            
if __name__ == '__main__':
    print PseudoCounterController.__doc__
