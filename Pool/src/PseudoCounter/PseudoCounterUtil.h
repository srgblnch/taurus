//=============================================================================
//
// file :        CTExpChannelUtil.h
//
// description : Include for the PoolBaseDev class.
//
// project :	Sardana Device Pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.1  2007/08/17 13:11:04  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.1  2007/01/16 14:23:19  etaurel
// - First release with Counter Timer
//
//
//
// copyleft :   CELLS/ALBA
//		Edifici Ciences Nord
//		Campus Universitari de Bellaterra
//		Universitat Autonoma de Barcelona
//		08193 Bellaterra, Barcelona, SPAIN
//
//=============================================================================

#ifndef _PSEUDOCOUNTERUTIL_H
#define _PSEUDOCOUNTERUTIL_H

#include <PoolBaseUtil.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Pool_ns
{
	struct PoolElement;
}

namespace PseudoCounter_ns
{

/**
 * The PseudoCounter utility class
 */
class PseudoCounterUtil:public Pool_ns::PoolBaseUtil
{
public:
	PseudoCounterUtil(Pool_ns::Pool *p_ptr):Pool_ns::PoolBaseUtil(p_ptr) {}
	virtual ~PseudoCounterUtil() {}
	
	virtual void remove_object(Tango::Device_3Impl *dev);
	virtual long get_static_attr_nb(Tango::DeviceClass *);
	virtual void add_2_full_name(Pool_ns::PoolElement *);
};

}	// namespace_ns

#endif	// _PSEUDOCOUNTERUTIL_H
