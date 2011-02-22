""" The standard pseudo motor controller library for the device pool """

from pool import PseudoMotorController

class PseudoMotorProxy(PseudoMotorController):
    """ The simplest pseudo motor you can have: It just forwards the physical 
        motor requests"""
    
    pseudo_motor_roles = ("proxy",)
    motor_roles = ("motor",)

    def calc_physical(self,index,pseudo_pos):
        return pseudo_pos[0]
    
    def calc_pseudo(self,index,physical_pos):
        return physical_pos[0]
    
    
class Slit(PseudoMotorController):
    """A Slit pseudo motor controller for handling gap and offset pseudo 
       motors. The system uses to real motors sl2t (top slit) and sl2b (bottom
       slit)"""
    
    gender = "Slit"
    model  = "Default Slit"
    organization = "CELLS - ALBA"
    image = "slit.png"
    logo = "ALBA_logo.png"

    pseudo_motor_roles = ("Gap", "Offset")
    motor_roles = ("sl2t", "sl2b")
                           
    def calc_physical(self,index,pseudo_pos):
        half_gap = pseudo_pos[0]/2.0
        if index == 1:
            return pseudo_pos[1] - half_gap
        else:
            return pseudo_pos[1] + half_gap
    
    def calc_pseudo(self,index,physical_pos):
        if index == 1:
            return physical_pos[1] - physical_pos[0]
        else:
            return (physical_pos[0] + physical_pos[1])/2.0
        
        
class BasePower(PseudoMotorController):
    """A pseudo motor system for controlling base and power for 
       the \"real motor\"."""

    pseudo_motor_roles = ("Base", "Power")
    motor_roles = ("Real",)

    def calc_physical(self,index,pseudo_pos):
        return pseudo_pos[0] * (10 ** pseudo_pos[1])
    
    def calc_pseudo(self,index,physical_pos):
        base = physical_pos[0]
        power = 0
        if base >= 1:
            while base >= 10:
                base /= 10.0
                power += 1
        else :
            while base < 1:
                base *= 10.0
                power -= 1
        
        if index == 1:
            return base
        else:
            return power
            
            
class PseudoTheta(PseudoMotorController):
    """A pseudo motor for the theta and 2theta motors."""

    pseudo_motor_roles = ("pm_theta",)
    motor_roles = ("theta","2theta")

    def calc_physical(self,index,pseudo_pos):
        if index == 0:
            return pseudo_pos[0]
        elif index == 1:
            return 2 * pseudo_pos[0]
        return 0
        
    def calc_pseudo(self,index,physical_pos):
        print "calc_pseudo "+str(index)+" "+str(physical_pos)
        if index == 0:
            return physical_pos[0]
        elif index == 1:
            return physical_pos[1] / 2
        return 0