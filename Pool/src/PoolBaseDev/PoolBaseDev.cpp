static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         PoolBaseDev.cpp
//
// description :  C++ source for the PoolBaseDev class
//
// project :      Sardana device Pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.10  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.9  2007/05/25 12:48:10  tcoutinho
// fix the same dead locks found on motor system to the acquisition system since release labeled for Josep Ribas
//
// Revision 1.8  2007/05/22 13:43:09  tcoutinho
// - added new method
//
// Revision 1.7  2007/02/22 12:03:04  tcoutinho
// - additional "get extra attribute"  needed by the measurement group
//
// Revision 1.6  2007/02/08 07:55:54  etaurel
// - Changes after compilation -Wall. Handle case of different ctrl for the
// same class of device but with same extra attribute name
//
// Revision 1.5  2007/01/30 16:42:09  etaurel
// - Fix bug in PoolBaseDev data member initialization
//
// Revision 1.4  2007/01/30 15:57:41  etaurel
// - Add a missing data member init
// - Add code to manage case with different controller of the same Tango class
// with extra attribute of the same name but with different data type
//
// Revision 1.3  2007/01/26 08:34:32  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.2  2007/01/18 16:07:07  etaurel
// - Fix abug in the ctrl_dev_built data initialisation
//
// Revision 1.1  2007/01/16 14:22:25  etaurel
// - Initial revision
//
//
// copyleft :   CELLS/ALBA
//		Edifici Ciences Nord
//		Campus Universitari de Bellaterra
//		Universitat Autonoma de Barcelona
//		08193 Bellaterra, Barcelona, SPAIN
//
//-=============================================================================

#include <CtrlFiCa.h>
#include <Pool.h>
#include <PoolBaseDev.h>
#include <PoolBaseUtil.h>
#include <pool/Ctrl.h>

namespace Pool_ns
{

//------------------------------------------------------------------------------
// PoolBaseDev::PoolBaseDev
//
PoolBaseDev::PoolBaseDev(Tango::DeviceClass *cl,string &s)
:Tango::Device_3Impl(cl,s.c_str())
{
}

//------------------------------------------------------------------------------
// PoolBaseDev::PoolBaseDev
//
PoolBaseDev::PoolBaseDev(Tango::DeviceClass *cl,const char *s)
:Tango::Device_3Impl(cl,s)
{
}

//------------------------------------------------------------------------------
// PoolBaseDev::PoolBaseDev
//
PoolBaseDev::PoolBaseDev(Tango::DeviceClass *cl,const char *s,const char *d)
:Tango::Device_3Impl(cl,s,d)
{
}

//------------------------------------------------------------------------------
// PoolBaseDev::base_init
//
void PoolBaseDev::base_init()
{
//
// Init some variables
//
// WARNING: Do not init the data member init_cmd to false
// here, it will make the Init command to fail (at least)
//

	abort_cmd_executed = false;
	mov_th_id = 0;
	pool_sd = false;
	
//
// Get device alias
//
	
	Tango::Util *tg = Tango::Util::instance();
	try
	{
		tg->get_database()->get_alias(get_name(),alias);
	}
	catch(Tango::DevFailed &e)
	{
		DEBUG_STREAM << "No alias defined for " << get_name() <<
						". This is normal if this is a ghost group" << endl;
	}
	
//
// Get the pool object(s) and retrieve my controller and my index
// Each Pool device server has only one device
// Therefore, dev_list[0] is the pool device
//

	vector<Tango::DeviceImpl *> &dev_list = tg->get_device_list_by_class("Pool");
	pool_dev = static_cast<Pool_ns::Pool *>(dev_list[0]);
}

//------------------------------------------------------------------------------
// PoolBaseDev::init_pool_element
//
void PoolBaseDev::init_pool_element(PoolElement *pe)
{
	pe->name = alias;
	pe->obj_alias_lower = alias;
	transform(pe->obj_alias_lower.begin(),
			  pe->obj_alias_lower.end(),
			  pe->obj_alias_lower.begin(),
			  ::tolower);
	pe->obj_tango_name = get_name();

	pe->user_full_name = alias + " (" + pe->obj_tango_name + ")";
	utils->add_2_full_name(pe);		
	pe->obj_proxy = NULL;
	pe->id = get_id();
}

//------------------------------------------------------------------------------
// PoolBaseDev::delete_from_pool
//
void PoolBaseDev::delete_from_pool()
{
	init_cmd = true;
	{
		Tango::AutoTangoMonitor atm(pool_dev);	
		utils->remove_object(this);
	}
}

//------------------------------------------------------------------------------
// PoolBaseDev::delete_utils
//
void PoolBaseDev::delete_utils()
{ 
	if (utils!=NULL)
		delete utils; 
}

//------------------------------------------------------------------------------
// PoolBaseDev::dbg_dvector
//
void PoolBaseDev::dbg_dvector(vector<double>& v, const char* optcstr)
{
	if(get_logger()->is_debug_enabled() == false)
		return;
	
	stringstream s;
	s << optcstr;
	vector<double>::iterator pos = v.begin();
	for(; pos != v.end() ; ++pos)
	{
		s << (*pos) << ' ';
	}
		
	DEBUG_STREAM << s.str() << endl;
}


}	//	namespace
