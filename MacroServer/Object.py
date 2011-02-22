class Object:

    def __init__(self):
        pass
        
    def call__init__(self, klass, *args, **kw):
        if 'inited_class_list' not in self.__dict__:
            self.inited_class_list = []

        if klass not in self.inited_class_list:
            self.inited_class_list.append(klass)
            klass.__init__(self, *args, **kw)

    def getAttrDict(self):
        attr = dict(self.__dict__)
        if 'inited_class_list' in attr:
            del attr['inited_class_list']
        return attr
        
    def updateAttrDict(self, other):
        attr = other.getAttrDict()
        self.__dict__.update(attr)
        
