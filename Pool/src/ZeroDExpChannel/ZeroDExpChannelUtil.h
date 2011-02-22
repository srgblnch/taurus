//=============================================================================
//
// file :        ZeroDExpChannelUtil.h
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
// Revision 1.1  2007/01/26 08:35:02  etaurel
// - We now have a first release of ZeroDController
//
//
// copyleft :   CELLS/ALBA
//		Edifici Ciences Nord
//		Campus Universitari de Bellaterra
//		Universitat Autonoma de Barcelona
//		08193 Bellaterra, Barcelona, SPAIN
//
//=============================================================================

#ifndef _ZERODEXPCHANNELUTIL_H
#define _ZERODEXPCHANNELUTIL_H

#include <PoolBaseUtil.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Pool_ns
{
struct PoolElement;
}

namespace ZeroDExpChannel_ns
{

/**
 * The 0D Experiment Channel utility class
 */
class ZeroDExpChannelUtil:public Pool_ns::PoolBaseUtil
{
public:
	/// Constructor
	ZeroDExpChannelUtil(Pool_ns::Pool *p_ptr):Pool_ns::PoolBaseUtil(p_ptr) {}
	/// Destructor
	virtual ~ZeroDExpChannelUtil() {}
	
	virtual void remove_object(Tango::Device_3Impl *dev);
	virtual long get_static_attr_nb(Tango::DeviceClass *);
	virtual void add_2_full_name(Pool_ns::PoolElement *);
};

}	// namespace_ns

#endif	// _ZERODEXPCHANNELUTIL_H
