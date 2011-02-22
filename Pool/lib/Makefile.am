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


pkgpyexec_LTLIBRARIES = pool.la

pool_la_LIBADD = -L@TANGO_ROOT@/lib \
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

pool_la_LDFLAGS = -module -avoid-version
pool_la_CXXFLAGS = -Wno-invalid-offsetof
pool_la_CPPFLAGS = -I@TANGO_ROOT@/include \
			-I@OMNI_ROOT@/include \
			-I$(PY_PREFIX)/include/python \
			-I$(srcdir)/../include \
			-DREENTRANT 

pool_ladir = $(pkgincludedir)

pool_la_SOURCES =  $(srcdir)/PoolAPI.cpp \
				$(srcdir)/PyPoolAPI.cpp
