static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         PoolUtil.cpp
//
// description :  C++ source for the Pool and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Pool are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.62  2007/09/07 15:00:06  tcoutinho
// safety commit
//
// Revision 1.61  2007/08/30 12:40:39  tcoutinho
// - changes to support Pseudo counters.
//
// Revision 1.60  2007/08/24 15:55:26  tcoutinho
// safety weekend commit
//
// Revision 1.59  2007/08/23 10:32:44  tcoutinho
// - basic pseudo counter check
// - some fixes regarding pseudo motors
//
// Revision 1.58  2007/08/20 06:37:32  tcoutinho
// development commit
//
// Revision 1.57  2007/08/17 15:37:29  tcoutinho
// - fix bug: in case pseudo motor controller is in error
//
// Revision 1.56  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.55  2007/08/08 10:46:35  tcoutinho
// Fix bug 17 : Controller properties should be Pool properties and not global properties
//
// Revision 1.54  2007/08/08 09:10:36  tcoutinho
// - dropped feature : controller class properties are no longer supported. This is a preliminary step towards moving controller properties from global properties in the database to properties in the pool device were the controller belongs (resolution of bug 17)
//
// Revision 1.53  2007/08/08 08:47:55  tcoutinho
// Fix bug 18 : CreateController should be a one step operation
//
// Revision 1.52  2007/07/30 11:01:00  tcoutinho
// Fix bug 5 : Additional information for controllers
//
// Revision 1.51  2007/07/26 10:25:15  tcoutinho
// - Fix bug 1 :  Automatic temporary MotorGroup/MeasurementGroup deletion
//
// Revision 1.50  2007/07/17 11:41:57  tcoutinho
// replaced comunication with communication
//
// Revision 1.49  2007/07/02 14:46:37  tcoutinho
// first stable comunication channel commit
//
// Revision 1.48  2007/06/28 16:22:38  tcoutinho
// safety commit during comunication channels development
//
// Revision 1.47  2007/06/28 07:15:34  tcoutinho
// safety commit during comunication channels development
//
// Revision 1.46  2007/06/27 08:56:28  tcoutinho
// first commit for comuncation channels
//
// Revision 1.45  2007/04/30 15:47:05  tcoutinho
// - new attribute "Channels"
// - new device property "Channel_List"
// - when add/remove channel, pool sends a change event on the MeasurementGroupList
//
// Revision 1.44  2007/04/30 14:51:20  tcoutinho
// - make possible to Add/Remove elements on motorgroup that are part of other motor group(s)
//
// Revision 1.43  2007/04/23 15:23:06  tcoutinho
// - changes according to Sardana metting 26-03-2007: ActiveMeasurementGroup attribute became obsolete
//
// Revision 1.42  2007/02/28 16:21:52  tcoutinho
// - support for 0D channels
// - basic fixes after running first battery of tests on measurement group
//
// Revision 1.41  2007/02/22 12:02:00  tcoutinho
// - added support for ghost measurement group
// - added support for measurement group in init/reload controller operations
// - added support of internal events on measurement group
//
// Revision 1.40  2007/02/14 11:20:12  tcoutinho
// - fix valgrind error by splitting a string concatenation operation in two operations (a little strange this valgrind error...)
//
// Revision 1.39  2007/02/13 14:39:43  tcoutinho
// - fix bug in motor group when a motor or controller are recreated due to an InitController command
//
// Revision 1.38  2007/02/08 10:49:29  etaurel
// - Some small changes after the merge
//
// Revision 1.37  2007/02/08 08:51:16  etaurel
// - Many changes. I don't remember the list
//
// Revision 1.36  2007/02/06 09:41:03  tcoutinho
// - added MeasurementGroup
//
// Revision 1.35  2007/01/30 16:00:47  etaurel
// - The CT Value attribute is now a Double
// WARNING, this change has made us discouvering the GCC BUG  2834...
// Some code is now between ifdef/endif statement to compile using gcc 3.3
//
// Revision 1.34  2007/01/26 08:36:48  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.33  2007/01/23 08:27:22  tcoutinho
// - fix some pm bugs found with the test procedure
// - added internal event for MotionEnded
//
// Revision 1.32  2007/01/17 09:38:14  tcoutinho
// - internal events bug fix.
//
// Revision 1.31  2007/01/16 14:32:22  etaurel
// - Coomit after a first release with CT
//
// Revision 1.30  2007/01/09 07:52:10  tcoutinho
// - changes to make it gcc 4.1 compatible
//
// Revision 1.29  2007/01/05 15:02:38  etaurel
// - First implementation of the Counter Timer class
//
// Revision 1.28  2007/01/05 13:02:05  tcoutinho
// - changes to support internal event propagation
//
// Revision 1.27  2006/12/28 15:36:58  etaurel
// - Fire events also on the motor limit_switches attribute
// - Throw events even in simulation mode
// - Mange motor position limit switches dependant of the offset attribute
//
// Revision 1.26  2006/12/20 10:25:35  tcoutinho
// - changes to support internal event propagation
// - bug fix in motor groups containing other motor groups or pseudo motors
//
// Revision 1.25  2006/12/12 11:09:16  tcoutinho
// - support for pseudo motors and motor groups in a motor group
//
// Revision 1.24  2006/11/24 08:48:45  tcoutinho
// - minor changes
//
// Revision 1.23  2006/11/21 14:40:40  tcoutinho
// bug fix on group_exit method
//
// Revision 1.22  2006/11/20 14:32:43  etaurel
// - Add ghost group and event on motor group position attribute
//
// Revision 1.21  2006/11/07 14:57:09  etaurel
// - Now, the pool really supports different kind of controllers (cpp and py)
//
// Revision 1.20  2006/11/03 15:48:59  etaurel
// - Miscellaneous changes that I don't remember
//
// Revision 1.19  2006/10/27 14:43:02  etaurel
// - New management of the MaxDevice stuff
// - SendToCtrl cmd added
// - Some bug fixed in prop management
//
// Revision 1.18  2006/10/27 14:02:19  tcoutinho
// added support for class level properties in the Database
//
// Revision 1.17  2006/10/23 15:12:36  etaurel
// - Fix memory leak in several places
//
// Revision 1.16  2006/10/23 13:36:57  etaurel
// - First implementation of controller properties for CPP controller
//
// Revision 1.15  2006/10/20 15:37:30  etaurel
// - First release with GetControllerInfo command supported and with
// controller properties
//
// Revision 1.14  2006/10/17 14:28:10  tcoutinho
// bug fixes on properties
//
// Revision 1.13  2006/10/06 15:41:03  tcoutinho
// bug fixes: - error report in GetPseudoMotorInfo.
//                 - missed instatiation of pseudo_proxy in the PseudoMotorPool structure.
//
// Revision 1.12  2006/10/06 13:36:39  tcoutinho
// changed info command names, added properties functionality
//
// Revision 1.11  2006/10/02 09:19:12  etaurel
// - Motor controller now supports extra features (both CPP and Python)
//
// Revision 1.10  2006/09/29 12:53:22  tcoutinho
// *** empty log message ***
//
// Revision 1.9  2006/09/27 15:15:50  etaurel
// - ExternalFile and CtrlFile has been splitted in several classes:
//   ExternalFile, CppCtrlFile, PyExternalFile and PyCtrlFile
//
// Revision 1.8  2006/09/22 15:31:01  etaurel
// - Miscellaneous changes
//
// Revision 1.7  2006/09/21 10:20:54  etaurel
// - The motor group do not ID any more
//
// Revision 1.6  2006/09/21 08:01:31  etaurel
// - Now all test suite is OK withou ID on motor interface
//
// Revision 1.5  2006/09/21 07:25:58  etaurel
// - Changes due to the removal of Motor ID in the Tango interface
//
// Revision 1.4  2006/09/20 16:00:36  tcoutinho
// pseudo motor API changed
//
// Revision 1.3  2006/09/20 13:11:12  etaurel
// - For the user point of view, the controller does not have ID any more.
// We are now using the controller instance name (uniq) to give them a name
//
// Revision 1.2  2006/09/18 10:32:22  etaurel
// - Commit after merge with pseudo-motor branch
//
// Revision 1.1  2006/07/07 13:39:14  etaurel
// - A new file
//
// Revision 1.4  2006/06/28 15:56:46  etaurel
// - Commit after first series of tests
//
// Revision 1.3  2006/06/22 14:49:24  etaurel
// - Some more changes
//
// Revision 1.2  2006/06/21 14:48:12  etaurel
// - Don't remember the changes I did..
//
// Revision 1.1  2006/06/19 12:31:50  etaurel
// -Split Pool.cpp file in two
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//+=============================================================================

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <CtrlFiCa.h>
#include <PoolUtil.h>
#include <PoolClass.h>
#include <Motor.h>
#include <MotorGroup.h>
#include <PseudoMotor.h>
#include <CTExpChannel.h>
#include <ZeroDExpChannel.h>
#include <PseudoCounter.h>
#include <MeasurementGroup.h>
#include <CommunicationChannel.h>
#include <pool/Ctrl.h>
#include <pool/ComCtrl.h>
#include <pool/MotCtrl.h>
#include <pool/CoTiCtrl.h>
#include <pool/ZeroDCtrl.h>
#include <pool/PseudoCoCtrl.h>


