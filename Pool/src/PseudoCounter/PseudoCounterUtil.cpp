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
// Revision 1.3  2007/08/23 10:33:42  tcoutinho
// - basic pseudo counter check
// - some fixes regarding pseudo motors
//
// Revision 1.2  2007/08/20 06:37:31  tcoutinho
// development commit
//
// Revision 1.1  2007/08/17 13:11:04  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.2  2007/06/27 12:23:02  tcoutinho
// string changes for consistency sake
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

#include <Python.h>
#include <Pool.h>
#include <PseudoCounterUtil.h>
#include <PseudoCounter.h>
#include <PseudoCounterClass.h>

namespace PseudoCounter_ns
{
	
void PseudoCounterUtil::remove_object(Tango::Device_3Impl *dev)
{
	pool_dev->remove_pseudo_counter(static_cast<PseudoCounter_ns::PseudoCounter *>(dev));
}

long PseudoCounterUtil::get_static_attr_nb(Tango::DeviceClass *cl_ptr)
{
	return (static_cast<PseudoCounter_ns::PseudoCounterClass *>(cl_ptr))->nb_static_attr;
}

void PseudoCounterUtil::add_2_full_name(Pool_ns::PoolElement *pe)
{
	Pool_ns::PseudoCounterPool *pce = static_cast<Pool_ns::PseudoCounterPool *>(pe);
	pce->user_full_name += " PseudoCounter";
	pce->user_full_name_extra = pce->user_full_name + " Counter list: ";
}

}	// namespace_ns

