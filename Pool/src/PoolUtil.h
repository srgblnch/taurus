//=============================================================================
//
// file :        PoolUtil.h
//
// description : Include for some utility classes for the Sardana project
//				 pool device
//
// project :	Sardana pool device
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.4  2007/02/08 16:18:14  tcoutinho
// - controller safety on PoolGroupBaseDev
//
// Revision 1.3  2007/02/06 09:41:03  tcoutinho
// - added MeasurementGroup
//
// Revision 1.2  2006/07/07 12:38:43  etaurel
// - Some changes in file header
// - Commit after implementing the group multi motor read
//
// Revision 1.1  2006/06/19 12:31:50  etaurel
// -Split Pool.cpp file in two
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//=============================================================================


#ifndef POOLUTIL_H_
#define POOLUTIL_H_

#include <Utils.h>
#include <Pool.h>

namespace Pool_ns
{

/**
 * This class stores the State of a specific Pool device at the creation
 * time of this object.
 * When the destructor is called the new state is compared with the previously
 * stored state. If there is a difference, a state event is sent.
 */
class PoolStateEvent
{
public:
	
	/**
	 * Constructor
	 * @param dev a pointer to the Pool device
	 */
	PoolStateEvent(Pool *dev):the_pool(dev) {d_state = the_pool->get_state();}
	
	/// Destructor
	~PoolStateEvent();
	
protected:
	/** a pointer to the Pool device */
	Pool				*the_pool;
	
	/** the state of the Pool device */
	Tango::DevState		d_state;
};

}

#endif /*POOLUTIL_H_*/