namespace Pool_ns
{

//
// Three binary functions used to sort our lists (Controller and Motor)
//
bool PoolElemNameSort(const PoolElement *p1, const PoolElement *p2)
{ 
	return p1->name < p2->name; 
}

//------------------------------------------------------------------------------
// Pool::group_exist
// 
bool Pool::group_exist(vector<long> &g_in,string &g_name)
{
	list<MotorGroupPool>::iterator ite;
	vector<long> v_in(g_in);
	sort(v_in.begin(),v_in.end());
	
	unsigned long in_size = g_in.size();
	for (ite = mot_group_list.begin();ite != mot_group_list.end();++ite)
	{
		if (ite->mot_ids.size() != in_size)
			continue;

		vector<long> v_pool(ite->mot_ids);
		sort(v_pool.begin(),v_pool.end());
		
		if (v_pool == v_in)
		{
			g_name = ite->name;
			return true;
		}
	}
	
	return false;
}

//------------------------------------------------------------------------------
// Pool::group_exist
// 
bool Pool::group_exist(vector<string> &g_in,string &g_name)
{
	list<MotorGroupPool>::iterator ite;
	vector<string> v_in(g_in);
	sort(v_in.begin(),v_in.end());
	
	unsigned long in_size = g_in.size();
	for (ite = mot_group_list.begin();ite != mot_group_list.end();++ite)
	{
		if (ite->mot_ids.size() != in_size)
			continue;

		//TODO: wait for motor group to have a list of motor names
		vector<PoolElement*> v_pool(ite->group_elts);  
		sort(v_pool.begin(),v_pool.end(), PoolElemNameSort);
		
		for(unsigned long l = 0; l < g_in.size(); l++)
		{
			if(v_in[l] != v_pool[l]->name)
				return false;
		}
		g_name = ite->name;
		return true;
	}
	
	return false;
}

//------------------------------------------------------------------------------
// Pool::measurement_group_exist
// 
bool Pool::measurement_group_exist(vector<string> &g_in,string &g_name)
{
	list<MeasurementGroupPool>::iterator ite;
	vector<string> v_in(g_in);
	sort(v_in.begin(),v_in.end());

	unsigned long in_size = g_in.size();
	for (ite = measurement_group_list.begin();ite != measurement_group_list.end();++ite)
	{
		if (ite->ch_ids.size() != in_size)
			continue;

		vector<PoolElement*> v_pool(ite->group_elts);  
		sort(v_pool.begin(),v_pool.end(), PoolElemNameSort);
		
		bool is_equal = true;
		for(unsigned long l = 0; l < g_in.size(); l++)
		{
			string v_in_lower(v_in[l]);
			transform(v_in_lower.begin(),v_in_lower.end(),v_in_lower.begin(),::tolower);
			string v_pool_lower(v_pool[l]->name);
			transform(v_pool_lower.begin(),v_pool_lower.end(),v_pool_lower.begin(),::tolower);
			
			if(v_in_lower != v_pool_lower)
			{
				is_equal = false;
				break;
			}
		}
		
		if(is_equal == true)
		{
			g_name = ite->name;
			return true;
		}
	}
	
	return false;
}



//------------------------------------------------------------------------------
// Pool::f_name_from_db_prop
// 
string &Pool::f_name_from_db_prop(string &ctrl_name)
{
	long nb_ctrl = ctrl_list.size();

	string::size_type pos;
	pos = ctrl_name.find('/');
	string ctrl = ctrl_name.substr(0,pos);
	string inst = ctrl_name.substr(pos + 1);
	long ind_array;
 
	for (long l = 0;l < nb_ctrl;l++)
	{
		string f_name_db = ctrl_info[(l * PROP_BY_CTRL) + 1];
		if ((pos = f_name_db.find('.')) != string::npos)
			f_name_db.erase(pos);
		f_name_db = f_name_db + '.';
		string ctrl_class_db = ctrl_info[(l * PROP_BY_CTRL) + 2];
		ctrl_class_db.insert(0,f_name_db);
		string inst_db = ctrl_info[(l * PROP_BY_CTRL) + 3];
		
		transform(ctrl_class_db.begin(),ctrl_class_db.end(),ctrl_class_db.begin(),::tolower);
		transform(inst_db.begin(),inst_db.end(),inst_db.begin(),::tolower);
		
		if ((ctrl_class_db == ctrl) && (inst_db == inst))
		{
			ind_array = l * PROP_BY_CTRL;
			break;
		}
	}
	
	vector<string>::iterator v_ite = ctrl_info.begin();
	v_ite += ind_array;
	
	return *(v_ite + 1);
}

//------------------------------------------------------------------------------
// Pool::dev_type_from_db_prop
// 
CtrlType Pool::dev_type_from_db_prop(string &ctrl_name)
{
	long nb_ctrl = ctrl_list.size();
	string::size_type pos;

	pos = ctrl_name.find('/');
	string ctrl = ctrl_name.substr(0,pos);
	string inst = ctrl_name.substr(pos + 1);
	long ind_array;
 
	for (long l = 0;l < nb_ctrl;l++)
	{
		string ctrl_db = ctrl_info[(l * PROP_BY_CTRL) + 2];
		string inst_db = ctrl_info[(l * PROP_BY_CTRL) + 3];
		transform(ctrl_db.begin(),ctrl_db.end(),ctrl_db.begin(),::tolower);
		transform(inst_db.begin(),inst_db.end(),inst_db.begin(),::tolower);
		if ((ctrl_db == ctrl) && (inst_db == inst))
		{
			ind_array = l * PROP_BY_CTRL;
			break;
		}
	}
	
	vector<string>::iterator v_ite = ctrl_info.begin();
	v_ite += ind_array;
	
	return static_cast<PoolClass *>(device_class)->str_2_CtrlType(*v_ite);
}

//------------------------------------------------------------------------------
// Pool::restore_ctrl_obj
// 
void Pool::restore_ctrl_obj(vector<Pool::CtrlBkp> *ctrls,vector<Pool::ObjBkp> *objs)
{
//
// First, restore controllers
//

    if (ctrls != NULL)
    {
        vector<Pool::CtrlBkp>::iterator ctrl_ite;
        for (ctrl_ite = ctrls->begin();ctrl_ite != ctrls->end();++ctrl_ite)
        {
            list<ControllerPool>::iterator ct_ite = ctrl_list.begin();
            advance(ct_ite,ctrl_ite->dist);
            
            ct_ite->ctrl = ctrl_ite->good_old_ctrl;
        }
    }
    
//
// Now, restore objects
//

    if (objs != NULL)
    {
        vector<Pool::ObjBkp>::iterator obj_ite;
        for (obj_ite = objs->begin();obj_ite != objs->end();++obj_ite)
        {  
        	
//
// Is it a motor ?
//
 
        	if (obj_ite->type == MOTOR_CTRL)
        	{
	            list<MotorPool>::iterator ite_mot_rest = mot_list.begin();
	            advance(ite_mot_rest,obj_ite->dist);
	            
	            ControllerPool &ct = get_ctrl_from_motor(*ite_mot_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					MotorController *tmp_mc = 
						static_cast<MotorController *>(obj_ite->old_ctrl);
	            	ite_mot_rest->motor->set_ctrl(tmp_mc);
	           		ite_mot_rest->motor->ctrl_online();
				}
        	}

//
// Is it a pseudo motor ?
//
 
        	if (obj_ite->type == PSEUDO_MOTOR_CTRL)
        	{
	            list<PseudoMotorPool>::iterator ite_mot_rest = 
	            										pseudo_mot_list.begin();
	            advance(ite_mot_rest,obj_ite->dist);
	            
	            ControllerPool &ct = 
	            	get_ctrl_from_pool_elem(*ite_mot_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					PseudoMotorController *tmp_mc = 
						static_cast<PseudoMotorController *>(obj_ite->old_ctrl);
	            	ite_mot_rest->pseudo_motor->set_ctrl(tmp_mc);
	           		ite_mot_rest->pseudo_motor->ctrl_online();
				}
        	}        	

//
// Is it a pseudo counter ?
//
 
        	if (obj_ite->type == PSEUDO_COUNTER_CTRL)
        	{
	            list<PseudoCounterPool>::iterator ite_co_rest = 
	            										pseudo_co_list.begin();
	            advance(ite_co_rest,obj_ite->dist);
	            
	            ControllerPool &ct = 
	            	get_ctrl_from_pool_elem(*ite_co_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					PseudoCounterController *tmp_mc = 
						static_cast<PseudoCounterController *>(obj_ite->old_ctrl);
	            	ite_co_rest->pseudo_counter->set_ctrl(tmp_mc);
	           		ite_co_rest->pseudo_counter->ctrl_online();
				}
        	}          	
//
// Is it a Counter Timer ?
//

        	else if (obj_ite->type == COTI_CTRL)
        	{
	            list<CTExpChannelPool>::iterator ite_ct_rest = ct_list.begin();
	            advance(ite_ct_rest,obj_ite->dist);
	            
	            ControllerPool &ct = 
	            	get_ctrl_from_pool_elem(*ite_ct_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					CoTiController *tmp_ctc = 
						static_cast<CoTiController *>(obj_ite->old_ctrl);
	            	ite_ct_rest->ct_channel->set_ctrl(tmp_ctc);
	           		ite_ct_rest->ct_channel->ctrl_online();
				}
        	}
        	
//
// Is it a Zero D Exp Channel ?
//

        	else if (obj_ite->type == ZEROD_CTRL)
        	{
	            list<ZeroDExpChannelPool>::iterator ite_zerod_rest = zerod_list.begin();
	            advance(ite_zerod_rest,obj_ite->dist);
	            
	            ControllerPool &ct = 
	            	get_ctrl_from_pool_elem(*ite_zerod_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					ZeroDController *tmp_zdc = 
						static_cast<ZeroDController *>(obj_ite->old_ctrl);
	            	ite_zerod_rest->zerod_channel->set_ctrl(tmp_zdc);
	           		ite_zerod_rest->zerod_channel->ctrl_online();
				}
        	}
//
// Is it a Communication Channel ?
//

        	else if (obj_ite->type == COM_CTRL)
        	{
	            list<CommunicationChannelPool>::iterator ite_ch_rest = com_channel_list.begin();
	            advance(ite_ch_rest,obj_ite->dist);
	            
	            ControllerPool &ct = 
	            	get_ctrl_from_pool_elem(*ite_ch_rest);
				vct_ite ct_fica = ct.ite_ctrl_fica; 
				{
					AutoPoolLock lo((*ct_fica)->get_mon());
					ComController *tmp_ccc = 
						static_cast<ComController *>(obj_ite->old_ctrl);
						ite_ch_rest->com_channel->set_ctrl(tmp_ccc);
						ite_ch_rest->com_channel->ctrl_online();
				}
        	}      	
//
// Is it a Constraint?
//        	
        	else if (obj_ite->type == CONSTRAINT_CTRL)
        	{
        		// Nothing to restore
        	}
        }
    }
}

//------------------------------------------------------------------------------
// Pool::create_proxies
// 
void Pool::create_proxies()
{
	list<MotorPool>::iterator ite;
	for (ite = mot_list.begin();ite != mot_list.end();++ite)
	{
		if (ite->obj_proxy == NULL)
		{
			ite->obj_proxy = new Tango::DeviceProxy(ite->obj_tango_name);
			ite->obj_proxy->set_transparency_reconnection(true);
		}
	}

	list<MotorGroupPool>::iterator grp_ite;	
	for (grp_ite = mot_group_list.begin();grp_ite != mot_group_list.end();++grp_ite)
	{
		if (grp_ite->obj_proxy == NULL)
		{
			grp_ite->obj_proxy = new Tango::DeviceProxy(grp_ite->obj_tango_name);
			grp_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}
	
	list<PseudoMotorPool>::iterator pm_ite;	
	for (pm_ite = pseudo_mot_list.begin();pm_ite != pseudo_mot_list.end();++pm_ite)
	{
		if (pm_ite->obj_proxy == NULL)
		{
			pm_ite->obj_proxy = new Tango::DeviceProxy(pm_ite->obj_tango_name);
			pm_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}
	
	list<CTExpChannelPool>::iterator cte_ite;	
	for (cte_ite = ct_list.begin();cte_ite != ct_list.end();++cte_ite)
	{
		if (cte_ite->obj_proxy == NULL)
		{
			cte_ite->obj_proxy = new Tango::DeviceProxy(cte_ite->obj_tango_name);
			cte_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}

	list<ZeroDExpChannelPool>::iterator zerod_ite;	
	for (zerod_ite = zerod_list.begin();zerod_ite != zerod_list.end();++zerod_ite)
	{
		if (zerod_ite->obj_proxy == NULL)
		{
			zerod_ite->obj_proxy = new Tango::DeviceProxy(zerod_ite->obj_tango_name);
			zerod_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}

	list<MeasurementGroupPool>::iterator mntgrp_ite;	
	for (mntgrp_ite = measurement_group_list.begin();mntgrp_ite != measurement_group_list.end();++mntgrp_ite)
	{
		if (mntgrp_ite->obj_proxy == NULL)
		{
			mntgrp_ite->obj_proxy = new Tango::DeviceProxy(mntgrp_ite->obj_tango_name);
			mntgrp_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}
	
	list<CommunicationChannelPool>::iterator comch_ite;	
	for (comch_ite = com_channel_list.begin();comch_ite != com_channel_list.end();++comch_ite)
	{
		if (comch_ite->obj_proxy == NULL)
		{
			comch_ite->obj_proxy = new Tango::DeviceProxy(comch_ite->obj_tango_name);
			comch_ite->obj_proxy->set_transparency_reconnection(true);
		}
	}
}

//------------------------------------------------------------------------------
// Pool::motor_exist
// 
bool Pool::motor_exist(string &mot_name)
{
	list<MotorPool>::iterator ite;
	string mot_name_lower(mot_name);
	transform(mot_name_lower.begin(),mot_name_lower.end(),mot_name_lower.begin(),::tolower);
	
	for (ite = mot_list.begin();ite != mot_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == mot_name_lower) || (tg_name_lower == mot_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::group_exist
// 
bool Pool::group_exist(string &group_name)
{
	list<MotorGroupPool>::iterator ite;
	string group_name_lower(group_name);
	transform(group_name_lower.begin(),group_name_lower.end(),group_name_lower.begin(),::tolower);
	
	for (ite = mot_group_list.begin();ite != mot_group_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == group_name_lower) || (tg_name_lower == group_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::ct_channel_exist
// 
bool Pool::ct_channel_exist(string &ct_name)
{
	list<CTExpChannelPool>::iterator ite;
	string ct_name_lower(ct_name);
	transform(ct_name_lower.begin(),ct_name_lower.end(),ct_name_lower.begin(),::tolower);
	
	for (ite = ct_list.begin();ite != ct_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == ct_name_lower) || (tg_name_lower == ct_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::zerod_channel_exist
// 
bool Pool::zerod_channel_exist(string &zerod_name)
{
	list<ZeroDExpChannelPool>::iterator ite;
	string zerod_name_lower(zerod_name);
	transform(zerod_name_lower.begin(),zerod_name_lower.end(),zerod_name_lower.begin(),::tolower);
	
	for (ite = zerod_list.begin();ite != zerod_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == zerod_name_lower) || (tg_name_lower == zerod_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::oned_channel_exist
// 
bool Pool::oned_channel_exist(string &oned_name)
{
	// TODO
	/*	
	list<OneDChannelPool>::iterator ite;
	string oned_name_lower(oned_name);
	transform(oned_name_lower.begin(),oned_name_lower.end(),oned_name_lower.begin(),::tolower);
	
	for (ite = oned_channel_list.begin();ite != oned_channel_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == oned_name_lower) || (tg_name_lower == oned_name_lower))
		{
			return true;
		}
	}
	*/
	return false;
}

//------------------------------------------------------------------------------
// Pool::twod_channel_exist
// 
bool Pool::twod_channel_exist(string &twod_name)
{
	// TODO
	/*	
	list<TwoDChannelPool>::iterator ite;
	string twod_name_lower(twod_name);
	transform(twod_name_lower.begin(),twod_name_lower.end(),twod_name_lower.begin(),::tolower);
	
	for (ite = twod_channel_list.begin();ite != twod_channel_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == twod_name_lower) || (tg_name_lower == twod_name_lower))
		{
			return true;
		}
	}
	*/
	return false;
}

//------------------------------------------------------------------------------
// Pool::com_channel_exist
// 
bool Pool::com_channel_exist(string &comch_name)
{
	list<CommunicationChannelPool>::iterator ite;
	string comch_name_lower(comch_name);
	transform(comch_name_lower.begin(),comch_name_lower.end(),comch_name_lower.begin(),::tolower);
	
	for (ite = com_channel_list.begin();ite != com_channel_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == comch_name_lower) || (tg_name_lower == comch_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::measurement_group_exist
// 
bool Pool::measurement_group_exist(string &group_name)
{
	list<MeasurementGroupPool>::iterator ite;
	string group_name_lower(group_name);
	transform(group_name_lower.begin(),group_name_lower.end(),group_name_lower.begin(),::tolower);
	
	for (ite = measurement_group_list.begin();ite != measurement_group_list.end();++ite)
	{
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((ite->obj_alias_lower == group_name_lower) || (tg_name_lower == group_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::except_2_ctrl_status
// 
 void Pool::except_2_ctrl_status(Tango::DevFailed &e,string &ctrl_status)
{
	if (get_logger()->is_info_enabled())
		Tango::Except::print_exception(e);

	string except_desc_0(e.errors[0].desc);
	string except_desc_1;
	long except_size = e.errors.length();
	if (except_size >= 2)
		except_desc_1 = e.errors[1].desc;
					
	ctrl_status = ctrl_status + ".\nThe reported error description is";
	ctrl_status = ctrl_status + "\n\t" + except_desc_0;
	if (except_size >= 2)
		ctrl_status = ctrl_status + "\n\t" + except_desc_1; 	
}
 
//------------------------------------------------------------------------------
// Pool::set_moving_state
// 
void Pool::set_moving_state()
{
	Tango::AutoTangoMonitor atm(this);
	
	if (moving_state_requested == false)
	{
		moving_state_requested = true;
	
		list<MotorPool>::iterator mot_ite;
		for (mot_ite = mot_list.begin();mot_ite != mot_list.end();++mot_ite)
		{
			mot_ite->motor->pool_shutdown();
		}
	
		list<CTExpChannelPool>::iterator ct_ite;
		for (ct_ite = ct_list.begin();ct_ite != ct_list.end();++ct_ite)
		{
			ct_ite->ct_channel->pool_shutdown();
		}
		
//
// Send an event on the state attribute in case some client(s)
// are listenning on Pool device state
//

		set_state(Tango::MOVING);
		Tango::Attribute &state_att = dev_attr->get_attr_by_name("state");
		state_att.fire_change_event();
	}
}

//------------------------------------------------------------------------------
// Pool::pseudo_motor_exist
// 
bool Pool::pseudo_motor_exist(string &pseudo_name)
{
	list<PseudoMotorPool>::iterator ite;
	string pseudo_name_lower(pseudo_name);
	transform(pseudo_name_lower.begin(), pseudo_name_lower.end(), 
			  pseudo_name_lower.begin(), ::tolower);
	
	for (ite = pseudo_mot_list.begin();ite != pseudo_mot_list.end();++ite)
	{
		string alias_lower(ite->name);
		transform(alias_lower.begin(), alias_lower.end(), 
		          alias_lower.begin(), ::tolower);
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(), tg_name_lower.end(),
		          tg_name_lower.begin(), ::tolower);
		
		if ((alias_lower == pseudo_name_lower) || 
		    (tg_name_lower == pseudo_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::pseudo_counter_exist
// 
bool Pool::pseudo_counter_exist(string &pseudo_name)
{
	list<PseudoCounterPool>::iterator ite;
	string pseudo_name_lower(pseudo_name);
	transform(pseudo_name_lower.begin(),pseudo_name_lower.end(),pseudo_name_lower.begin(),::tolower);
	
	for (ite = pseudo_co_list.begin();ite != pseudo_co_list.end();++ite)
	{
		string alias_lower(ite->name);
		transform(alias_lower.begin(),alias_lower.end(),alias_lower.begin(),::tolower);
		string tg_name_lower(ite->obj_tango_name);
		transform(tg_name_lower.begin(),tg_name_lower.end(),tg_name_lower.begin(),::tolower);
		
		if ((alias_lower == pseudo_name_lower) || (tg_name_lower == pseudo_name_lower))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::check_property_data
// 
void Pool::check_property_data(string &inst_name,string &class_name,
							   vector<string> &info, int start)
{
	Tango::Util *util = Tango::Util::instance();
  	Tango::Database *db = util->get_database();

	Tango::DbData db_data_instance_level; 
	
//
// Try to get all the properties from the Database 
//
	vector<string>::iterator ite;
	for(ite = info.begin() + start; ite != info.end(); ite += 4)
	{
		string final_prop_name = inst_name + "/" + (*ite);
		db_data_instance_level.push_back(Tango::DbDatum(final_prop_name));
	}
	
	db->get_device_property(get_name(), db_data_instance_level);
	
	int i = 0;
	for(ite = info.begin() + start; ite != info.end(); ite += 4)
	{
		string &dft_value = *(ite+3);  
//
// If property is not defined in the database and the property has no default 
// value it is an error
//
		if(db_data_instance_level[i].is_empty() && dft_value == "")
		{
			TangoSys_OMemStream o;
			o << "Pool Property '" << (*ite) << "' must be defined in the "
				 " database" << ends;
			Tango::Except::throw_exception(
					(const char *)"Pool_MissingDatabaseProperty",o.str(),
					(const char *)"Pool::check_property_data");
			
		}
		i++;
	}
}

//------------------------------------------------------------------------------
// Pool::check_property_data
// 
void Pool::check_property_data(vector<PropertyData *> &prop_list)
{
	for (unsigned long loop = 0;loop < prop_list.size();loop++)
	{
		if ((prop_list[loop]->is_defined_in_db() == false) && 
			(prop_list[loop]->has_dft_value == false))
		{
			TangoSys_OMemStream o;
			o << "Property '" << prop_list[loop]->name;
			o << "' does not have any value defined in database or as default "
				 "value" << ends;
			Tango::Except::throw_exception(
					(const char *)"Pool_MissingPropertyValue",o.str(),
					(const char *)"Pool::check_property_data");
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_property_data
// 
PropertyData *
Pool::find_property_data(vector<PropertyData*> &prop_data,string &prop_name)
{
	if(prop_data.size() > 0)
	{
		string prop_name_lower(prop_name);
		transform(prop_name_lower.begin(), prop_name_lower.end(),
				  prop_name_lower.begin(), ::tolower);
		
		vector<PropertyData*>::iterator ite = prop_data.begin();
		for(; ite != prop_data.end(); ite++)
		{
			string curr_prop_name_lower((*ite)->name);
			transform(curr_prop_name_lower.begin(), curr_prop_name_lower.end(),
			          curr_prop_name_lower.begin(), ::tolower);
			if(prop_name_lower == curr_prop_name_lower)
				return (*ite);
		}
	}
	TangoSys_OMemStream o;
	o << "Property '" << prop_name << "' does not exist ";
	o << "on the given arrray" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_UnknownProperty",o.str(),
			(const char *)"Pool::find_property_data");
}

//------------------------------------------------------------------------------
// Pool::build_property_data
// 
void Pool::build_property_data(string &inst_name, string &class_name,
							   vector<pair<string,string> > &prop_pairs,
							   vector<PropertyData*> &prop_data)
{
	if(prop_data.size() == 0)
		return;
	
	Tango::DbData db_data_put, db_data_instance_level;  
	
	if(prop_pairs.size() > 0)
	{
//
// First put all given properties in the Database 
//
		vector<pair<string,string> >::iterator pair_ite;
		for(pair_ite = prop_pairs.begin(); pair_ite != prop_pairs.end(); pair_ite++)
		{
			string final_prop_name = inst_name + "/" + pair_ite->first;
			
			PropertyData *pdata = find_property_data(prop_data,pair_ite->first);
			
			Tango::DbDatum datum(final_prop_name);
			string &data = pair_ite->second;
			
//
// Special case for string arrays
//
			if(pdata->get_type() == Tango::DEVVAR_STRINGARRAY)
			{
				vector<string> data_arr;
				string::iterator data_ite = data.begin();
				string curr_elem;
				for(;data_ite != data.end(); ++data_ite)
				{
					if('\n' == (*data_ite))
					{
						if('\n' == (*(data_ite+1)) )
						{
							curr_elem += *data_ite;
							++data_ite;
						}
						else
						{
							data_arr.push_back(curr_elem);
							curr_elem.clear();
						}
					}
					else
					{
						curr_elem += *data_ite;
					}
				}
				data_arr.push_back(curr_elem);
				datum << data_arr;
			}
			else
				datum << data;		
			
			db_data_put.push_back(datum);
		}
		get_db_device()->put_property(db_data_put);
	}

//
// Try to get all the properties from the Database 
//

	vector<PropertyData*>::iterator ite;
	for(ite = prop_data.begin(); ite != prop_data.end(); ite++)
	{
		string final_prop_name = inst_name + "/" + (*ite)->name;
		db_data_instance_level.push_back(Tango::DbDatum(final_prop_name));
		(*ite)->clear_db_flag();
	}
	get_db_device()->get_property(db_data_instance_level);

	int i = 0;
	for(ite = prop_data.begin(); ite != prop_data.end(); ite++)
	{
		Tango::DbDatum *db_data = NULL;

		PropertyData &prop = *(*ite);
		
		// If data exists at the instance level use it
		if(!db_data_instance_level[i].is_empty())
		{
			db_data = &db_data_instance_level[i];
		}
		// Otherwise use the Default Value.

		if(db_data == NULL)
		{
//
// If there is no DefaultValue we make a default value 
//
			if(!prop.has_dft_value)
			{
				Tango::CmdArgType type = prop.get_type();
				if(type == Tango::DEV_BOOLEAN)
					prop.set_value(false);
				else if(type == Tango::DEV_LONG)
					prop.set_value(0L);
				else if(type == Tango::DEV_DOUBLE)
					prop.set_value(0.0);
				else if(type == Tango::DEV_STRING)
					prop.set_value("");
				// for array data types we keep them with zero elements
			}
		}
		else
		{			
//
// The property is defined in the Database: Update the PropertyData vector with 
// its value.
//
			string s; *db_data >> s; prop.set_value(s);
			
			Tango::CmdArgType type = prop.get_type();
			if(type == Tango::DEV_BOOLEAN)
			{ bool b; *db_data >> b; prop.set_value(b); }
			else if(type == Tango::DEV_LONG)
			{ long l; *db_data >> l; prop.set_value(l); }
			else if(type == Tango::DEV_DOUBLE)
			{ double d; *db_data >> d; prop.set_value(d); }
			else if(type == Tango::DEV_STRING)
			{ /* nothing to do on string. value_str member already contains the value.*/ }
			else if(type == Tango::DEVVAR_BOOLEANARRAY)
			{
				vector<bool> &prop_v = *prop.get_bool_v();
				vector<long> tmp_vect;
				*db_data >> tmp_vect;
				prop_v.clear();
				for(vector<long>::iterator it = tmp_vect.begin(); it != tmp_vect.end(); ++it )
					prop_v.push_back(*it);
				// force the string value to be updated
				prop.set_value(&prop_v); 
			}
			else if(type == Tango::DEVVAR_LONGARRAY)
			{
				vector<long> *v = (*ite)->get_long_v();
				*db_data >> *v;
				// force the string value to be updated
				prop.set_value(v);
			}
			else if(type == Tango::DEVVAR_DOUBLEARRAY)
			{
				vector<double> *v = (*ite)->get_double_v();
				*db_data >> *v;
				// force the string value to be updated
				prop.set_value(v);
			}
			else if(type == Tango::DEVVAR_STRINGARRAY)
			{
				vector<string> *v = (*ite)->get_string_v();
				*db_data >> *v;
				// force the string value to be updated
				prop.set_value(v);
			}
				
		 	prop.is_in_db();
		}
		i++;
	}	
}

//------------------------------------------------------------------------------
// Pool::properties_2_py_dict
// 
PyObject *Pool::properties_2_py_dict(vector<PropertyData*> &properties)
{
	PyObject *arg_prop = PyDict_New();
	
	vector<PropertyData*>::iterator ite;
	for(ite = properties.begin(); ite != properties.end(); ite++)
	{
		PyPropertyData *prop = (PyPropertyData*)(*ite);
		PyObject *value = prop->to_py(); 
		PyDict_SetItemString(arg_prop,prop->name.c_str(),value);
		Py_DECREF(value);
	}
	
	return arg_prop;
}

//------------------------------------------------------------------------------
// Pool::properties_2_cpp_vect
// 
vector<Controller::Properties> *Pool::properties_2_cpp_vect(
											  vector<PropertyData*> &properties)
{
	vector<Controller::Properties> *arg_prop = 
		new vector<Controller::Properties>;
	
	vector<PropertyData*>::iterator ite;
	for(ite = properties.begin(); ite != properties.end(); ite++)
	{
		CppPropertyData *prop = (CppPropertyData*)(*ite);
		Controller::Properties val = prop->to_cpp();
		arg_prop->push_back(val);
	}
	
	return arg_prop;
}

//------------------------------------------------------------------------------
// Pool::post_init_device
// 
void Pool::post_init_device()
{
//
// For each pseudo counter subscribe for events coming from each channel is
// uses
//
	list<PseudoCounterPool>::iterator pc_ite = pseudo_co_list.begin();
	for(; pc_ite != pseudo_co_list.end(); pc_ite++)
	{
		PseudoCounterPool &pcp = *pc_ite;
		
		vector<PoolElement*>::iterator ite = pcp.ch_elts.begin();
		for(; ite != pcp.ch_elts.end(); ite++)
		{
			(*ite)->add_pool_elem_listener(&pcp);
		}
	}	

//
// For each pseudo motor fill the missing motor group information
// and subscribe to events coming from the motor group.
// Also fill information about siblings
// 
	map<long,vector<PseudoMotor_ns::PseudoMotor*> > ctrl_pseudos;
	
	// First initialize the necessary temporary map
	list<ControllerPool>::iterator ctrl_ite = ctrl_list.begin();
	for(; ctrl_ite  != ctrl_list.end(); ctrl_ite++)
	{
		if(ctrl_ite->ctrl_fica_built == true)
		{
			if(ctrl_ite->get_ctrl_obj_type() == PSEUDO_MOTOR_CTRL)
			{
				vector<PseudoMotor_ns::PseudoMotor*>::size_type n = ctrl_ite->MaxDevice;
				vector<PseudoMotor_ns::PseudoMotor*> v(n,NULL);
				ctrl_pseudos.insert(make_pair(ctrl_ite->id,v));
			}
		}
	}
	
	list<PseudoMotorPool>::iterator pm_ite = pseudo_mot_list.begin();
	for(; pm_ite != pseudo_mot_list.end(); pm_ite++)
	{
		MotorGroupPool &mgp = get_motor_group_from_name(pm_ite->mot_grp_name);
		PseudoMotorPool &pmp = *pm_ite;
		
		pmp.mot_grp = &mgp;
		mgp.add_pool_elem_listener(&pmp);
		ctrl_pseudos[pmp.ctrl_id][pmp.pseudo_motor->get_controller_idx() - 1] = pmp.pseudo_motor;
	}
	
	//Set the siblings for each pseudo motor
	for(pm_ite = pseudo_mot_list.begin(); pm_ite != pseudo_mot_list.end(); pm_ite++)
		pm_ite->pseudo_motor->set_siblings(ctrl_pseudos[pm_ite->ctrl_id]);
	
//
// For each motor group subscribe to the internal events coming from
// each motor of the motor group
//
	list<MotorGroupPool>::iterator mg_ite = mot_group_list.begin();
	for(; mg_ite != mot_group_list.end(); mg_ite++)
	{
		MotorGroupPool &mgp = *mg_ite;
		
		//TODO decide if ghost has to be informed of internal events 
		if(mgp.id == 0)
			continue;
		
		vector<PoolElement*>::iterator ite = mgp.group_elts.begin();
		for(; ite != mgp.group_elts.end(); ite++)
		{
			(*ite)->add_pool_elem_listener(&mgp);
		}
	}

//
// For each measurement group subscribe to the internal events coming from
// each channel of the measurement group
//
	list<MeasurementGroupPool>::iterator mntgrp_ite = 
		measurement_group_list.begin();
	for(; mntgrp_ite != measurement_group_list.end(); mntgrp_ite++)
	{
		MeasurementGroupPool &mgp = *mntgrp_ite;
		
		//TODO decide if ghost has to be informed of internal events 
		if(mgp.id == 0)
			continue;

		vector<PoolElement*>::iterator ite = mgp.group_elts.begin();
		for(; ite != mgp.group_elts.end(); ite++)
		{
			(*ite)->add_pool_elem_listener(&mgp);
		}
	}
}

//------------------------------------------------------------------------------
// PoolElementEvent::PoolElementEvent
//
PoolElementEvent::PoolElementEvent(PoolEventType evt_type, PoolElement* src_elem)
:type(evt_type),priority(false),src(src_elem)
{}

//------------------------------------------------------------------------------
// PoolElementEvent::PoolElementEvent
//
PoolElementEvent::PoolElementEvent(const PoolElementEvent &rhs)
:type(rhs.type),priority(rhs.priority),src(rhs.src),dim(rhs.dim)
{ 
  	if(type == StateChange)
  	{
  		old_state = rhs.old_state;
  		new_state = rhs.new_state;
  	}
  	else if(type == PositionChange)
  	{
  		old_position = rhs.old_position;
  		new_position = rhs.new_position;
  	}
  	else if(type == PositionArrayChange)
  	{
  		old_position_array = rhs.old_position_array;
  		new_position_array = rhs.new_position_array;
  	}
  	else if(type == CTValueChange ||
  			type == ZeroDValueChange ||
  			type == PseudoCoValueChange)
  	{
  		old_value = rhs.old_value;
  		new_value = rhs.new_value;
  	}
  	
  	else if(type == MotionEnded)
  	{
  			
  	}
}

//------------------------------------------------------------------------------
// PoolElement::add_pool_elem_listener
// 
void PoolElement::add_pool_elem_listener(IPoolElementListener *listener)
{
	if(listener != NULL)
	{
		if(find(pool_elem_listeners.begin(),pool_elem_listeners.end(),listener)
				== pool_elem_listeners.end())
		{
			pool_elem_listeners.push_back(listener);
		}
	}
}

//------------------------------------------------------------------------------
// PoolElement::remove_pool_elem_listener
// 
void PoolElement::remove_pool_elem_listener(IPoolElementListener *listener)
{
	if(listener != NULL)
	{
		list<IPoolElementListener*>::iterator it = 
			find(pool_elem_listeners.begin(),pool_elem_listeners.end(),listener);
		if(it != pool_elem_listeners.end())
			pool_elem_listeners.erase(it);
	}
}

//------------------------------------------------------------------------------
// Pool::get_pool_element_from_name
// 
PoolElement &Pool::get_pool_element_from_name(string &name)
{
	try
	{
		return get_ctrl_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_phy_pool_element_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_group_pool_element_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}	
	
	try
	{
		return get_pseudo_pool_element_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	TangoSys_OMemStream o;
	o << "No element with name " << name << " found in the pool" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_ElementNotFound",o.str(),
			(const char *)"Pool::get_pool_element_from_name");	
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_pool_element_from_name
// 
PoolElement &Pool::get_pseudo_pool_element_from_name(string &name)
{
	try
	{
		return get_pseudo_motor_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_pseudo_counter_from_name(name);
	}
	catch (Tango::DevFailed &e)	{}
	
	TangoSys_OMemStream o;
	o << "No element with name " << name << " found in the pool" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_ElementNotFound",o.str(),
			(const char *)"Pool::get_pseudo_pool_element_from_name");
}

//------------------------------------------------------------------------------
// Pool::get_phy_pool_element_from_name
// 
PoolElement &Pool::get_phy_pool_element_from_name(string &name)
{
	try
	{
		return get_motor_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_ct_from_name(name);
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_zerod_from_name(name);
	}
	catch (Tango::DevFailed &e)	{}
	
	try
	{
		return get_com_channel_from_name(name);
	}
	catch (Tango::DevFailed &e)	{}
	
	TangoSys_OMemStream o;
	o << "No element with name " << name << " found in the pool" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_ElementNotFound",o.str(),
			(const char *)"Pool::get_phy_pool_element_from_name");
}

//------------------------------------------------------------------------------
// Pool::get_phy_pool_element_from_name
// 
PoolElement &Pool::get_group_pool_element_from_name(string &name)
{
	try
	{
		return get_motor_group_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}

	try
	{
		return get_measurement_group_from_name(name);	
	}
	catch (Tango::DevFailed &e)	{}
	
	TangoSys_OMemStream o;
	o << "No element with name " << name << " found in the pool" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_ElementNotFound",o.str(),
			(const char *)"Pool::get_group_pool_element_from_name");
}



//------------------------------------------------------------------------------
// PoolElement::fire_pool_elem_change
// 
void PoolElement::fire_pool_elem_change(PoolElementEvent *evt, 
										IPoolElementListener *exclude /* = NULL*/)
{
	list<IPoolElementListener*>::iterator it = pool_elem_listeners.begin();
	
	PoolElemEventList evt_lst;
	evt_lst.push_back(evt);
	
	for(; it != pool_elem_listeners.end(); it++)
	{
		IPoolElementListener *listener = *it;
		
		if(listener != exclude)
			listener->pool_elem_changed(evt_lst);
	}
}

//------------------------------------------------------------------------------
// PoolElement::propagate_event
// 
void PoolElement::propagate_event(PoolElemEventList &evt_lst)
{
	list<IPoolElementListener*>::iterator it = pool_elem_listeners.begin();
	
	for(; it != pool_elem_listeners.end(); it++)
	{
		// Avoid generating an event on the element that originated the events
		if(evt_lst.front()->src != (*it))
			(*it)->pool_elem_changed(evt_lst);
	}
}

//------------------------------------------------------------------------------
// PoolElement::pool_elem_changed
// 
void PoolElement::pool_elem_changed(PoolElemEventList &evt_lst)
{
	propagate_event(evt_lst);
}

//------------------------------------------------------------------------------
// ControllerPool::pool_elem_changed
// 
void ControllerPool::pool_elem_changed(PoolElemEventList &evt_lst)
{
	// nothing to be done here
	// so far a controller does not listen for events on other PoolElements 
	// because it is the lowest level in the hierarchy

//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
}

//------------------------------------------------------------------------------
// MotorPool::pool_elem_changed
// 
void MotorPool::pool_elem_changed(PoolElemEventList &evt_lst)
{
	// nothing to be done here
	// so far a motor does not listen for events on other PoolElements because
	// it is the lowest level in the hierarchy
	
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);	
}

//------------------------------------------------------------------------------
// MotorPool::MotorPool
// 
Tango::Device_3Impl *MotorPool::get_device()
{
	return motor;
}

//------------------------------------------------------------------------------
// MotorGroupPool::pool_elem_changed
// 
void MotorGroupPool::pool_elem_changed(PoolElemEventList &evt_lst)
{

	PoolElementEvent *last_evt = evt_lst.back();
	PoolElementEvent new_evt(last_evt->type,this);

//
// Forward the event to the MotorGroup tango object. He will know want to do
// with the event
//	
	group->pool_elem_changed(evt_lst,new_evt);
	
	evt_lst.push_back(&new_evt);
	
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
	
//
// Remove the evt object created by this element from the event list
// 
	evt_lst.pop_back();
}

//------------------------------------------------------------------------------
// MotorGroupPool::get_device
// 
Tango::Device_3Impl *MotorGroupPool::get_device()
{
	return group;
}

//------------------------------------------------------------------------------
// MotorGroupPool::is_user_member
// 
bool MotorGroupPool::is_user_member(string &name)
{
	bool returned = true;
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	vector<PoolElement*>::iterator ite;
	for (ite = group_elts.begin();ite != group_elts.end();++ite)
	{
		string elt_name= (*ite)->name;
		transform(elt_name.begin(),elt_name.end(),elt_name.begin(),::tolower);
		if (elt_name == name_lower)
			break;
	}

	if (ite == group_elts.end())
	{
		returned = false;	
	}	
	return returned;
}

//------------------------------------------------------------------------------
// MotorGroupPool::is_member
// 
bool MotorGroupPool::is_member(string &name)
{
	if(is_user_member(name) == true)
		return true;
	
	vector<PseudoMotorPool*>::iterator pm_ite = pm_elts.begin();
	for(; pm_ite != pm_elts.end(); pm_ite++)
		if((*pm_ite)->is_member(name) == true)
			return true;

	vector<MotorGroupPool*>::iterator mg_ite = mg_elts.begin();
	for(; mg_ite != mg_elts.end(); mg_ite++)
		if((*mg_ite)->is_member(name) == true)
			return true;
		
	return false;
}

//------------------------------------------------------------------------------
// MeasurementGroupPool::pool_elem_changed
// 
void MeasurementGroupPool::pool_elem_changed(
											   PoolElemEventList &evt_lst)
{
	if(pool_elem_listeners.size() == 0)
		return;

	PoolElementEvent *last_evt = evt_lst.back();
	PoolElementEvent new_evt(last_evt->type,this);
	
//
// Forward the event to the MeasurementGroup tango object. He will know want to 
// do with the event
//	
	group->pool_elem_changed(evt_lst,new_evt);
	
	evt_lst.push_back(&new_evt);
	
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
		
//
// Remove the evt object created by this element from the event list
// 
	evt_lst.pop_back();
}

//------------------------------------------------------------------------------
// MeasurementGroupPool::get_device
// 
Tango::Device_3Impl *MeasurementGroupPool::get_device()
{
	return group;
}

//------------------------------------------------------------------------------
// MeasurementGroupPool::is_member
// 
bool MeasurementGroupPool::is_member(string &name)
{
	bool returned = true;
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	vector<PoolElement*>::iterator ite;
	for (ite = group_elts.begin();ite != group_elts.end();++ite)
	{
		string elt_name= (*ite)->name;
		transform(elt_name.begin(),elt_name.end(),elt_name.begin(),::tolower);
		if (elt_name == name_lower)
			break;
	}

	if (ite == group_elts.end())
	{
		returned = false;	
	}	
	return returned;
}

//------------------------------------------------------------------------------
// PseudoMotorPool::pool_elem_changed
// 
void PseudoMotorPool::pool_elem_changed(PoolElemEventList &evt_lst)
{

	PoolElementEvent *last_evt = evt_lst.back();
	PoolElementEvent new_evt(last_evt->type,this);

//
// Forward the event to the PseudoMotor tango object. He will know want to do
// with the event
//	
	pseudo_motor->pool_elem_changed(evt_lst,new_evt);

	evt_lst.push_back(&new_evt);
	
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
	
//
// Remove the evt object created by this element from the event list
// 
	evt_lst.pop_back();	
}

//------------------------------------------------------------------------------
// PseudoMotorPool::get_device
// 
Tango::Device_3Impl *PseudoMotorPool::get_device()
{
	return pseudo_motor;
}

//------------------------------------------------------------------------------
// PseudoMotorPool::is_member
// 
bool PseudoMotorPool::is_member(string &name)
{
	bool returned = true;
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	vector<PoolElement*>::iterator ite;
	for (ite = mot_elts.begin();ite != mot_elts.end();++ite)
	{
		string elt_name= (*ite)->name;
		transform(elt_name.begin(),elt_name.end(),elt_name.begin(),::tolower);
		if (elt_name == name_lower)
			break;
	}

	if (ite == mot_elts.end())
	{
		returned = false;	
	}	
	return returned;
}

//------------------------------------------------------------------------------
// CTExpChannelPool::pool_elem_changed
// 
void CTExpChannelPool::pool_elem_changed(PoolElemEventList &evt_lst)
{
	// nothing to be done here
	// so far a controller does not listen for events on other PoolElements
	// because it is the lowest level in the hierarchy

//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
}

//------------------------------------------------------------------------------
// CTExpChannelPool::get_device
//
Tango::Device_3Impl *CTExpChannelPool::get_device()
{
	return ct_channel;
}

//------------------------------------------------------------------------------
// CTExpChannelPool::pool_elem_changed
//
void ZeroDExpChannelPool::pool_elem_changed(PoolElemEventList &evt_lst)
{
	// nothing to be done here
	// so far a controller does not listen for events on other PoolElements 
	// because it is the lowest level in the hierarchy

//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
}

//------------------------------------------------------------------------------
// ZeroDExpChannelPool::get_device
//
Tango::Device_3Impl *ZeroDExpChannelPool::get_device()
{
	return zerod_channel;
}

//------------------------------------------------------------------------------
// PseudoCounterPool::is_member
//
bool PseudoCounterPool::is_member(string &name)
{
	bool returned = true;
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	vector<PoolElement*>::iterator ite;
	for (ite = ch_elts.begin();ite != ch_elts.end();++ite)
	{
		string elt_name= (*ite)->name;
		transform(elt_name.begin(),elt_name.end(),elt_name.begin(),::tolower);
		if (elt_name == name_lower)
			break;
	}

	if (ite == ch_elts.end())
	{
		returned = false;	
	}	
	return returned;
}

//------------------------------------------------------------------------------
// PseudoCounterPool::pool_elem_changed
//
void PseudoCounterPool::pool_elem_changed(PoolElemEventList &evt_lst)
{

	PoolElementEvent *last_evt = evt_lst.back();
	PoolElementEvent new_evt(last_evt->type,this);

//
// Forward the event to the PseudoCounter tango object. He will know want to do
// with the event
//	
	pseudo_counter->pool_elem_changed(evt_lst,new_evt);

	evt_lst.push_back(&new_evt);
	
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
	
//
// Remove the evt object created by this element from the event list
// 
	evt_lst.pop_back();	

//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
}

//------------------------------------------------------------------------------
// PseudoCounterPool::get_device
//
Tango::Device_3Impl *PseudoCounterPool::get_device()
{
	return pseudo_counter;
}

//------------------------------------------------------------------------------
// CommunicationChannelPool::pool_elem_changed
//
void CommunicationChannelPool::pool_elem_changed(
											   PoolElemEventList &evt_lst)
{
//	
// Call super method to propagate received event to all listeners
// 
	PoolElement::pool_elem_changed(evt_lst);
}

//------------------------------------------------------------------------------
// CommunicationChannelPool::get_device
//
Tango::Device_3Impl *CommunicationChannelPool::get_device()
{
	return com_channel;
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_motor
//
void Pool::find_and_invalidate_motor(vector<CtrlBkp> &ctrls,
									 vector<ObjBkp> &objs)
{		
	list<MotorPool>::iterator ite_mot;
	list<MotorPool>::iterator list_beg = mot_list.begin();
		
	for (ite_mot = mot_list.begin();ite_mot != mot_list.end();++ite_mot)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_mot->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get motor device monitor to be sure that nobody else will do something
// on the motor. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_mot->motor);
					Tango::DevState sta;
					try
					{
						sta = ite_mot->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}

//
// If one motor is moving, refuse to reload controller code
// in this code, reset all motor previously found to valid controller code
//
					
					if (sta == Tango::MOVING)
					{
						restore_ctrl_obj((vector<Pool::CtrlBkp> *)NULL,&objs);
					
						TangoSys_OMemStream o;
						o << "Motor " << ite_mot->name << " ("; 
						o << ite_mot->obj_tango_name << ") is moving. ";
						o << "It is not allowed to reload controller code while"
							 " a motor is moving" << ends;
			
						Tango::Except::throw_exception(
							(const char *)"Pool_CantUnloadController",o.str(),
							(const char *)"Pool::reload_controller_code");						
					}

//
// Invalidate motor
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_mot->motor->ctrl_offline();
						ite_mot->motor->set_ctrl(NULL);
					}
				}
					
//
// Memorize some motor info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_mot->motor->get_ctrl();
				obj_bkp.idx = ite_mot->id;
				obj_bkp.dist = distance(list_beg,ite_mot);
				obj_bkp.type = MOTOR_CTRL;
				obj_bkp.listeners = ite_mot->pool_elem_listeners;
				objs.push_back(obj_bkp);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_pseudo_motor
//
void Pool::find_and_invalidate_pseudo_motor(vector<CtrlBkp> &ctrls,
									 		vector<ObjBkp> &objs)
{		
	list<PseudoMotorPool>::iterator ite_mot;
	list<PseudoMotorPool>::iterator list_beg = pseudo_mot_list.begin();
		
	for(ite_mot = pseudo_mot_list.begin();ite_mot != pseudo_mot_list.end();++ite_mot)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_mot->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get pseudo motor device monitor to be sure that nobody else will do something
// on the pseudo motor. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_mot->pseudo_motor);
					Tango::DevState sta;
					try
					{
						sta = ite_mot->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}

//
// If one pseudo motor is moving, refuse to reload controller code. In this 
// code, reset all pseudo motor previously found to valid controller code
//
					
					if (sta == Tango::MOVING)
					{
						restore_ctrl_obj((vector<Pool::CtrlBkp> *)NULL,&objs);
					
						TangoSys_OMemStream o;
						o << "Pseudo Motor " << ite_mot->name << " ("; 
						o << ite_mot->obj_tango_name << ") is moving. ";
						o << "It is not allowed to reload controller code while"
							 " a pseudo motor is moving" << ends;
			
						Tango::Except::throw_exception(
							(const char *)"Pool_CantUnloadController",o.str(),
							(const char *)"Pool::reload_controller_code");						
					}

//
// Invalidate pseudo motor
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_mot->pseudo_motor->ctrl_offline();
						ite_mot->pseudo_motor->set_ctrl(NULL);
					}
				}
					
//
// Memorize some pseudo motor info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_mot->pseudo_motor->get_ctrl();
				obj_bkp.idx = ite_mot->id;
				obj_bkp.dist = distance(list_beg,ite_mot);
				obj_bkp.type = PSEUDO_MOTOR_CTRL;
				obj_bkp.listeners = ite_mot->pool_elem_listeners;
				objs.push_back(obj_bkp);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_pseudo_counter
//
void Pool::find_and_invalidate_pseudo_counter(vector<CtrlBkp> &ctrls,
									 		vector<ObjBkp> &objs)
{		
	list<PseudoCounterPool>::iterator ite_co;
	list<PseudoCounterPool>::iterator list_beg = pseudo_co_list.begin();
		
	for(ite_co = pseudo_co_list.begin();ite_co != pseudo_co_list.end();++ite_co)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_co->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get pseudo motor device monitor to be sure that nobody else will do something
// on the pseudo motor. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_co->pseudo_counter);
					Tango::DevState sta;
					try
					{
						sta = ite_co->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}

//
// If one pseudo counter is counting, refuse to reload controller code. In this 
// code, reset all pseudo counter previously found to valid controller code
//
					
					if (sta == Tango::MOVING)
					{
						restore_ctrl_obj((vector<Pool::CtrlBkp> *)NULL,&objs);
					
						TangoSys_OMemStream o;
						o << "Pseudo Counter " << ite_co->name << " ("; 
						o << ite_co->obj_tango_name << ") is moving. ";
						o << "It is not allowed to reload controller code while"
							 " a pseudo counter is counting" << ends;
			
						Tango::Except::throw_exception(
							(const char *)"Pool_CantUnloadController",o.str(),
							(const char *)"Pool::reload_controller_code");						
					}

//
// Invalidate pseudo counter
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_co->pseudo_counter->ctrl_offline();
						ite_co->pseudo_counter->set_ctrl(NULL);
					}
				}
					
//
// Memorize some pseudo counter info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_co->pseudo_counter->get_ctrl();
				obj_bkp.idx = ite_co->id;
				obj_bkp.dist = distance(list_beg,ite_co);
				obj_bkp.type = PSEUDO_COUNTER_CTRL;
				obj_bkp.listeners = ite_co->pool_elem_listeners;
				objs.push_back(obj_bkp);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_ct
//
void Pool::find_and_invalidate_ct(vector<CtrlBkp> &ctrls,vector<ObjBkp> &objs)
{		
	list<CTExpChannelPool>::iterator ite_ct;
	list<CTExpChannelPool>::iterator list_beg = ct_list.begin();
		
	for (ite_ct = ct_list.begin();ite_ct != ct_list.end();++ite_ct)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_ct->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get CT device monitor to be sure that nobody else will do something
// on the CT. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_ct->ct_channel);
					Tango::DevState sta;
					try
					{
						sta = ite_ct->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}

//
// If one CT is moving, refuse to reload controller code
// in this code, reset all CT previously found to valid controller code
//
					
					if (sta == Tango::MOVING)
					{
						restore_ctrl_obj((vector<Pool::CtrlBkp> *)NULL,&objs);
					
						TangoSys_OMemStream o;
						o << "CounterTimer " << ite_ct->name << " (";
						o << ite_ct->obj_tango_name << ") is counting. ";
						o << "It is not allowed to reload controller code while"
							 " a CounterTimer is counting" << ends;
			
						Tango::Except::throw_exception(
							(const char *)"Pool_CantUnloadController",o.str(),
							(const char *)"Pool::reload_controller_code");						
					}

//
// Invalidate C/T
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_ct->ct_channel->ctrl_offline();
						ite_ct->ct_channel->set_ctrl(NULL);
					}
				}
					
//
// Memorize some C/T info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_ct->ct_channel->get_ctrl();
				obj_bkp.idx = ite_ct->id;
				obj_bkp.dist = distance(list_beg,ite_ct);
				obj_bkp.type = COTI_CTRL;
				obj_bkp.listeners = ite_ct->pool_elem_listeners;
				objs.push_back(obj_bkp);
				
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_zerod
//
void Pool::find_and_invalidate_zerod(vector<CtrlBkp> &ctrls,
									 vector<ObjBkp> &objs)
{		
	list<ZeroDExpChannelPool>::iterator ite_ct;
	list<ZeroDExpChannelPool>::iterator list_beg = zerod_list.begin();
		
	for (ite_ct = zerod_list.begin();ite_ct != zerod_list.end();++ite_ct)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_ct->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get ZeroD device monitor to be sure that nobody else will do something
// on the ZeroD. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_ct->zerod_channel);
					Tango::DevState sta;
					try
					{
						sta = ite_ct->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}
					
//
// If one channel is acquiring data, refuse to reload controller code
// in this code, reset all channel previously found to valid controller code
//
					
					if (sta == Tango::MOVING)
					{
						restore_ctrl_obj((vector<Pool::CtrlBkp> *)NULL,&objs);
					
						TangoSys_OMemStream o;
						o << "Experiment Channel " << ite_ct->name << " (";
						o << ite_ct->obj_tango_name << ") is acquiring data. ";
						o << "It is not allowed to reload controller code while "
							 "a channel is acquiring" << ends;
			
						Tango::Except::throw_exception(
							(const char *)"Pool_CantUnloadController",o.str(),
							(const char *)"Pool::reload_controller_code");						
					}

//
// Invalidate channel
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_ct->zerod_channel->ctrl_offline();
						ite_ct->zerod_channel->set_ctrl(NULL);
					}
				}
					
//
// Memorize some Channel info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_ct->zerod_channel->get_ctrl();
				obj_bkp.idx = ite_ct->id;
				obj_bkp.dist = distance(list_beg,ite_ct);
				obj_bkp.type = ZEROD_CTRL;
				obj_bkp.listeners = ite_ct->pool_elem_listeners;
				objs.push_back(obj_bkp);
				
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::find_and_invalidate_comch
//
void Pool::find_and_invalidate_comch(vector<CtrlBkp> &ctrls,vector<ObjBkp> &objs)
{		
	list<CommunicationChannelPool>::iterator ite_com;
	list<CommunicationChannelPool>::iterator list_beg = com_channel_list.begin();
		
	for (ite_com = com_channel_list.begin();ite_com != com_channel_list.end();++ite_com)
	{
		vector<Pool::CtrlBkp>::iterator ite;
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			if (ite_com->ctrl_id == ite->idx)
			{

				list<ControllerPool>::iterator ct = ctrl_list.begin();
				advance(ct,ite->dist);
									
//
// Get com device monitor to be sure that nobody else will do something
// on the com. As the monitor is re-entrant, it is still possible for
// us to call its state command
//

				if (ct->ctrl_fica_built == true)
				{
					Tango::AutoTangoMonitor atm(ite_com->com_channel);
					Tango::DevState sta;
					try
					{
						sta = ite_com->obj_proxy->state();
					}
					catch(Tango::DevFailed &e)
					{
						sta = Tango::UNKNOWN;
					}

//
// Invalidate com com channel
//
					
					vct_ite ct_fica = ct->ite_ctrl_fica; 
					{
						AutoPoolLock lo((*ct_fica)->get_mon());
						ite_com->com_channel->ctrl_offline();
						ite_com->com_channel->set_ctrl(NULL);
					}
				}
					
//
// Memorize some Com channel info
//

				Pool::ObjBkp obj_bkp;
				obj_bkp.old_ctrl = ite_com->com_channel->get_ctrl();
				obj_bkp.idx = ite_com->id;
				obj_bkp.dist = distance(list_beg,ite_com);
				obj_bkp.type = COM_CTRL;
				obj_bkp.listeners = ite_com->pool_elem_listeners;
				objs.push_back(obj_bkp);
				
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::restore_motor
//
void Pool::restore_motor(vector<ObjBkp> &mots,
						 list<ControllerPool>::iterator &ctl,
						 vector<Pool::CtrlBkp>::iterator &ite)
{
	list<IPoolElementListener*> listeners;
	vector<Pool::ObjBkp>::iterator mot_ite;
	for (mot_ite = mots.begin();mot_ite != mots.end();++mot_ite)
	{
		list<MotorPool>::iterator mtl = mot_list.begin();
 		advance(mtl,mot_ite->dist);

		if (ctl->id == mtl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				mtl->motor->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(mtl->motor);
					long m_id = mtl->id;
					mtl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						MotorPool &mot_in_pool = get_motor_from_id(m_id);
						mot_in_pool.motor->create_dyn_attr();
						mot_in_pool.motor->remove_unwanted_dyn_attr_from_device();
					}
				}
			}
			
			MotorPool &mot_in_pool = get_motor_from_id(mot_ite->idx);
//
// Restore the listeners
//
			mot_in_pool.pool_elem_listeners = mot_ite->listeners;
			
//
// Inform the listeners that the motor changed structure
//			
			if(mot_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&mot_in_pool);
				mot_in_pool.fire_pool_elem_change(&evt);
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::restore_pseudo_motor
//
void Pool::restore_pseudo_motor(vector<ObjBkp> &mots,
								list<ControllerPool>::iterator &ctl,
								vector<Pool::CtrlBkp>::iterator &ite)
{
	list<IPoolElementListener*> listeners;
	vector<Pool::ObjBkp>::iterator mot_ite;
	for (mot_ite = mots.begin();mot_ite != mots.end();++mot_ite)
	{
		list<PseudoMotorPool>::iterator mtl = pseudo_mot_list.begin();
 		advance(mtl,mot_ite->dist);

		if (ctl->id == mtl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				mtl->pseudo_motor->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(mtl->pseudo_motor);
					long m_id = mtl->id;
					mtl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						PseudoMotorPool &mot_in_pool = get_pseudo_motor_from_id(m_id);
						mot_in_pool.pseudo_motor->create_dyn_attr();
						mot_in_pool.pseudo_motor->remove_unwanted_dyn_attr_from_device();
					}
				}
			}
			
			PseudoMotorPool &mot_in_pool = get_pseudo_motor_from_id(mot_ite->idx);
//
// Restore the listeners
//
			mot_in_pool.pool_elem_listeners = mot_ite->listeners;
			
//
// Inform the listeners that the motor changed structure
//			
			if(mot_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&mot_in_pool);
				mot_in_pool.fire_pool_elem_change(&evt);
			}
		}
	}
}	

//------------------------------------------------------------------------------
// Pool::restore_pseudo_counter
//
void Pool::restore_pseudo_counter(vector<ObjBkp> &pcs,
								list<ControllerPool>::iterator &ctl,
								vector<Pool::CtrlBkp>::iterator &ite)
{
	list<IPoolElementListener*> listeners;
	vector<Pool::ObjBkp>::iterator co_ite;
	for (co_ite = pcs.begin();co_ite != pcs.end();++co_ite)
	{
		list<PseudoCounterPool>::iterator pcl = pseudo_co_list.begin();
 		advance(pcl,co_ite->dist);

		if (ctl->id == pcl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				pcl->pseudo_counter->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(pcl->pseudo_counter);
					long pc_id = pcl->id;
					pcl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						PseudoCounterPool &pc_in_pool = get_pseudo_counter_from_id(pc_id);
						pc_in_pool.pseudo_counter->create_dyn_attr();
						pc_in_pool.pseudo_counter->remove_unwanted_dyn_attr_from_device();
					}
				}
			}
			
			PseudoCounterPool &pc_in_pool = get_pseudo_counter_from_id(co_ite->idx);
//
// Restore the listeners
//
			pc_in_pool.pool_elem_listeners = co_ite->listeners;
			
//
// Inform the listeners that the pseudo counter changed structure
//			
			if(pc_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&pc_in_pool);
				pc_in_pool.fire_pool_elem_change(&evt);
			}
		}
	}
}	

//------------------------------------------------------------------------------
// Pool::restore_ct
//
void Pool::restore_ct(vector<ObjBkp> &cts,list<ControllerPool>::iterator &ctl,
					  vector<Pool::CtrlBkp>::iterator &ite)
{
	list<IPoolElementListener*> listeners;
	vector<Pool::ObjBkp>::iterator ct_ite;
	for (ct_ite = cts.begin();ct_ite != cts.end();++ct_ite)
	{
		list<CTExpChannelPool>::iterator mtl = ct_list.begin();
 		advance(mtl,ct_ite->dist);

		if (ctl->id == mtl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				mtl->ct_channel->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(mtl->ct_channel);
					long m_id = mtl->id;
					mtl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						CTExpChannelPool &ct_in_pool = 
							static_cast<CTExpChannelPool &>(
									get_exp_channel_from_id(m_id));
							
						ct_in_pool.ct_channel->create_dyn_attr();
						ct_in_pool.ct_channel->remove_unwanted_dyn_attr_from_device();
					}
				}								
			}
			PoolElement &ct_in_pool = get_exp_channel_from_id(ct_ite->idx);
//
// Restore the listeners
//
			ct_in_pool.pool_elem_listeners = ct_ite->listeners;
			
//
// Inform the listeners that the channel changed structure
//			
			if(ct_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&ct_in_pool);
				ct_in_pool.fire_pool_elem_change(&evt);
			}
		}
	}
}	

//------------------------------------------------------------------------------
// Pool::restore_zerod
//
void Pool::restore_zerod(vector<ObjBkp> &cts,
						 list<ControllerPool>::iterator &ctl,
						 vector<Pool::CtrlBkp>::iterator &ite)
{
	vector<Pool::ObjBkp>::iterator ct_ite;
	for (ct_ite = cts.begin();ct_ite != cts.end();++ct_ite)
	{
		list<ZeroDExpChannelPool>::iterator mtl = zerod_list.begin();
 		advance(mtl,ct_ite->dist);

		if (ctl->id == mtl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				mtl->zerod_channel->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(mtl->zerod_channel);
					long m_id = mtl->id;
					mtl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						ZeroDExpChannelPool &zerod_in_pool = 
							static_cast<ZeroDExpChannelPool &>(
									get_exp_channel_from_id(m_id));
							
						zerod_in_pool.zerod_channel->create_dyn_attr();
						zerod_in_pool.zerod_channel->remove_unwanted_dyn_attr_from_device();
					}
				}								
			}
			PoolElement &zerod_in_pool = get_exp_channel_from_id(ct_ite->idx);
//
// Restore the listeners
//
			zerod_in_pool.pool_elem_listeners = ct_ite->listeners;
			
//
// Inform the listeners that the channel changed structure
//			
			if(zerod_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&zerod_in_pool);
				zerod_in_pool.fire_pool_elem_change(&evt);
			}			
		}
	}
}	

//------------------------------------------------------------------------------
// Pool::restore_comch
//
void Pool::restore_comch(vector<ObjBkp> &cts,
						 list<ControllerPool>::iterator &ctl,
						 vector<Pool::CtrlBkp>::iterator &ite)
{
	list<IPoolElementListener*> listeners;
	vector<Pool::ObjBkp>::iterator comch_ite;
	for (comch_ite = cts.begin();comch_ite != cts.end();++comch_ite)
	{
		list<CommunicationChannelPool>::iterator mtl = com_channel_list.begin();
 		advance(mtl,comch_ite->dist);

		if (ctl->id == mtl->ctrl_id)
		{
			vct_ite ct_fica = ctl->ite_ctrl_fica; 
			{
				AutoPoolLock lo((*ct_fica)->get_mon());
   				mtl->com_channel->ctrl_online();
   				
				{
					Tango::AutoTangoMonitor atm(mtl->com_channel);
					long m_id = mtl->id;
					mtl->obj_proxy->command_inout("Init");
					if (ite->old_fica_built == false)
					{
						CommunicationChannelPool &comch_in_pool = 
							static_cast<CommunicationChannelPool &>(
									get_com_channel_from_id(m_id));
							
						comch_in_pool.com_channel->create_dyn_attr();
						comch_in_pool.com_channel->remove_unwanted_dyn_attr_from_device();
					}
				}								
			}
			PoolElement &comch_in_pool = get_com_channel_from_id(comch_ite->idx);
//
// Restore the listeners
//
			comch_in_pool.pool_elem_listeners = comch_ite->listeners;
			
//
// Inform the listeners that the channel changed structure
//			
			if(comch_in_pool.has_listeners())
			{
				PoolElementEvent evt(ElementStructureChange,&comch_in_pool);
				comch_in_pool.fire_pool_elem_change(&evt);
			}
		}
	}
}	

//------------------------------------------------------------------------------
// Pool::motor_group_elts_changed
//
void Pool::motor_group_elts_changed(long mg_id)
{
	Tango::AutoTangoMonitor synch(this);
	Tango::Attribute &mgs = dev_attr->get_attr_by_name("MotorGroupList");
  	read_MotorGroupList(mgs);
  	mgs.fire_change_event();
}

//------------------------------------------------------------------------------
// Pool::measurement_group_elts_changed
//
void Pool::measurement_group_elts_changed(long mg_id)
{
	Tango::AutoTangoMonitor synch(this);
	Tango::Attribute &mgs = dev_attr->get_attr_by_name("MeasurementGroupList");
  	read_MeasurementGroupList(mgs);
  	mgs.fire_change_event();
}

//------------------------------------------------------------------------------
// Pool::handle_tmp_motor_groups
//
void Pool::handle_tmp_motor_groups()
{
	list<MotorGroupPool>::iterator it = mot_group_list.begin();
	vector<string> del_vec;
	for(; it != mot_group_list.end(); it++)
	{
		if(!it->group->is_temporary())
			continue;
//
// First increase the age of the temporary motor group
//
		it->group->aging();
		
//
// If it as been inactive for long enough mark it for deletion
//
		if(it->group->get_age() >= tmpElement_MaxInactTime)
		{
			DEBUG_STREAM << "Marking temporary motor group " << it->name 
						 << " for garbage collection due to inactivity" << endl;
			del_vec.push_back(it->name);
		}
	}

//
// Delete all marked temporary motor groups
//
	unsigned long del_nb = del_vec.size();
	if (del_nb > 0)
	{
		Tango::DeviceProxy tg_pool(device_name);
	
		for(unsigned long l = 0; l < del_nb; l++)
		{
			DEBUG_STREAM << "Motor group " << it->name 
						 << " was garbage collected" << endl;
			Tango::DeviceData din,dout;
			din << del_vec[l];
			try
			{
				dout = tg_pool.command_inout("DeleteMotorGroup", din);
			}
			catch(Tango::DevFailed &e)
			{
				WARN_STREAM << "Failed to delete temporary motor group " 
				            << it->name << endl;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::handle_tmp_measurement_groups
//
void Pool::handle_tmp_measurement_groups()
{
	list<MeasurementGroupPool>::iterator it = measurement_group_list.begin();
	vector<string> del_vec;
	for(; it != measurement_group_list.end(); it++)
	{
		if(!it->group->is_temporary())
			continue;
//
// First increase the age of the temporary motor group
//
		it->group->aging();
		
//
// If it as been inactive for long enough mark it for deletion
//
		if(it->group->get_age() >= tmpElement_MaxInactTime)
		{
			DEBUG_STREAM << "Marking temporary measurement group " << it->name 
						 << " for garbage collection due to inactivity" << endl;
			del_vec.push_back(it->name);
		}
	}

//
// Delete all marked temporary motor groups
//
	unsigned long del_nb = del_vec.size();
	if (del_nb > 0)
	{
		Tango::DeviceProxy tg_pool(device_name);
	
		for(unsigned long l = 0; l < del_nb; l++)
		{
			DEBUG_STREAM << "Measurement group " << it->name 
						 << " was garbage collected" << endl;
			Tango::DeviceData din,dout;
			din << del_vec[l];
			try
			{
				dout = tg_pool.command_inout("DeleteMeasurementGroup", din);
			}
			catch(Tango::DevFailed &e)
			{
				WARN_STREAM << "Failed to delete temporary measurement group " 
				            << it->name << endl;
			}
		}
	}
}

//------------------------------------------------------------------------------
// Pool::build_property_params
//
void Pool::build_property_params(const Tango::DevVarStringArray *argin,
								 vector<pair<string,string> > & value_pairs,
								 int start)
{
	for(int i = start; i < argin->length(); i++)
	{
		string prop_name((*argin)[i++].in());
		string prop_value((*argin)[i].in());
		value_pairs.push_back(make_pair(prop_name,prop_value));
	}
}

//------------------------------------------------------------------------------
// Pool::create_pseudo_motor_ctrl_elems
//
void Pool::create_pseudo_motor_ctrl_elems(string &ctrl_inst_name,
									      PseudoMotCtrlFiCa *fica,
										  vector<string> &pseudo_mot_names,
										  vector<string> &mot_names)
{
//----------------------------------------------------
// Check if we don't have already enough pseudo motors
//
	unsigned long pm_nb = pseudo_mot_names.size();
	unsigned long m_nb = mot_names.size();
	if (pseudo_mot_list.size() + pm_nb > MAX_PSEUDO_MOTOR)
	{
		Tango::Except::throw_exception((const char *)"Pool_TooManyPseudoMotor",
					       (const char *)"Too many pseudo motor in your pool",
					       (const char *)"Pool::create_pseudo_motor");
	}
	
//---------------------------------------------------------
// Check if motors are member of this pool
//
	
	
	string mg_name;
	{
		stringstream mg_name_stream;
		mg_name_stream << PSEUDO_MOTOR_MG_PREFIX << "_" << ctrl_inst_name;
		mg_name  = mg_name_stream.str();
	}
	
	for (unsigned long ul = 0; ul < m_nb ;ul++)
	{
		string &motor_name = mot_names[ul];
		if (motor_exist(motor_name) == false)
		{
			TangoSys_OMemStream o;
			o << "Motor " << motor_name << " is not defined in this pool. ";
			o << "Can't create the pseudo motor controller." << ends;
	
			Tango::Except::throw_exception(
					(const char *)"Pool_MotorNotDefined",o.str(),
					(const char *)"Pool::create_pseudo_motor");
		}
	}

//
// Build tango names
//
	vector<string> pm_tg_dev_names;
	
	for (unsigned long ul = 0; ul < pm_nb ;ul++)
	{
		string &pseudo_motor_name = pseudo_mot_names[ul];
		stringstream role_idx;
		role_idx << ul + 1;
		string dev_name = "pm/" + ctrl_inst_name + "/" + role_idx.str();
		pm_tg_dev_names.push_back(dev_name);
	}	

//---------------------------------------------------------
// Check if the Motor Group already exists
//
	bool grp_exists = group_exist(mg_name);
	
	if(grp_exists)
	{
		DEBUG_STREAM << "Group " << mg_name << " already exists."
					 << " Will use this one" << endl;
	}

	vector<string> &pseudo_motor_roles = fica->get_pseudo_motor_roles();
	
//---------------------------------------------------------
// Build Tango device name for Pseudo Motor and Motor Group
//
	
	Tango::Util	*tg = Tango::Util::instance();
			
//----------------------------------------------------
// Check if at least one device is already defined in database
// Check by device alias and by Tango device name
//
	Tango::Database *db = tg->get_database();
	
	Tango::DbDevImportInfo my_device_import;
	bool device_exist = false;
	bool by_alias = false;	
	vector<string> existing_pm;
	
	for (long i = 0; i < pm_nb ;i++)
	{
		bool curr_device_exist = false;
		try
		{
			my_device_import = db->import_device(pseudo_mot_names[i]);
			curr_device_exist = device_exist = true;
			by_alias = true;
			existing_pm.push_back(pseudo_mot_names[i]);
		}
		catch (Tango::DevFailed &e)
		{
			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
			{
				curr_device_exist = device_exist = true;
				existing_pm.push_back(pm_tg_dev_names[i]);
			}
		}

		if (curr_device_exist == false)
		{
			try
			{		
				my_device_import = db->import_device(pm_tg_dev_names[i]);
				device_exist = true;
				existing_pm.push_back(pm_tg_dev_names[i]);
			}
			catch (Tango::DevFailed &e)
			{
				if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined")!=0)
				{
					device_exist = true;
					existing_pm.push_back(pm_tg_dev_names[i]);
				}
			}
		}
	}
	
	if (device_exist == true)
	{
		TangoSys_OMemStream o;
		o << "The following device" << (existing_pm.size() > 1) ? "s " : ""  ;
		for(unsigned long i = 0; i < existing_pm.size(); i++)
			o << existing_pm[i] << " ";
		o << "are already defined" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoMotorAlreadyCreated",o.str(),
				(const char *)"Pool::create_pseudo_motor");
	}

//---------------------------------------------------------
// If the devices are not defined in database, create them in database, set 
// their alias and define its properties used to store its ID, its device pool 
// and its motor group
//

	if (device_exist == false)
	{
		
		DEBUG_STREAM << "Trying to create devices entry in database" << endl;
		
		//
		// Prepare the Motor Group
		//
		if(!grp_exists)
		{
			Tango::DevVarStringArray in;
			Tango::DeviceData din,dout;	
			
			in.length(m_nb + 1);
			
			in[0] = CORBA::string_dup(mg_name.c_str());
			for (unsigned long ul = 0; ul < m_nb ;ul++) 
				in[ul+1] = CORBA::string_dup(mot_names[ul].c_str());
			din << in;
			
			try 
			{
				Tango::DeviceProxy tg_pool(device_name);
				dout = tg_pool.command_inout("CreateMotorGroup", din);
				DEBUG_STREAM << "Created Motor Group Device " << mg_name << 
							    " for pseudo motor" << endl;
			}
			catch(Tango::DevFailed &ex)
			{
				TangoSys_OMemStream o;
				o << "Can't create motor group for pseudo motor controller ";
				o << ctrl_inst_name << ends;
					
				Tango::Except::re_throw_exception(ex,
						(const char *)"Pool_CantCreatePseudoMotor",o.str(),
						(const char *)"Pool::create_pseudo_motor");			
			}
		}
		
		vector<string> created_pm_dev; 
    	try
		{
			for(unsigned long ul = 0; ul < pm_nb ;ul++)
			{
				Tango::DbDevInfo my_device_info;
	    		my_device_info.name = pm_tg_dev_names[ul].c_str();
	    		my_device_info._class = "PseudoMotor";
	    		my_device_info.server = tg->get_ds_name().c_str();
				
	    		db->add_device(my_device_info);
				
				created_pm_dev.push_back(pm_tg_dev_names[ul]);
				db->put_device_alias(pm_tg_dev_names[ul],pseudo_mot_names[ul]);
	
				Tango::DbDatum id_prop(PSEUDO_MOTOR_ID_PROP);
				Tango::DbDatum motor_group_prop(PSEUDO_MOTOR_MG);
				Tango::DbDatum motor_list_prop(PSEUDO_MOTOR_LIST);
				Tango::DbDatum role_prop(PSEUDO_MOTOR_ROLE);
				Tango::DbDatum role_idx_prop(PSEUDO_MOTOR_ROLE_IDX);
				Tango::DbData db_data;
				
				pseudo_mot_id++;
				id_prop << pseudo_mot_id;
				db_data.push_back(id_prop);
				
				motor_group_prop << mg_name;
				db_data.push_back(motor_group_prop);
				
				motor_list_prop << mot_names;
				db_data.push_back(motor_list_prop);
				
				role_prop << pseudo_motor_roles[ul];
				db_data.push_back(role_prop);
				
				role_idx_prop << ul + 1;
				db_data.push_back(role_idx_prop);
				
				db->put_device_property(pm_tg_dev_names[ul].c_str(),db_data);
				
				Tango::DbDatum pos("Position"),abs_ch("abs_change");
				db_data.clear();
				pos << (long)1;
				abs_ch << defaultMotPos_AbsChange;
				db_data.push_back(pos);
				db_data.push_back(abs_ch);
				db->put_device_attribute_property(pm_tg_dev_names[ul].c_str(),
												  db_data);
				
				DEBUG_STREAM << "Device " << pseudo_mot_names[ul] << " created"
								" in database (with alias)" << endl;
			}
		}
		catch (Tango::DevFailed &e)
		{
			TangoSys_OMemStream o;
			// Something went wrong: Clean the Motor Group and pseudo motors 
			// if necessary

			if(!grp_exists)
			{		
				try 
				{
					Tango::DeviceProxy tg_pool(device_name);
					Tango::DeviceData din;
					din << mg_name;
					tg_pool.command_inout("DeleteMotorGroup", din);
				}
				catch(Tango::DevFailed &ex)
				{
					TangoSys_OMemStream o;
					o << "Can't delete motor group for pseudo motor and "; 
				}
			}
			
			for(unsigned long ul = 0; ul < created_pm_dev.size(); ul++)
			{
				try 
				{
					db->delete_device(created_pm_dev[ul]);
				}
				catch(Tango::DevFailed &ex)
				{
					TangoSys_OMemStream o;
					o << "Can't delete temporary created pseudo motor and ";
					o << pseudo_mot_names[ul] << endl;
				}
			}
						
			o << "Can't create pseudo motor in database" << ends;
			
			Tango::Except::re_throw_exception(e,
					(const char *)"Pool_CantCreatePseudoMotor",o.str(),
					(const char *)"Pool::create_pseudo_motor");
		}
		
//
// Find the Tango PseudoMotor class and create the pseudo motor
//
		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
		for (unsigned long i = 0;i < cl_list->size();i++)
		{
			if ((*cl_list)[i]->get_name() == "PseudoMotor")
			{
				DEBUG_STREAM << "Found PseudoMotor Class" << endl;
				try
				{
					for(long pm_index = 0; pm_index < pm_nb ;pm_index++)
					{
						Tango::DevVarStringArray na;
						na.length(1);
						na[0] = pm_tg_dev_names[pm_index].c_str();
						(*cl_list)[i]->device_factory(&na);
					}
					break;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Exception while trying to create "
									"PseudoMotor device" << endl;

					TangoSys_OMemStream o;
					// Something went wrong: Clean the Motor Group if necessary
					
					if(!grp_exists)
					{
						try 
						{
							DEBUG_STREAM << "Motor group " << mg_name << " was"
							" created specifically for this pseudo motor. "
							"Deleting it..." << endl;
							
							Tango::DeviceProxy tg_pool(device_name);
							Tango::DeviceData din;
							din << mg_name;
							tg_pool.command_inout("DeleteMotorGroup", din);
						}
						catch(Tango::DevFailed &ex)
						{
							TangoSys_OMemStream o;
							o << "Can't delete motor group for pseudo motor"
								 " and " << endl;
						}
					}

//
// The delete_device will also delete device property(ies)
//
					o << "Can't create pseudo motor device " << ends;
			
					Tango::Except::re_throw_exception(e,
							(const char *)"Pool_CantCreatePseudoMotor",o.str(),
							(const char *)"Pool::create_pseudo_motor");
				}
			}
		}
	}
	
//
// Create a Tango device proxy on the newly created pseudo motor(s)
// and set its connection to automatic re-connection
// Also subscribe to events comming from the motor group
//
	long pm_count = 0;
	list<PseudoMotorPool>::reverse_iterator ite = pseudo_mot_list.rbegin();
	for(; ite != pseudo_mot_list.rend() && pm_count <= pm_nb ;ite++)
	{
		PseudoMotorPool &pmp = *ite;
		
		pmp.obj_proxy = new Tango::DeviceProxy(ite->obj_tango_name);
		pmp.obj_proxy->set_transparency_reconnection(true);
		pmp.mot_grp->add_pool_elem_listener(&pmp);
	}
	
}

//------------------------------------------------------------------------------
// Pool::create_pseudo_counter_ctrl_elems
//
void Pool::create_pseudo_counter_ctrl_elems(string &ctrl_inst_name,
											PseudoCoCtrlFiCa *fica,
											string &pseudo_counter_name,
											vector<string> &counter_names)
{
//----------------------------------------------------
// Check if we don't have already enough pseudo counters
//
	unsigned long co_nb = counter_names.size();
	if (pseudo_co_list.size() + 1 > MAX_PSEUDO_COUNTER)
	{
		Tango::Except::throw_exception(
			(const char *)"Pool_TooManyPseudoMotor",
			(const char *)"Too many pseudo counters in your pool",
			(const char *)"Pool::create_pseudo_counter");
	}
	
//---------------------------------------------------------
// Check if counters are member of this pool
//
	bool is_zerod = false;
	for (unsigned long ul = 0; ul < co_nb ;ul++)
	{
		string &counter_name = counter_names[ul];
		
		if(ct_channel_exist(counter_name) == false)
		{
			is_zerod = zerod_channel_exist(counter_name);
			if(is_zerod == false)
			{
				TangoSys_OMemStream o;
				o << "Counter/0D channel " << counter_name;
				o << " is not defined in this pool. ";
				o << "Can't create the pseudo counter controller." << ends;
		
				Tango::Except::throw_exception(
						(const char *)"Pool_CounterNotDefined",o.str(),
						(const char *)"Pool::create_pseudo_counter");
			}
		}
	}

//
// Build tango name
//
	string pc_tg_dev_name = "pseudocounter/" + ctrl_inst_name + "/0";
		
//---------------------------------------------------------
// Build Tango device name for Pseudo Motor and Motor Group
//
	
	Tango::Util	*tg = Tango::Util::instance();
			
//----------------------------------------------------
// Check if at least one device is already defined in database
// Check by device alias and by Tango device name
//
	Tango::Database *db = tg->get_database();
	
	Tango::DbDevImportInfo my_device_import;
	bool device_exist = false;
	bool by_alias = false;	
	
	try
	{
		my_device_import = db->import_device(pseudo_counter_name);
		device_exist = true;
		by_alias = true;
	}
	catch (Tango::DevFailed &e)
	{
		if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
		{
			device_exist = true;
		}
	}

	if (device_exist == false)
	{
		try
		{		
			my_device_import = db->import_device(pc_tg_dev_name);
			device_exist = true;
		}
		catch (Tango::DevFailed &e)
		{
			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined")!=0)
			{
				device_exist = true;
			}
		}
	}
	
	
	if (device_exist == true)
	{
		TangoSys_OMemStream o;
		o << "The following device " << pc_tg_dev_name;
		o << " is already defined" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoCounterAlreadyCreated",o.str(),
				(const char *)"Pool::create_pseudo_counter");
	}

//---------------------------------------------------------
// If the devices are not defined in database, create them in database, set 
// their alias and define its properties used to store its ID, its device pool 
//

	if (device_exist == false)
	{
		DEBUG_STREAM << "Trying to create device entry in database" << endl;
		
		bool device_created = false;
    	try
		{
			Tango::DbDevInfo my_device_info;
    		my_device_info.name = pc_tg_dev_name.c_str();
    		my_device_info._class = "PseudoCounter";
    		my_device_info.server = tg->get_ds_name().c_str();
			
    		db->add_device(my_device_info);
			
			device_created = true;
			
			db->put_device_alias(pc_tg_dev_name,pseudo_counter_name);

			Tango::DbDatum id_prop(PSEUDO_COUNTER_ID_PROP);
			Tango::DbDatum channel_list_prop(PSEUDO_COUNTER_LIST);
			Tango::DbData db_data;
			
			exp_channel_id++;
			id_prop << exp_channel_id;
			db_data.push_back(id_prop);
			
			channel_list_prop << counter_names;
			db_data.push_back(channel_list_prop);
			
			db->put_device_property(pc_tg_dev_name.c_str(),db_data);
			
			Tango::DbDatum val("Value"),abs_ch("abs_change");
			db_data.clear();
			val << (long)1;
			abs_ch << defaultCtVal_AbsChange;
			db_data.push_back(val);
			db_data.push_back(abs_ch);
			
			db->put_device_attribute_property(pc_tg_dev_name.c_str(),
											  db_data);
			
			DEBUG_STREAM << "Device " << pseudo_counter_name << " created"
							" in database (with alias)" << endl;
		}
		catch (Tango::DevFailed &e)
		{
			TangoSys_OMemStream o;
			// Something went wrong: Cleanup
			
			if(device_created == true)
			{
				try 
				{
					db->delete_device(pc_tg_dev_name);
				}
				catch(Tango::DevFailed &ex)
				{
					TangoSys_OMemStream o;
					o << "Can't delete temporary created pseudo counter ";
					o << pseudo_counter_name << " " << endl;
				}
			}
						
			o << "Can't create pseudo counter in database" << ends;
			
			Tango::Except::re_throw_exception(e,
					(const char *)"Pool_CantCreatePseudoCounter",o.str(),
					(const char *)"Pool::create_pseudo_counter");
		}
		
//
// Find the Tango PseudoCounter class and create the pseudo counter
//
		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
		for (unsigned long i = 0;i < cl_list->size();i++)
		{
			if ((*cl_list)[i]->get_name() == "PseudoCounter")
			{
				DEBUG_STREAM << "Found PseudoCounter Class" << endl;
				try
				{
					Tango::DevVarStringArray na;
					na.length(1);
					na[0] = pc_tg_dev_name.c_str();
					(*cl_list)[i]->device_factory(&na);
					break;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Exception while trying to create "
									"PseudoCounter device" << endl;

					TangoSys_OMemStream o;
					// Something went wrong: Cleanup

//
// The delete_device will also delete device property(ies)
//
					o << "Can't create pseudo counter device " << ends;
			
					Tango::Except::re_throw_exception(e,
							(const char *)"Pool_CantCreatePseudoMotor",o.str(),
							(const char *)"Pool::create_pseudo_motor");
				}
			}
		}
	}
	
//
// Create a Tango device proxy on the newly created pseudo counter
// and set its connection to automatic re-connection
// Also subscribe to events comming from the channels
//
	Tango::DeviceProxy *tmp_dev = 
		new Tango::DeviceProxy(pseudo_co_list.back().obj_tango_name);
	tmp_dev->set_transparency_reconnection(true);
	pseudo_co_list.back().obj_proxy = tmp_dev;
}

//------------------------------------------------------------------------------
// Pool::user_elems_to_phy_elems
//
void Pool::user_elems_to_phy_elems(vector<PoolElement*> &user_elems, 
								   vector<string> &phy_elems, bool unique /* = true */)
{
	vector<PoolElement*>::iterator user_ite = user_elems.begin();
	for ( ; user_ite != user_elems.end() ; user_ite++)
		user_elem_to_phy_elems((*user_ite)->name, phy_elems, unique);
}

//------------------------------------------------------------------------------
// Pool::user_elems_to_phy_elems
//
void Pool::user_elems_to_phy_elems(vector<string> &user_elems, 
								   vector<string> &phy_elems, bool unique)
{
	vector<string>::iterator user_ite = user_elems.begin();
	for ( ; user_ite != user_elems.end() ; user_ite++)
		user_elem_to_phy_elems(*user_ite, phy_elems, unique);
}

//------------------------------------------------------------------------------
// Pool::user_elem_to_phy_elems
//
void Pool::user_elem_to_phy_elems(string &user_elem, vector<string> &phy_elems,
								  bool unique)
{
	bool throw_ex = false;
	Tango::DevFailed ex;
	
	// If the user element is a physical element
	try
	{
		PoolElement &pe = get_phy_pool_element_from_name(user_elem);
		
		if(unique == true)
		{
			string user_elem_lower(user_elem);
			transform(user_elem_lower.begin(),user_elem_lower.end(),user_elem_lower.begin(),::tolower);
			for(unsigned long ul = 0; ul < phy_elems.size(); ul++)
			{
				string elem_lower(phy_elems[ul]);
				transform(elem_lower.begin(),elem_lower.end(),elem_lower.begin(),::tolower);
				if(user_elem_lower == elem_lower)
				{
					return;
				}
			}
		}
		
		phy_elems.push_back(user_elem);
		return;
	}
	catch(Tango::DevFailed &e) {}
	
	// If the user element is a group element 
	try
	{
		PoolElement &pe = get_group_pool_element_from_name(user_elem);
		vector<PoolElement*> &user_elems = *pe.get_elems();
		try
		{
			user_elems_to_phy_elems(user_elems, phy_elems, unique);
		}
		catch(Tango::DevFailed &e) 
		{
			throw_ex = true;
			throw;
		}
		return;
	}
	catch(Tango::DevFailed &e) 
	{
		if(throw_ex == true) throw;
	}
	
	
	// If the user element is a pseudo element
	try
	{
		PoolElement &pe = get_pseudo_pool_element_from_name(user_elem);
		vector<PoolElement*> &user_elems = *pe.get_elems();
		try
		{
			user_elems_to_phy_elems(user_elems, phy_elems, unique);
		}
		catch(Tango::DevFailed &e) 
		{
			throw_ex = true;
			throw;
		}
		return;
	}
	catch(Tango::DevFailed &e) 
	{
		if(throw_ex == true) throw;
	}
	
	TangoSys_OMemStream o;
	o << "No element with name " << user_elem << " found in the pool" << ends;
	Tango::Except::throw_exception(
			(const char *)"Pool_ElementNotFound",o.str(),
			(const char *)"Pool::user_elem_to_phy_elems");	
}

}	//	namespace
