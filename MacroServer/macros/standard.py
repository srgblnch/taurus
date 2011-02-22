from Macro    import *

import time

################################################################################
#
# Motion related macros
#
################################################################################

class lsm(Macro):
    """Lists all existing motors"""
    
    def run(self):

        self.all_mot = self.findObjs('.*', type_class=Type.Motor)

        nr_mot = len(self.all_mot)
        if nr_mot == 0:
            self.output('No motors defined')
            return
        
        out = List(['Name', 'Controller', 'Axis'])
        
        for mot in self.all_mot:
            out.appendRow([mot.getName(), mot.info.ctrl_name,
                           mot.info.ctrl_idx])
        for line in out.genOutput():
            self.output(line)

class _wm(Macro):
    """Show motor positions"""

    param_def = [
        ['motor_list',
         ParamRepeat(['motor', Type.Motor, None, 'Motor to move']),
         None, 'List of motor to show'],
    ]

    def prepare(self, *motor_list, **opts):
        self.table_opts = opts
    
    def run(self, *motor_list):
        motor_width = 9
        motor_names = []
        motor_pos   = []
        for motor in motor_list:
            name = motor.getName()
            motor_names.append([name])
            pos_data = [motor.getUserPos(), motor.getDialPos()]
            motor_pos.append(pos_data)
            motor_width = max(motor_width,len(name))

        fmt = '%c*.%df' % ('%',motor_width - 5)

        table = Table(motor_pos, elem_fmt=[fmt],
                      col_head_str=motor_names, col_head_width=motor_width,
                      **self.table_opts)
        for line in table.genOutput():
            self.output(line)
            
class wa(Macro):
    """Show all motor positions"""

    def prepare(self, **opts):
        self.all_motors = self.findObjs('.*', type_class=Type.Motor)
        self.table_opts = opts
    
    def run(self):
        nr_motors = len(self.all_motors)
        if nr_motors == 0:
            self.output('No motor defined')
            return

        self.output('Current Positions  (user, dial)')
        self.output('')
        
        self.execMacro('_wm',*self.all_motors)
        
class pwa(Macro):
    """Show all motor positions in a pretty table"""

    def run(self):
        self.execMacro('wa', **Table.PrettyOpts)


class set_lim(Macro):
    """Sets the software limits on the specified motor"""
    param_def = [
        ['motor', Type.Motor, None, 'Motor name'],
        ['low',   Type.Float, None, 'lower limit'],
        ['high',   Type.Float, None, 'upper limit']
    ]
    
    def run(self, motor, low, high):
        name = motor.getName()
        self.debug("Setting user limits for %s" % name)
        motor.setUserLimits(low,high)
        self.output("%s limits set to %.4f %.4f (user units)" % (name, low, high))

    
class set_lm(Macro):
    """Sets the dial limits on the specified motor"""
    param_def = [
        ['motor', Type.Motor, None, 'Motor name'],
        ['low',   Type.Float, None, 'lower limit'],
        ['high',   Type.Float, None, 'upper limit']
    ]
    
    def run(self, motor, low, high):
        name = motor.getName()
        self.debug("Setting dial limits for %s" % name)
        motor.setDialLimits(low,high)
        self.output("%s limits set to %.4f %.4f (dial units)" % (name, low, high))


class set_pos(Macro):
    """Sets the position of the motor to the specified value"""
    
    param_def = [
        ['motor', Type.Motor, None, 'Motor name'],
        ['pos',   Type.Float, None, 'Position to move to']
    ]
    
    def run(self, motor, pos):
        name = motor.getName()
        old_pos = motor.getUserPos()
        motor.definePosition(pos)
        self.output("%s reset from %.4f to %.4f" % (name, old_pos, pos))


