import PyTango


class Callable:
    def __init__(self, callable_obj):
        self.__call__ = callable_obj
        

class PyDeviceClass(PyTango.PyDeviceClass):

    class_dict = {}

    def get_class_data(klass, class_dict=class_dict):
        if klass not in class_dict.keys():
            class_dict[klass] = {'Devices': {}, 'Name': 'Unknown'}
        return class_dict[klass]

    get_class_data = Callable(get_class_data)
    
    def __init__(self, *arg, **kw):
        apply(PyTango.PyDeviceClass.__init__, [self] + list(arg), kw)
        class_data = self.get_instance_class_data()
        class_data['Instance'] = self
        class_data['Name'] = self.class_name

    def get_instance_class_data(self, klass=None):
        if klass is None:
            klass = self.__class__
        return self.get_class_data(klass)

    def get_device_list(self):
        return self.get_instance_class_data()['Devices']

    def add_device(self, dev):
        dev_list = self.get_device_list()
        dev_name = dev.get_name()
        if dev_name in dev_list.keys():
            raise RuntimeError, 'Class %s already has device %s' % \
                  (self.get_instance_class_data()['Name'], dev_name)
        dev_list[dev_name] = dev
        
    def remove_device(self, dev):
        dev_list = self.get_device_list()
        del dev_list[dev.get_name()]

    def get_class_instance(self, name):
        for data in self.class_dict.values():
            if data['Name'] == name:
                return data['Instance']
        return None
    
    def get_instance_class_name(self):
        return self.get_instance_class_data()['Name']
