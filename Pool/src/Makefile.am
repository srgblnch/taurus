# Sardana device pool tango system
# Adapted from Jens Meyer Makefile.am

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

bin_PROGRAMS = Pool

Pool_CPPFLAGS =	-I@TANGO_ROOT@/include \
				-I@OMNI_ROOT@/include \
				-I$(PY_PREFIX)/include/python \
				-I$(srcdir)/../include \
				-I$(srcdir)/Motor \
				-I$(srcdir)/MotorGroup \
				-I$(srcdir)/PseudoMotor \
				-I$(srcdir)/CTExpChannel \
				-I$(srcdir)/ZeroDExpChannel \
				-I$(srcdir)/MeasurementGroup \
				-I$(srcdir)/CommunicationChannel \
				-I$(srcdir)/PoolBaseDev \
				-I$(srcdir)/PoolIndBaseDev \
				-I$(srcdir)/PoolGroupBaseDev \
				-I$(srcdir)/PseudoCounter \
				-I$(srcdir)/Ctrl

Pool_LDADD = -L../lib \
        -L@TANGO_ROOT@/lib \
		-L@OMNI_ROOT@/lib \
		-L$(PY_PREFIX)/lib \
		-ltango \
		-llog4tango \
		-lomniORB4 \
		-lomniDynamic4 \
		-lomnithread \
		-lCOS4 \
		-lltdl \
		-lpthread \
		-lrt \
		-lpython@PYTHON_VERSION@ \
		../lib/pool.la

Pool_CXXFLAGS = -D_REENTRANT
Pool_LDFLAGS = -export-dynamic

