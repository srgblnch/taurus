from Macro import *

import time
import array
import os

################################################################################
#
# Configuration related macros
#
################################################################################
    

class defm(Macro):
    """Creates a new motor in the active pool"""
    
    param_def = [['controller', Type.Controller, None, 'Controller name'],
                 ['axis', Type.Integer, None, 'motor axis'],
                 ['motor name', Type.String, None, 'motor name']]
    
    def run(self,controller,axis,name):
        pool = controller.getPool()
        pool.CreateMotor([[int(axis)],[name,controller.name]])
        
class defmeas(Macro):
    """Create a new measurement group"""

    param_def = [
       ['name',  Type.String,       None, 'Measurement group name'],
       ['channel_list',
            ParamRepeat(['channel', Type.ExpChannel, None, 'Measurement Channel'],),
            None, 'List of measurement channels'],
    ]

    def prepare(self, name, *channel_list, **opts):

        mntgrp_list = self.findObjs(name, type_class=Type.MeasurementGroup)
        
        if len(mntgrp_list) != 0:
            raise "A measurement group with that name already exists"
        
    def run(self, name, *channel_list):
        manager = self.getManager()
        pool = manager.getPool()
        #TODO change the way we access the pool
        channels = []
        for c in channel_list:
            channels.append(c.getName())
        pool.createMeasurementGroup(name, channels)

class load(Macro):
    """Load a macro file"""

    param_def = [
       ['file_name', Type.String, None, 'Macro file name']
    ]

    def prepare(self, file_name, **opts):
        manager = self.getManager()
        mac_path = manager.getMacroPath()
        fname = file_name
        exists = os.path.exists
        if not os.path.isabs(fname):
            for path in mac_path:
                self.debug("path=%s", path)
                fname = os.path.join(path, file_name)
                ext = '.py'
                if not exists(fname) and exists(fname + ext):
                    fname += ext
                if exists(fname):
                    break
        if not exists(fname):
            raise RuntimeError, 'Could not find %s in %s' % \
                  (file_name, mac_path)
        if not os.access(fname, os.R_OK):
            raise RuntimeError, 'User does not have access to %s' % fname

    def run(self, file_name):
        manager = self.getManager()
        fname = manager.loadMacroFile(file_name)
        self.output("Loaded macro file %s", fname)
        
################################################################################
#
# Controller related macros
#
################################################################################

class lsctrl(Macro):
    """Lists all existing controllers"""
    
    def run(self):

        self.all_ctrls = self.findObjs('.*', type_class=Type.Controller)

        nr_ctrls = len(self.all_ctrls)
        if nr_ctrls == 0:
            self.output('No controllers defined')
            return
        
        out = List(['Name', 'Type', 'Class'])
        
        for ctrl in self.all_ctrls:
            info = ctrl.info
            out.appendRow([info.name, info.type, info.klass])
        for line in out.genOutput():
            self.output(line)

        
class lslib(Macro):
    """Lists all existing controller classes"""
    
    def run(self):

        self.ctrl_classes = self.findObjs('.*', type_class=Type.ControllerClass)

        nr_ctrls = len(self.ctrl_classes)
        if nr_ctrls == 0:
            self.output('No controller classes defined')
            return
        
        out = List(['Name', 'Type', 'Library', 'Family'])
        
        for ctrl_class_data in self.ctrl_classes:
            out.appendRow([ctrl_class_data.getName(),
                           ctrl_class_data.getType(),
                           ctrl_class_data.getLib(),
                           ctrl_class_data.getGender()])
        
        for line in out.genOutput():
            self.output(line)
        
class send2ctrl(Macro):
    """Sends the given data directly to the controller"""
    
    param_def = [['controller', Type.Controller, None, 'Controller name'],
                 ['data',
                  ParamRepeat(['string item', Type.String, None, 'a string item'],),
                  None, 'data to be sent']]
    
    def run(self, controller, *data):
        name = controller.getName()
        pool = controller.getPool()
        str_data = " ".join(data)
        res = pool.SendToController([name,str_data])
        self.output(res)
        
################################################################################
#
# Library handling related macros
#
################################################################################

class edctrl(Macro):
    """Returns the contents of the library file which contains the given controller
       code."""

    param_def = [
        ['ctrlclass',  Type.ControllerClass, None, 'Controller class name']
    ]
    
    result_def = [
        ['filedata',  Type.File, None, 'The file data object.']
    ]
    
    hints = { 'commit_cmd' : 'commit_ctrllib' }
    
    def run(self,ctrlclass):
       f_name = ctrlclass.file
       pool = ctrlclass.getPool()
       data = pool.GetFile(f_name)
       data = array.array('B',data).tostring()
       line_nb = 1
       for line in data.splitlines():
           line = line.strip(' \t')
           if line.startswith('class') and line.find(ctrlclass.name)>0 and line.endswith(":"):
               break
           line_nb = line_nb + 1
       return [f_name,data,line_nb]