class motor_par(Macro):
    """sets configuration parameters for motor 'motor'.
       Generic recognized values for the string 'par' follow.
        "Step_per_unit"
            returns the current step-size parameter. The units are generally in 
            steps per degree or steps per millimeter. If val is given, then the 
            parameter is set to that value.

        "Acceleration" (R/W)
            returns the value of the current acceleration parameter. The units 
            of acceleration are the time in milliseconds for the motor to 
            accelerate to full speed. If val is given, then the acceleration is 
            set to that value.
        
        "Decceleration" (R/W)
            returns the value of the current deceleration parameter. The units 
            of deceleration are the time in milliseconds for the motor to stop. 
            If val is given, then the deceleration is set to that value.

        "Base_rate" (R/W)
            returns the current base-rate parameter. The units are steps per 
            second. If val is given, then the base rate is set to that value.
        
        "Velocity" (R/W)
            returns the current steady-state velocity parameter. The units are 
            steps per second. If val is given, then the steady-state velocity 
            is set to that value.
        
        "Backlash" (R/W)
            returns the value of the backlash parameter. Its sign and magnitude 
            determine the direction and extent of the motor's backlash 
            correction. If val is given, then the backlash is set to that value. 
            Setting the backlash to zero disables the backlash correction.
        
        "Controller" (R)
            returns the name of the controller for the given motor.
    """
    
    param_def = [
        ['motor', Type.Motor, None, 'Motor name'],
        ['par',
         ParamRepeat(['par',Type.MotorParam, None, 'parameter name'], min=0, max=1),
         None, 'parameter name'],
        ['val',   
         ParamRepeat(['val', Type.Float, None, 'new value'], min=0, max=1), 
         None, 'new value']
    ]
    
    def getAllValues(self,motor):    
        attr_name_list = self.getManager().getType("MotorParam").getItemList()
        for attr_name in attr_name_list:
            self.getValue(motor,attr_name)
    
    def getValue(self,motor,par):
        par_lower = par.lower()
        if par_lower == "controller":
            self.output("%15s = %s" % ("Controller",motor.getControllerName()))
        else:
            attr = motor.getAttrObj(par_lower)
            if not attr is None:
                self.output(str(attr))

    def getSingleValue(self,motor,par):
        par_lower = par.lower()
        if par_lower == "controller":
            self.output("%15s = %s" % ("Controller",motor.getControllerName()))
        else:
            attr = motor.getAttrObj(par_lower)
            if not attr is None:
                self.output(attr.getFullStr())
    
    def setValue(self,motor,par,val):
        name = motor.getName()
        attr = motor.getAttrObj(par)
        if attr is None:
            self.error(name + " does not have " + par)
            return
        attr.write(val)
        self.output("%s set to %.4f on %s" % (par,val,name))
        
    def run(self, motor, *pars):
        
        par = pars[0]
        val = pars[1]

        if par is None:
            self.getAllValues(motor)
        else:
            if val is None:
                self.getSingleValue(motor,par)
            else:
                self.setValue(motor,par,val)
                    
class wm(Macro):
    """Show the position of the specified motors"""

    param_def = [
        ['motor_list',
         ParamRepeat(['motor', Type.Motor, None, 'Motor to move']),
         None, 'List of motor to show'],
    ]

    def prepare(self, *motor_list, **opts):
        self.table_opts = opts
        
    def run(self, *motor_list):
        motor_width = 10
        motor_names = []
        motor_pos   = []
        
        for motor in motor_list:
            name = motor.getName()
            motor_names.append([name] * 2)
            lims = motor.getStrUserLimits()
            upos = [lims[1], motor.getStrUserPos(), lims[0]]
            lims = motor.getStrDialLimits()
            dpos = [lims[1], motor.getStrDialPos(), lims[0]]
            pos_data = [''] + upos + [''] + dpos
            
            motor_pos.append(pos_data)

        elem_fmt = (['%*s'] + ['%*s'] * 3) * 2
        row_head_str = ['User', ' High', ' Current', ' Low',
                        'Dial', ' High', ' Current', ' Low']
        table = Table(motor_pos, elem_fmt=elem_fmt, row_head_str=row_head_str,
                      col_head_str=motor_names, col_head_width=motor_width,
                      **self.table_opts)
        for line in table.genOutput():
            self.output(line)


class pwm(Macro):
    """Show the position of the specified motors in a pretty table"""

    param_def = [
        ['motor_list',
         ParamRepeat(['motor', Type.Motor, None, 'Motor to move']),
         None, 'List of motor to show'],
    ]

    def run(self, *motor_list):
        self.execMacro('wm', *motor_list, **Table.PrettyOpts)

      
class mv(Macro):
    """Move motor(s) to the specified position(s)"""

    param_def = [
       ['motor_pos_list',
        ParamRepeat(['motor', Type.Motor, None, 'Motor to move'],
                    ['pos',   Type.Float, None, 'Position to move to']),
        None, 'List of motor/position pairs'],
    ]

    def run(self, *motor_pos_list):
        motors, positions = [], []
        for motor, pos in motor_pos_list:
            motors.append(motor)
            positions.append(pos)
            self.debug("Starting %s movement to %s", motor.getName(), pos)
        motion = self.getMotion(motors)
        motion.startMove(positions)
        motion.waitMove()


