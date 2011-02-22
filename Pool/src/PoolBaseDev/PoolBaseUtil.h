//=============================================================================
//
// file :        PoolBaseUtil.h
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
// Revision 1.1  2007/01/16 14:22:25  etaurel
// - Initial revision
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

#ifndef _POOLBASEUTIL_H
#define _POOLBASEUTIL_H

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Tango
{
	class Device_3Impl;
	class DeviceClass;
}

namespace Pool_ns
{

class Pool;
struct PoolElement;

/**
 * The Base utility class
 */
class PoolBaseUtil
{
public:
	PoolBaseUtil(Pool *p_ptr):pool_dev(p_ptr) {}
	virtual ~PoolBaseUtil() {}
	
	virtual void remove_object(Tango::Device_3Impl *) = 0;
	virtual long get_static_attr_nb(Tango::DeviceClass *) = 0;
	virtual void add_2_full_name(PoolElement *) {}

protected:	
	Pool 				*pool_dev;
};

}	// namespace_ns

#endif	// _POOLBASEUTIL_H
