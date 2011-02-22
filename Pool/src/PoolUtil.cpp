static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         PoolUtil.cpp
//
// description :  C++ source for the Pool and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Pool are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.6  2007/02/06 09:41:03  tcoutinho
// - added MeasurementGroup
//
// Revision 1.5  2006/07/07 12:38:43  etaurel
// - Some changes in file header
// - Commit after implementing the group multi motor read
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//+=============================================================================

#include <PoolUtil.h>

namespace Pool_ns
{
	
PoolStateEvent::~PoolStateEvent()
{
	Tango::DevState new_d_state;
	try
	{
		new_d_state = the_pool->dev_state();
	}
	catch (Tango::DevFailed &e)
	{
		new_d_state = Tango::UNKNOWN;
	}
	
	if (new_d_state != d_state)
	{
		Tango::Attribute &state_att = the_pool->get_device_attr()->get_attr_by_name("state");
		state_att.fire_change_event();
	}
}

}