Pool_SOURCES = 	$(srcdir)/CppCtrlFile.h \
		$(srcdir)/CppMotCtrlFile.h \
		$(srcdir)/CTPoolThread.h \
		$(srcdir)/CtrlFiCa.h \
		$(srcdir)/ExternalFiCa.h \
		$(srcdir)/ExternalFile.h \
		$(srcdir)/CtrlFile.h \
		$(srcdir)/PoolDef.h \
		$(srcdir)/PoolFactory.h \
		$(srcdir)/PoolClass.h \
		$(srcdir)/Pool.h \
		$(srcdir)/PoolThread.h \
		$(srcdir)/PoolUtil.h \
		$(srcdir)/PropertyData.h \
		$(srcdir)/PyCtrlFile.h \
		$(srcdir)/PyExternalFile.h \
		$(srcdir)/PyMotCtrlFile.h \
		$(srcdir)/Utils.h \
		$(srcdir)/Profile.h \
		$(srcdir)/Ctrl/PyCtrl.h \
		$(srcdir)/Motor/Motor.h \
		$(srcdir)/Motor/MotorClass.h \
		$(srcdir)/Motor/MotorFeatures.h \
		$(srcdir)/Motor/MotorUtil.h \
		$(srcdir)/MotorGroup/MotorGroup.h \
		$(srcdir)/MotorGroup/MotorGroupClass.h \
		$(srcdir)/MotorGroup/MotorGroupUtil.h \
		$(srcdir)/PseudoMotor/PseudoMotor.h \
		$(srcdir)/PseudoMotor/PseudoMotorClass.h \
		$(srcdir)/PseudoMotor/PseudoMotorUtil.h \
		$(srcdir)/CTExpChannel/CTExpChannel.h \
		$(srcdir)/CTExpChannel/CTExpChannelClass.h \
		$(srcdir)/CTExpChannel/CTExpChannelUtil.h \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannel.h \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannelClass.h \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannelUtil.h \
		$(srcdir)/ZeroDExpChannel/ZeroDThread.h \
		$(srcdir)/PseudoCounter/PseudoCounter.h \
		$(srcdir)/PseudoCounter/PseudoCounterClass.h \
		$(srcdir)/PseudoCounter/PseudoCounterUtil.h \
		$(srcdir)/MeasurementGroup/MeasurementGroup.h \
		$(srcdir)/MeasurementGroup/MeasurementGroupClass.h \
		$(srcdir)/MeasurementGroup/MeasurementGroupUtil.h \
		$(srcdir)/PoolBaseDev/PoolBaseUtil.h \
		$(srcdir)/PoolBaseDev/PoolBaseDev.h \
		$(srcdir)/PoolIndBaseDev/PoolIndBaseDev.h \
		$(srcdir)/PoolIndBaseDev/PoolExtraAttrTypes.h \
		$(srcdir)/PoolIndBaseDev/PoolDynAttr.h \
		$(srcdir)/PoolGroupBaseDev/PoolGroupBaseUtil.h \
		$(srcdir)/PoolGroupBaseDev/PoolGroupBaseDev.h \
		$(srcdir)/CommunicationChannel/CommunicationChannel.h \
		$(srcdir)/CommunicationChannel/CommunicationChannelClass.h \
		$(srcdir)/CommunicationChannel/CommunicationChannelUtil.h \
		$(srcdir)/main.cpp	\
		$(srcdir)/Utils.cpp \
		$(srcdir)/Profile.cpp \
		$(srcdir)/PoolFactory.cpp \
		$(srcdir)/ClassFactory.cpp	\
		$(srcdir)/PoolClass.cpp \
		$(srcdir)/PoolStateMachine.cpp	\
		$(srcdir)/Pool.cpp \
		$(srcdir)/PoolThread.cpp \
		$(srcdir)/PoolUtilMeth.cpp \
		$(srcdir)/Pool_AddRemove.cpp \
		$(srcdir)/Pool_GetFind.cpp \
		$(srcdir)/PoolUtil.cpp \
		$(srcdir)/ExternalFile.cpp \
		$(srcdir)/CtrlFile.cpp \
		$(srcdir)/PyExternalFile.cpp \
		$(srcdir)/PyCtrlFile.cpp \
		$(srcdir)/PyMotCtrlFile.cpp \
		$(srcdir)/CppCtrlFile.cpp \
		$(srcdir)/CppMotCtrlFile.cpp \
		$(srcdir)/ExternalFiCa.cpp \
		$(srcdir)/CtrlFiCa.cpp \
		$(srcdir)/PropertyData.cpp \
		$(srcdir)/Ctrl/ComCtrl.cpp \
		$(srcdir)/Ctrl/Constraint.cpp \
		$(srcdir)/Ctrl/CoTiCtrl.cpp \
		$(srcdir)/Ctrl/MotCtrl.cpp \
		$(srcdir)/Ctrl/PyCtrl.cpp \
		$(srcdir)/Motor/MotorClass.cpp \
		$(srcdir)/Motor/Motor.cpp \
		$(srcdir)/Motor/MotorStateMachine.cpp \
		$(srcdir)/Motor/MotorUtil.cpp \
		$(srcdir)/MotorGroup/MotorGroupClass.cpp \
		$(srcdir)/MotorGroup/MotorGroup.cpp \
		$(srcdir)/MotorGroup/MotorGroupStateMachine.cpp \
		$(srcdir)/MotorGroup/MotorGroupUtil.cpp \
		$(srcdir)/PseudoMotor/PseudoMotorClass.cpp \
		$(srcdir)/PseudoMotor/PseudoMotorUtil.cpp \
		$(srcdir)/PseudoMotor/PseudoMotor.cpp \
		$(srcdir)/PseudoMotor/PseudoMotorStateMachine.cpp \
		$(srcdir)/PoolBaseDev/PoolBaseDev.cpp \
		$(srcdir)/PoolIndBaseDev/PoolIndBaseDev.cpp \
		$(srcdir)/PoolIndBaseDev/PoolDynAttr.cpp \
		$(srcdir)/PoolGroupBaseDev/PoolGroupBaseDev.cpp \
		$(srcdir)/CTExpChannel/CTExpChannelClass.cpp \
		$(srcdir)/CTExpChannel/CTExpChannel.cpp \
		$(srcdir)/CTExpChannel/CTExpChannelStateMachine.cpp \
		$(srcdir)/CTExpChannel/CTExpChannelUtil.cpp \
		$(srcdir)/CTPoolThread.cpp \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannelClass.cpp \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannel.cpp \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannelStateMachine.cpp \
		$(srcdir)/ZeroDExpChannel/ZeroDExpChannelUtil.cpp \
		$(srcdir)/ZeroDExpChannel/ZeroDThread.cpp \
		$(srcdir)/PseudoCounter/PseudoCounterClass.cpp \
		$(srcdir)/PseudoCounter/PseudoCounter.cpp \
		$(srcdir)/PseudoCounter/PseudoCounterUtil.cpp \
		$(srcdir)/PseudoCounter/PseudoCounterStateMachine.cpp \
		$(srcdir)/MeasurementGroup/MeasurementGroupClass.cpp \
		$(srcdir)/MeasurementGroup/MeasurementGroup.cpp \
		$(srcdir)/MeasurementGroup/MeasurementGroupStateMachine.cpp \
		$(srcdir)/MeasurementGroup/MeasurementGroupUtil.cpp \
		$(srcdir)/CommunicationChannel/CommunicationChannelClass.cpp \
		$(srcdir)/CommunicationChannel/CommunicationChannel.cpp \
		$(srcdir)/CommunicationChannel/CommunicationChannelStateMachine.cpp \
		$(srcdir)/CommunicationChannel/CommunicationChannelUtil.cpp
		

