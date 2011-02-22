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

if MACRO_SERVER

SCRIPTS_IN = MacroServer.in

bin_SCRIPTS = MacroServer
CLEANFILES = $(bin_SCRIPTS)
EXTRA_DIST = $(SCRIPTS_IN)

do_subst = sed -e 's,[@]pkgpythondir[@],$(pkgpythondir),g' \
	-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
	-e 's,[@]VERSION[@],$(VERSION),g'

MacroServer: $(SCRIPTS_IN)
	$(do_subst) < $(top_srcdir)/MacroServer.in > MacroServer
	chmod +x MacroServer

pkgpython_PYTHON = $(top_srcdir)/__init__.py \
		$(top_srcdir)/DataRecorder.py \
		$(top_srcdir)/Door.py \
		$(top_srcdir)/Macro.py \
		$(top_srcdir)/MacroServer.py \
		$(top_srcdir)/Manager.py \
		$(top_srcdir)/Params.py \
		$(top_srcdir)/PyTangoExt.py \
		$(top_srcdir)/Sardana.py \
		$(top_srcdir)/ScanData.py \
		$(top_srcdir)/Types.py \
		$(top_srcdir)/PoolTimer.py \
		$(top_srcdir)/Motion.py \
		$(top_srcdir)/Attribute.py \
		$(top_srcdir)/Event.py \
		$(top_srcdir)/Log.py \
		$(top_srcdir)/Object.py \
		$(top_srcdir)/utils.py

RSYNC_OPTS = -r --cvs-exclude

if EXTRA_MACROS

install-data-local:
	rsync -r $(RSYNC_OPTS) --exclude=*.pyc $(top_srcdir)/macros $(pkgpythondir)

dist-hook:
	rsync -r $(RSYNC_OPTS) --exclude=*.pyc $(top_srcdir)/macros $(distdir)

endif

	
endif