class mvr(Macro):
    """Move motor(s) relative to the current position(s)"""

    param_def = [
       ['motor_disp_list',
        ParamRepeat(['motor', Type.Motor, None, 'Motor to move'],
                    ['disp',  Type.Float, None, 'Relative displacement']),
        None, 'List of motor/displacement pairs'],
    ]

    def run(self, *motor_disp_list):
        motor_pos_list = []
        for motor, disp in motor_disp_list:
            pos = motor.getUserPos() + disp
            motor_pos_list.append([motor, pos])
        self.execMacro('mv', *motor_pos_list)


class umv(Macro):
    """Move motor(s) to the specified position(s) and update"""

    param_def = [
       ['motor_pos_list',
        ParamRepeat(['motor', Type.Motor, None, 'Motor to move'],
                    ['pos',   Type.Float, None, 'Position to move to']),
        None, 'List of motor/position pairs'],
    ]

    def prepare(self, *motor_pos_list, **opts):
        self.all_names = []
        self.all_pos = []
        idx = 0
        self.print_pos = False
        self.first_time = True
        for motor, pos in motor_pos_list:
            self.all_names.append([motor.getName()])
            pos = motor.getPosObj()
            self.all_pos.append([pos.read()])
            pos.subscribeEvent(self.positionChanged, motor)
        
    def run(self, *motor_pos_list):
        self.print_pos = True
        
        self.execMacro('mv', *motor_pos_list)

        for motor, pos in motor_pos_list:
            pos = motor.getPosObj()
            pos.unsubscribeEvent(self.positionChanged, motor)
        self.printAllPos()
        
    def positionChanged(self, motor, position):
        idx = self.all_names.index([motor.getName()])
        self.all_pos[idx] = [position]
        if self.print_pos:
            self.printAllPos()

    def printAllPos(self):
        if not self.first_time:
            self.goLogBlockStart()
        self.setLogBlockStart()
        self.first_time = False
        motor_width = 10
        table = Table(self.all_pos, elem_fmt=['%*.4f'],
                      col_head_str=self.all_names, col_head_width=motor_width)
        for line in table.genOutput():
            self.output(line)
        self.flushOutput()
        

class umvr(Macro):
    """Move motor(s) relative to the current position(s) and update"""

    param_def = [
       ['motor_disp_list',
        ParamRepeat(['motor', Type.Motor, None, 'Motor to move'],
                    ['disp',  Type.Float, None, 'Relative displacement']),
        None, 'List of motor/displacement pairs'],
    ]

    def run(self, *motor_disp_list):
        motor_pos_list = []
        for motor, disp in motor_disp_list:
            pos = motor.getUserPos() + disp
            motor_pos_list.append([motor, pos])
        self.execMacro('umv', *motor_pos_list)
         

################################################################################
#
# Data acquisition related macros
#
################################################################################


