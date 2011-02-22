
class MotorController:

    class_prop = {}

    def __init__(self,inst,props):
        self.inst_name = inst

        for prop_key in props:
           setattr(self,prop_key,props.get(prop_key))
