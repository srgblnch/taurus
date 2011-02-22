import PyTango
import pool
from pool import MotorController
import array
import sys

class Motor:
	def __init__(self):
		self.velocity = 0.0
		self.acceleration = 0.0
		self.deceleration = 0.0
		self.backlash = 0.0
		self.step_per_unit = 0.0
		self.offset = 0.0
		self.base_rate = 0.0
		self.comch = None
		self.comch_name = None
		self.currpos = 0.0
		self.pos_w = None

class DummyMotorController(MotorController):
	"""This class is the Tango Sardana motor controller. It uses a EchoCommunicationChannel"""

	ctrl_features = ['Home_speed','Home_acceleration']

	gender = "Dummy"
	model  = "Dummy"
	organization = "CELLS - ALBA"
	image = "dummy_motor_ctrl.png"
	logo = "ALBA_logo.png"

	MaxDevice = 1024
	
	def __init__(self,inst,props):
		MotorController.__init__(self,inst,props)
		self.log_pref = "[DummyMotorController %s]" % inst 
		self.m = self.MaxDevice*[None,]

	def AddDevice(self,axis):
		print self.log_pref,"AddDevice for axis",axis
		idx = axis - 1
		if len(self.m) < axis:
			raise Exception("Invalid axis %d" % axis)
		if not self.m[idx]:
			self.m[idx] = Motor()
		print self.log_pref,"Finished AddDevice for axis",axis
			
	def DeleteDevice(self,axis):
		print self.log_pref,"DeleteDevice for axis",axis
		idx = axis - 1
		if len(self.m) < axis or not self.m[idx]:
			raise Exception("Invalid axis %d" % axis)
		self.m[idx] = None
	
	def StateOne(self,axis):
#		print self.log_pref,"In StateOne method for axis",axis
		state = PyTango.DevState.ON
		switchstate = 0
		return (int(state), switchstate)

	def PreReadAll(self):
		print self.log_pref,"PreReadAll"

	def PreReadOne(self,axis):
		print self.log_pref,"PreReadOne method for axis",axis

	def ReadAll(self):
		print self.log_pref,"ReadAll method"

	def ReadOne(self,axis):
		print self.log_pref,"ReadOne method for axis",axis
		
		idx = axis - 1
		return self.m[idx].currpos
	
	def PreStartAll(self):
		print self.log_pref,"PreStartAll method"

	def PreStartOne(self,axis,pos):
		print self.log_pref,"PreStartOne method for axis",axis," with pos",pos
		return True

	def StartOne(self,axis,pos):
		print self.log_pref,"StartOne method for axis",axis," with pos",pos
		idx = axis - 1
		self.m[idx].pos_w = pos
		
	def StartAll(self):
		print self.log_pref,"StartAll method"
		for i,mot in enumerate(self.m):
			if mot and mot.pos_w is not None:
				mot.currpos = mot.pos_w
				mot.pos_w = None
		
	def SetPar(self,axis,name,value):
		#print self.log_pref,"SetPar method for axis",axis," name=",name," value=",value
		idx = axis - 1
		setattr(self.m[idx],name.lower(),value)

	def GetPar(self,axis,name):
		#print self.log_pref,"GetPar method for axis",axis," name=",name
		idx = axis - 1
		return getattr(self.m[idx],name.lower())

	def GetExtraAttributePar(self,axis,name):
		pass
	
	def SetExtraAttributePar(self,axis,name,value):
		pass

	def AbortOne(self,axis):
		print self.log_pref,"Abort for axis",axis

	def StopOne(self,axis):
		print self.log_pref,"StopOne for axis",axis

	def DefinePosition(self, axis, position):
		print self.log_pref,"DefinePosition for axis",axis," pos=",position
		idx = axis - 1
		self.m[idx].offset = position - self.m[idx].currpos
		self.m[idx].currpos = position

	def __del__(self):
		print self.log_pref,"Deleting...",
		print "[DONE]"


