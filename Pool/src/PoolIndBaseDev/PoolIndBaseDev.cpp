static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         PoolIndBaseDev.cpp
//
// description :  C++ source for the PoolIndBaseDev class
//
// project :      Sardana device Pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.1  2007/08/17 13:10:09  tcoutinho
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

#include <Utils.h>
#include <CtrlFiCa.h>
#include <tango.h>

#include <Pool.h>
#include <PoolIndBaseDev.h>
#include <Motor.h>

namespace Pool_ns
{

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev: ctor
// 
// description : 	Ctor of the PoolIndBaseDev class
//
//-----------------------------------------------------------------------------
PoolIndBaseDev::PoolIndBaseDev(Tango::DeviceClass *cl,string &s)
:PoolBaseDev(cl,s.c_str())
{
	base_init();
}

PoolIndBaseDev::PoolIndBaseDev(Tango::DeviceClass *cl,const char *s)
:PoolBaseDev(cl,s)
{
	base_init();
}

PoolIndBaseDev::PoolIndBaseDev(Tango::DeviceClass *cl,const char *s,const char *d)
:PoolBaseDev(cl,s,d)
{
	base_init();
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev: base_init
// 
// description : 	Method really doing the ctor work
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::base_init()
{
	PoolBaseDev::base_init();
	
//
// Init some variables
//
// WARNING: Do not init the data member init_cmd to false
// here, it will make the motor Init command to fail (at least)
//

	ctrl_code_online = true;
	simu = false;
	ctrl_dev_built = false;
	sf_index = 0;
	unknown_state = false;
	
//
// Extract ctrl name and channel id from device name
//

	analyse_name(get_name(),ctrl_inst_name,ct_idx);
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev: init_pool_element
// 
// description : 	Initialise all the data memebers of a PoolElement
//					structure. This method is used when a new device
//					is added to the Pool
//
// arg(s) : In : - pe : Pointer to the PoolElement structure which will
//						be initialized
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::init_pool_element(PoolElement *pe)
{
	PoolBaseDev::init_pool_element(pe);

	long ctrl_id;
	
	Pool_ns::ControllerPool &p_cp = 
		pool_dev->get_ctrl_from_inst_name(ctrl_inst_name);

	my_ctrl = p_cp.ctrl;
	ctrl_id = p_cp.id;
	fica_built = p_cp.ctrl_fica_built;
	if (fica_built == true)
		fica_ptr = *(p_cp.ite_ctrl_fica);
	else
		fica_ptr = NULL;
		
	pe->obj_idx = ct_idx;
	pe->ctrl_id = ctrl_id;
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::analyse_name
// 
// description : 	Extract controller name and channel number from a experiment channel
//					device name
//
// arg(s) : In : - dev_name : The device name
//			Out : - ctrl_inst_name : The controller instance name
//			      - id : The channel number
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::analyse_name(string &dev_name,string &ctrl_inst_name,long &id)
{
	string::size_type pos;
	pos = dev_name.rfind('/');
	ctrl_inst_name = dev_name.substr(0,pos);
	ctrl_inst_name.erase(0,ctrl_inst_name.find('/') + 1);
	
	pos++;
	string tmp = dev_name.substr(pos);
	
	id = atol(tmp.c_str());
}



//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::a_new_child
// 
// description : 	Inform the device controller that we now exist
//
// arg(s) : In : - ctrl_id : The controller identifier
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::a_new_child(long ctrl_id)
{
	if (my_ctrl != NULL)
	{
		try
		{
			Pool_ns::AutoPoolLock lo(fica_ptr->get_mon());
			Pool_ns::ControllerPool &ctrl_ref = pool_dev->get_ctrl_from_id(ctrl_id);
			ctrl_ref.nb_dev++;

			my_ctrl->AddDevice(ct_idx);
			ctrl_dev_built = true;
			ctrl_add_dev_error_str.clear();
		}
		catch (Tango::DevFailed &e)
		{
			ctrl_add_dev_error_str = "\nError reported by the controller during its AddDevice method:";
			ctrl_add_dev_error_str = ctrl_add_dev_error_str + "\n\t" + e.errors[0].desc.in();
			set_state(Tango::FAULT);
		}
	}
	else
		set_state(Tango::FAULT);
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::suicide
// 
// description : 	Inform the device controller that we are dying
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::suicide()
{
	if (my_ctrl != NULL)
	{
		Pool_ns::AutoPoolLock lo(fica_ptr->get_mon());
		if ((my_ctrl != NULL) && (this->ctrl_code_online == true))
		{
			my_ctrl->DeleteDevice(ct_idx);
			Pool_ns::ControllerPool &ctrl_ref = pool_dev->get_ctrl_from_inst_name(ctrl_inst_name);
			ctrl_ref.nb_dev--;
		}
	}
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::delete_from_pool
// 
// description : 	Delete a device first from its controller and then from
//					the pool structure
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::delete_from_pool()
{
	suicide();
	PoolBaseDev::delete_from_pool();
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::base_always_executed_hook
// 
// description : 	The basic code to be executed in the always_executed_hook
//					method of Pool based device
//
// Arg(s) : In : - motor: Boolean set to true if the device is a motor for
//						  which the limit switches (with ALARM state) has
//						  to be taken into account
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::base_always_executed_hook(bool motor,bool propagate)
{
	if (simu == false)
	{
		Tango::DevState old_state = get_state();
		
		if (fica_built == true)
		{
			unknown_state = false;
			Pool_ns::PoolLock &pl = fica_ptr->get_mon();
			Pool_ns::AutoPoolLock lo(pl);
			if ((my_ctrl == NULL) || (this->ctrl_code_online == false))
			{
				set_state(Tango::FAULT);
			}
			else
			{
				if (ctrl_dev_built == true)
				{
					MotorController::MotorState mi;
					try
					{
						
//
// There is a trick here for client getting position using polling mode
// The motion thread stores motor position in the polling buffer and
// the client is getting position from this polling buffer
// When the motion thread detects that the motion is over
// (state != MOVING), it invalidates data from the polling buffer and
// therefore all clients will get data from hardware access.
// What could happens, is that a client thread detects first the
// end of the motion (before the motion thread). If this thread
// immediately reads the position after it detects the motion end, it will
// get the last value written in the polling buffer because the mov thread has not
// yet invalidate it.
// Therefore, if the thread executing this code is not the mov thread and if the state
// changed from MOVING to ON, delay the state changes that it will be detected by the
// motion thread. This motion thread is doing a motor call every 10 mS
//

						int th_id = omni_thread::self()->id();

						read_state_from_ctrl(mi,false);
						
						set_state((Tango::DevState)mi.state);
//						if (motor == true)
//							static_cast<Motor_ns::Motor *>(this)->store_switches(mi.switches);

						if (mov_th_id != 0)
						{						
							if ((old_state == Tango::MOVING) && 
							    ((get_state() == Tango::ON) || (get_state() == Tango::ALARM)) && 
							    (th_id != mov_th_id) &&
							    (abort_cmd_executed == false))
									set_state(Tango::MOVING);
						}

						if (motor == true)
						{
							if ((mi.switches >= 2) && (get_state() != Tango::MOVING))
								set_state(Tango::ALARM);
						}
						if ((Tango::DevState)mi.state == Tango::FAULT)
							ctrl_error_str = mi.status;
					}
					catch(Tango::DevFailed &e)
					{
						set_state(Tango::UNKNOWN);
						ctrl_error_str = "\nError reported from controller when requesting for object state";
						ctrl_error_str = ctrl_error_str + "\n\t" + e.errors[0].desc.in();
					}					
				}
				else
					set_state(Tango::FAULT);
			}
		}
		else
		{
			set_state(Tango::FAULT);
		}
		
//
// If necessary notify the ghost group of changes in this element.
// The ghost group will itself notify any internal listeners. 
//
		if(propagate == true)
		{
			inform_ghost(old_state,get_state());
		}
	}	
}


//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::read_state_from_ctrl
// 
// description : 	Read a single motor state from the controller.
//
// args : - sta : The structure used to pass object state
//		  - lock : A boolean set to true if the method must lock the controller
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::read_state_from_ctrl(Controller::CtrlState &sta,bool lock)
{
	try
	{
		if (lock == true)
		{
			Pool_ns::AutoPoolLock lo(fica_ptr->get_mon());
		
			my_ctrl->PreStateAll();
			my_ctrl->PreStateOne(ct_idx);
			my_ctrl->StateAll();
			my_ctrl->StateOne(ct_idx,&sta);
		}
		else
		{
			my_ctrl->PreStateAll();
			my_ctrl->PreStateOne(ct_idx);
			my_ctrl->StateAll();
			my_ctrl->StateOne(ct_idx,&sta);
		}
	}
	SAFE_CATCH(fica_ptr->get_name(),"read_state_from_controller");
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::base_dev_status
// 
// description : 	
//
//-----------------------------------------------------------------------------

void PoolIndBaseDev::base_dev_status(Tango::ConstDevString base_status)
{
	tmp_status = base_status;
	Tango::DevState sta = get_state();
	
//
// If the counter is in FAULT, it could be because the ctrl is not OK.
// Otherwise, checks if the controller send an error string
//

	if (sta == Tango::FAULT)
	{
		if (fica_built == true)
		{
			if (ctrl_dev_built == false)
			{
				tmp_status = tmp_status + ctrl_add_dev_error_str;
			}
			else
			{
				bool ctrl_ok = true;
				{
					Pool_ns::AutoPoolLock lo(fica_ptr->get_mon());
		
					if (my_ctrl == NULL)
					{
						tmp_status = tmp_status + "\nMy controller object (" + ctrl_inst_name + ") is not initialized";
						ctrl_ok = false;
					}
				
					if (this->ctrl_code_online == false)
					{
						tmp_status = tmp_status + "\nMy controller code (" + ctrl_inst_name + ") is not loaded in memory";
						ctrl_ok = false;
					}
				}
					
				if (unknown_state == true)
				{
					tmp_status = tmp_status + "\nThe controller returns an unforeseen state";
				}
				
				if ((ctrl_ok == true) && (ctrl_error_str.size() != 0))
					tmp_status = tmp_status + '\n' + ctrl_error_str;
			}
		}
		else
		{
			tmp_status = tmp_status + "\nMy controller object (" + ctrl_inst_name + ") is not initialized";
		}
	}
	else if (sta == Tango::UNKNOWN)
	{
		tmp_status = tmp_status + ctrl_error_str;
	}
	
	if (simu == true)
		tmp_status = tmp_status + "\nDevice is in simulation mode";
}

//+----------------------------------------------------------------------------
//
// method : 		PoolIndBaseDev::get_extra_attributes
// 
// description : 	Obtains a list of extra attribute information for this device
//
//-----------------------------------------------------------------------------
vector<PoolExtraAttr> &PoolIndBaseDev::get_extra_attributes()
{
	if(!fica_built)
	{
		TangoSys_OMemStream o;
		o << "Can't get extra attribute information from " << alias << ".";
		o << "The FiCa is not build." << ends;
		Tango::Except::throw_exception((const char *)"Pool_CantGetExtraAttributes",o.str(),
									   (const char *)"MeasurementGroup::get_extra_attributes()");	
	}
	
	return fica_ptr->get_extra_attributes();
}

}	//	namespace
