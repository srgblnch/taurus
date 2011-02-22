#include <Pool.h>
#include <MotorGroupUtil.h>
#include <MotorGroup.h>
#include <MotorGroupClass.h>


namespace MotorGroup_ns
{
	
void MotorGroupUtil::remove_object(Tango::Device_3Impl *dev)
{
	pool_dev->remove_motor_group(static_cast<MotorGroup_ns::MotorGroup *>(dev));
}

long MotorGroupUtil::get_static_attr_nb(Tango::DeviceClass *cl_ptr)
{
	// So far there are no dynamic attributes for the motor group
	// So the three static attributes are: State, Status, Position
	return 3;
}

void MotorGroupUtil::add_2_full_name(Pool_ns::PoolElement *pe)
{
	pe->user_full_name = pe->user_full_name + " Motor list: ";
}

}	// namespace_ns