class DummyMotorControllerExtra(MotorController):
	"""This class is the Tango Sardana motor controller. It uses a DummyCommunicationChannel"""

	ctrl_features = ['Home_speed','Home_acceleration']

	class_prop = {'CommunicationChannel':{'Type':'PyTango.DevVarStringArray','Description':'Communication channel names'}}
	
	gender = "Null"
	model  = "Simplest Null"
	organization = "CELLS - ALBA"

	MaxDevice = 1024
	
	def __init__(self,inst,props):
		MotorController.__init__(self,inst,props)
		self.m = self.MaxDevice*[None,]

	def AddDevice(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In AddDevice method for axis",axis
		idx = axis - 1
		if len(self.m) < axis:
			raise Exception("CommunicationChannel property does not define a valid communication channel for axis %d" % axis)
		print self.CommunicationChannel[idx]
		if not self.m[idx]:
			self.m[idx] = Motor()
			self.m[idx].comch_name = self.CommunicationChannel[idx]
			self.m[idx].comch = pool.PoolUtil().get_com_channel(self.inst_name,self.m[idx].comch_name)
		print "[DummyMotorControllerExtra]",self.inst_name,": Finished AddDevice method for axis",axis
			
	def DeleteDevice(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In DeleteDevice method for axis",axis
		idx = axis - 1
		if len(self.m) < axis:
			raise Exception("CommunicationChannel property does not define a valid communication channel for axis %d" % axis)
		self.m[idx].comch = None
		self.m[idx].comch_name = None
	
	def StateOne(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In StateOne method for axis",axis

		state = PyTango.DevState.ON
		switchstate = 0
		return (int(state), switchstate)

	def PreReadAll(self):
		print "[DummyMotorControllerExtra]",self.inst_name,": In PreReadAll method"

	def PreReadOne(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In PreReadOne method for axis",axis

	def ReadAll(self):
		print "[DummyMotorControllerExtra]",self.inst_name,": In ReadAll method"

	def ReadOne(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In ReadOne method for axis",axis
		idx = axis - 1
		res = self.m[idx].comch.read(-1)
		s = array.array('B', res).tostring()
		try:
			self.m[idx].currpos = float(s)
		except:
			self.m[idx].currpos = 0.0
		return self.m[idx].currpos
	
	def PreStartAll(self):
		print "[DummyMotorControllerExtra]",self.inst_name,": In PreStartAll method"

	def PreStartOne(self,axis,pos):
		print "[DummyMotorControllerExtra]",self.inst_name,": In PreStartOne method for axis",axis," with pos",pos
		return True

	def StartOne(self,axis,pos):
		print "[DummyMotorControllerExtra]",self.inst_name,": In StartOne method for axis",axis," with pos",pos
		idx = axis - 1
		self.m[idx].pos_w = str(pos)
		
	def StartAll(self):
		print "[DummyMotorControllerExtra]",self.inst_name,": In StartAll method"
		for i,mot in enumerate(self.m):
			if mot and mot.pos_w:
				cmdarray = array.array('B', mot.pos_w)
				mot.comch.write(cmdarray)
				mot.pos_w = None
		
	def SetPar(self,axis,name,value):
		print "[DummyMotorControllerExtra]",self.inst_name,": In SetPar method for axis",axis," name=",name," value=",value
		idx = axis - 1
		setattr(self.m[idx],name.lower(),value)

	def GetPar(self,axis,name):
		print "[DummyMotorControllerExtra]",self.inst_name,": In GetPar method for axis",axis," name=",name
		idx = axis - 1
		return getattr(self.m[idx],name.lower())

	def GetExtraAttributePar(self,axis,name):
		pass
	
	def SetExtraAttributePar(self,axis,name,value):
		pass

	def AbortOne(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In Abort for axis",axis

	def StopOne(self,axis):
		print "[DummyMotorControllerExtra]",self.inst_name,": In StopOne for axis",axis

	def DefinePosition(self, axis, position):
		print "[DummyMotorControllerExtra]",self.inst_name,": In DefinePosition for axis",axis," pos=",position
		idx = axis - 1
		self.m[idx].offset = position - self.m[idx].currpos
		self.m[idx].currpos = position

	def __del__(self):
		print "[DummyMotorControllerExtra]",self.inst_name,": Exiting...",
		self.comch = None
		print "[DONE]"
