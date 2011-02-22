#ifndef MEASUREMENTGROUPUTIL_H_
#define MEASUREMENTGROUPUTIL_H_

#include <PoolBaseUtil.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Pool_ns
{
struct PoolElement;
}

namespace MeasurementGroup_ns
{

/**
 * The MeasurementGroup utility class
 */
class MeasurementGroupUtil : public Pool_ns::PoolBaseUtil
{
public:
	MeasurementGroupUtil(Pool_ns::Pool *p_ptr):Pool_ns::PoolBaseUtil(p_ptr) {}
	virtual ~MeasurementGroupUtil() {}
	
	virtual void remove_object(Tango::Device_3Impl *dev);
	virtual long get_static_attr_nb(Tango::DeviceClass *);
	virtual void add_2_full_name(Pool_ns::PoolElement *);
};

}	// namespace_ns

#endif /*MEASUREMENTGROUPUTIL_H_*/
