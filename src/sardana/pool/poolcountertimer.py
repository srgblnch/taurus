#!/usr/bin/env python

##############################################################################
##
## This file is part of Sardana
##
## http://www.tango-controls.org/static/sardana/latest/doc/html/index.html
##
## Copyright 2011 CELLS / ALBA Synchrotron, Bellaterra, Spain
## 
## Sardana is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## Sardana is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public License
## along with Sardana.  If not, see <http://www.gnu.org/licenses/>.
##
##############################################################################

"""This module is part of the Python Pool libray. It defines the base classes
for CounterTimer"""

__all__ = [ "PoolCounterTimer" ]

__docformat__ = 'restructuredtext'

from poolbase import *
from pooldefs import *
from poolelement import *
from poolacquisition import *

class PoolCounterTimer(PoolElement):

    def __init__(self, **kwargs):
        PoolElement.__init__(self, **kwargs)
        self._value = None
        self._wvalue = None
        self.set_action_cache(PoolAcquisition("%s.Acquisition" % self._name))
        self._aborted = False
    
    def get_type(self):
        return ElementType.CTExpChannel
    
    # --------------------------------------------------------------------------
    # value
    # --------------------------------------------------------------------------

    def read_value(self):
        return self.acquisition.read_value()[self]
    
    def put_value(self, value, propagate=1):
        self._set_value(value, propagate=propagate)
    
    def get_value(self, cache=True, propagate=1):
        if not cache or self._value is None:
            value = self.read_value()
            self._set_value(value, propagate=propagate)
        return self._value
    
    def set_value(self, value, propagate=1):
        self._wvalue = value
        self._set_value(value, propagate=propagate)
        
    def _set_value(self, value, propagate=1):
        self._value = value
        if not propagate:
            return
        self.fire_event(EventType("value", priority=propagate), value)
    
    value = property(get_value, set_value, doc="ct value")
    
    # --------------------------------------------------------------------------
    # acquisition
    # --------------------------------------------------------------------------

    def get_acquisition(self):
        return self.get_action_cache()
    
    acquisition = property(get_acquisition, doc="acquisition object")
    
    def start_acquisition(self, value=None):
        self._aborted = False
        value = value or self._wvalue
        if not self._simulation_mode:
            self.acquisition.run(items=(self,), integration_time=value, master=self,
                termination_mode=AcquisitionTerminationMode.TerminateOnMaster)

    