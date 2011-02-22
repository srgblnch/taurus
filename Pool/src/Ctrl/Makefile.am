# Sardana device pool tango system - pool lib
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


pkglib_LTLIBRARIES = PyConstraint.la \
				PyMotorCtrl.la \
				PyPseudoMotorCtrl.la \
				PyCounterTimerCtrl.la \
				PyZeroDExpChannelCtrl.la \
				PyPseudoCounterCtrl.la \
				PyCommunicationCtrl.la

AM_CPPFLAGS = -I@TANGO_ROOT@/include \
				-I@OMNI_ROOT@/include \
				-I$(PY_PREFIX)/include/python \
				-I$(top_srcdir)/include 
				 
AM_CXXFLAGS = -DREENTRANT 
AM_LDFLAGS = -module -avoid-version

LIBADD = -L@TANGO_ROOT@/lib \
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
		-lpython@PYTHON_VERSION@

PyConstraint_ladir = $(pkgincludedir)
PyMotorCtrl_ladir = $(pkgincludedir)
PyPseudoMotorCtrl_ladir = $(pkgincludedir)
PyCounterTimerCtrl_ladir = $(pkgincludedir)
PyZeroDExpChannelCtrl_ladir = $(pkgincludedir)
PyPseudoCounterCtrl_ladir = $(pkgincludedir)
PyCommunicationCtrl_ladir = $(pkgincludedir)

PyConstraint_la_SOURCES = $(srcdir)/PyConstraint.cpp \
					$(srcdir)/PyConstraint.h 

PyMotorCtrl_la_SOURCES = $(srcdir)/PyMotCtrl.cpp \
					$(srcdir)/PyMotCtrl.h
					
PyPseudoMotorCtrl_la_SOURCES = $(srcdir)/PyPseudoMotCtrl.cpp \
					$(srcdir)/PyPseudoMotCtrl.h
					
PyCounterTimerCtrl_la_SOURCES = $(srcdir)/PyCoTiCtrl.cpp \
					$(srcdir)/PyCoTiCtrl.h
					
PyZeroDExpChannelCtrl_la_SOURCES = $(srcdir)/PyZeroDCtrl.cpp \
					$(srcdir)/PyZeroDCtrl.h
					
PyPseudoCounterCtrl_la_SOURCES = $(srcdir)/PyPseudoCoCtrl.cpp \
					$(srcdir)/PyPseudoCoCtrl.h
					
PyCommunicationCtrl_la_SOURCES = $(srcdir)/PyComCtrl.cpp \
					$(srcdir)/PyComCtrl.h