class ct(Macro):
    """Count for the specified time on the active measurement group"""

    param_def = [
       ['integ_time', Type.Float, 1.0, 'Integration time']
    ]

    def prepare(self, integ_time, **opts):
        name = '.*'
        
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs(name, type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break

        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        
        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            self.channels.append( ch_attr ) 
        
        
    def run(self, integ_time):
        self.debug("Counting for %s sec", integ_time)
        self.flushOutput()
        
        self.outputDate()
        self.output('')
        
        self.mnt_grp.setIntegrationTime(integ_time)
        self.mnt_grp.startCount()
        self.mnt_grp.waitCount()
        
        counters = []
        names    = []
        counts   = []
        width    = 30

        for channel in self.channels:
            name = channel.name[:channel.name.index("_value")]
            names.append('  ' + name)
            counts.append(channel.read())

        table = Table([counts], row_head_str=names, row_head_fmt='%*s',
                      col_sep='  =  ')
        for line in table.genOutput():
            self.output(line)


class uct(Macro):
    """Count on the active measurement group and update"""

    param_def = [
       ['integ_time', Type.Float, 1.0, 'Integration time']
    ]

    def prepare(self, integ_time, **opts):
        
        self.values = []
        self.names = []
        self.print_value = False
        self.first_time = True
        
        name = '.*'
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs(name, type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break

        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        
        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            self.names.append( [ ch_attr.name[:ch_attr.name.rfind("_value")] ] )
            self.values.append( [ch_attr.read()] ) 
            self.channels.append( ch_attr ) 
            ch_attr.subscribeEvent(self.counterChanged, ch_attr)

    def printAllValues(self):
        if not self.first_time:
            self.goLogBlockStart()
        self.setLogBlockStart()
        self.first_time = False
        ch_width = 10
        table = Table(self.values, elem_fmt=['%*.4f'],
                      col_head_str=self.names, col_head_width=ch_width)
        for line in table.genOutput():
            self.output(line)
        self.flushOutput()
    
    def counterChanged(self, ch_attr, value):
        idx = self.channels.index(ch_attr)
        self.values[idx] = [value]
        if self.print_value:
            self.printAllValues()
        
    def run(self, integ_time):
        self.print_value = True
        self.debug("Counting for %s sec", integ_time)
        self.flushOutput()
        self.mnt_grp.setIntegrationTime(integ_time)
        self.mnt_grp.startCount()
        self.mnt_grp.waitCount()

        for ch_attr in self.mnt_grp.getChannelAttrList():
            ch_attr.unsubscribeEvent(self.counterChanged, ch_attr)
        self.printAllValues()
        
class lsmeas(Macro):
    """List existing measurement groups"""

    def run(self):

        self.all_groups = self.findObjs('.*', type_class=Type.MeasurementGroup)

        active_mntgrp_name = self.getEnv('ActiveMntgrp')
        if not active_mntgrp_name is None:
            active_mntgrp_name = active_mntgrp_name.lower()

        nr_groups = len(self.all_groups)
        if nr_groups == 0:
            self.output('No acq group defined')
            return
        
        active_chr = ' '
        out = List(['Active', 'Name', 'Timer', 'Experim. channels'])
        
        for group in self.all_groups:
            name = group.getName()
            timer_ch = group.getTimer()
            timername = "Not defined"
            if not timer_ch is None:
                timername = timer_ch.getName()

            channels   = group.getChanList()
            chnamelist = []
            for ch in channels:
                 chname = ch.getName()
                 if chname != timername:
                     chnamelist.append(chname)
            active_chr = ' '
            if name.lower() == active_mntgrp_name:
                active_chr = '*'
            out.appendRow([active_chr, name, timername, ', '.join(chnamelist)])
            
        for line in out.genOutput():
            self.output(line)
                

class settimer(Macro):
    """Defines the timer channel for the active measurement group"""
    
    param_def = [
       ['timer', Type.ExpChannel,   None, 'Timer'],
    ]
    
    def run(self,timer):
        
        mntgrp_name = self.getEnv('ActiveMntgrp')
        
        if mntgrp_name is None or len(mntgrp_name) == 0:
            self.output("ActiveMntgrp is not defined or has invalid value.")
            self.output("please define a valid active measurement group before setting a timer")
            return 
        
        mntgrp_list = self.findObjs(mntgrp_name, type_class=Type.MeasurementGroup)
        
        if mntgrp_list == None or len(mntgrp_list) != 1:
            self.output("ActiveMntgrp is not valid.")
            self.output("please define a valid active measurement group before setting a timer")
        
        mntgrp = mntgrp_list[0]
        
        try:
            timer_name = timer.getName()
            mntgrp.setTimer(timer_name)
        except:
            self.output("%s is not a valid channel in the active measurement group" % timer_name)
        

class show_cnts(Macro):
    """Shows the current values for each counter of the active measurement 
    group"""
    

    def prepare(self, **opts):
        name = '.*'
        
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs(name, type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break

        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        
        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            self.channels.append( ch_attr )
            
    def run(self):
        self.outputDate()
        self.output('')
        
        counters = []
        names    = []
        counts   = []
        width    = 30

        for channel in self.channels:
            name = channel.name[:channel.name.index("_value")]
            names.append('  ' + name)
            counts.append(channel.read())

        table = Table([counts], row_head_str=names, row_head_fmt='%*s',
                      col_sep='  =  ')
        for line in table.genOutput():
            self.output(line)

class twice(Macro):
    """Returns the double of the given number"""
    
    param_def = [
       ['value',  Type.Float,   None, 'a number']
    ]

    result_def = [
        ['double_value',  Type.Float, None, 'The double of the given value']
    ]
    
    def run(self,value):
        self.info("Calculating 2*%f. This is a very intensive calculation so it may take some time :-)" % value)
        time.sleep(5)
        return 2*value
    
    