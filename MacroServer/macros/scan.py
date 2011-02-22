"""
    Macro library containning scan macros for the macros server Tango device 
    server as part of the Sardana project.
    
    Available scan macros are:
    - ascan 
        absolute step scan on a single motor.
    - ascan_mot
        absolute step scan on a single motor. Can contain list of additional motors.
        The positions of these additional motors will be shown at each step.
    - mesh
        absolute steo scan on two motors in mesh mode.
"""

from Macro    import *
from ScanData import *

class mscan(Macro):
    """Do an absolute scan of the specified motor. In each step nr_samples 
    of samples are taken"""    

    param_def = [
       ['motor',      Type.Motor,   None, 'Motor to move'],
       ['start_pos',  Type.Float,   None, 'Scan start position'],
       ['final_pos',  Type.Float,   None, 'Scan final position'],
       ['nr_interv',  Type.Integer, None, 'Number of scan intervals'],
       ['integ_time', Type.Float,   None, 'Integration time'],
       ['nr_samples', Type.Integer,   None, 'number of samples to take in each step']
    ]

    def prepare(self, motor, start_pos, final_pos, nr_interv, integ_time, nr_samples,
                **opts):
        self.nr_points   = nr_interv + 1
        self.interv_size = ( final_pos - start_pos + 0.0) / nr_interv
        self.total_time  = self.nr_points * integ_time
        self.sample_time = integ_time / nr_samples
        
        name             = 'mscan'

        if 'name' in opts:
            name = opts['name']

        self.cmd = '%s %s %s %s %d %s %d' % (name, motor.getName(),
                                     start_pos, final_pos, nr_interv,
                                     integ_time,nr_samples)

        self.motor      = motor
        self.integ_time = integ_time
        self.nr_samples = nr_samples
        
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs('.*', type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break
        
        self.counters = []
        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        
        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        timer_name = self.timer.getName().lower()
        
        for cnt in self.mnt_grp.getChannelList():
            if cnt.getName().lower() != timer_name:
                self.counters.append( cnt )
        
        timer_name = timer_name + "_value"
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            if ch_attr.name.lower() != timer_name:
                self.channels.append( ch_attr ) 
            
    def scan_begin(self): 
        
        data_handler = self.getEnv("data_handler")
        assert(not data_handler is None)
        
        self.data = ScanData(data_handler = data_handler)

        labelstr    = " #Pt No   %8s " % self.motor.getName()
        labelstr   += " %8s " % self.timer.getName()

        labels = []
        labels.append("point_no")
        labels.append( self.motor.getName() )
        labels.append( self.timer.getName() )

        for cnt in self.counters:
          labelstr += "  %8s" % cnt.getName()
          labels.append( cnt.getName() )

        # Extend labels if needed
        labelstr = self.extend_labelstr(labelstr)
        labels = self.extend_labels(labels)
        
        self.output( labelstr )

        self.data.setEnvironValue('title',  self.cmd )
        self.data.setEnvironValue('labels', labels )

        self.data.start()

        self.mnt_grp.setIntegrationTime(self.sample_time)
        
    def scan_end(self):
        self.data.end()

    def run(self, motor, start_pos, final_pos, nr_interv, integ_time, nr_samples):

        if self.mnt_grp is None:
            self.output("ActiveMntgrp is not defined or has invalid value")
            return

        if self.timer is None:
            self.output("%s has not timer defined" % self.mnt_grp.getName())
            return
        
        # prepare things

        self.scan_begin()

        # now the loop

        for point_no in range(self.nr_points):

            point_pos = start_pos + point_no * self.interv_size

            motor.startMove(point_pos)
            motor.waitMove()

            for sample_no in range(self.nr_samples):
                self.flushOutput()
                data_line  = {}
                data_line['point_no']             = point_no
                data_line[ self.motor.getName() ] = motor.getUserPos()
    
                scan_line  = " % 4d    %8.4f" % ( point_no, motor.getUserPos() )
                
                # now count
                self.mnt_grp.startCount()
                self.mnt_grp.waitCount()
            
                # read and record data
                scan_line += " %8.4f " % self.timer.getCount()
                data_line[ self.timer.getName() ] = self.timer.getCount()
    
                #for cnt in self.counters:
                #   scan_line += " %8s " % cnt.getCount()
                #   data_line[ cnt.getName() ] = cnt.getCount()
                for channel in self.channels:
                   scan_line += " %8s " % channel.read()
                   ch_name = channel.name[:channel.name.index("_value")]
                   data_line[ ch_name ] = channel.read()
                
                # Extend data if needed
                scan_line = self.extend_scanline(scan_line)
                self.extend_dataline(data_line)
                
                self.data.addRecord( data_line )
                self.output( scan_line )

        self.scan_end()

    def extend_labelstr(self,labelstr):
        return labelstr

    def extend_labels(self,labels):
        return labels

    def extend_scanline(self,scanline):
        return scanline

    def extend_dataline(self,dataline):
        # NO RETURN NEEDED, THE DATALINE IS UPDATED ITSELF
        pass

class ascan(Macro):
    """Do an absolute scan of the specified motor"""
    
    param_def = [
       ['motor',      Type.Motor,   None, 'Motor to move'],
       ['start_pos',  Type.Float,   None, 'Scan start position'],
       ['final_pos',  Type.Float,   None, 'Scan final position'],
       ['nr_interv',  Type.Integer, None, 'Number of scan intervals'],
       ['integ_time', Type.Float,   None, 'Integration time']
    ]

    def prepare(self, motor, start_pos, final_pos, nr_interv, integ_time,
                **opts):
        self.nr_points   = nr_interv + 1
        self.interv_size = ( final_pos - start_pos + 0.0) / nr_interv
        self.total_time  = self.nr_points * integ_time

        name             = 'ascan'

        if 'name' in opts:
            name = opts['name']

        self.cmd = '%s %s %s %s %d %s' % (name, motor.getName(),
                                     start_pos, final_pos, nr_interv,
                                     integ_time)

        self.motor      = motor
        self.integ_time = integ_time
        
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs('.*', type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break
        
        self.counters = []
        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        
        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        timer_name = self.timer.getName().lower()
        
        for cnt in self.mnt_grp.getChannelList():
            if cnt.getName().lower() != timer_name:
                self.counters.append( cnt )
        
        timer_name = timer_name + "_value"
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            if ch_attr.name.lower() != timer_name:
                self.channels.append( ch_attr ) 
            
    def scan_begin(self): 
        
        data_handler = self.getEnv("data_handler")
        assert(not data_handler is None)
        
        self.data = ScanData(data_handler = data_handler)

        labelstr    = " #Pt No   %8s " % self.motor.getName()
        labelstr   += " %8s " % self.timer.getName()

        labels = []
        labels.append("point_no")
        labels.append( self.motor.getName() )
        labels.append( self.timer.getName() )

        for cnt in self.counters:
          labelstr += "  %8s" % cnt.getName()
          labels.append( cnt.getName() )

        # Extend labels if needed
        labelstr = self.extend_labelstr(labelstr)
        labels = self.extend_labels(labels)
        
        self.output( labelstr )

        self.data.setEnvironValue('title',  self.cmd )
        self.data.setEnvironValue('labels', labels )

        self.data.start()

        self.mnt_grp.setIntegrationTime(self.integ_time)
        
    def scan_end(self):
        self.data.end()

    def run(self, motor, start_pos, final_pos, nr_interv, integ_time):

        if self.mnt_grp is None:
            self.output("ActiveMntgrp is not defined or has invalid value")
            return

        if self.timer is None:
            self.output("%s has not timer defined" % self.mnt_grp.getName())
            return
        
        # prepare things

        self.scan_begin()

        # now the loop

        for point_no in range(self.nr_points):

            point_pos = start_pos + point_no * self.interv_size

            self.flushOutput()
            motor.startMove([point_pos])
            motor.waitMove()

            data_line  = {}
            data_line['point_no']             = point_no
            data_line[ self.motor.getName() ] = motor.getUserPos()

            scan_line  = " % 4d    %8.4f" % ( point_no, motor.getUserPos() )
            
            # now count
            self.mnt_grp.startCount()
            self.mnt_grp.waitCount()
        
            # read and record data
            scan_line += " %8.4f " % self.timer.getCount()
            data_line[ self.timer.getName() ] = self.timer.getCount()

            #for cnt in self.counters:
            #   scan_line += " %8s " % cnt.getCount()
            #   data_line[ cnt.getName() ] = cnt.getCount()
            for channel in self.channels:
               scan_line += " %8s " % channel.read()
               ch_name = channel.name[:channel.name.index("_value")]
               data_line[ ch_name ] = channel.read()
            
            # Extend data if needed
            scan_line = self.extend_scanline(scan_line)
            self.extend_dataline(data_line)
            
            self.data.addRecord( data_line )
            self.output( scan_line )

        self.scan_end()

    def extend_labelstr(self,labelstr):
        return labelstr

    def extend_labels(self,labels):
        return labels

    def extend_scanline(self,scanline):
        return scanline

    def extend_dataline(self,dataline):
        # NO RETURN NEEDED, THE DATALINE IS UPDATED ITSELF
        pass
   
    
class ascan_mot(ascan):
    """Do an absolute scan of the specified motor appending motor positions as counters"""
    
    param_def = [
       ['motor',      Type.Motor,   None, 'Motor to move'],
       ['start_pos',  Type.Float,   None, 'Scan start position'],
       ['final_pos',  Type.Float,   None, 'Scan final position'],
       ['nr_interv',  Type.Integer, None, 'Number of scan intervals'],
       ['integ_time', Type.Float,   None, 'Integration time'],
       ['motor_counters',
        ParamRepeat(['mot_counter', Type.Motor, None, 'Motors to use as counters']),
        None, 'List of motor to user as counters']
    ]

    def prepare(self, motor, start_pos, final_pos, nr_interv, integ_time, *motor_counters,
                **opts):
        ascan.prepare(self,motor,start_pos,final_pos,nr_interv,integ_time,**opts)

        self.motor_counters = motor_counters

    def run(self, motor, start_pos, final_pos, nr_interv, integ_time, *motor_counters):
        ascan.run(self,motor, start_pos, final_pos, nr_interv, integ_time)


    def extend_labelstr(self,labelstr):
        for motor in self.motor_counters:
            labelstr += "  %8s" % motor.getName()
        return labelstr

    def extend_labels(self,labels):
        for motor in self.motor_counters:
            labels += "  %8s" % motor.getName()
        return labels

    def extend_scanline(self,scanline):
        for motor in self.motor_counters:
            scanline += "  %8g" % motor.getPosObj().read(False)
        return scanline

    def extend_dataline(self,dataline):
        for motor in self.motor_counters:
            dataline[motor.getName()] = motor.getPosObj().read(False)


class mesh(Macro):
    """Do an absolute mesh scan of the specified pair of motors"""
    
    param_def = [
       ['motor1',      Type.Motor,   None, 'First motor to move'],
       ['m1_start_pos',Type.Float,   None, 'Scan start position for first motor'],
       ['m1_final_pos',Type.Float,   None, 'Scan final position for first motor'],
       ['m1_nr_interv',Type.Integer, None, 'Number of scan intervals'],
       ['motor2',      Type.Motor,   None, 'Second motor to move'],
       ['m2_start_pos',Type.Float,   None, 'Scan start position for second motor'],
       ['m2_final_pos',Type.Float,   None, 'Scan final position for second motor'],
       ['m2_nr_interv',Type.Integer, None, 'Number of scan intervals'],
       ['integ_time',  Type.Float,   None, 'Integration time']
    ]

    def prepare(self, m1, m1_start_pos, m1_final_pos, m1_nr_interv,
                m2, m2_start_pos, m2_final_pos, m2_nr_interv, integ_time,
                **opts):
        self.m1_interv_size = ( m1_final_pos - m1_start_pos + 0.0) / m1_nr_interv
        self.m1_nr_points   = m1_nr_interv + 1
        self.m2_interv_size = ( m2_final_pos - m2_start_pos + 0.0) / m2_nr_interv
        self.m2_nr_points   = m2_nr_interv + 1
        
        self.total_time     = (self.m1_nr_points + self.m2_nr_points) * integ_time

        name             = 'mesh'

        if 'name' in opts:
            name = opts['name']

        self.cmd = '%s %s %s %s %d %s %s %s %d %s' % (name, m1.getName(),m1_start_pos, m1_final_pos, m1_nr_interv,m2.getName(),m2_start_pos, m2_final_pos, m2_nr_interv,integ_time)

        self.m1         = m1
        self.m2         = m2
        self.integ_time = integ_time
        
        mntgrp_name = self.getEnv('ActiveMntgrp').lower()
        mntgrp_list = self.findObjs('.*', type_class=Type.MeasurementGroup)
        self.mnt_grp = None
        
        if not mntgrp_name is None:
            for mntgrp in mntgrp_list:
                if mntgrp.getName().lower() == mntgrp_name:
                    self.mnt_grp = mntgrp
                    break
        
        self.counters = []
        self.channels = []
        self.timer = None
        
        if self.mnt_grp is None:
            return
        

        self.timer = self.mnt_grp.getTimer()
        
        if self.timer is None:
            return
        
        timer_name = self.timer.getName().lower()
        
        for cnt in self.mnt_grp.getChannelList():
            if cnt.getName().lower() != timer_name:
                self.counters.append( cnt )
        
        timer_name = timer_name + "_value"
        
        for ch_attr in self.mnt_grp.getChannelAttrList():
            if ch_attr.name.lower() != timer_name:
                self.channels.append( ch_attr ) 
                        
    def scan_begin(self): 
        
        data_handler = self.getEnv("data_handler")
        assert(not data_handler is None)
        
        self.data = ScanData(data_handler = data_handler)

        labelstr    = " #Pt No\t%8s\t%8s\t" % (self.m1.getName(), self.m2.getName())
        labelstr   += " %8s" % self.timer.getName()

        labels = []
        labels.append("point_no")
        labels.append( self.m1.getName() )
        labels.append( self.m2.getName() )
        labels.append( self.timer.getName() )

        for cnt in self.counters:
          labelstr += "\t%8s" % cnt.getName()
          labels.append( cnt.getName() )

        # Extend labels if needed
        labelstr = self.extend_labelstr(labelstr)
        labels = self.extend_labels(labels)
        
        self.output( labelstr )

        self.data.setEnvironValue('title',  self.cmd )
        self.data.setEnvironValue('labels', labels )

        self.data.start()

        self.mnt_grp.setIntegrationTime(self.integ_time)


    def scan_end(self):
        self.data.end()

    def run(self, m1, m1_start_pos, m1_final_pos, m1_nr_interv,
            m2, m2_start_pos, m2_final_pos, m2_nr_interv,
            integ_time):

        if self.mnt_grp is None:
            self.output("ActiveMntgrp is not defined or has invalid value")
            return

        if self.timer is None:
            self.output("%s has not timer defined" % self.mnt_grp.getName())
            return
        
        # prepare things

        self.scan_begin()

        # now the loop

        for m2_point_no in range(self.m2_nr_points):
            
            m2_point_pos = m2_start_pos + m2_point_no * self.m2_interv_size
            
            self.flushOutput()
            m2.startMove(m2_point_pos)
            m2.waitMove()

            for m1_point_no in range(self.m1_nr_points):
            
                m1_point_pos = m1_start_pos + m1_point_no * self.m1_interv_size

                self.flushOutput()
                m1.startMove(m1_point_pos)
                m1.waitMove()
                absolute_point_no = m2_point_no * self.m1_nr_points + m1_point_no

                data_line  = {}
                data_line['point_no']             = absolute_point_no
                data_line[ self.m1.getName() ] = m1.getUserPos()
                data_line[ self.m2.getName() ] = m2.getUserPos()

                scan_line  = "%4d\t%8g\t%8g" % ( absolute_point_no, m1.getUserPos(), m2.getUserPos() )
            
                # now count
                self.mnt_grp.startCount()
                self.mnt_grp.waitCount()
        
                # read and record data
                scan_line += "\t%8.4f" % self.timer.getCount()
                data_line[ self.timer.getName() ] = self.timer.getCount()

                #for cnt in self.counters:
                #   scan_line += " %8s " % cnt.getCount()
                #   data_line[ cnt.getName() ] = cnt.getCount()
                for channel in self.channels:
                    scan_line += "\t%8g" % channel.read()
                    ch_name = channel.name[:channel.name.index("_value")]
                    data_line[ ch_name ] = channel.read()
            
                # Extend data if needed
                scan_line = self.extend_scanline(scan_line)
                self.extend_dataline(data_line)
            
                self.data.addRecord( data_line )
                self.output( scan_line )

        self.scan_end()

    def extend_labelstr(self,labelstr):
        return labelstr

    def extend_labels(self,labels):
        return labels

    def extend_scanline(self,scanline):
        return scanline

    def extend_dataline(self,dataline):
        # NO RETURN NEEDED, THE DATALINE IS UPDATED ITSELF
        pass
