//=============================================================================
//
// file :        CTExpChannelUtil.h
//
// description : Include for the PoolBaseUtil class.
//
// project :	Sardana Device Pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.2  2007/08/17 13:07:29  tcoutinho
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

/**
 * @author	$Author$
 * @version	$Revision$
 */

#ifndef _CTEXPCHANNELUTIL_H
#define _CTEXPCHANNELUTIL_H

#include <PoolBaseUtil.h>


namespace Pool_ns
{
struct PoolElement;
}

namespace CTExpChannel_ns
{

/**
 * The CTExpChannel utility class
 */
class CTExpChannelUtil:public Pool_ns::PoolBaseUtil
{
public:
	CTExpChannelUtil(Pool_ns::Pool *p_ptr):Pool_ns::PoolBaseUtil(p_ptr) {}
	virtual ~CTExpChannelUtil() {}
	
	virtual void remove_object(Tango::Device_3Impl *dev);
	virtual long get_static_attr_nb(Tango::DeviceClass *);
	virtual void add_2_full_name(Pool_ns::PoolElement *);
};

}	// namespace_ns

#endif	// _CTEXPCHANNELUTIL_H
