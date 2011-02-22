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

if SPOCK

SCRIPTS_IN = spock.in

bin_SCRIPTS = spock
CLEANFILES = $(bin_SCRIPTS)
EXTRA_DIST = $(SCRIPTS_IN)

do_subst = sed -e 's,[@]pkgpythondir[@],$(pkgpythondir),g' \
	-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
	-e 's,[@]VERSION[@],$(VERSION),g'

spock: $(SCRIPTS_IN)
	$(do_subst) < $(top_srcdir)/spock.in > spock
	chmod +x spock

pkgpython_PYTHON = $(top_srcdir)/__init__.py \
		$(top_srcdir)/imacro.py \
		$(top_srcdir)/ipy_user_conf.py \
		$(top_srcdir)/Attribute.py \
		$(top_srcdir)/Event.py \
		$(top_srcdir)/Log.py \
		$(top_srcdir)/Object.py \
		$(top_srcdir)/utils.py

install-data-local:
	rsync $(top_srcdir)/ipy_profile_spock.py $(pkgpythondir)/

dist-hook:
	rsync $(top_srcdir)/ipy_profile_spock.py $(distdir)/
	
endif

