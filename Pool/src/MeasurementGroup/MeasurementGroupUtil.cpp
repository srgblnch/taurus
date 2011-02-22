#include <Pool.h>
#include <MeasurementGroupUtil.h>
#include <MeasurementGroup.h>
#include <MeasurementGroupClass.h>

namespace MeasurementGroup_ns
{

void MeasurementGroupUtil::remove_object(Tango::Device_3Impl *dev)
{
	pool_dev->remove_measurement_group(static_cast<MeasurementGroup_ns::MeasurementGroup *>(dev));
}

long MeasurementGroupUtil::get_static_attr_nb(Tango::DeviceClass *cl_ptr)
{
	return (static_cast<MeasurementGroup_ns::MeasurementGroupClass *>(cl_ptr))->nb_static_attr;
}

void MeasurementGroupUtil::add_2_full_name(Pool_ns::PoolElement *pe)
{
	pe->user_full_name = pe->user_full_name + " ExpChannel list: ";
}

}	// namespace_ns