class edctrllib(Macro):
    """Returns the contents of the given library file"""

    param_def = [
        ['filename',  Type.Filename, None, 'Absolute path and file name or simple filename. Relative paths are not allowed.']
    ]
    
    result_def = [
        ['filedata',  Type.File, None, 'The file data object']
    ]
    
    hints = { 'commit_cmd' : 'commit_ctrllib' }
    
    def run(self,filename):
       pool = self.getManager().getPool()
       data = pool.GetFile(filename)
       return [filename,array.array('B',data).tostring(),0]
   
class commit_ctrllib(Macro):
    """Puts the contents of the given data in a file inside the pool"""
    
    param_def = [
        ['filename',  Type.Filename, None, 'Absolute path and file name'],
        ['username',  Type.User, None, 'The user name'],
        ['comment',  Type.String, None, 'A description of the changes made to the file'],
        ['filedata',  Type.File, None, 'The file data object']
    ]
    
    def run(self,filename,username,comment,filedata):
       pool = self.getManager().getPool()
       meta = filename + '\0' + username + '\0' + comment + '\0'
       data = array.array('B',meta)
       data.extend(array.array('B',filedata))
       pool.PutFile(data.tolist())
    
################################################################################
#
# Macro handling related macros
#
################################################################################

class prdef(Macro):
     """Returns the the macro code for the given macro name."""

     param_def = [
          ['macro_name',  Type.Macro, None, 'macro name']
     ]
     
     def run(self,macro_name):
         macro_data = self.getManager().getMacroData(macro_name)
         
         if macro_data == None:
             self.output("Unknown macro")
             return
         
         code_lines, first_line = macro_data.getMacroCode()
         
         for code_line in code_lines:
             self.output(code_line.strip('\n'))

class edmac(Macro):
     """Returns the contents of the macro file which contains the macro code for
     the given macro name."""

     param_def = [
          ['macro_name',  Type.Macro, None, 'macro name']
     ]
    
     result_def = [
         ['filedata',  Type.File, None, 'The file data object.']
     ]
     
     hints = { 'commit_cmd' : 'commit_maclib' }
     
     def run(self,macro_name):
         macro_data = self.getManager().getMacroData(macro_name)
         
         if macro_data == None:
             self.output("Unknown macro")
             return
         
         f_name = macro_data.lib.f_path
         f = open(f_name)
         
         try:
             data = f.read()
         finally:
             f.close()
         
         if data == None:
             return
         
         code_lines, first_line = macro_data.getMacroCode()
         return [f_name,data,first_line]
     
class edmaclib(Macro):
     """Returns the contents of the macro file for the given macro filename."""

     param_def = [
         ['filename',  Type.Filename, None, 'Absolute path and file name or simple filename. Relative paths are not allowed.']
     ]
    
     result_def = [
         ['filedata',  Type.File, None, 'The file data object']
     ]
     
     hints = { 'commit_cmd' : 'commit_maclib' }
    
     def run(self,filename):
         m = filename[filename.rfind('/')+1:]
         if m.index('.py') >= 0:
             m = m[:m.rfind('.py')]
         macro_lib = self.getManager().getMacroLib(m)
         f_name = macro_lib.f_path
         f = open(f_name)
         
         try:
             data = f.read()
         finally:
             f.close()
         
         if data == None:
             return
         return [f_name,data,0]
     
class commit_maclib(Macro):
    """Puts the contents of the given data in a file inside the pool"""
    
    param_def = [
        ['filename', Type.Filename, None, 'Absolute path and file name'],
        ['username', Type.User,     None, 'The user name'],
        ['comment',  Type.String,   None, 'A description of the changes made to the file'],
        ['filedata', Type.File,     None, 'The file data object']
    ]
    
    def run(self,filename,username,comment,filedata):
       f = open(filename,'w')
       lines = filedata.splitlines(True)
       idx = 0
       
       if lines[0].startswith('#!'):
           idx = 1
       
       descr = "# " + comment + '\n'
       lines.insert(idx,descr)
       descr = "# " + self.getDateString() + " by " + username + '\n'
       lines.insert(idx,descr)
       
       try:
           f.writelines(lines)
       finally:
           f.close()
       
class relmaclib(Macro):
    """
    """
    param_def = [
        ['filename',  Type.Filename, None, 'Absolute path and file name or simple filename. Relative paths are not allowed.']
    ]
    
    def run(self,filename):
        m = filename[filename.rfind('/')+1:]
        if m.index('.py') >= 0:
            m = m[:m.rfind('.py')]
        
        self.getManager().reloadMacroFile(m)
        
class relmac(Macro):
    """Reloads the given macro code from the macro server filesystem.
       Attention: All macros inside the same file will also be reloaded."""
       
    param_def = [
        ['macro_name',  Type.Macro, None, 'macro name']
    ]
    
    def run(self,macro_name):
        macro_data = self.getManager().getMacroData(macro_name)
        module_name = macro_data.lib.name
        self.getManager().reloadMacroFile(module_name)
