//=============================================================================
//
// file :        CommunicationChannelUtil.h
//
// description : Include for the CommunicationChannelUtil class.
//
// project :	Sardana Device Pool

// copyleft :   CELLS/ALBA
//		Edifici Ciences Nord
//		Campus Universitari de Bellaterra
//		Universitat Autonoma de Barcelona
//		08193 Bellaterra, Barcelona, SPAIN
//
//=============================================================================

#include <Python.h>
#include <Pool.h>
#include <CommunicationChannelUtil.h>
#include <CommunicationChannel.h>
#include <CommunicationChannelClass.h>


namespace CommunicationChannel_ns
{

	
void CommunicationChannelUtil::remove_object(Tango::Device_3Impl *dev)
{
	pool_dev->remove_com_channel(static_cast<CommunicationChannel_ns::CommunicationChannel *>(dev));
}

long CommunicationChannelUtil::get_static_attr_nb(Tango::DeviceClass *cl_ptr)
{
	return (static_cast<CommunicationChannel_ns::CommunicationChannelClass *>(cl_ptr))->nb_static_attr;
}

}	// namespace_ns

