static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         Pool.cpp
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
// Revision 1.120  2007/09/07 15:00:07  tcoutinho
// safety commit
//
// Revision 1.119  2007/08/30 12:40:39  tcoutinho
// - changes to support Pseudo counters.
//
// Revision 1.118  2007/08/24 15:55:26  tcoutinho
// safety weekend commit
//
// Revision 1.117  2007/08/23 10:32:43  tcoutinho
// - basic pseudo counter check
// - some fixes regarding pseudo motors
//
// Revision 1.116  2007/08/20 06:37:32  tcoutinho
// development commit
//
// Revision 1.115  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.114  2007/08/08 08:47:55  tcoutinho
// Fix bug 18 : CreateController should be a one step operation
//
// Revision 1.113  2007/08/07 09:54:50  tcoutinho
// Small code changes
//
// Revision 1.112  2007/08/01 14:07:18  tcoutinho
// - bug fix returning null deviceproxy
// - prevent python errors by activating python lock
//
// Revision 1.111  2007/07/30 11:00:59  tcoutinho
// Fix bug 5 : Additional information for controllers
//
// Revision 1.110  2007/07/26 10:25:15  tcoutinho
// - Fix bug 1 :  Automatic temporary MotorGroup/MeasurementGroup deletion
//
// Revision 1.109  2007/07/17 11:41:57  tcoutinho
// replaced comunication with communication
//
// Revision 1.108  2007/07/16 11:00:24  tcoutinho
// - fix problem with communication controller information
// - fix problem reloading python modules
//
// Revision 1.107  2007/07/12 13:13:33  tcoutinho
// - added poolapi features
//
// Revision 1.106  2007/07/05 13:18:04  tcoutinho
// bug solve commit
//
// Revision 1.105  2007/07/05 13:13:17  tcoutinho
// development commit
//
// Revision 1.104  2007/07/04 15:06:38  tcoutinho
// first commit with stable Pool API
//
// Revision 1.103  2007/07/02 14:46:37  tcoutinho
// first stable comunication channel commit
//
// Revision 1.102  2007/06/28 16:22:37  tcoutinho
// safety commit during comunication channels development
//
// Revision 1.101  2007/06/28 07:15:34  tcoutinho
// safety commit during comunication channels development
//
// Revision 1.100  2007/06/27 10:24:45  tcoutinho
// scond commit for comuncation channels. Added command/attribute skeleton with pogo
//
// Revision 1.99  2007/06/13 15:18:58  etaurel
// - Fix memory leak
//
// Revision 1.98  2007/05/30 15:02:07  etaurel
// - Add init of the boolean used to inform ghost groups of an Init
// command being executed
// - Add init of a "language" data in the ReloadCtrl code command
//
// Revision 1.97  2007/05/17 07:06:37  etaurel
// - Use some new methods defined in Tango to create/delete device
//
// Revision 1.96  2007/05/15 15:02:41  tcoutinho
// - fix bugs
//
// Revision 1.95  2007/05/11 08:07:48  tcoutinho
// - added new propertie to allow different sleep time in CounterTimer
// - added new property to allow sleep time in 0D experiment channel
//
// Revision 1.94  2007/05/07 10:13:39  tcoutinho
// - small changes to 0D channel classes
// - fix bug in delete experiment channel
//
// Revision 1.93  2007/05/07 09:47:51  etaurel
// - Small changes for better 64 bits portability
//
// Revision 1.92  2007/04/30 15:47:05  tcoutinho
// - new attribute "Channels"
// - new device property "Channel_List"
// - when add/remove channel, pool sends a change event on the MeasurementGroupList
//
// Revision 1.91  2007/04/30 14:51:20  tcoutinho
// - make possible to Add/Remove elements on motorgroup that are part of other motor group(s)
//
// Revision 1.90  2007/04/26 08:24:57  tcoutinho
// - safe commit
//
// Revision 1.89  2007/04/23 15:23:06  tcoutinho
// - changes according to Sardana metting 26-03-2007: ActiveMeasurementGroup attribute became obsolete
//
// Revision 1.88  2007/03/02 16:34:27  tcoutinho
// - fix bugs with measurement group - event related, attribute quality, active measurement group management, etc
//
// Revision 1.87  2007/03/01 13:12:17  tcoutinho
// - measurement group event related fixes
//
// Revision 1.86  2007/02/28 16:21:52  tcoutinho
// - support for 0D channels
// - basic fixes after running first battery of tests on measurement group
//
// Revision 1.85  2007/02/26 09:53:00  tcoutinho
// - Introduction of properties for defult abs_change values in measurement group with fix
//
// Revision 1.84  2007/02/26 09:46:04  tcoutinho
// - Introduction of properties for defult abs_change values in measurement group
//
// Revision 1.83  2007/02/22 12:02:00  tcoutinho
// - added support for ghost measurement group
// - added support for measurement group in init/reload controller operations
// - added support of internal events on measurement group
//
// Revision 1.82  2007/02/16 09:59:57  tcoutinho
// - fix memory leak related with Active measurement group attribute
//
// Revision 1.81  2007/02/14 11:18:27  tcoutinho
// - fix bug on motorgroup when initcontroller is called
//
// Revision 1.80  2007/02/13 14:39:43  tcoutinho
// - fix bug in motor group when a motor or controller are recreated due to an InitController command
//
// Revision 1.79  2007/02/08 08:51:14  etaurel
// - Many changes. I don't remember the list
//
// Revision 1.78  2007/02/06 09:41:03  tcoutinho
// - added MeasurementGroup
//
// Revision 1.77  2007/01/26 08:36:47  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.76  2007/01/23 08:27:22  tcoutinho
// - fix some pm bugs found with the test procedure
// - added internal event for MotionEnded
//
// Revision 1.75  2007/01/17 09:38:14  tcoutinho
// - internal events bug fix.
//
// Revision 1.74  2007/01/16 14:32:21  etaurel
// - Coomit after a first release with CT
//
// Revision 1.73  2007/01/05 15:02:37  etaurel
// - First implementation of the Counter Timer class
//
// Revision 1.72  2007/01/05 13:03:37  tcoutinho
// -changes to internal event mechanism
// -support for gcc 4.1.1 compilation without errors
//
// Revision 1.71  2007/01/04 11:55:04  etaurel
// - Added the CounterTimer controller
//
// Revision 1.70  2006/12/28 15:36:58  etaurel
// - Fire events also on the motor limit_switches attribute
// - Throw events even in simulation mode
// - Mange motor position limit switches dependant of the offset attribute
//
// Revision 1.69  2006/12/20 10:23:47  tcoutinho
// - changes to support internal event propagation
// - bug fix in motor groups containing other motor groups or pseudo motors
//
// Revision 1.68  2006/12/18 11:37:09  etaurel
// - Features are only boolean values invisible from the external world
// - ExtraFeature becomes ExtraAttribute with data type of the old features
//
// Revision 1.67  2006/12/12 11:09:17  tcoutinho
// - support for pseudo motors and motor groups in a motor group
//
// Revision 1.66  2006/12/01 07:57:58  etaurel
// - Change a DEBUG message in the read_controllerClassList() method which was confusing
//
// Revision 1.65  2006/11/24 08:49:04  tcoutinho
// - changes to support pseudo motors in motor groups
//
// Revision 1.64  2006/11/23 11:46:37  tcoutinho
// - delete_pseudo_motor now additionally checks if it belongs to a motor group
//
// Revision 1.63  2006/11/21 14:39:55  tcoutinho
// bug fix
//
// Revision 1.62  2006/11/20 14:32:43  etaurel
// - Add ghost group and event on motor group position attribute
//
// Revision 1.61  2006/11/07 14:57:08  etaurel
// - Now, the pool really supports different kind of controllers (cpp and py)
//
// Revision 1.60  2006/11/06 13:23:16  tcoutinho
// - changed PseudoMotor interface. Now all methods require Python file name (with extension) as one parameter and a class name as a second paramenter
//
// Revision 1.59  2006/11/03 17:04:49  tcoutinho
// - read_PseudoMotorClassList now returns a format consistent with read_ControllerClassList
//
// Revision 1.58  2006/11/03 15:48:59  etaurel
// - Miscellaneous changes that I don't remember
//
// Revision 1.57  2006/10/30 11:37:18  etaurel
// - Some changes in the motor init sequence
//
// Revision 1.56  2006/10/27 14:43:02  etaurel
// - New management of the MaxDevice stuff
// - SendToCtrl cmd added
// - Some bug fixed in prop management
//
// Revision 1.55  2006/10/27 14:02:19  tcoutinho
// added support for class level properties in the Database
//
// Revision 1.54  2006/10/25 16:00:42  etaurel
// - The updated test suite is now running fine
//
// Revision 1.53  2006/10/25 10:04:30  etaurel
// - Complete implementation of the ReloadControllerCode command
// - Handle end of movment when reading position in polling mode
//
// Revision 1.52  2006/10/24 14:50:52  etaurel
// - The test suite is now back into full success
//
// Revision 1.51  2006/10/23 15:12:35  etaurel
// - Fix memory leak in several places
//
// Revision 1.50  2006/10/23 13:36:57  etaurel
// - First implementation of controller properties for CPP controller
//
// Revision 1.49  2006/10/20 15:37:30  etaurel
// - First release with GetControllerInfo command supported and with
// controller properties
//
// Revision 1.48  2006/10/17 14:28:09  tcoutinho
// bug fixes on properties
//
// Revision 1.47  2006/10/16 16:11:53  tcoutinho
// fix bug on pseudo motor creation
//
// Revision 1.46  2006/10/06 15:41:03  tcoutinho
// bug fixes: - error report in GetPseudoMotorInfo.
//                 - missed instatiation of pseudo_proxy in the PseudoMotorPool structure.
//
// Revision 1.45  2006/10/06 13:28:24  tcoutinho
// changed info command names, added properties functionality
//
// Revision 1.44  2006/10/05 14:53:32  etaurel
// - Test suite of motor controller features is now working
//
// Revision 1.43  2006/10/02 09:19:11  etaurel
// - Motor controller now supports extra features (both CPP and Python)
//
// Revision 1.42  2006/09/29 12:51:16  tcoutinho
// *** empty log message ***
//
// Revision 1.41  2006/09/28 09:22:24  etaurel
// - End of the ControllerClassList attribute implementation
//
// Revision 1.40  2006/09/27 15:15:49  etaurel
// - ExternalFile and CtrlFile has been splitted in several classes:
//   ExternalFile, CppCtrlFile, PyExternalFile and PyCtrlFile
//
// Revision 1.39  2006/09/25 15:27:30  tcoutinho
// python memory leak fix
//
// Revision 1.38  2006/09/22 15:31:01  etaurel
// - Miscellaneous changes
//
// Revision 1.37  2006/09/22 07:57:07  tcoutinho
// - Changes to the python in xxxFile classes
//
// Revision 1.36  2006/09/21 12:44:41  tcoutinho
// - python path changes
//
// Revision 1.35  2006/09/21 10:20:53  etaurel
// - The motor group do not ID any more
//
// Revision 1.34  2006/09/21 07:25:57  etaurel
// - Changes due to the removal of Motor ID in the Tango interface
//
// Revision 1.33  2006/09/20 15:59:21  tcoutinho
// pseudo motor API changed
//
// Revision 1.32  2006/09/20 13:11:11  etaurel
// - For the user point of view, the controller does not have ID any more.
// We are now using the controller instance name (uniq) to give them a name
//
// Revision 1.31  2006/09/19 15:57:03  tcoutinho
// get_pseudo_path fix
//
// Revision 1.30  2006/09/19 13:57:35  tcoutinho
// pseudo motor with test procedure clear.
//
// Revision 1.29  2006/09/19 09:57:12  etaurel
// - Commit after the controller, motor and motor_group test sequences works after the merge
//
// Revision 1.28  2006/09/19 07:24:32  tcoutinho
// - changes to make pseudo motor interface homogenous with the rest of the Pool
//
// Revision 1.27  2006/09/18 10:32:21  etaurel
// - Commit after merge with pseudo-motor branch
//
// Revision 1.11.2.9  2006/09/15 13:27:43  tcoutinho
// pseudo changes to reload pseudo code method
//
// Revision 1.11.2.8  2006/09/12 15:44:46  tcoutinho
// the pseudo motor at version 2.1
//
// Revision 1.11.2.7  2006/08/22 08:50:49  tcoutinho
// the pseudo motor at version 2.1
//
// Revision 1.11.2.6  2006/08/03 12:10:33  tcoutinho
// Pseudo Motor after first success in tests.
//
// Revision 1.11.2.5  2006/07/10 09:52:43  tcoutinho
// development
//
// Revision 1.11.2.4  2006/07/03 12:44:34  tcoutinho
// pseudo motor basic operations on the pool done as well as initial python support
//
// Revision 1.11.2.3  2006/06/27 09:26:28  tcoutinho
// commit before adding python pseudo function support to pseudo motors
//
// Revision 1.11.2.2  2006/05/23 14:29:56  tcoutinho
// minor changes
//
// Revision 1.11.2.1  2006/05/23 08:54:59  tcoutinho
// initial pseudo motor changes
//
// Revision 1.13  2006/05/23 07:31:29  tcoutinho
// Initial addons for Pseudo Motor.
//
// Revision 1.12  2006/05/22 09:46:17  tcoutinho
// Initial addons for Pseudo Motor.
//
// Revision 1.11  2006/04/27 07:29:42  etaurel
// - Many changes after the travel to Boston
//
// Revision 1.10  2006/03/29 07:08:59  etaurel
// - Added motor group features
//
// Revision 1.9  2006/03/27 12:52:32  etaurel
// - Commit before adding MotorGroup class
//
// Revision 1.8  2006/03/20 08:25:52  etaurel
// - Commit changes before changing the Motor interface
//
// Revision 1.7  2006/03/17 13:39:53  etaurel
// - Before modifying commands
//
// Revision 1.6  2006/03/16 08:05:45  etaurel
// - Added code for the ControllerCode load and unload commands
// - Test and debug InnitController cmd with motor attached to the controller
//
// Revision 1.5  2006/03/14 15:08:51  etaurel
// - Send a Init command to motor device after a successfull InitController command on a controller
//
// Revision 1.4  2006/03/14 14:54:09  etaurel
// - Again new changes in the internal structure
//
// Revision 1.3  2006/03/14 08:44:06  etaurel
// - Change the orders of the CreateController command arguments
//
// Revision 1.2  2006/03/14 08:25:10  etaurel
// - Change the way objects are aorganized within the pool device
//
// Revision 1.1.1.1  2006/03/10 13:40:57  etaurel
// Initial import
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================



//===================================================================
//
//	The following table gives the correspondance
//	between commands and method's name.
//
//  Command's name          |  Method's name
//	----------------------------------------
//  State                   |  dev_state()
//  Status                  |  dev_status()
//  CreateController        |  create_controller()
//  CreateExpChannel        |  create_exp_channel()
//  CreateMotor             |  create_motor()
//  CreateMotorGroup        |  create_motor_group()
//  DeleteController        |  delete_controller()
//  DeleteExpChannel        |  delete_exp_channel()
//  DeleteMotor             |  delete_motor()
//  DeleteMotorGroup        |  delete_motor_group()
//  DeletePseudoMotor       |  delete_pseudo_motor()
//  GetControllerInfo       |  get_controller_info()
//  InitController          |  init_controller()
//  ReloadControllerCode    |  reload_controller_code()
//  SendToController        |  send_to_controller()
//  CreateMeasurementGroup  |  create_measurement_group()
//  DeleteMeasurementGroup  |  delete_measurement_group()
//  CreateComChannel        |  create_com_channel()
//  DeleteComChannel        |  delete_com_channel()
//  GetControllerInfoEx     |  get_controller_info_ex()
//  DeletePseudoCounter     |  delete_pseudo_counter()
//  GetFile                 |  get_file()
//  PutFile                 |  put_file()
//
//===================================================================

#include <CtrlFiCa.h>
#include <tango.h>
#include <eventsupplier.h>
#include <Pool.h>
#include <PoolClass.h>
#include <PoolUtil.h>
#include <Motor.h>
#include <MotorGroup.h>
#include <CTExpChannel.h>
#include <ZeroDExpChannel.h>
#include <PseudoMotor.h>
#include <PseudoCounter.h>
#include <MeasurementGroup.h>
#include <CommunicationChannel.h>
#include <CppMotCtrlFile.h>

#include <algorithm>
#include <ltdl.h>
#include <functional>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pool/MotCtrl.h>

namespace Pool_ns
{	

//+----------------------------------------------------------------------------
//
// method : 		Pool::Pool(string &s)
// 
// description : 	constructor for simulated Pool
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
Pool::Pool(Tango::DeviceClass *cl,string &s)
:Tango::Device_3Impl(cl,s.c_str())
{
	init_cmd = false;
	init_device();
}

Pool::Pool(Tango::DeviceClass *cl,const char *s)
:Tango::Device_3Impl(cl,s)
{
	init_cmd = false;
	init_device();
}

Pool::Pool(Tango::DeviceClass *cl,const char *s,const char *d)
:Tango::Device_3Impl(cl,s,d)
{
	init_cmd = false;
	init_device();
}
//+----------------------------------------------------------------------------
//
// method : 		Pool::delete_device()
// 
// description : 	will be called at device destruction or at init command.
//
//-----------------------------------------------------------------------------
void Pool::delete_device()
{
	//	Delete device's allocated object

//
// Clear ctrl info vector
//
	ctrl_info.clear();

//
// Get ghost groups (motor and measurement) to inform them that we invalidate
// pool object in case a polling arrive while the Pool device init command
// is being executed
//

	MotorGroup_ns::MotorGroup *motor_ghost_ptr;
	MeasurementGroup_ns::MeasurementGroup *meas_ghost_ptr;
	
	try
	{		
		motor_ghost_ptr = get_ghost_motor_group_ptr();
		motor_ghost_ptr->pool_init_cmd = true;
		meas_ghost_ptr = get_ghost_measurement_group_ptr();
		meas_ghost_ptr->pool_init_cmd = true;
	}
	catch (Tango::DevFailed &e) {}
		
//
// Mark all motors with controller code offline because
// we will delete all controllers objects
// But before, suicide them...
//

	DEBUG_STREAM << "Cleaning motor data (" << mot_list.size() << ")...";
	list<MotorPool>::iterator m_ite;
	
	for (m_ite = mot_list.begin();m_ite != mot_list.end();++m_ite)
	{
		Motor_ns::Motor *mot = m_ite->motor;
		Tango::AutoTangoMonitor atm(mot);
		mot->suicide();
		mot->ctrl_offline();
	}
	
//
// Mark all pseudo motors with controller code offline because
// we will delete all controllers objects
//

	DEBUG_STREAM << "Cleaning pseudo motor data (" << pseudo_mot_list.size() << ")...";
	list<PseudoMotorPool>::iterator pm_ite;

	for (pm_ite = pseudo_mot_list.begin();pm_ite != pseudo_mot_list.end();++pm_ite)
	{
		PseudoMotor_ns::PseudoMotor *pm = pm_ite->pseudo_motor; 
		Tango::AutoTangoMonitor atm(pm);
		pm->suicide();
		pm->ctrl_offline();
	}	
	
//
// Mark all pseudo counters with controller code offline because
// we will delete all controllers objects
//

	DEBUG_STREAM << "Cleaning pseudo counter data (" << pseudo_mot_list.size() << ")...";
	list<PseudoCounterPool>::iterator pc_ite;

	for (pc_ite = pseudo_co_list.begin();pc_ite != pseudo_co_list.end();++pc_ite)
	{
		PseudoCounter_ns::PseudoCounter *pc = pc_ite->pseudo_counter; 
		Tango::AutoTangoMonitor atm(pc);
		pc->suicide();
		pc->ctrl_offline();
	}		

//
// Mark all communication channels with controller code offline because
// we will delete all controllers objects
// But before, suicide them...
//

	DEBUG_STREAM << "Cleaning comm channel data (" 
				 <<	com_channel_list.size() << ")...";
	list<CommunicationChannelPool>::iterator com_ch_ite;
	
	for (com_ch_ite = com_channel_list.begin();
		 com_ch_ite != com_channel_list.end();++com_ch_ite)
	{
		CommunicationChannel_ns::CommunicationChannel *com = com_ch_ite->com_channel; 
		Tango::AutoTangoMonitor atm(com);
		com->suicide();
		com->ctrl_offline();
	}	
//
// Mark all counter timer with controller code offline because
// we will delete all controllers objects
// But before, suicide them...
//

	DEBUG_STREAM << "Cleaning counter timer data (" << ct_list.size() << ")...";
	list<CTExpChannelPool>::iterator ct_ite;
	
	for (ct_ite = ct_list.begin();ct_ite != ct_list.end();++ct_ite)
	{
		CTExpChannel_ns::CTExpChannel *ch = ct_ite->ct_channel; 
		Tango::AutoTangoMonitor atm(ch);
		ch->suicide();
		ch->ctrl_offline();
	}
	
//
// Mark all Zero D with controller code offline because
// we will delete all controllers objects
// But before, suicide them...
//

	DEBUG_STREAM << "Cleaning Zero D data (" << ct_list.size() << ")...";
	list<ZeroDExpChannelPool>::iterator z_ite;
	
	for (z_ite = zerod_list.begin();z_ite != zerod_list.end();++z_ite)
	{
		ZeroDExpChannel_ns::ZeroDExpChannel *ch = z_ite->zerod_channel;
		Tango::AutoTangoMonitor atm(ch);
		ch->suicide();
		ch->ctrl_offline();
	}
	
//
// Clear the controller lists
//

	DEBUG_STREAM << "Cleaning controller data...";
	list<ControllerPool>::iterator c_ite;

	for (c_ite = ctrl_list.begin();c_ite != ctrl_list.end();++c_ite)
	{
		if (c_ite->ctrl != NULL)
		{
			AutoPoolLock lo(c_ite->get_ctrl_fica_mon());
			delete c_ite->ctrl;
			if (c_ite->cpp_ctrl_prop != NULL)
				delete c_ite->cpp_ctrl_prop;
		}
	}
	ctrl_list.clear();
	

	
//
// Do not clear the controller vector in PoolClass
// object in case of several pool devices within the
// same DS
//

	init_cmd = true;
	
//
// A trick to inform client(s) listening on events that the pool device is down.
// Without this trick, the clients will have to wait for 3 seconds before being informed 
// This is the Tango device time-out.
// To know that we are executing this code due to a pool shutdown and not due to a
// "Init" command, we are using the polling thread ptr which is cleared in the DS
// shutdown down sequence before the device destruction
//

	Tango::Util *tg = Tango::Util::instance();
	if (tg->get_polling_thread_object() == NULL)
	{
		//Also clear the API
		{
			AutoPythonLock apl;
			Py_DECREF(pool_util);
			delete factory;
		}

		set_state(Tango::UNKNOWN);
		Tango::Attribute &state_att = dev_attr->get_attr_by_name("state");
		state_att.fire_change_event();
	}
	
	int nerr = lt_dlexit();
	
	if(nerr > 0)
	{
		TangoSys_OMemStream o;
		o << nerr << " occured in dynamic library shutdown";
		Tango::Except::throw_exception((const char *)"Pool_DLExitError",
						o.str(), (const char *)"Pool::delete_device");		
	}
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::init_device()
// 
// description : 	will be called at device initialization.
//
//-----------------------------------------------------------------------------
void Pool::init_device()
{
	INFO_STREAM << "Pool::Pool() create device " << device_name << endl;

	// Initialise variables to default values
	//--------------------------------------------

if(!init_cmd) cout << "Starting device pool..." << endl;	
	
//
// Check that the process is using a correct release of Tango
//

	Tango::Util *tg = Tango::Util::instance();
	long vers = tg->get_tango_lib_release();
	if (vers < MIN_TANGO_VERSION)
	{
		TangoSys_OMemStream o;
		o << "You are using Tango release " << vers;
		o << "\nThe Sardana device pool needs Tango release " << MIN_TANGO_VERSION << " or more";
		o << "\nPlease update Tango" << ends;

		Tango::Except::throw_exception((const char *)"Pool_WrongTangoRelease",o.str(),
			        	(const char *)"Pool::init_device");
	}	
		
	poolPath.clear();
	poolPath.push_back("NotDefined");
		
	get_device_property();
	
	poolPath_splitted = false;
	last_ctrl_id = 0;
	mot_id = 0;
	mot_group_id = 1;
	pseudo_mot_id = 0;
	exp_channel_id = 0;
	measurement_group_id = 1;
	com_channel_id = 0;
	proxy_created = false;
	moving_state_requested = false;
	
	set_state(Tango::ON);
	
	int nerr = lt_dlinit();
	
	if(nerr > 0)
	{
		TangoSys_OMemStream o;
		o << nerr << " occured in dynamic library initialization";
		Tango::Except::throw_exception((const char *)"Pool_DLInitError",
						o.str(), (const char *)"Pool::init_device");		
	}
	
	if(!init_cmd)
	{
		PoolClass *ds_class = (static_cast<PoolClass *>(get_device_class()));
		ds_class->init_py_env();
		
		{
			AutoPythonLock apl;
			factory = new LoggingDeviceFactory();
			pool_util = (PyObject*)PoolUtil::init(factory);
		}
	}
	
	attr_SimulationMode_read = &attr_SimulationMode_write;
	
	Tango::WAttribute &att = get_device_attr()->get_w_attr_by_name("SimulationMode");
	attr_SimulationMode_write = false;
	att.set_write_value(attr_SimulationMode_write);

//
// We will push change event on several attributes
//

	Tango::Attribute &state_att = dev_attr->get_attr_by_name("state");
	state_att.set_change_event(true,false);

	Tango::Attribute &mgl = dev_attr->get_attr_by_name("MotorGroupList");
	mgl.set_change_event(true,false);

	Tango::Attribute &ml = dev_attr->get_attr_by_name("MotorList");
	ml.set_change_event(true,false);

	Tango::Attribute &pml = dev_attr->get_attr_by_name("PseudoMotorList");
	pml.set_change_event(true,false);
	
	Tango::Attribute &pcl = dev_attr->get_attr_by_name("PseudoCounterList");
	pcl.set_change_event(true,false);

	Tango::Attribute &cl = dev_attr->get_attr_by_name("ControllerList");
	cl.set_change_event(true,false);
	
	Tango::Attribute &ecl = dev_attr->get_attr_by_name("ExpChannelList");
	ecl.set_change_event(true,false);

	Tango::Attribute &mntgl = dev_attr->get_attr_by_name("MeasurementGroupList");
	mntgl.set_change_event(true,false);

	Tango::Attribute &ccl = dev_attr->get_attr_by_name("ComChannelList");
	ccl.set_change_event(true,false);

	Tango::Attribute &sm = dev_attr->get_attr_by_name("SimulationMode");
	sm.set_change_event(true,false);

//
// Create all the controller(s) we found in DB
//

	Tango::DbData db_data;
	db_data.push_back(Tango::DbDatum(CTRL_PROP));

	get_db_device()->get_property(db_data);
	
	if (db_data[0].is_empty() == false)
	{
		vector<string> ctrls;
		db_data[0] >> ctrls;
		
		long nb_data = ctrls.size();
		if ((nb_data % PROP_BY_CTRL) != 0)
		{
			TangoSys_OMemStream o;
			o << "Can't create pool " << get_name();
			o << "\nWrong number of properties defining pool controller(s)";
			o << "\nCheck pool device property called Controller" << ends;

			Tango::Except::throw_exception((const char *)"Pool_ControllerAlreadyCreated",o.str(),
				        	(const char *)"Pool::create_controller");
		}

		long nb_ctrl = nb_data / PROP_BY_CTRL;
		
		upd_db = false;
		for (long loop = 0;loop < nb_ctrl;loop++)
		{
			Tango::DevVarStringArray in;
			in.length(PROP_BY_CTRL);
			
			in[0] = ctrls[loop * PROP_BY_CTRL].c_str();
			in[1] = ctrls[(loop * PROP_BY_CTRL) + 1].c_str();
			in[2] = ctrls[(loop * PROP_BY_CTRL) + 2].c_str();
			in[3] = ctrls[(loop * PROP_BY_CTRL) + 3].c_str();
			in[4] = ctrls[(loop * PROP_BY_CTRL) + 4].c_str();

			string ctrl_status;
			
			try
			{			
				add_ctrl(&in, true);
			}
			catch (Tango::DevFailed &e)
			{
				if (get_logger()->is_info_enabled())
					Tango::Except::print_exception(e);

				string except_desc_0(e.errors[0].desc);
				string except_desc_1;
				long except_size = e.errors.length();
				if (except_size >= 2)
					except_desc_1 = e.errors[1].desc;
					
				ctrl_status = "Error reported when trying to create controller ";
				ctrl_status = ctrl_status + ctrls[(loop * PROP_BY_CTRL) + 2].c_str() + " with instance ";
				ctrl_status = ctrl_status + ctrls[(loop * PROP_BY_CTRL) + 3].c_str() + " from file ";
				ctrl_status = ctrl_status + ctrls[loop * PROP_BY_CTRL + 1].c_str();
				ctrl_status = ctrl_status + ".\nThe reported error description is";
				ctrl_status = ctrl_status + "\n\t" + except_desc_0;
				if (except_size >= 2)
					ctrl_status = ctrl_status + "\n\t" + except_desc_1;
			}

			string::size_type pos;
			string cp_name(in[1]);
			
			if ((pos = cp_name.find('.')) != string::npos)
				cp_name.erase(pos);
			cp_name = cp_name + '.' + in[2].in() + '/' + in[3].in();
			transform(cp_name.begin(),cp_name.end(),cp_name.begin(),::tolower);
			
			ControllerPool &cp = get_ctrl_from_name(cp_name);
			cp.error_status = ctrl_status;
		}

		if (ctrl_list.size() >= 2)
			ctrl_list.sort(CtrlComp());	
	
	}
	upd_db = true;

//
// If we are called due to a Init command, we need for each motor to
// force an Init command on each of them
// First, memorize motor names then, clear the pool motor list
// and finally send them an init command.
// The motor init command will register themself in the pool motor list
//
// WARNING: There is a trick here. At this point the pool controller list has been
// rebuilt. Therefore, the iterator stored in the motor objects on the controller list
// is invalid. The Motor class always_executed_hook() method uses this iterator....
// Setting the simulation flag to True will make the Motor always_executed_hook()
// method to not use this iterator. The Motor Init command will update the
// iterator to a new valid value and will reset the simulation flag to False
// (on top of other things). 
//
	
/*	if (init_cmd == true)
	{*/
//
// Handle motors
//
		
		vector<string> tg_dev_name;
		list<MotorPool>::iterator ite;

		for (ite = mot_list.begin();ite != mot_list.end();++ite)
		{
			tg_dev_name.push_back(ite->obj_tango_name);
			ite->motor->simulation(true);
		}

		vector<string>::iterator name_ite;		
		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy mot_dev(*name_ite);
			mot_dev.command_inout("Init");
		}

//
// Handle pseudo motors
//

		vector<string> pm_tg_dev_name;
		list<PseudoMotorPool>::iterator pm_ite;
		for (pm_ite = pseudo_mot_list.begin();pm_ite != pseudo_mot_list.end();++pm_ite)
		{
			pm_tg_dev_name.push_back(pm_ite->obj_tango_name);
		}

		vector<string>::iterator pm_name_ite;		
		for (pm_name_ite = pm_tg_dev_name.begin();pm_name_ite != pm_tg_dev_name.end();++pm_name_ite)
		{
			Tango::DeviceProxy pm_dev(*pm_name_ite);
			pm_dev.command_inout("Init");
		}
		
//
// Handle pseudo counters
//

		vector<string> pc_tg_dev_name;
		list<PseudoCounterPool>::iterator pc_ite;
		for (pc_ite = pseudo_co_list.begin();pc_ite != pseudo_co_list.end();++pc_ite)
		{
			pc_tg_dev_name.push_back(pc_ite->obj_tango_name);
		}

		vector<string>::iterator pc_name_ite;		
		for (pc_name_ite = pc_tg_dev_name.begin();pc_name_ite != pc_tg_dev_name.end();++pc_name_ite)
		{
			Tango::DeviceProxy pc_dev(*pc_name_ite);
			pc_dev.command_inout("Init");
		}
				
//
// Handle CTExpChannel
//

		tg_dev_name.clear();
		list<CTExpChannelPool>::iterator cte_ite;
		for (cte_ite = ct_list.begin();cte_ite != ct_list.end();++cte_ite)
		{
			tg_dev_name.push_back(cte_ite->obj_tango_name);
		}

		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy cte_dev(*name_ite);
			cte_dev.command_inout("Init");
		}
		
//
// Handle ZeroDExpChannel
//

		tg_dev_name.clear();
		list<ZeroDExpChannelPool>::iterator z_ite;
		for (z_ite = zerod_list.begin();z_ite != zerod_list.end();++z_ite)
		{
			tg_dev_name.push_back(z_ite->obj_tango_name);
		}

		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy cte_dev(*name_ite);
			cte_dev.command_inout("Init");
		}

//
// Handle measurement group
//

		tg_dev_name.clear();
		list<MeasurementGroupPool>::iterator mtng_ite = measurement_group_list.begin();
		for (;mtng_ite != measurement_group_list.end();++mtng_ite)
		{
			tg_dev_name.push_back(mtng_ite->obj_tango_name);
		}

		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy mg_dev(*name_ite);
			mg_dev.command_inout("Init");
		}
		

//
// Handle motor group
//

		tg_dev_name.clear();
		list<MotorGroupPool>::iterator mg_ite;
		for (mg_ite = mot_group_list.begin();mg_ite != mot_group_list.end();++mg_ite)
		{
			tg_dev_name.push_back(mg_ite->obj_tango_name);
		}

		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy gm_dev(*name_ite);
			gm_dev.command_inout("Init");
		}
		
//
// Handle CommunicationChannel
//

		tg_dev_name.clear();
		list<CommunicationChannelPool>::iterator cce_ite;
		for (cce_ite = com_channel_list.begin();cce_ite != com_channel_list.end();++cce_ite)
		{
			tg_dev_name.push_back(cce_ite->obj_tango_name);
		}

		for (name_ite = tg_dev_name.begin();name_ite != tg_dev_name.end();++name_ite)
		{
			Tango::DeviceProxy cce_dev(*name_ite);
			cce_dev.command_inout("Init");
		}		
		
//
// Handle the ghost group
//
			
		try
		{		
			MotorGroup_ns::MotorGroup *ghost_ptr = get_ghost_motor_group_ptr();
		
			Tango::AutoTangoMonitor atm(ghost_ptr);
			ghost_ptr->delete_device();
			ghost_ptr->init_device();
		}
		catch (Tango::DevFailed &e) {}
		
//
// Handle the ghost measurement group
//
			
		try
		{		
			MeasurementGroup_ns::MeasurementGroup *ghost_ptr = 
				get_ghost_measurement_group_ptr();
		
			Tango::AutoTangoMonitor atm(ghost_ptr);
			ghost_ptr->delete_device();
			ghost_ptr->init_device();
		}
		catch (Tango::DevFailed &e) {}		
			
/*		init_cmd = false;
	}*/
	
//
// Push change_event to inform client listenning on events
//

	read_MotorGroupList(mgl);
	mgl.fire_change_event();
	
	read_MotorList(ml);	
	ml.fire_change_event();
	
	read_ControllerList(cl);	
	cl.fire_change_event();	
	
	read_ExpChannelList(ecl);	
	ecl.fire_change_event();
	
	read_PseudoMotorList(pml);
	pml.fire_change_event();	

	read_PseudoCounterList(pcl);
	pcl.fire_change_event();
	
	read_MeasurementGroupList(mntgl);
	mntgl.fire_change_event();

	read_ComChannelList(ccl);
	ccl.fire_change_event();

	read_SimulationMode(sm);
	sm.fire_change_event();
}


//+----------------------------------------------------------------------------
//
// method : 		Pool::get_device_property()
// 
// description : 	Read the device properties from database.
//
//-----------------------------------------------------------------------------
void Pool::get_device_property()
{
	//	Initialize your default values here (if not done with  POGO).
	//------------------------------------------------------------------

	//	Read device properties from database.(Automatic code generation)
	//------------------------------------------------------------------
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("PoolPath"));
	dev_prop.push_back(Tango::DbDatum("DefaultMotPos_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("DefaultMotGrpPos_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("GhostGroup_PollingPeriod"));
	dev_prop.push_back(Tango::DbDatum("MotThreadLoop_SleepTime"));
	dev_prop.push_back(Tango::DbDatum("NbStatePerRead"));
	dev_prop.push_back(Tango::DbDatum("DefaultCtVal_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("ZeroDNbReadPerEvent"));
	dev_prop.push_back(Tango::DbDatum("DefaultZeroDVal_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("DefaultCtGrpVal_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("DefaultZeroDGrpVal_AbsChange"));
	dev_prop.push_back(Tango::DbDatum("CTThreadLoop_SleepTime"));
	dev_prop.push_back(Tango::DbDatum("ZeroDThreadLoop_SleepTime"));
	dev_prop.push_back(Tango::DbDatum("TmpElement_MaxInactTime"));

	//	Call database and extract values
	//--------------------------------------------
	if (Tango::Util::instance()->_UseDb==true)
		get_db_device()->get_property(dev_prop);
	Tango::DbDatum	def_prop, cl_prop;
	PoolClass	*ds_class =
		(static_cast<PoolClass *>(get_device_class()));
	int	i = -1;

	//	Try to initialize PoolPath from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  poolPath;
	//	Try to initialize PoolPath from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  poolPath;
	//	And try to extract PoolPath value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  poolPath;

	//	Try to initialize DefaultMotPos_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultMotPos_AbsChange;
	//	Try to initialize DefaultMotPos_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultMotPos_AbsChange;
	//	And try to extract DefaultMotPos_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultMotPos_AbsChange;

	//	Try to initialize DefaultMotGrpPos_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultMotGrpPos_AbsChange;
	//	Try to initialize DefaultMotGrpPos_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultMotGrpPos_AbsChange;
	//	And try to extract DefaultMotGrpPos_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultMotGrpPos_AbsChange;

	//	Try to initialize GhostGroup_PollingPeriod from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  ghostGroup_PollingPeriod;
	//	Try to initialize GhostGroup_PollingPeriod from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  ghostGroup_PollingPeriod;
	//	And try to extract GhostGroup_PollingPeriod value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  ghostGroup_PollingPeriod;

	//	Try to initialize MotThreadLoop_SleepTime from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  motThreadLoop_SleepTime;
	//	Try to initialize MotThreadLoop_SleepTime from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  motThreadLoop_SleepTime;
	//	And try to extract MotThreadLoop_SleepTime value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  motThreadLoop_SleepTime;

	//	Try to initialize NbStatePerRead from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  nbStatePerRead;
	//	Try to initialize NbStatePerRead from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  nbStatePerRead;
	//	And try to extract NbStatePerRead value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  nbStatePerRead;

	//	Try to initialize DefaultCtVal_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultCtVal_AbsChange;
	//	Try to initialize DefaultCtVal_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultCtVal_AbsChange;
	//	And try to extract DefaultCtVal_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultCtVal_AbsChange;

	//	Try to initialize ZeroDNbReadPerEvent from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  zeroDNbReadPerEvent;
	//	Try to initialize ZeroDNbReadPerEvent from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  zeroDNbReadPerEvent;
	//	And try to extract ZeroDNbReadPerEvent value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  zeroDNbReadPerEvent;

	//	Try to initialize DefaultZeroDVal_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultZeroDVal_AbsChange;
	//	Try to initialize DefaultZeroDVal_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultZeroDVal_AbsChange;
	//	And try to extract DefaultZeroDVal_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultZeroDVal_AbsChange;

	//	Try to initialize DefaultCtGrpVal_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultCtGrpVal_AbsChange;
	//	Try to initialize DefaultCtGrpVal_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultCtGrpVal_AbsChange;
	//	And try to extract DefaultCtGrpVal_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultCtGrpVal_AbsChange;

	//	Try to initialize DefaultZeroDGrpVal_AbsChange from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  defaultZeroDGrpVal_AbsChange;
	//	Try to initialize DefaultZeroDGrpVal_AbsChange from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  defaultZeroDGrpVal_AbsChange;
	//	And try to extract DefaultZeroDGrpVal_AbsChange value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  defaultZeroDGrpVal_AbsChange;

	//	Try to initialize CTThreadLoop_SleepTime from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  cTThreadLoop_SleepTime;
	//	Try to initialize CTThreadLoop_SleepTime from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  cTThreadLoop_SleepTime;
	//	And try to extract CTThreadLoop_SleepTime value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  cTThreadLoop_SleepTime;

	//	Try to initialize ZeroDThreadLoop_SleepTime from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  zeroDThreadLoop_SleepTime;
	//	Try to initialize ZeroDThreadLoop_SleepTime from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  zeroDThreadLoop_SleepTime;
	//	And try to extract ZeroDThreadLoop_SleepTime value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  zeroDThreadLoop_SleepTime;

	//	Try to initialize TmpElement_MaxInactTime from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  tmpElement_MaxInactTime;
	//	Try to initialize TmpElement_MaxInactTime from default device value
	def_prop = ds_class->get_default_device_property(dev_prop[i].name);
	if (def_prop.is_empty()==false)	def_prop  >>  tmpElement_MaxInactTime;
	//	And try to extract TmpElement_MaxInactTime value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  tmpElement_MaxInactTime;



	//	End of Automatic code generation
	//------------------------------------------------------------------

}
//+----------------------------------------------------------------------------
//
// method : 		Pool::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void Pool::always_executed_hook()
{	
	if (proxy_created == false)
	{
		create_proxies();
		proxy_created = true;
	}	
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_attr_hardware
// 
// description : 	Hardware acquisition for attributes.
//
//-----------------------------------------------------------------------------
void Pool::read_attr_hardware(vector<long> &attr_list)
{
	DEBUG_STREAM << "Pool::read_attr_hardware(vector<long> &attr_list) entering... "<< endl;
	//	Add your own code here
}
//+----------------------------------------------------------------------------
//
// method : 		Pool::read_PseudoCounterList
// 
// description : 	Extract real attribute values for PseudoCounterList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_PseudoCounterList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_PseudoCounterList(Tango::Attribute &attr) entering... "<< endl;
	
	list<PseudoCounterPool>::iterator ite;
	long l = 0;
	for (ite = pseudo_co_list.begin();ite != pseudo_co_list.end();++ite,++l)
	{
		pseudo_counter_name_list[l] = const_cast<char *>(ite->user_full_name_extra.c_str());
	}
	
	attr.set_value(pseudo_counter_name_list,pseudo_co_list.size());	
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_ComChannelList
// 
// description : 	Extract real attribute values for ComChannelList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_ComChannelList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_ComChannelList(Tango::Attribute &attr) entering... "<< endl;
	
	list<CommunicationChannelPool>::iterator ite;
	long l = 0;
	for (ite = com_channel_list.begin();ite != com_channel_list.end();++ite,++l)
	{
		com_channel_name_list[l] = const_cast<char *>(ite->user_full_name.c_str());
	}
	
	attr.set_value(com_channel_name_list,com_channel_list.size());	
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_MeasurementGroupList
// 
// description : 	Extract real attribute values for MeasurementGroupList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_MeasurementGroupList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_MeasurementGroupList(Tango::Attribute &attr) entering... "<< endl;
	
	list<MeasurementGroupPool>::iterator ite;
	long l = 0;
	for (ite = measurement_group_list.begin();ite != measurement_group_list.end();++ite,++l)
	{
		measurement_group_name_list[l] = const_cast<char *>(ite->user_full_name.c_str());
	}
	
	attr.set_value(measurement_group_name_list,measurement_group_list.size());	
}


//+----------------------------------------------------------------------------
//
// method : 		Pool::read_ExpChannelList
// 
// description : 	Extract real attribute values for ExpChannelList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_ExpChannelList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_ExpChannelList(Tango::Attribute &attr) entering... "<< endl;
	
	list<CTExpChannelPool>::iterator ct_ite = ct_list.begin();
	long l = 0;
	for (;ct_ite != ct_list.end();++ct_ite,++l)
	{
		expch_name_list[l] = const_cast<char *>(ct_ite->user_full_name.c_str());
	}
	
	list<ZeroDExpChannelPool>::iterator zero_ite = zerod_list.begin();
	for (;zero_ite != zerod_list.end();++zero_ite,++l)
	{
		expch_name_list[l] = const_cast<char *>(zero_ite->user_full_name.c_str());
	}

	list<PseudoCounterPool>::iterator pc_ite = pseudo_co_list.begin();
	for (;pc_ite != pseudo_co_list.end();++pc_ite,++l)
	{
		expch_name_list[l] = const_cast<char *>(pc_ite->user_full_name.c_str());
	}
	
	attr.set_value(expch_name_list,l);
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_ControllerClassList
// 
// description : 	Extract real attribute values for ControllerClassList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_ControllerClassList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_ControllerClassList(Tango::Attribute &attr) entering... "<< endl;
	
	PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
	
	cpp_ctrl_classes.clear();
	py_ctrl_classes.clear();
	
//
// First add to the list all the possible Controller class(es) for the
// controller we already have in memory
//

	vector<string> tmp_ctrl_list;
	vector<string> ctrl_type_list;
	list<ControllerPool>::iterator ctrl_ite;
	for (ctrl_ite = ctrl_list.begin();ctrl_ite != ctrl_list.end();++ctrl_ite)
	{
		tmp_ctrl_list.clear();
		ctrl_type_list.clear();
		if (ctrl_ite->ctrl_fica_built == true)
		{
			string ctrl_full_f_name = ctrl_ite->get_path() + '/' + ctrl_ite->get_f_name();
			long nb_class = ctrl_ite->get_ctrl_file().get_classes(tmp_ctrl_list,ctrl_type_list);
			Language lang = ctrl_ite->get_language();

			for (long loop = 0;loop < nb_class;loop++)
			{
				string tmp_str("Type: ");
				tmp_str += ctrl_type_list[loop];
				tmp_str += " - Class: ";
				tmp_str = tmp_str + tmp_ctrl_list[loop].substr(tmp_ctrl_list[loop].find('.') + 1);
				tmp_str = tmp_str + " - File: " + ctrl_full_f_name;

//
// If it a file and a class that that we already have, forget it
//
				
				bool leave_loop = false;
				if (lang == CPP)
				{
					unsigned long nb = cpp_ctrl_classes.size();
					for (unsigned long ll = 0;ll < nb;ll++)
					{
						if (tmp_str == cpp_ctrl_classes[ll])
						{
							leave_loop = true;
							break;
						}
					}
				
					if (leave_loop == true)
						continue;
					else
						cpp_ctrl_classes.push_back(tmp_str);
				}
				else
				{
					
//
// If it a file that we already have, forget it
//
					unsigned long nb = py_ctrl_classes.size();
					for (unsigned long ll = 0;ll < nb;ll++)
					{
						if (tmp_str == py_ctrl_classes[ll])
						{
							leave_loop = true;
							break;
						}
					}
				
					if (leave_loop == true)
						continue;
					else
						py_ctrl_classes.push_back(tmp_str);
				}
			}
		}
	}
		
	vector<string> py_files;
	vector<string> cpp_files;
	vector<string>::iterator path_ite;
	
//
// Get all the directories in the PoolPath
//

	vector<string> &paths = get_pool_path();

//
// Get list of python files in each directories
//

	for(path_ite = paths.begin(); path_ite != paths.end(); path_ite++)
		get_files_with_extension(*path_ite,".py",py_files);

//
// Get list of .la files in each directories
//
	
	for (path_ite = paths.begin();path_ite != paths.end(); path_ite++)
		get_files_with_extension(*path_ite,".la",cpp_files);

//
// Check which of the .la files are valid controller
//
		
	vector<string>::iterator ite;
	
	for (ite = cpp_files.begin();ite != cpp_files.end();ite++)
	{
		try
		{
			
//
// Is it a file we already have in the list ?
// If yes, forget it
//

			bool leave_loop = false;
			unsigned long nb = cpp_ctrl_classes.size();
			for (unsigned long ll = 0;ll < nb;ll++)
			{
				string tmp_file_name = cpp_ctrl_classes[ll].substr(cpp_ctrl_classes[ll].find("File: ") + 6);
				if (tmp_file_name == *ite)
				{
					leave_loop = true;
					break;
				}
			}
			
			if (leave_loop == true)
				continue;

//
// Build the ctrl as an undefined type ctrl
//
			CppUndefCtrlFile tmp_undef_ctrl(*ite,cl_ptr,this);
			
			tmp_ctrl_list.clear();
			ctrl_type_list.clear();
			bool exit_search = false;
			long class_count;

//
// Is it a motor controller?
//

			try
			{
				class_count = 0;
				CppMotCtrlFile tmp_mot_ctrl(tmp_undef_ctrl,cl_ptr,this);			
				class_count = tmp_mot_ctrl.get_classes(tmp_ctrl_list,ctrl_type_list);
				DEBUG_STREAM << "Found " << class_count << " Motor Controller Classes in " << *ite << endl;
				if (class_count != 0)
					exit_search = true;
			}
			catch (Tango::DevFailed &e)
			{
				DEBUG_STREAM << "Cpp file " << *ite << " is not a valid motor controller" << endl;
			}

//
// Is it a pseudo motor controller?
//
			if (exit_search == false)
			{
				try
				{						
					class_count = 0;
					CppPseudoMotCtrlFile tmp_mot_ctrl(tmp_undef_ctrl,cl_ptr,this);			
					class_count = tmp_mot_ctrl.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Pseudo Motor Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Cpp file " << *ite << " is not a valid pseudo motor controller" << endl;
				}
			}
			
//
// Is it a pseudo counter controller?
//
			if (exit_search == false)
			{
				try
				{
					class_count = 0;
					CppPseudoCoCtrlFile tmp_counter_ctrl(tmp_undef_ctrl,cl_ptr,this);			
					class_count = tmp_counter_ctrl.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Pseudo Counter Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Cpp file " << *ite << " is not a valid pseudo counter controller" << endl;
				}
			}
						
//
// Is it a Counter Timer ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					CppCoTiCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " CounterTimer Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Cpp file " << *ite << " is not a valid CounterTimer controller" << endl;
				}
			}
			
//
// Is it a Zero D Exp Channel ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					CppZeroDCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " ZeroDExpChannel Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Cpp file " << *ite << " is not a valid ZeroDExpChannel controller" << endl;
				}
			}
			
//
// Is it a Communication Channel ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					CppComCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Communication Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Cpp file " << *ite << " is not a valid Communication controller" << endl;
					throw;
				}
			}
			
			
			for (long loop = 0;loop < class_count;loop++)
			{
				string tmp_str("Type: ");
				tmp_str = tmp_str + ctrl_type_list[loop] + " - Class: ";
				tmp_str = tmp_str + tmp_ctrl_list[loop].substr(tmp_ctrl_list[loop].find('.') + 1);
				tmp_str = tmp_str + " - File: " + *ite;
				cpp_ctrl_classes.push_back(tmp_str);
			}
			
		}
		catch (Tango::DevFailed &e)
		{
			string reason = e.errors[0].reason.in();
			if (reason == "Pool_ControllerNotFound")
			{
				WARN_STREAM << "\tReason: " << e.errors[0].reason << endl;
				WARN_STREAM << "\tDescription: " << e.errors[0].desc << endl;
			}
			else
			{
				DEBUG_STREAM << "\tReason: " << e.errors[0].reason << endl;
				DEBUG_STREAM << "\tDescription: " << e.errors[0].desc << endl;
			}
			
		}
		
	}

//
// Check which of the .py files are valid controller
//

	for(ite = py_files.begin(); ite != py_files.end(); ite++)
	{
		try
		{
			
//
// Is it a file we already have in the list ?
// If yes, forget it
//

			bool leave_loop = false;
			unsigned long nb = py_ctrl_classes.size();
			for (unsigned long ll = 0;ll < nb;ll++)
			{
				string tmp_file_name = py_ctrl_classes[ll].substr(py_ctrl_classes[ll].find("File: ") + 6);
				if (tmp_file_name == *ite)
				{
					leave_loop = true;
					break;
				}
			}
			
			if (leave_loop == true)
				continue;

//
// Build the ctrl as an undefined type ctrl
//

			PyUndefCtrlFile tmp_undef_ctrl(*ite,cl_ptr,this);
							
			tmp_ctrl_list.clear();
			ctrl_type_list.clear();
			long class_count;
			bool exit_search = false;

//
// Is it a motor controller?
//

			try
			{	
				class_count = 0;		
				PyMotCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
				class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
				DEBUG_STREAM << "Found " << class_count << " Motor Controller Classes in " << *ite << endl;
				if (class_count != 0)
					exit_search = true;
			}
			catch (Tango::DevFailed &e)
			{
				DEBUG_STREAM << "Python file " << *ite << " is not a valid motor controller" << endl;
			}

//
// Is it a pseudo motor controller?
//

			if (exit_search == false)
			{
				try
				{	
					class_count = 0;		
					PyPseudoMotCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Pseudo Motor Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Python file " << *ite << " is not a valid pseudo motor controller" << endl;
				}
			}

//
// Is it a pseudo counter controller?
//

			if (exit_search == false)
			{
				try
				{	
					class_count = 0;		
					PyPseudoCoCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Pseudo Counter Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Python file " << *ite << " is not a valid pseudo counter controller" << endl;
				}
			}
									
//
// Is it a Counter Timer ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					PyCoTiCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " CounterTimer Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Python file " << *ite << " is not a valid CounterTimer controller" << endl;
				}
			}
			
//
// Is it a Zero D Exp Channel ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					PyZeroDCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " ZeroDExpChannel Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Python file " << *ite << " is not a valid ZeroDExpChannel controller" << endl;
				}
			}

//
// Is it a Communication Channel ctrl?
//

			if (exit_search == false)
			{			
				try
				{
					class_count = 0;
					PyComCtrlFile tmp_ctrl_file(tmp_undef_ctrl,cl_ptr,this);
					class_count = tmp_ctrl_file.get_classes(tmp_ctrl_list,ctrl_type_list);
					DEBUG_STREAM << "Found " << class_count << " Communication Controller Classes in " << *ite << endl;
					if (class_count != 0)
						exit_search = true;
				}
				catch (Tango::DevFailed &e)
				{
					DEBUG_STREAM << "Python file " << *ite << " is not a valid Communication controller" << endl;
					throw;
				}
			}
			
			for (long loop = 0;loop < class_count;loop++)
			{
				string tmp_str("Type: ");
				tmp_str = tmp_str + ctrl_type_list[loop] + " - Class: ";
				tmp_str = tmp_str + tmp_ctrl_list[loop].substr(tmp_ctrl_list[loop].find('.') + 1);
				tmp_str = tmp_str + " - File: " + *ite;
				py_ctrl_classes.push_back(tmp_str);
			}
		}
		catch (Tango::DevFailed &e)
		{
			DEBUG_STREAM << "Python file " << *ite << " is not a valid controller" << endl;
		}
	}

//
// Fill in the array used to return attribute value
// after merging the 2 classes et in the Cpp vector
//
	
	cpp_ctrl_classes.insert(cpp_ctrl_classes.end(),py_ctrl_classes.begin(),py_ctrl_classes.end());
	sort(cpp_ctrl_classes.begin(),cpp_ctrl_classes.end());
	
	unsigned long nb_classes = cpp_ctrl_classes.size();
	for(unsigned long i = 0; i < nb_classes; i++)
	{
		ctrl_class_list[i] = const_cast<char *>(cpp_ctrl_classes[i].c_str());
		DEBUG_STREAM << "Adding class " << ctrl_class_list[i] << endl;
	}
	
	attr.set_value(ctrl_class_list, nb_classes);
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_PseudoMotorList
// 
// description : 	Extract real attribute values for PseudoMotorList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_PseudoMotorList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_PseudoMotorList(Tango::Attribute &attr) entering... "<< endl;
	
	list<PseudoMotorPool>::iterator ite;
	long l = 0;
	for (ite = pseudo_mot_list.begin();ite != pseudo_mot_list.end();++ite,++l)
	{
		pseudo_motor_name_list[l] = const_cast<char *>(ite->user_full_name_extra.c_str());
	}
	
	attr.set_value(pseudo_motor_name_list,pseudo_mot_list.size());	
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_MotorGroupList
// 
// description : 	Extract real attribute values for MotorGroupList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_MotorGroupList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_MotorGroupList(Tango::Attribute &attr) entering... "<< endl;
	
	list<MotorGroupPool>::iterator ite;
	long l = 0;
	for (ite = mot_group_list.begin();ite != mot_group_list.end();++ite,++l)
	{
		motor_group_name_list[l] = const_cast<char *>(ite->user_full_name.c_str());
	}
	
	attr.set_value(motor_group_name_list,mot_group_list.size());
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_SimulationMode
// 
// description : 	Extract real attribute values for SimulationMode acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_SimulationMode(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_SimulationMode(Tango::Attribute &attr) entering... "<< endl;
	
	attr.set_value(attr_SimulationMode_read);
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::write_SimulationMode
// 
// description : 	Write SimulationMode attribute values to hardware.
//
//-----------------------------------------------------------------------------
void Pool::write_SimulationMode(Tango::WAttribute &attr)
{
	DEBUG_STREAM << "Pool::write_SimulationMode(Tango::WAttribute &attr) entering... "<< endl;
	bool old_simu = attr_SimulationMode_write;
	attr.get_write_value(attr_SimulationMode_write);
	DEBUG_STREAM << "Wanted simulationMode = " << attr_SimulationMode_write << endl;
	
//
// Do nothing if the new value is the same one than the other
//

	if (old_simu == attr_SimulationMode_write)
		return;
	
//
// Send this new value to all Motors devices
//

	list<MotorPool>::iterator ite;
	for (ite = mot_list.begin();ite != mot_list.end();++ite)
	{
		Tango::AutoTangoMonitor sync(ite->motor);
		bool allo = ite->motor->is_SimulationMode_allowed(Tango::WRITE_REQ);
		if (allo == true)
		{
			ite->motor->simulation(attr_SimulationMode_write);
			if ((attr_SimulationMode_write == false) && (old_simu == true))
				ite->motor->restore_att_values();
			else
				ite->motor->save_att_values();
		}
		else
		{
			attr_SimulationMode_write = old_simu;
			list<MotorPool>::iterator ite_except;
			for (ite_except = mot_list.begin();ite_except != ite;++ite_except)
			{
				Tango::AutoTangoMonitor sy(ite_except->motor);
				ite_except->motor->simulation(attr_SimulationMode_write);
			}
			
			TangoSys_OMemStream o;
			o << "It is actually not allowed to switch motor ";
			o << ite->name << " (" << ite->obj_tango_name << ") ";
			o << "to simulation mode (Motor moving)" << ends;
	
			Tango::Except::throw_exception((const char *)"Pool_MotorMoving",o.str(),
			        		(const char *)"Pool::write_SimulationMode");
		}
	}

//
// Send the new value also to CT
//
	
	list<CTExpChannelPool>::iterator ite_ct;
	for (ite_ct = ct_list.begin();ite_ct != ct_list.end();++ite_ct)
	{
		Tango::AutoTangoMonitor sync(ite_ct->ct_channel);
		bool allo = ite_ct->ct_channel->is_SimulationMode_allowed(Tango::WRITE_REQ);
		if (allo == true)
		{
			ite_ct->ct_channel->simulation(attr_SimulationMode_write);
		}
		else
		{
			attr_SimulationMode_write = old_simu;
			list<CTExpChannelPool>::iterator ite_except;
			for (ite_except = ct_list.begin();ite_except != ite_ct;++ite_except)
			{
				Tango::AutoTangoMonitor sy(ite_except->ct_channel);
				ite_except->ct_channel->simulation(attr_SimulationMode_write);
			}
			
			TangoSys_OMemStream o;
			o << "It is actually not allowed to switch Counter/Timer ";
			o << ite_ct->name << " (" << ite_ct->obj_tango_name << ") ";
			o << "to simulation mode" << ends;
	
			Tango::Except::throw_exception((const char *)"Pool_MotorMoving",o.str(),
			        		(const char *)"Pool::write_SimulationMode");
		}
	}
	
//
// Send the new value also to ZeroDExpChannel
//
	
	list<ZeroDExpChannelPool>::iterator ite_zerod;
	for (ite_zerod = zerod_list.begin();ite_zerod != zerod_list.end();++ite_zerod)
	{
		Tango::AutoTangoMonitor sync(ite_zerod->zerod_channel);
		bool allo = ite_zerod->zerod_channel->is_SimulationMode_allowed(Tango::WRITE_REQ);
		if (allo == true)
		{
			ite_zerod->zerod_channel->simulation(attr_SimulationMode_write);
			if ((attr_SimulationMode_write == false) && (old_simu == true))
				ite_zerod->zerod_channel->restore_att_values();
			else
				ite_zerod->zerod_channel->save_att_values();
		}
		else
		{
			attr_SimulationMode_write = old_simu;
			list<ZeroDExpChannelPool>::iterator ite_except;
			for (ite_except = zerod_list.begin();ite_except != ite_zerod;++ite_except)
			{
				Tango::AutoTangoMonitor sy(ite_except->zerod_channel);
				ite_except->zerod_channel->simulation(attr_SimulationMode_write);
			}
			
			TangoSys_OMemStream o;
			o << "It is actually not allowed to switch ZerodExpChannel ";
			o << ite_zerod->name << " (" << ite_zerod->obj_tango_name << ") ";
			o << "to simulation mode" << ends;
	
			Tango::Except::throw_exception((const char *)"Pool_MotorMoving",o.str(),
			        		(const char *)"Pool::write_SimulationMode");
		}
	}

//
// Send the new value also to Communication channels
//
	
	list<CommunicationChannelPool>::iterator ite_comch;
	for (ite_comch = com_channel_list.begin();ite_comch != com_channel_list.end();++ite_comch)
	{
		Tango::AutoTangoMonitor sync(ite_comch->com_channel);
		bool allo = ite_comch->com_channel->is_SimulationMode_allowed(Tango::WRITE_REQ);
		if (allo == true)
		{
			ite_comch->com_channel->simulation(attr_SimulationMode_write);
		}
		else
		{
			attr_SimulationMode_write = old_simu;
			list<CommunicationChannelPool>::iterator ite_except;
			for (ite_except = com_channel_list.begin();ite_except != ite_comch;++ite_except)
			{
				Tango::AutoTangoMonitor sy(ite_except->com_channel);
				ite_except->com_channel->simulation(attr_SimulationMode_write);
			}
			
			TangoSys_OMemStream o;
			o << "It is actually not allowed to switch CommunicationChannel ";
			o << ite_comch->name << " (" << ite_comch->obj_tango_name << ") ";
			o << "to simulation mode" << ends;
	
			Tango::Except::throw_exception((const char *)"Pool_ComChMoving",o.str(),
			        		(const char *)"Pool::write_SimulationMode");
		}
	}
	
//
// Push a change_event to inform client listenning on event
//

	read_SimulationMode(attr);
	attr.fire_change_event();
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_MotorList
// 
// description : 	Extract real attribute values for MotorList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_MotorList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_MotorList(Tango::Attribute &attr) entering... "<< endl;

	list<MotorPool>::iterator m_ite = mot_list.begin();
	long l = 0;
	for (;m_ite != mot_list.end();++m_ite,++l)
	{
		motor_name_list[l] = const_cast<char *>(m_ite->user_full_name.c_str());
	}

	list<PseudoMotorPool>::iterator pm_ite = pseudo_mot_list.begin();
	for (;pm_ite != pseudo_mot_list.end();++pm_ite,++l)
	{
		motor_name_list[l] = const_cast<char *>(pm_ite->user_full_name_extra.c_str());
	}
	
	attr.set_value(motor_name_list,l);
}

//+----------------------------------------------------------------------------
//
// method : 		Pool::read_ControllerList
// 
// description : 	Extract real attribute values for ControllerList acquisition result.
//
//-----------------------------------------------------------------------------
void Pool::read_ControllerList(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Pool::read_ControllerList(Tango::Attribute &attr) entering... "<< endl;

	list<ControllerPool>::iterator ite;
	long l = 0;	
	for (ite = ctrl_list.begin();ite != ctrl_list.end();++ite,l++)
	{
		ctrl_name_list[l] = const_cast<char *>(ite->user_full_name.c_str());
	}
	
	attr.set_value(ctrl_name_list,ctrl_list.size());
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_controller
 *
 *	description:	method to execute "CreateController"
 *	This command creates a controller object in the pool. Each controller code has to be defined as a shared library
 *	and the user LD_LIBRARY_PATH environment variable has to be correctly set to find the controller shared library.
 *	Each controller shared library has to have a C function called "_create_<Controller name>" to create a controller object.
 *	This command has four arguments which are :
 *	
 *	1 - The controller device type (Motor, Ccd,....)
 *	2 - The controller shared library name (Cpp or Python)
 *	3 - The controller name
 *	4 - The controller instance name
 *	
 *	Each controller is represented in the pool device by a separate object. If you have several instance of the same controller
 *	within a pool, the instance name is used to retrieve the correct instance.
 *
 * @param	argin	in[0] = Controller dev type, in[1] = Controller lib, in[2] = Controller name, in[3] = Instance name
 *
 */
//+------------------------------------------------------------------
void Pool::create_controller(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "Pool::create_controller(): entering... !" << endl;
  	//	Add your own code to control device here
	add_ctrl(argin);
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_motor
 *
 *	description:	method to execute "CreateMotor"
 *	This command creates a new motor in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The motor number within the controller
 *	3 - The motor name. The motor name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"motor/controller_instance_name/motor_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created motors are automatically re-created at pool device startup time.
 *
 * @param	argin	long[0] = Motor number in Ctrl, string[0] = Motor name, string[1] = Controller instance name
 *
 */
//+------------------------------------------------------------------
void Pool::create_motor(const Tango::DevVarLongStringArray *argin)
{
  	DEBUG_STREAM << "Pool::create_motor(): entering... !" << endl;
  
  	//	Add your own code to control device here
  	
  	if ((argin->lvalue.length() != 1) || (argin->svalue.length() != 2))
  	{
  		Tango::Except::throw_exception(
  			(const char *)"Pool_WrongArgumentNumber",
  			(const char *)"Wrong number of argument for command CreateMotor. Needs 1 long and 2 strings",
  		    (const char *)"Pool::create_controller");
  	}		
  	
  	Tango::DevLong motor_idx = argin->lvalue[0];
  	string ctrl_inst_name(argin->svalue[1]);
  	string mot_name(argin->svalue[0]);
  	ControllerPool &mot_ctrl = get_ctrl_from_inst_name(ctrl_inst_name);
  	long ctrl_id = mot_ctrl.id;
  	
  	DEBUG_STREAM << "Controller ID = " << ctrl_id << endl;
  	DEBUG_STREAM << "Motor index = " << motor_idx << endl;
  	DEBUG_STREAM << "Motor name = " << mot_name << endl;
  
//
// Check if the controller has been successfully constructed
//
  
  	if (mot_ctrl.ctrl == NULL)
  	{
  		Tango::Except::throw_exception(
  			(const char *)"Pool_WrongControllerId",
  			(const char *)"Can't create a motor on a non-responding controller",
  			(const char *)"Pool::create_motor");
  	}
  
//
// Check if we don't have already  enough motor
//
  
  	DEBUG_STREAM << "Checking motor number" << endl;
  	
  	if (mot_list.size() == MAX_MOTOR)
  	{
  		Tango::Except::throw_exception(
  			(const char *)"Pool_TooManyMotor",
  			(const char *)"Too many motor in your pool",
  			(const char *)"Pool::create_motor");
  	}
  	
//
// Check that the controller still have some motors available
//
  
  	if (mot_ctrl.nb_dev == mot_ctrl.MaxDevice)
  	{
  		TangoSys_OMemStream o;
  		o << "Max number of motor reached (" << mot_ctrl.MaxDevice << ")" 
  		  << ends;
  		
  		Tango::Except::throw_exception(
  				(const char *)"Pool_MaxNbMotorInCtrl",o.str(),
  				(const char *)"Pool::create_motor()");
  	}
  		
//
// Build Tango device name
//
  
  	stringstream s;
  	s << motor_idx;
  
  	string tg_dev_name("motor");
  	string tmp_str(mot_ctrl.name);
  	tmp_str.erase(0,tmp_str.find('/'));
  	tg_dev_name = tg_dev_name + tmp_str + '/' + s.str();	
  	
  	DEBUG_STREAM << "Tango device name = " << tg_dev_name << endl;
  			
//
// Check if this device is already defined in database
// Check by device alias and by Tango device name
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	Tango::Database *db = tg->get_database();
  	
  	Tango::DbDevImportInfo my_device_import;
  	bool device_exist = false;
  	bool by_alias = false;	
  
  	try
  	{
  		my_device_import = db->import_device(mot_name);
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
  			my_device_import = db->import_device(tg_dev_name);
  			device_exist = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
  			{
  				device_exist = true;
  			}
  		}
  	}
  	
  	if (device_exist == true)
  	{
  		TangoSys_OMemStream o;
  		if (by_alias == false)
  			o << "Motor " << motor_idx << " on controller " 
  			  << ctrl_inst_name << " already defined" << ends;
  		else
  			o << "Motor " << mot_name << " is already defined" << ends;
  
  		Tango::Except::throw_exception(
  				(const char *)"Pool_WrongMotorName",o.str(),
  				(const char *)"Pool::create_motor");
  	}
  
//
// If the device is not defined in database, create it in database, set its 
// alias, define the property used to store its ID (called motor_id) and create
// the default abs_change property for the Position attribute
//
  
  	if (device_exist == false)
  	{
  		DEBUG_STREAM << "Trying to create device entry in database" << endl;
  		
      	try
  		{
  			Tango::DbDevInfo my_device_info;
      		my_device_info.name = tg_dev_name.c_str();
      		my_device_info._class = "Motor";
      		my_device_info.server = tg->get_ds_name().c_str();
  			
      		db->add_device(my_device_info);
  		
  			db->put_device_alias(tg_dev_name,mot_name);
  
  			Tango::DbDatum id(MOTOR_ID_PROP);
  			Tango::DbData db_data;
  			mot_id++;
  			id  << mot_id;
  			db_data.push_back(id);
  			db->put_device_property(tg_dev_name.c_str(),db_data);

			Tango::DbDatum pos("Position"),abs_ch("abs_change");
			db_data.clear();
			pos << (long)1;
			abs_ch << defaultMotPos_AbsChange;
			db_data.push_back(pos);
			db_data.push_back(abs_ch);
			db->put_device_attribute_property(tg_dev_name.c_str(),db_data);
  						
  			DEBUG_STREAM << "Device created in database (with alias)" << endl;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			DEBUG_STREAM << "Gasp an exception........" << endl;
  			TangoSys_OMemStream o;
  			o << "Can't create motor " << mot_name << " in database" << ends;
  			
  			Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateMotor",o.str(),
  					(const char *)"Pool::create_motor");
  		}
  	
//
// Find the Tango Motor class and create the motor
//
  
  		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
  		for (unsigned long i = 0;i < cl_list->size();i++)
  		{
  			if ((*cl_list)[i]->get_name() == "Motor")
  			{
  				try
  				{
  					Tango::DevVarStringArray na;
  					na.length(1);
  					na[0] = tg_dev_name.c_str();
  					(*cl_list)[i]->device_factory(&na);
  					break;
  				}
  				catch (Tango::DevFailed &e)
  				{
  					DEBUG_STREAM << "Exception while trying to create Motor device" << endl;
 
//
// Check if this motor has already been added into pool structures
// If yes, remove it from pool structures
//

					try
					{
						MotorPool &failed_mot = get_motor_from_name(mot_name);
						remove_motor(failed_mot.motor);
					}
					catch (Tango::DevFailed &ee) {}

//
// The delete_device will also delete device property(ies)
//
  					
  					db->delete_device(tg_dev_name);
  					
  					TangoSys_OMemStream o;
  					o << "Can't create motor device " << mot_name << ends;
  			
  					Tango::Except::re_throw_exception(e,
  							(const char *)"Pool_CantCreateMotor",o.str(),
  							(const char *)"Pool::create_motor");
  				}
  			}
  		}
  
//
// Create a Tango device proxy on the newly created motor
// and set its connection to automatic re-connection
//
  
  		Tango::DeviceProxy *tmp_dev = 
  			new Tango::DeviceProxy(mot_list.back().obj_tango_name);
  		tmp_dev->set_transparency_reconnection(true);
  		mot_list.back().obj_proxy = tmp_dev;
  	}

//
// Inform ghost group that there is a new motor
//
	
	MotorGroup_ns::MotorGroup *ghost_ptr = get_ghost_motor_group_ptr();
	{
		Tango::AutoTangoMonitor atm(ghost_ptr);
		ghost_ptr->add_motor_to_ghost_group(mot_list.back().id);
	}
	
//
// Push a change event for client listening on event
//

	Tango::Attribute &ml = dev_attr->get_attr_by_name("MotorList");
	read_MotorList(ml);
	ml.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_motor
 *
 *	description:	method to execute "DeleteMotor"
 *	Delete a motor from its name
 *	Once a motor is deleted, it is not available any more. All its information have been
 *	removed from the database
 *
 * @param	argin	Motor name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_motor(Tango::DevString argin)
{
  	DEBUG_STREAM << "Pool::delete_motor(): entering... with argin = " << argin << endl;
	//	Add your own code to control device here
  
//
// Find motor in motor list
//

	string user_name(argin);
	MotorPool &mot_to_del =  get_motor_from_name(user_name);
  		
  	DEBUG_STREAM << "Motor found" << endl;
//
// Check that the motor is not moving
//
  
  	if (mot_to_del.obj_proxy->state() == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete motor with name " << argin;
  		o << ". It is actually moving." << ends;
  				
  		Tango::Except::throw_exception((const char *)"Pool_CantDeleteMotor",o.str(),
  								 (const char *)"Pool::delete_motor");
  	}
  
//
// Check that the motor is not member of a motor group
//
  
  	list<MotorGroupPool>::iterator grp_ite;
  	for (grp_ite = mot_group_list.begin();grp_ite != mot_group_list.end();++grp_ite)
  	{
  		if (find(grp_ite->mot_ids.begin(),grp_ite->mot_ids.end(),mot_to_del.id) != grp_ite->mot_ids.end())
  		{
  			TangoSys_OMemStream o;
  			o << "Can't delete motor with name " << argin;
  			o << ". It is actually member of group " << grp_ite->name << ends;
  					
  			Tango::Except::throw_exception((const char *)"Pool_CantDeleteMotor",o.str(),
  									 (const char *)"Pool::delete_motor");
  		}
  	}
  	
//
// If this command is used after a Abort command, check that the pool
// internal thread has finished its job
//
  		
	long wait_ctr = 0;
	struct timespec wait,rem;
		
	wait.tv_sec = 0;
	wait.tv_nsec = 10000000;
	
	while (mot_to_del.motor->get_mov_th_id() != 0)
	{
		nanosleep(&wait,&rem);
		wait_ctr++;
		
		if (wait_ctr == 3)
		{
  			TangoSys_OMemStream o;
  			o << "Can't delete motor with name " << argin;
  			o << ". The pool internal thread still uses it" << ends;
  				
  			Tango::Except::throw_exception((const char *)"Pool_CantDeleteMotor",o.str(),
  								 			(const char *)"Pool::delete_motor");
		}
	}
  									
//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
  	Tango::Util *tg = Tango::Util::instance();
  	try
  	{
  		tg->get_database()->delete_device(mot_to_del.obj_tango_name);
  			
//
// Delete motor device from server but first find its Tango xxxClass instance
//
  
  		Tango::DeviceClass *dc = mot_to_del.motor->get_device_class();
  		dc->device_destroyer(mot_to_del.obj_tango_name); 	  				
  	}
  	catch (Tango::DevFailed &e)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete motor with name " << argin << ends;
  				
  		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeleteMotor",o.str(),
  								 (const char *)"Pool::delete_motor");
  	}
  		
//
// Before returning, send a change event for client listenning
// on event
//
  
  	Tango::Attribute &ml = dev_attr->get_attr_by_name("MotorList");
  	read_MotorList(ml);
  	ml.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::dev_state
 *
 *	description:	method to execute "State"
 *	This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *
 * @return	State Code
 *
 */
//+------------------------------------------------------------------
Tango::DevState Pool::dev_state()
{
//	Tango::DevState	argout = DeviceImpl::dev_state();
	DEBUG_STREAM << "Pool::dev_state(): entering... !" << endl;

	//	Add your own code to control device here

//
// If the moving_state_requested is set to true, this means that we have received
// an order to die while a motar was moving. In this case, set state to Moving
//

	if (moving_state_requested == true)
		set_state(Tango::MOVING);
	else
	{
		
//
// Check each controller state
//

		bool all_ok = true;
		
		string &_status = get_status();
		_status.clear();
		
		list<ControllerPool>::iterator ctrlPool_ite = ctrl_list.begin();
		for (;ctrlPool_ite != ctrl_list.end(); ++ctrlPool_ite)
		{
			if (ctrlPool_ite->ctrl == NULL)
			{
				set_state(Tango::ALARM);
				all_ok = false;
			}
		}

//
// If all controller are OK, set state to ON
// and status to default one
//
	
		if (all_ok == true)
		{
			set_state(Tango::ON);
			_status = StatusNotSet;
		}
	}
		
	Tango::DevState argout = DeviceImpl::dev_state();
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::reload_controller_code
 *
 *	description:	method to execute "ReloadControllerCode"
 *	This command reloads controller code in the device server process. For C++ controller, it will
 *	unload and load the controller shared library and for Python controller, it will re-load the controller module.
 *
 * @param	argin	Controller file name
 *
 */
//+------------------------------------------------------------------
void Pool::reload_controller_code(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::reload_controller_code(): entering... !" << endl;
	//	Add your own code to control device here

//
//Check that we receive only a file name and not a path
//

	string in_file(argin);
	if (in_file.find('/') != string::npos)
	{
		TangoSys_OMemStream o;
		o << in_file << " is not a valid file. Only file name is valid ";
		o << "not path" << ends;
		
		Tango::Except::throw_exception(
				(const char *)"Pool_FileUnsupportedType",o.str(),
				(const char *)"Pool::unload_controller_code");
	}
	
	
		
//
// Check which controller we have with this file name
//

	vector<Pool::CtrlBkp> ctrls;
	CtrlType ctrl_dev_type;
	
	list<ControllerPool>::iterator ite_ctrl;
	list<ControllerPool>::iterator first_ctrl = ctrl_list.begin();
	
	for (ite_ctrl = ctrl_list.begin();ite_ctrl != ctrl_list.end();++ite_ctrl)
	{
		string &f_name = f_name_from_db_prop(ite_ctrl->name);
		if (f_name == in_file)
		{								
			Pool::CtrlBkp cb;
			cb.dist = distance(first_ctrl,ite_ctrl);
			cb.idx = ite_ctrl->id;
			cb.good_old_ctrl = ite_ctrl->ctrl;
			cb.wrong_fica = false;
			cb.fica_ite = ite_ctrl->ite_ctrl_fica;
			cb.old_fica_built = ite_ctrl->ctrl_fica_built;
			cb.fica_already_reloaded = false;
			ctrls.push_back(cb);
			string con_name = ite_ctrl->name.substr(ite_ctrl->name.find('.') + 1);
			ctrl_dev_type = dev_type_from_db_prop(con_name);
		}
	}

	if (ctrls.empty() == true)
	{	
		TangoSys_OMemStream o;
		o << "No controller defined with file named " << argin << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_ControllerNotFound",o.str(),
				(const char *)"Pool::reload_controller_code");
	}

//
// Get Pool Class ptr
//

	PoolClass *po_class = static_cast<PoolClass *>(get_device_class());

//
// Check device. If their controller ID is the concerned controller,
// check that the controller code can be unloaded
// For motor, this means checking that the motor is not moving
//

	vector<Pool::ObjBkp> mots;

	switch (ctrl_dev_type)
	{
		case MOTOR_CTRL:
		find_and_invalidate_motor(ctrls,mots);
		break;

		case PSEUDO_MOTOR_CTRL:
		find_and_invalidate_pseudo_motor(ctrls,mots);
		break;

		case PSEUDO_COUNTER_CTRL:
		find_and_invalidate_pseudo_counter(ctrls,mots);
		break;
		
		case COTI_CTRL:
		find_and_invalidate_ct(ctrls,mots);
		break;
		
		case ZEROD_CTRL:
		find_and_invalidate_zerod(ctrls,mots);
		break;

		case COM_CTRL:
		find_and_invalidate_comch(ctrls,mots);
		break;
		
		case CONSTRAINT_CTRL:
		// constraint does not have any devices so there is no need to invalidate
		break;
			
		case UNDEF_CTRL:
		Tango::Except::throw_exception(
				(const char *)"Pool_BadCtrlType",
				(const char *)"Undefined controller type !!!",
				(const char *)"Pool::reload_controller_code");
		break;
	}	

//
// Starting from this point, we might change the device state
//

	PoolStateEvent pse(this);
	
//
// Mark all concerned controller as unusable
//

	vector<Pool::CtrlBkp>::iterator ite;
	for (ite = ctrls.begin();ite != ctrls.end();++ite)
	{
		list<ControllerPool>::iterator ctl = ctrl_list.begin();
		advance(ctl,ite->dist);
		if (ctl->ctrl_fica_built == true)
		{
			AutoPoolLock lo(ctl->get_ctrl_fica_mon());
			delete ctl->ctrl;
		}
        ctl->ctrl = NULL;
	}
    
//
// Reload controller code
//

	Language ctrl_lang;
	bool reloaded = false;
	list<ControllerPool>::iterator ctrl_ite;

	try
	{
		for (ite = ctrls.begin();ite != ctrls.end();++ite)
		{
			ctrl_ite = ctrl_list.begin();
			advance(ctrl_ite,ite->dist);
	
			if ((ite->wrong_fica == true) || (ite->fica_already_reloaded == true))
				continue;

			
			if (ctrl_ite->ctrl_fica_built == true)
			{
				ctrl_lang = ctrl_ite->get_language();
				if (ctrl_lang == CPP)
				{
					if (reloaded == false)
					{
						ctrl_ite->reload();
						reloaded = true;
					}
			   		(*(ctrl_ite->ite_ctrl_fica))->check_features(ctrl_lang,&ctrl_ite->get_ctrl_file(),ctrl_ite->ctrl_class_name);
				}
				else
				{
				
//
// Reload the new code and check that it is correct
//

					AutoPoolLock lo(ctrl_ite->get_ctrl_fica_mon());
					if (reloaded == false)
					{
						ctrl_ite->reload();
						reloaded = true;
					}
			
			   		(*(ctrl_ite->ite_ctrl_fica))->check_python(ctrl_ite->ctrl_class_name);
			   		(*(ctrl_ite->ite_ctrl_fica))->check_features(ctrl_lang,&ctrl_ite->get_ctrl_file(),ctrl_ite->ctrl_class_name);

//
// Reload a possible new list of properties
//

					vector<PropertyData *> &prop_list = (*(ctrl_ite->ite_ctrl_fica))->get_ctrl_prop_list();
					for (unsigned long loop = 0;loop < prop_list.size();loop++)
						delete prop_list[loop];
					prop_list.clear();
					
					ctrl_ite->get_ctrl_file().get_prop_info(ctrl_ite->ctrl_class_name,prop_list);
				}
			}
			else
			{
		
//
// We need to try to re-create the FiCa
// Retrieve ctrl info in the entry coming from database.
//

				long nb_ctrl = ctrl_list.size();
				string::size_type pos;
				
				pos = ctrl_ite->name.find('/');
				string ctrl = ctrl_ite->name.substr(0,pos);
				string inst = ctrl_ite->name.substr(pos + 1);
				long ind_array;
	 
				for (long l = 0;l < nb_ctrl;l++)
				{
					string file_db = ctrl_info[(l * PROP_BY_CTRL) + 1];
					string::size_type pos;
					if ((pos = file_db.find('.')) != string::npos)
						file_db.erase(pos);
					string ctrl_db(file_db);
					ctrl_db = ctrl_db + '.' + ctrl_info[(l * PROP_BY_CTRL) + 2];
					string inst_db = ctrl_info[(l * PROP_BY_CTRL) + 3];
					transform(ctrl_db.begin(),ctrl_db.end(),ctrl_db.begin(),::tolower);
					transform(inst_db.begin(),inst_db.end(),inst_db.begin(),::tolower);
					if ((ctrl_db == ctrl) && (inst_db == inst))
					{
						ind_array = l * PROP_BY_CTRL;
						break;
					}
				}

//
// Try to re-create FiCa entry from info stored in DB
//
	
				vector<string>::iterator v_ite = ctrl_info.begin();
				v_ite += ind_array;

//
// We here have 2 cases
//  1 - The FiCa has been correctly created but the properties are wrong.
//		We know that we are in this case because the FiCa is in the FiCa list
//		Then, we do not need to re-create it but we need to re-create the
//		property list because it could have changed
//  2 - The FiCa has not been built at all and then we need to build it
//

				PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
				bool re_create = true;
				try
				{
	  				vct_ite tmp_ite = cl_ptr->get_ctrl_fica_by_name(ctrl_ite->ctrl_fica_name,*v_ite);
	  				re_create = false;
	
					vector<PropertyData *> &prop_list = (*tmp_ite)->get_ctrl_prop_list();
					for (unsigned long loop = 0;loop < prop_list.size();loop++)
						delete prop_list[loop];
					prop_list.clear();
	
					{		
						AutoPoolLock lo = AutoPoolLock(ctrl_ite->get_ctrl_fica_mon());
						if (reloaded == false)
						{
		 					ctrl_ite->reload();
		 					reloaded = true;
						}

						ctrl_lang = PYTHON;
						CtrlFiCa &ctrl_fica = *(*(ctrl_ite->ite_ctrl_fica));

						ctrl_fica.check_python(ctrl_ite->ctrl_class_name);

						ctrl_fica.check_features(
										ctrl_lang,
										&ctrl_ite->get_ctrl_file(),
										ctrl_ite->ctrl_class_name);
						
						(*tmp_ite)->get_ctrl_file()->get_prop_info(ctrl_ite->ctrl_class_name,prop_list);
					}
				}
				catch (Tango::DevFailed &e)
				{
					if (re_create == true)
						ctrl_ite->ite_ctrl_fica = cl_ptr->add_ctrl_fica(ctrl_ite->ctrl_fica_name,*(v_ite + 1),ctrl_ite->ctrl_class_name,*v_ite,this);
					else
						throw;
				}
				ctrl_lang = ctrl_ite->get_language();				
			}
			
//
// Mark all controllers with this FiCa as FiCa already re-loaded
//

			vector<Pool::CtrlBkp>::iterator ite_2;
			for (ite_2 = ctrls.begin();ite_2 != ctrls.end();++ite_2)
		    {	        
		        if (ite_2->fica_ite == ite->fica_ite)
					ite_2->fica_already_reloaded = true;
		    }

		}
	}
	catch (Tango::DevFailed &e)
	{
		
//
// Memorize error for pool status, mark the FiCa as not built and
// remove the File entry in the CtrlFile vector before re-throwing the exception
//

		bool first_loop = true;
		string ct_stat;
		long nb_ctrl = ctrls.size();
		long loop = 0;
	
		if (reloaded == false)
		{
			
//
// We have a major error at file level
// Mark all controllers using this file as bad
// and re-throw the exception
//
	
		    for (ite = ctrls.begin();ite != ctrls.end();++ite)
		    {
		    	loop++;
		        list<ControllerPool>::iterator ctl = ctrl_list.begin();
		        advance(ctl,ite->dist);
		        
				ctl->error_status.clear();
				ctl->error_status = "Error reported when trying to create controller ";
				ctl->error_status += ctl->name;
				ctl->error_status += ".";
				if (first_loop == true)
				{
		        	except_2_ctrl_status(e,ct_stat);
		        	first_loop = false;
				}
		        ctl->error_status += ct_stat;
		        ctl->ctrl_fica_built = false;
		        
		        if (loop == nb_ctrl)
		        	po_class->remove_fica_by_name(ctl->ctrl_fica_name);
		    }	
	
			po_class->remove_ctrl_file_by_name(in_file);
			throw;
		}
		else
		{
			
//
// The error is now at the class level
// Mark only the controllers using this class as bad
//

			vector<Pool::CtrlBkp>::iterator ite_2;
			for (ite_2 = ctrls.begin();ite_2 != ctrls.end();++ite_2)
		    {	        
		        if (ite_2->fica_ite == ite->fica_ite)
		        {		
		       	 	list<ControllerPool>::iterator ctl = ctrl_list.begin();
		        	advance(ctl,ite_2->dist);

					ctl->error_status.clear();		        	
		    		ctl->error_status = "Error reported when trying to create controller ";
					ctl->error_status = ctl->error_status + ctrl_ite->name;
					if (first_loop == true)
					{
						except_2_ctrl_status(e,ct_stat);
						first_loop = false;
					}
					ctl->error_status += ct_stat;
					ctl->ctrl_fica_built = false;
					ite_2->wrong_fica = true;
		        }
		    }
		}
	}
	
//
// Re-create controller objects and mark all concerned controller as usable
//

	vector<Tango::DevFailed> save_exceptions;
	
    for (ite = ctrls.begin();ite != ctrls.end();++ite)
    {
        list<ControllerPool>::iterator ctl = ctrl_list.begin();
        advance(ctl,ite->dist);
 
 		if (ite->wrong_fica == true)
 		{
 			
//
// Add exception catched in previous loop in the save_exceptions vector
// 

			Tango::DevErrorList errors;
			errors.length(1);
			errors[0].desc = CORBA::string_dup(ctl->error_status.c_str());
			errors[0].severity = Tango::ERR;
			errors[0].origin = CORBA::string_dup("Pool::reload_controller_code()");
			errors[0].reason = CORBA::string_dup("Pool_BadController");
			
			Tango::DevFailed tmp_except(errors);
			save_exceptions.push_back(tmp_except);
			continue;
 		}
 		else
 		{
	 		string::size_type pos = ctl->name.find('/');
			pos++;
			string inst_name = ctl->name.substr(pos);
				       
		    Controller *ct;
	   		if (ctrl_lang == CPP)
			{	
			
//
// Retrieve the controller object creator C function
//
		
	   			lt_ptr sym;
			
				string sym_name("_create_");
				sym_name = sym_name + ctl->ctrl_class_name;
				
				DEBUG_STREAM << "Symbol name = " << sym_name << endl;
					
				sym = lt_dlsym(ctl->get_lib_ptr(),sym_name.c_str());
				if (sym == NULL)
				{
					TangoSys_OMemStream o;
					o << "Controller library " << in_file; 
					o << " does not have the C creator function "
						 "(_create_<Controller name>)" << ends;
			
					Tango::Except::throw_exception(
						(const char *)"Pool_CCreatorFunctionNotFound",o.str(),
						(const char *)"Pool::reload_controller_code");
				}
				
				DEBUG_STREAM << "lt_dlsym is a success" << endl;
	
//
// Create the controller
//
		
				Ctrl_creator_ptr ct_ptr = (Ctrl_creator_ptr)sym;
				vector<Controller::Properties> *prop = NULL;
				
				try
				{
					AutoPoolLock lo(ctl->get_ctrl_fica_mon());
					vct_ite	tmp_ite = ctl->ite_ctrl_fica;

					vector<pair<string,string> > prop_pairs;
					
					build_property_data(inst_name,ctrl_ite->ctrl_class_name,
										prop_pairs,
										(*tmp_ite)->get_ctrl_prop_list());
					check_property_data((*tmp_ite)->get_ctrl_prop_list());
					prop = properties_2_cpp_vect(
							(*tmp_ite)->get_ctrl_prop_list());
					
					ct = (*ct_ptr)(inst_name.c_str(),*prop);
					ctl->ctrl_fica_built = true;
					
					delete ctl->cpp_ctrl_prop;
					ctl->cpp_ctrl_prop = prop;
				}
				catch (Tango::DevFailed &e)
				{
					if (prop != NULL)
						delete prop;
						
					ct = NULL;
					save_exceptions.push_back(e);
					
					string ct_stat;
					ctl->error_status.clear();
					ctl->error_status = "Error reported when trying to create "
										"controller ";
					ctl->error_status = ctl->error_status + ctl->name;
		        	except_2_ctrl_status(e,ct_stat);
		        	ctl->error_status = ctl->error_status + ct_stat;
				}
			}
			else
			{
			
//
// Retrieve the controller object creator C function
//
		
				lt_ptr sym;
				string dev_type_str = po_class->CtrlType_2_str(ctrl_dev_type);
				
				string sym_name("_create_Py");
				sym_name = sym_name + dev_type_str + "Controller";
				DEBUG_STREAM << "Symbol name = " << sym_name << endl;
					
				sym = lt_dlsym(ctl->get_py_inter_lib_ptr(),sym_name.c_str());
				if (sym == NULL)
				{
					TangoSys_OMemStream o;
					o << "Controller library " << in_file;
					o << " does not have the C creator function "
						 "(_create_<Controller name>)" << ends;
			
					Tango::Except::throw_exception(
						(const char *)"Pool_CCreatorFunctionNotFound",o.str(),
						(const char *)"Pool::reload_controller_code");
				}
				
				DEBUG_STREAM << "lt_dlsym is a success" << endl;
			
//
// Create the Python controller object after building and checking
// its properties set
//
		
				PyCtrl_creator_ptr ct_ptr = (PyCtrl_creator_ptr)sym;
				try
				{
					AutoPoolLock lo(ctl->get_ctrl_fica_mon());
					vct_ite	tmp_ite = ctl->ite_ctrl_fica;
					
					vector<pair<string,string> > prop_pairs;
					
					build_property_data(inst_name,ctrl_ite->ctrl_class_name,
										prop_pairs,
										(*tmp_ite)->get_ctrl_prop_list());
					check_property_data((*tmp_ite)->get_ctrl_prop_list());
					PyObject *prop_dict = 
						properties_2_py_dict((*tmp_ite)->get_ctrl_prop_list());
					
					ct = (*ct_ptr)(inst_name.c_str(),
								   ctl->ctrl_class_name.c_str(),
								   ctl->get_py_module(),prop_dict);
					ctl->ctrl_fica_built = true;
				}
				catch (Tango::DevFailed &e)
				{
					ct = NULL;
					save_exceptions.push_back(e);
					
					string ct_stat;
					ctl->error_status.clear();
					ctl->error_status = "Error reported when trying to create "
										"controller ";
					ctl->error_status = ctl->error_status + ctl->name;
		        	except_2_ctrl_status(e,ct_stat);
		        	ctl->error_status = ctl->error_status + ct_stat;
				}
			}

//
// Retrieve the possibly changed MaxDevice property
//
	   		vct_ite	tmp_ite = ctl->ite_ctrl_fica;
	   		long MaxDevice = (*tmp_ite)->get_MaxDevice();
	   		
//
// Update info in pool list
//
		        
	        ctl->ctrl = ct;
	        ctl->MaxDevice = MaxDevice;

//
// Mark all devices using this controller as usable
// and restart them using an Init
// command. It is not enough to send them the Init command
// because there are some cases where the device interface
// could have changed and dynamic attributes have to be created
// This is the case where the ctrl FiCa was not built
// and is built by this command.
// Do this only if the controller creation was a success
//

			if (ct != NULL)
			{
				ctl->error_status.clear();

				switch (ctrl_dev_type)
				{
					case MOTOR_CTRL:
					restore_motor(mots,ctl,ite);
					break;

					case PSEUDO_MOTOR_CTRL:
					restore_pseudo_motor(mots,ctl,ite);
					break;
					
					case PSEUDO_COUNTER_CTRL:
					restore_pseudo_counter(mots,ctl,ite);
					break;

					case COTI_CTRL:
					restore_ct(mots,ctl,ite);
					break;
					
					case ZEROD_CTRL:
					restore_zerod(mots,ctl,ite);
					break;

					case COM_CTRL:
					restore_comch(mots,ctl,ite);
					break;
					
					case CONSTRAINT_CTRL:
					// No devices on constraint so no need to invalidate
					break;
					
					case UNDEF_CTRL:
					Tango::Except::throw_exception(
							(const char *)"Pool_BadCtrlType",
							(const char *)"Undefined controller type !!!",
						  	(const char *)"Pool::reload_controller_code");
					break;
				}				
		        
//
// The motor Init command clears the Motor proxy. Mark the
// proxy created flag as false to recreate the proxy
//

				proxy_created = false;
			} 
 		}
    }
    
//
// If it was not possible to create controller object(s),
// send exception to caller to inform him(her)
//

	if (save_exceptions.size() != 0)
	{
		TangoSys_OMemStream o;
		o << "Not possible to create " << save_exceptions.size() << " controller(s)";
		o << "\nError messages are: ";
		for (unsigned long loo = 0;loo < save_exceptions.size();loo++)
			o << "\n" << save_exceptions[loo].errors[0].desc;
		o << ends;
		
		Tango::Except::throw_exception((const char *)"Pool_CantCreateController",o.str(),
					  (const char *)"Pool::reload_controller_code");
	}
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_motor_group
 *
 *	description:	method to execute "CreateMotorGroup"
 *	This command creates a motor group. The name of the group is the first element in the input
 *	argument followed by the group element name.
 *
 * @param	argin	Group name followed by motor's name
 *
 */
//+------------------------------------------------------------------
void Pool::create_motor_group(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "Pool::create_motor_group(): entering... !" << endl;

	//	Add your own code to control device here

//
// Basic check on input parameters
//
  
  	string group_name((*argin)[0]);
  	long input_nb = argin->length() - 1;
  	
  	if (input_nb <= 0)
  	{
  		TangoSys_OMemStream o;
  		o << "Cant create motor group " << group_name << ". You haven't defined any element as group member" << ends;
  
  		Tango::Except::throw_exception((const char *)"Pool_BadArgument",o.str(),
  				        (const char *)"Pool::create_motor_group");
  	}
  	
  	DEBUG_STREAM << "Group name = " << group_name;
  	vector<GrpEltType> in_type;
  	long i;
  	for (i = 1;i < input_nb + 1;i++)
  		DEBUG_STREAM << "Input elt name = " << (*argin)[i];
  		
//
// Check if each element are member of this pool
// as motor, pseudo-motor or group
//
  
  	for (i = 1;i < input_nb + 1;i++)
  	{
  		string elt_name((*argin)[i]);
  		if (motor_exist(elt_name) == false)
  		{
  			if (pseudo_motor_exist(elt_name) == false)
  			{
  				if (group_exist(elt_name) == false)
  				{
  					TangoSys_OMemStream o;
  					o << "Element " << elt_name << " is neither a motor, pseudo-motor or group defined in this pool. Can't create the group." << ends;
  	
  					Tango::Except::throw_exception((const char *)"Pool_BadArgument",o.str(),
  					        				(const char *)"Pool::create_motor_group");
  				}
  				else
  					in_type.push_back(GROUP);
  			}
  			else
  				in_type.push_back(PSEUDO_MOTOR);
  		}
  		else
  			in_type.push_back(MOTOR);
  	}
  	
//
// Check if we don't have already enough motor groups
//
  
  	DEBUG_STREAM << "Checking motor group number" << endl;
  	
  	if (mot_group_list.size() == MAX_MOTOR_GROUP)
  	{
  		Tango::Except::throw_exception((const char *)"Pool_TooManyMotorGroup",
  					       (const char *)"Too many motor groups in your pool",
  					       (const char *)"Pool::create_motor_group");
  	}
  	
//
// Build Tango device name
//
  
  	Tango::Util	*tg = Tango::Util::instance();
  	string tg_dev_name("mg/");
  	tg_dev_name = tg_dev_name + tg->get_ds_inst_name() +  '/' + group_name;
  	
  	DEBUG_STREAM << "Tango motor group device name = " << tg_dev_name << endl;
  			
//
// Check if this device is already defined in database
// Check by device alias and by Tango device name
//
  
  	Tango::Database *db = tg->get_database();
  	
  	Tango::DbDevImportInfo my_device_import;
  	bool device_exist = false;
  	bool by_alias = false;	
  
  	try
  	{
  		my_device_import = db->import_device(group_name);
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
  			my_device_import = db->import_device(tg_dev_name);
  			device_exist = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
  			{
  				device_exist = true;
  			}
  		}
  	}
  	
  	if (device_exist == true)
  	{
  		TangoSys_OMemStream o;
  		o << "Motor group ";
  		if (by_alias == false)
  			o << tg_dev_name;
  		else
  			o << group_name;
  		o << " already defined" << ends;
  
  		Tango::Except::throw_exception((const char *)"Pool_WrongMotorGroupName",o.str(),
  				        (const char *)"Pool::create_motor_group");
  	}

//
// Build group physical structure
// (replace group by its motor and
// pseudo-motor by its physical motors)
//

	long wanted_in_nb = 0;
	vector<string> grp_mot_name;
	
	vector<pair<string,Controller *> > m_to_pmctrl; 
	
  	for (i = 1;i < input_nb + 1;i++)
  	{
  		string in_elt((*argin)[i]);
  		if (in_type[i - 1] == MOTOR)
  		{
  			transform(in_elt.begin(),in_elt.end(),in_elt.begin(),::tolower);
  			grp_mot_name.push_back(in_elt);
  			wanted_in_nb++;
  		}
  		else if (in_type[i - 1] == PSEUDO_MOTOR)
  		{
  			PseudoMotorPool &pm = get_pseudo_motor_from_name(in_elt);
  			wanted_in_nb++;
  			for (unsigned long loop = 0;loop < pm.mot_elts.size();loop++)
  			{
  				string tmp_mot_name(pm.mot_elts[loop]->name);
  				transform(tmp_mot_name.begin(),tmp_mot_name.end(),tmp_mot_name.begin(),::tolower);
  				
  				pair<string,Controller*> m_ctrl(tmp_mot_name, pm.pseudo_motor->get_ctrl());
  				
//
// if we find a motor that has not yet been used by the current pseudo motor controller we
// had it to the list. 
// This is done to allow the same motors to be used by different pseudo motors that belong
// to the same pseudo motor controller
//
  				if(find(m_to_pmctrl.begin(), m_to_pmctrl.end(), m_ctrl) == m_to_pmctrl.end())
  				{
  					m_to_pmctrl.push_back(m_ctrl);
  					grp_mot_name.push_back(tmp_mot_name);
  				}
  			}
  		}
  		else
  		{
  			MotorGroupPool &grp = get_motor_group_from_name(in_elt);
  			for (unsigned long loop = 0;loop < grp.mot_ids.size();loop++)
  			{
  				MotorPool &mot = get_motor_from_id(grp.mot_ids[loop]);
  				grp_mot_name.push_back(mot.obj_alias_lower);
  				wanted_in_nb++;
  			}
  		}
  	}

//
// Check that each motor is not used several times within the group
//
  	
	for (unsigned long hh = 0;hh < grp_mot_name.size();hh++)
	{
		int num;
		if ((num = count(grp_mot_name.begin(),grp_mot_name.end(),grp_mot_name[hh])) > 1)
		{
  			TangoSys_OMemStream o;
  			o << "Motor " << grp_mot_name[hh] << " is used " << num << " times in this group";
  			o << "\nEach motor should be used only once in a group" << ends;
  
  			Tango::Except::throw_exception((const char *)"Pool_WrongMotorGroup",o.str(),
  				        (const char *)"Pool::create_motor_group");
		}
	}  
	
//
// If the device is not defined in database, create it in database, set its alias
// and define its properties used to store its ID, its device pool and its motor list
//
  
  	if (device_exist == false)
  	{
  		DEBUG_STREAM << "Trying to create device entry in database" << endl;
  		
      	try
  		{
  			Tango::DbDevInfo my_device_info;
      		my_device_info.name = tg_dev_name.c_str();
      		my_device_info._class = "MotorGroup";
      		my_device_info.server = tg->get_ds_name().c_str();
  			
      		db->add_device(my_device_info);
      		db->put_device_alias(tg_dev_name,group_name);
  
  			Tango::DbDatum id(MOTOR_GROUP_ID_PROP);
  			Tango::DbDatum pool_name(POOL_DEV_NAME_PROP);
  			Tango::DbDatum phy_list(PHYS_GROUP_ELT);
  			Tango::DbDatum usr_list(USER_GROUP_ELT);
  			Tango::DbDatum mot_list(MOTOR_GROUP_LIST);
  			Tango::DbDatum mot_grp_list(MOTOR_GROUP_GROUP_LIST);
  			Tango::DbDatum pm_list(PSEUDO_MOTOR_GROUP_LIST);
  			Tango::DbDatum want_in(POS_SPECTRUM_DIM_X);
  			Tango::DbData db_data;
  
  			mot_group_id++;
  			id << mot_group_id;
  			db_data.push_back(id);			
  			
  			pool_name << get_name();
  			db_data.push_back(pool_name);
  			
  			phy_list << grp_mot_name;
  			db_data.push_back(phy_list);
 
 			vector<string> str_user_list, str_mot_list, str_mg_list, str_pm_list;		
  			for (long loop = 1;loop < input_nb + 1;loop++)
  			{
  				if (in_type[loop - 1] == MOTOR)
  				{
  					str_mot_list.push_back((*argin)[loop].in());
  				}
  				else if (in_type[loop - 1] == GROUP)
  				{
  					str_mg_list.push_back((*argin)[loop].in());
  				}
  				else
  				{
  					str_pm_list.push_back((*argin)[loop].in());
  				}
  				str_user_list.push_back((*argin)[loop].in());
  			}
  			usr_list << str_user_list;
  			db_data.push_back(usr_list);
  			
  			mot_list << str_mot_list;
  			db_data.push_back(mot_list);

  			mot_grp_list << str_mg_list;
  			db_data.push_back(mot_grp_list);

  			pm_list << str_pm_list;
  			db_data.push_back(pm_list);
  			
  			want_in << wanted_in_nb;
  			db_data.push_back(want_in);
  			
  			db->put_device_property(tg_dev_name.c_str(),db_data);
  			
			Tango::DbDatum pos("Position"),abs_ch("abs_change");
			db_data.clear();
			pos << (long)1;
			abs_ch << defaultMotGrpPos_AbsChange;
			db_data.push_back(pos);
			db_data.push_back(abs_ch);
			db->put_device_attribute_property(tg_dev_name.c_str(),db_data);
  			
  			DEBUG_STREAM << "Device created in database (with alias)" << endl;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			DEBUG_STREAM << "Gasp an exception........" << endl;
  			TangoSys_OMemStream o;
  			o << "Can't create motor group " << group_name << " in database" << ends;
  			
  			Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateMotorGroup",o.str(),
  							  (const char *)"Pool::create_motor_group");
  		}
  		
//
// Find the Tango MotorGroup class and create the motor group
//
  
  		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
  		for (unsigned long i = 0;i < cl_list->size();i++)
  		{
  			if ((*cl_list)[i]->get_name() == "MotorGroup")
  			{
  				try
  				{
  					Tango::DevVarStringArray na;
  					na.length(1);
  					na[0] = tg_dev_name.c_str();
  					(*cl_list)[i]->device_factory(&na);
  					break;
  				}
  				catch (Tango::DevFailed &e)
  				{
  					DEBUG_STREAM << "Exception while trying to create MotorGroup device" << endl;
   
//
// The delete_device will also delete device property(ies)
//
  					
  					db->delete_device(tg_dev_name);
  					
  					TangoSys_OMemStream o;
  					o << "Can't create motor group device " << group_name << ends;
  			
  					Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateMotorGroup",o.str(),
  							  (const char *)"Pool::create_motor_group");
  				}
  			}
  		}
  
//
// Create a Tango device proxy on the newly created motor
// and set its connection to automatic re-connection
//
  
  		Tango::DeviceProxy *tmp_dev = new Tango::DeviceProxy(mot_group_list.back().obj_tango_name);
  		tmp_dev->set_transparency_reconnection(true);
  		mot_group_list.back().obj_proxy = tmp_dev;
  	}

//
// subscribe to the internal events coming from each element of the motor group
//
	MotorGroupPool &mgp = get_motor_group_from_id(mot_group_id);

	vector<PoolElement*>::iterator ite = mgp.group_elts.begin();
	for(; ite != mgp.group_elts.end(); ite++)
	{
		(*ite)->add_pool_elem_listener(&mgp);
	}

//
// Push a change event for clients listenning on events
//
  
  	Tango::Attribute &mgl = dev_attr->get_attr_by_name("MotorGroupList");
  	read_MotorGroupList(mgl);
  	mgl.fire_change_event();
}



//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_motor_group
 *
 *	description:	method to execute "DeleteMotorGroup"
 *	This command delete a motor group from its name
 *
 * @param	argin	The motor group name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_motor_group(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_motor_group(): entering... !" << endl;

	//	Add your own code to control device here
  	
//
// Find motor group in motor group list
//

	string user_name(argin);
	
	MotorGroupPool &mg_to_del = get_motor_group_from_name(user_name);  
  		
  	DEBUG_STREAM << "Motor group found" << endl;
  
//
// Check that the group is not moving
//
  
  	if (mg_to_del.obj_proxy->state() == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete group with name " << argin;
  		o << ". It is actually moving." << ends;
  		
  		Tango::Except::throw_exception((const char *)"Pool_CantDeleteGroup",o.str(),
  								 (const char *)"Pool::delete_motor_group");
  	}
  
//
// Check that this group is not member of any other group
//

	vector<string> used_by_mg;
	if (get_motor_groups_containing_elt(user_name,used_by_mg) == true)
	{
		TangoSys_OMemStream o;
		o << "Can't delete group with name " << argin;
		o << ". It is used in group(s): ";
		
		vector<string>::iterator ite = used_by_mg.begin();
		for(;ite != used_by_mg.end(); ite++)
			o << *ite << ", " << ends;
		o << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantDeleteGroup",o.str(),
							 (const char *)"Pool::delete_motor_group");
	}
	
//
// Check that this group is not used by any pseudo motor
//

	vector<string> used_by_pm;
	if (get_pseudo_motors_that_use_mg(mg_to_del.id,used_by_pm) == true)
	{
		TangoSys_OMemStream o;
		o << "Can't delete group with name " << argin;
		o << ". It is used by pseudo motor(s): ";

		vector<string>::iterator ite = used_by_pm.begin();
		for(;ite != used_by_pm.end(); ite++)
			o << *ite << ", " << ends;
		o << ends;
			
		Tango::Except::throw_exception((const char *)"Pool_CantDeleteGroup",o.str(),
							 (const char *)"Pool::delete_motor_group");
	}

//
// Unsubscribe to the internal events for each element of the motor group
//

	vector<PoolElement*>::iterator it = mg_to_del.group_elts.begin();
	for(; it != mg_to_del.group_elts.end();++it)
	{
		(*it)->remove_pool_elem_listener(&mg_to_del);		
	}
		  			
//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	try
  	{
  		tg->get_database()->delete_device(mg_to_del.obj_tango_name);
  			
//
// Delete motor group device from server but first find its Tango xxxClass instance
//
  
  		Tango::DeviceClass *dc = mg_to_del.group->get_device_class();
  		dc->device_destroyer(mg_to_del.obj_tango_name);
  	}
  	catch (Tango::DevFailed &e)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete motor group with name " << argin << ends;
  				
  		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeleteMotorGroup",o.str(),
  								 (const char *)"Pool::delete_motor_group");
  	}
	  	
//
// Push a change event for clients listenning on events
//
  
  	Tango::Attribute &mgl = dev_attr->get_attr_by_name("MotorGroupList");
  	read_MotorGroupList(mgl);
  	mgl.fire_change_event();
  	
}

		
//+------------------------------------------------------------------
/**
 *	method:	Pool::dev_status
 *
 *	description:	method to execute "Status"
 *	This command gets the device status (stored in its <i>device_status</i> data member) and returns it to the caller.
 *
 * @return	Status descrition
 *
 */
//+------------------------------------------------------------------
Tango::ConstDevString Pool::dev_status()
{
	Tango::ConstDevString	argout = DeviceImpl::dev_status();
	DEBUG_STREAM << "Pool::dev_status(): entering... !" << endl;

	//	Add your own code to control device here

	tmp_status = argout;
	
	if (get_state() == Tango::MOVING)
	{
		tmp_status = tmp_status + '\n';
		tmp_status = tmp_status + "The pool device is shuting down but is waiting for the end of motor(s) movement";
	}
	else
	{
		list<ControllerPool>::iterator ite;
		bool first_ctrl_in_error = true;
	
		for (ite = ctrl_list.begin();ite != ctrl_list.end();++ite)
		{
			if (ite->ctrl == NULL)
			{	
				if (first_ctrl_in_error == false)
					tmp_status = tmp_status + '\n';
	
				tmp_status = tmp_status + ite->error_status;			
				first_ctrl_in_error = false;
			}
		}
	}
	
	argout = tmp_status.c_str();
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_pseudo_motor
 *
 *	description:	method to execute "DeletePseudoMotor"
 *	This command deletes a pseudo motor
 *
 * @param	argin	Pseudo Motor name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_pseudo_motor(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_pseudo_motor(): entering... !" << endl;

	//	Add your own code to control device here

//
// Find pseudo motor in pseudo motor list
//
	string user_name(argin);
	PseudoMotorPool &pm_to_del =  get_pseudo_motor_from_name(user_name);
  		
  	DEBUG_STREAM << "Pseudo Motor found" << endl;
	
//
// Check that the pseudo motor is not moving
//
  	if (pm_to_del.obj_proxy->state() == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete pseudo motor with name " << argin;
  		o << ". It is actually moving." << ends;
  				
  		Tango::Except::throw_exception((const char *)"Pool_CantDeletePseudoMotor",o.str(),
  								 (const char *)"Pool::delete_pseudo_motor");
  	}

//
// Check that the pseudo motor is not member of a motor group
// Also check if the underlying motor group used by this pseudo motor is not used by another motor group
//
	bool mg_shared_by_other_mg = false;
  	list<MotorGroupPool>::iterator grp_ite;
  	for (grp_ite = mot_group_list.begin();grp_ite != mot_group_list.end();++grp_ite)
  	{
  		if(grp_ite->is_user_member(pm_to_del.name))
  		{
  			TangoSys_OMemStream o;
  			o << "Can't delete pseudo motor with name " << argin;
  			o << ". It is actually member of group " << grp_ite->name << ends;
  					
  			Tango::Except::throw_exception((const char *)"Pool_CantDeletePseudoMotor",o.str(),
  									 (const char *)"Pool::delete_pseudo_motor");
  		}
  		
  		if(grp_ite->is_user_member(pm_to_del.mot_grp->name))
  		{
  			mg_shared_by_other_mg = true;
  		}
  	}
  	
//
// Check if the underlying motor group used by this pseudo motor should be deleted.
// It should be deleted if and only if:
// - there is no other pseudo motor using it and
// - there is no other motor group using it (this condition was checked in the code above)
// 
	bool mg_shared_by_other_pm = false;
	list<PseudoMotorPool>::iterator pm_ite;
	for (pm_ite = pseudo_mot_list.begin();pm_ite != pseudo_mot_list.end();++pm_ite)
	{
		if((pm_ite->mot_grp->id == pm_to_del.mot_grp->id) && (pm_ite->name != pm_to_del.name))
		{
			mg_shared_by_other_pm = true;
			break;
		}
	}

//
// Delete the motor group attached to this pseudo motor if it is not shared by other element
//
	if(!mg_shared_by_other_pm && !mg_shared_by_other_mg)
	{
		Tango::DeviceProxy tg_pool(this->get_name());
		Tango::DeviceData din,dout;	
		
		din << pm_to_del.mot_grp->name;
		
//
// A little trick here: The DeleteMotorGroup command checks if there is any pseudo motor
// using the motor groups before it deletes the motor group. It does this by checking
// the mg_id in each pseudo motor with its own id. To skip this test for the pseudo motor
// being deleted we set mg_id value temporarily to -1 
//
		MotorGroupPool *mg_ptr = pm_to_del.mot_grp;
		pm_to_del.mot_grp = NULL;
		try 
		{
			dout = tg_pool.command_inout("DeleteMotorGroup", din);
		}
		catch(Tango::DevFailed &ex)
		{
			WARN_STREAM << "Delete of Motor Group for Pseudo Motor " << pm_to_del.name << " failed" << endl;
		}
		pm_to_del.mot_grp = mg_ptr;
	}
			
	Tango::Util *util = Tango::Util::instance();
	Tango::Database *db = util->get_database();	
	
//
// Inform siblings that a sibling will die 
//
	//Set the siblings for each pseudo motor
	for(pm_ite = pseudo_mot_list.begin(); pm_ite != pseudo_mot_list.end(); pm_ite++)
		if(pm_ite->ctrl_id == pm_to_del.ctrl_id)
			pm_ite->pseudo_motor->sibling_died(pm_to_del.role_idx);

//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
	try
	{
		db->delete_device(pm_to_del.obj_tango_name);

//
// Delete pseudo motor device from server but first find its Tango xxxClass instance
//
		Tango::DeviceClass *dc = pm_to_del.pseudo_motor->get_device_class();
		dc->device_destroyer(pm_to_del.obj_tango_name);
	}
	catch (Tango::DevFailed &e)
	{
		TangoSys_OMemStream o;
		o << "Can't delete pseudo motor '" << argin << "'" << ends;
		
		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeletePseudoMotor",o.str(),
						                    (const char *)"Pool::delete_pseudo_motor");
	}
		
//
// Push a change event for clients listenning on events
//
  
  	Tango::Attribute &pml = dev_attr->get_attr_by_name("PseudoMotorList");
  	read_PseudoMotorList(pml);
  	pml.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_controller
 *
 *	description:	method to execute "DeleteController"
 *	Delete controller from its name.
 *	Before executing this command, you must first delete all the elements associated
 *	with this controller
 *
 * @param	argin	The controller name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_controller(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_controller(): entering... !" << endl;

	//	Add your own code to control device here
	
//
// Find controller in controller list
//

	string user_ctrl(argin);
	
	ControllerPool &ctrl_to_del = get_ctrl_from_inst_name(user_ctrl);

//
// Get controller object type and check that we don't have any object
// still using the controller
//

	if (ctrl_to_del.ctrl_fica_built == true)
	{
		CtrlType type = ctrl_to_del.get_ctrl_obj_type();
	
		switch(type)
		{
			case MOTOR_CTRL:
			{
	
//
// Check that we don't have any motor left on this controller
//

				list<MotorPool>::iterator itm;
				long nb_remain_motor = 0;
				TangoSys_OMemStream o;
					
				for (itm = mot_list.begin();itm != mot_list.end();++itm)
				{
			  		if (itm->ctrl_id == ctrl_to_del.id)
			  		{
						if (nb_remain_motor == 0)
			      		{
				      		o << "Can't delete controller, motor(s) ";
				      		o << itm->name;
			      		}
			      		else
			      		{
				      		o << ", " << itm->name;
				      	}
						nb_remain_motor++;
			      	}	
			  	}
				if (nb_remain_motor != 0)
			    {
				   	o << " are still defined using this controller" << ends;
				      
			      	Tango::Except::throw_exception(
			      			(const char *)"Pool_CantDeleteController",o.str(),
							(const char *)"Pool::delete_controller");
			  	}
			}
	      	break;
      	
			case PSEUDO_MOTOR_CTRL:
			{
	
//
// Check that we don't have any pseudo motor left on this controller
//

				list<PseudoMotorPool>::iterator itm;
				long nb_remain_pseudo_motor = 0;
				TangoSys_OMemStream o;
					
				for (itm = pseudo_mot_list.begin();itm != pseudo_mot_list.end();++itm)
				{
			  		if (itm->ctrl_id == ctrl_to_del.id)
			  		{
						if (nb_remain_pseudo_motor == 0)
			      		{
				      		o << "Can't delete controller, pseudo motor(s) ";
				      		o << itm->name;
			      		}
			      		else
			      		{
				      		o << ", " << itm->name;
				      	}
						nb_remain_pseudo_motor++;
			      	}	
			  	}
			  
				if (nb_remain_pseudo_motor != 0)
			    {
					o << " are still defined using this controller" << ends;
					      
					Tango::Except::throw_exception(
							(const char *)"Pool_CantDeleteController",o.str(),
							(const char *)"Pool::delete_controller");
			  	}
			}
	      	break;

			case PSEUDO_COUNTER_CTRL:
			{	      	
//
// Check that we don't have any pseudo counter left on this controller
// In principle there can be only one but...we check
//

				list<PseudoCounterPool>::iterator itm;
					
				for (itm = pseudo_co_list.begin();itm != pseudo_co_list.end();++itm)
				{
			  		if (itm->ctrl_id == ctrl_to_del.id)
			  		{
						TangoSys_OMemStream o;
			      		o << "Can't delete controller, pseudo counter ";
			      		o << itm->name;
				      	o << " is still defined using this controller" << ends;
				      	Tango::Except::throw_exception(
				      		(const char *)"Pool_CantDeleteController",o.str(),
							(const char *)"Pool::delete_controller");
			  		}
			  	}
			}
	      	break;		      	
	      	
      		case COTI_CTRL:
      		{
//
// Check that we don't have any counter timer left on this controller
//

				list<CTExpChannelPool>::iterator itm;
				long nb_remain_channel = 0;
				TangoSys_OMemStream o;
					
				for (itm = ct_list.begin();itm != ct_list.end();++itm)
		  		{
		      		if (itm->ctrl_id == ctrl_to_del.id)
		      		{
						if (nb_remain_channel == 0)
			      		{
				      		o << "Can't delete controller, channel(s) ";
				      		o << itm->name;
			      		}
			      		else
			      		{
				      		o << ", " << itm->name;
			      		}
			      		nb_remain_channel++;
		      		}	
		  		}
		  
		      	if (nb_remain_channel != 0)
		      	{
			      	o << " are still defined using this controller" << ends;
					      
			      	Tango::Except::throw_exception((const char *)"Pool_CantDeleteController",o.str(),
									     (const char *)"Pool::delete_controller");
		      	}
	      	}
	      	break;
	      	
      		case ZEROD_CTRL:
      		{
//
// Check that we don't have any channel left on this controller
//

				list<ZeroDExpChannelPool>::iterator itm;
				long nb_remain_channel = 0;
				TangoSys_OMemStream o;
					
				for (itm = zerod_list.begin();itm != zerod_list.end();++itm)
		  		{
		      		if (itm->ctrl_id == ctrl_to_del.id)
		      		{
						if (nb_remain_channel == 0)
			      		{
				      		o << "Can't delete controller, channel(s) ";
				      		o << itm->name;
			      		}
			      		else
			      		{
				      		o << ", " << itm->name;
			      		}
			      		nb_remain_channel++;
		      		}	
		  		}
		  
		      	if (nb_remain_channel != 0)
		      	{
			      	o << " are still defined using this controller" << ends;
					      
			      	Tango::Except::throw_exception((const char *)"Pool_CantDeleteController",o.str(),
									     (const char *)"Pool::delete_controller");
		      	}
	      	}
	      	break;
	      	
      		case COM_CTRL:
      		{
//
// Check that we don't have any communication channel left on this controller
//

				list<CommunicationChannelPool>::iterator itm;
				long nb_remain_channel = 0;
				TangoSys_OMemStream o;
					
				for (itm = com_channel_list.begin();
				     itm != com_channel_list.end();++itm)
		  		{
		      		if (itm->ctrl_id == ctrl_to_del.id)
		      		{
						if (nb_remain_channel == 0)
			      		{
				      		o << "Can't delete controller, channel(s) ";
				      		o << itm->name;
			      		}
			      		else
			      		{
				      		o << ", " << itm->name;
			      		}
			      		nb_remain_channel++;
		      		}	
		  		}
		  
		      	if (nb_remain_channel != 0)
		      	{
			      	o << " are still defined using this controller" << ends;
					      
			      	Tango::Except::throw_exception((const char *)"Pool_CantDeleteController",o.str(),
									     (const char *)"Pool::delete_controller");
		      	}
	      	}
	      	break;	      	
	      	
      		case CONSTRAINT_CTRL:
      		{
      			// no devices in constraint controller so nothing to be done.
      		}
      		break;
	      	
			case UNDEF_CTRL:
			Tango::Except::throw_exception((const char *)"Pool_BadCtrlType",
									   	   (const char *)"Undefined controller type !!!",
						  			   	   (const char *)"Pool::delete_controller");
			break;
		}
	}

//
// Starting from this point, we might change the device state
//

	PoolStateEvent pse(this);
													      
//
// Remove its entry in database. First find our infos in the ctrl_info
// vector
//

  	long nb_ctrl = ctrl_list.size();
  	string::size_type pos;
	      
  	pos = ctrl_to_del.name.find('/');
	string ctrl_class = ctrl_to_del.name.substr(0,pos);
	string ctrl_inst = ctrl_to_del.name.substr(pos + 1);
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
      
      	if ((ctrl_class_db == ctrl_class) && (inst_db == ctrl_inst))
      	{
	      	ind_array = l * PROP_BY_CTRL;
	      	break;
      	}
  	}

//
// Remove controller infos in vector
//
      
  	vector<string>::iterator v_ite = ctrl_info.begin();
  	v_ite += ind_array;
  	ctrl_info.erase(v_ite,v_ite+PROP_BY_CTRL);
      
//
// Update database
//

  	Tango::DbDatum ctrl_prop(CTRL_PROP);
  	Tango::DbData db_data;

  	ctrl_prop << ctrl_info;
  	db_data.push_back(ctrl_prop);	      
      
  	try
  	{       
      	get_db_device()->put_property(db_data);
  	}
  	catch (Tango::DevFailed &e)
  	{
      	TangoSys_OMemStream o;
      	o << "Can't update Db for controller with ID " << argin << ends;

      	Tango::Except::re_throw_exception(e,
      			(const char *)"Pool_CantUpdateDb",o.str(),
				(const char *)"Pool::delete_controller");
  	}
  	
//
// Remove all properties of this controller in the database 
//
  	
  	if (ctrl_to_del.ctrl_fica_built == true)
  	{
  		vector<PropertyData *> &props = 
  			(*ctrl_to_del.ite_ctrl_fica)->get_ctrl_prop_list();
  		
  		Tango::DbData ctrl_db_data;
  		
  		for(unsigned long l = 0; l < props.size(); l++)
  		{
  			string final_prop_name = ctrl_to_del.ctrl->get_name() + "/" + 
  									 props[l]->name;
  			ctrl_db_data.push_back(Tango::DbDatum(final_prop_name));
  		}
  		
  	  	Tango::Util *tg = Tango::Util::instance();
  	  	Tango::Database *db = tg->get_database();

  	  	get_db_device()->delete_property(ctrl_db_data);
  	}
  	

//
// Is there any other FiCa using the controller file ?
// If we are the last one, mark the FiCa and the File
// as unused. This will force a reload of the controller
// code next time a controller using this file is created
//

  	PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
  	long fica_f_nb,fica_nb;
  	long ctrl_fica_nb = 0;
  	string f_name;
  	string fica_name_safe;
	      
  	if (ctrl_to_del.ctrl_fica_built == true)
  	{
      	string &fica_name = ctrl_to_del.get_fica_name();
      	fica_name_safe = fica_name;
      	f_name = fica_name.substr(0,fica_name.find('/'));
      	ctrl_fica_nb = get_ctrl_nb_by_fica_name(fica_name);
      	if (ctrl_fica_nb == 1)
      	{ 	      
	      	fica_f_nb = cl_ptr->get_fica_nb_by_f_name(f_name);
	      	fica_nb = cl_ptr->get_fica_nb_by_name(fica_name);
      	}
  	}
				      
//
// Delete controller object and erase controller entry in pool
//

  	if (ctrl_to_del.ctrl != NULL)
  	{
      	AutoPoolLock lo(ctrl_to_del.get_ctrl_fica_mon());
      	PoolUtil::instance()->clean_ctrl_elems(ctrl_to_del.ctrl->get_name());
      	delete ctrl_to_del.ctrl;

		if (ctrl_to_del.cpp_ctrl_prop != NULL)
      		delete ctrl_to_del.cpp_ctrl_prop;
  	}
	      
  	list<ControllerPool>::iterator ctrl_ite;
  	for (ctrl_ite = ctrl_list.begin();ctrl_ite != ctrl_list.end();++ctrl_ite)
  	{
      	if (ctrl_ite->id == ctrl_to_del.id)
      	{
	      	ctrl_list.erase(ctrl_ite);
	      	break;
      	}
  	}
		      
//
// Remove entry in FiCa and File vectors if needed
//

  	if (ctrl_fica_nb == 1)
  	{
      	if (fica_f_nb == 1)
      	{
	      	cl_ptr->remove_fica_by_name(fica_name_safe);
		      
	      	if (fica_nb == 1)
		      	cl_ptr->remove_ctrl_file_by_name(f_name);
      	}
  	}

//
// Before returning, push a change_event to client(s) listening
// on event
//
      
  	Tango::Attribute &cl = dev_attr->get_attr_by_name("ControllerList");
  	read_ControllerList(cl);
  	cl.fire_change_event();

}

//+------------------------------------------------------------------
/**
 *	method:	Pool::init_controller
 *
 *	description:	method to execute "InitController"
 *	Initialiaze a controller. Needed if the pool has been started when a controller was down in order to
 *	connect the controller to the pool without needs to restart the complete pool once it has
 *	been switched up.
 *
 * @param	argin	Controller name
 *
 */
//+------------------------------------------------------------------
void Pool::init_controller(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::init_controller(): entering... !" << endl;

	//	Add your own code to control device here
	
  	
//
// Find controller in controller list
//
  
  	string user_ctrl(argin);
  	
  	ControllerPool &ctrl_to_init = get_ctrl_from_inst_name(user_ctrl);
  	
  	DEBUG_STREAM << "init_ctrl: after get_ctrl_from_inst_name" << endl;
//
// If the controller is marked as OK, do nothing
//
  
  	if (ctrl_to_init.ctrl != NULL)
  	{
  		DEBUG_STREAM << "init_ctrl: ctrl was null. Exiting!" << endl;
  		return;
  	}
//
// If the controller FiCa is not created, throw exception
//
  
  	if (ctrl_to_init.ctrl_fica_built == false)
  	{
  		TangoSys_OMemStream o;
  		o << "Controller code from file " << f_name_from_db_prop(ctrl_to_init.name) << " is not loaded in memory";
  		o << "\nTry the ReloadControllerCode command first" << ends;
  		
  		Tango::Except::throw_exception((const char *)"Pool_ControllerNotLoaded",o.str(),
  					       (const char *)"Pool::init_controller");
  	}
  
//
// Starting from this point, we might change the device state
//
  
  	PoolStateEvent pse(this);
  	DEBUG_STREAM << "init_ctrl: after poolstateevent" << endl;
//
// Try to re-create controller object
//
  
  	Controller *ct;
  	vector<CtrlFiCa *>::iterator ctrl_fica_ite = ctrl_to_init.ite_ctrl_fica;
  	Language lang = (*ctrl_fica_ite)->get_language();
  
  	string::size_type pos = ctrl_to_init.name.find('/');
  	pos++;
  	string inst_name_lower = ctrl_to_init.name.substr(pos);
  	
  	CtrlType obj_type = ctrl_to_init.get_ctrl_obj_type();
  	
  	if (lang == CPP)
  	{	
  		
//
// Retrieve the controller object creator C function
//
  	
  		lt_ptr sym;
  	
  		string sym_name("_create_");
  		sym_name = sym_name + ctrl_to_init.ctrl_class_name;
  		
  		DEBUG_STREAM << "Symbol name = " << sym_name << endl;
  			
  		sym = lt_dlsym((*ctrl_fica_ite)->get_lib_ptr(),sym_name.c_str());
  		if (sym == NULL)
  		{
  			TangoSys_OMemStream o;
  			o << "Controller library " << f_name_from_db_prop(ctrl_to_init.name) << " does not have the C creator function (_create_<Controller name>)" << ends;
  			
  			Tango::Except::throw_exception((const char *)"Pool_CCreatorFunctionNotFound",o.str(),
  						       (const char *)"Pool::create_controller");
  		}
  		
  		DEBUG_STREAM << "lt_dlsym is a success" << endl;
  
//
// Create the controller
//
  	
  		Ctrl_creator_ptr ct_ptr = (Ctrl_creator_ptr)sym;
  		vector<Controller::Properties> *prop = NULL;
  				
  		try
  		{
  			vector<pair<string,string> > prop_pairs;
  			AutoPoolLock lo((*ctrl_fica_ite)->get_mon());
			build_property_data(inst_name_lower,ctrl_to_init.ctrl_class_name,
								prop_pairs,
								(*ctrl_fica_ite)->get_ctrl_prop_list());
			check_property_data((*ctrl_fica_ite)->get_ctrl_prop_list());
  			prop = 
  				properties_2_cpp_vect((*ctrl_fica_ite)->get_ctrl_prop_list());
  			ct = (*ct_ptr)(inst_name_lower.c_str(),*prop);
 			ctrl_to_init.cpp_ctrl_prop = prop;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (prop != NULL)
  				delete prop;
  				
  			Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateController",
  					(const char *)"Can't create controller!!!",
  					(const char *)"Pool::create_controller");
  		}
  	}
  	else
  	{
  		
//
// Retrieve the controller object creator C function
//
  	
  		lt_ptr sym;

  		string sym_name("_create_Py");
  		
  		CtrlType type = (*ctrl_fica_ite)->get_obj_type();
  		PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
  		string type_str = cl_ptr->CtrlType_2_str(type);
  		
  		sym_name = sym_name + type_str;
  		
  		if(obj_type != CONSTRAINT_CTRL)
  			sym_name += "Controller";
  		
  		DEBUG_STREAM << "Symbol name = " << sym_name << endl;
  			
  		sym = lt_dlsym((*ctrl_fica_ite)->get_py_inter_lib_ptr(),sym_name.c_str());
  		if (sym == NULL)
  		{
  			TangoSys_OMemStream o;
  			o << "Controller library " << f_name_from_db_prop(ctrl_to_init.name); 
  			o << " does not have the C creator function "
  				 "(_create_<Controller name>)" << ends;
  			
  			Tango::Except::throw_exception(
  					(const char *)"Pool_CCreatorFunctionNotFound",o.str(),
  					(const char *)"Pool::create_controller");
  		}
  		
  		DEBUG_STREAM << "lt_dlsym is a success" << endl;
  		
//
// Create the Python controller object but before re-read the property
//
  	
  		PyCtrl_creator_ptr ct_ptr = (PyCtrl_creator_ptr)sym;
  		try
  		{
  			vector<pair<string,string> > prop_pairs;
  			AutoPoolLock lo((*ctrl_fica_ite)->get_mon());
			build_property_data(inst_name_lower,ctrl_to_init.ctrl_class_name,
								prop_pairs,
								(*ctrl_fica_ite)->get_ctrl_prop_list());
			check_property_data((*ctrl_fica_ite)->get_ctrl_prop_list());
  			PyObject *prop_dict = 
  				properties_2_py_dict((*ctrl_fica_ite)->get_ctrl_prop_list());
  			ct = (*ct_ptr)(inst_name_lower.c_str(),
  						   ctrl_to_init.ctrl_class_name.c_str(),
  						   (*ctrl_fica_ite)->get_py_module(),prop_dict);
  		}
  		catch (Tango::DevFailed &e)
  		{
  			Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateController",
  					(const char *)"Can't create controller!!!",
  					(const char *)"Pool::create_controller");
  		}
  	}
  			
//
// Update controller object ptr and the possibly changed MaxDevice prop
//
  
  	ctrl_to_init.ctrl = ct;

	vct_ite	tmp_ite = ctrl_to_init.ite_ctrl_fica;

//
// Send a Init command to all objects belonging to this controller
// Warning: The object init command delete and re-insert entry
// in the object list. Take care of this in iterator management
//

				
	if (obj_type == MOTOR_CTRL)
	{	  
	  	list<MotorPool>::iterator itm;
	  	for (itm = mot_list.begin();itm != mot_list.end();++itm)
	  	{
	  		if (itm->ctrl_id == ctrl_to_init.id)
	  		{
	  			long dist = distance(mot_list.begin(),itm);
//
// Get the listeners
//	  			
	  			list<IPoolElementListener*> listeners = itm->pool_elem_listeners;
	  			itm->obj_proxy->command_inout("Init");
	  			itm = mot_list.begin();
	  			advance(itm,dist);
//
// Restore the listeners to the new MotorPool object
//	  			
	  			itm->pool_elem_listeners = listeners;
	  			
//
// Inform the listeners that the motor changed structure
//	
				if(itm->has_listeners())
				{
					PoolElementEvent evt(ElementStructureChange,&(*itm));
					itm->fire_pool_elem_change(&evt);
				}
	  		}
	  	}
	}
	else if (obj_type == COTI_CTRL)
	{
	  	list<CTExpChannelPool>::iterator itm;
	  	for (itm = ct_list.begin();itm != ct_list.end();++itm)
	  	{
	  		if (itm->ctrl_id == ctrl_to_init.id)
	  		{
	  			long dist = distance(ct_list.begin(),itm);
//
// Get the listeners
//	  			
	  			list<IPoolElementListener*> listeners = itm->pool_elem_listeners;
	  			itm->obj_proxy->command_inout("Init");
	  			itm = ct_list.begin();
	  			advance(itm,dist);
//
// Restore the listeners to the new CTPool object
//	  			
	  			itm->pool_elem_listeners = listeners;
	  			
//
// Inform the listeners that the channel changed structure
//	
				if(itm->has_listeners())
				{
					PoolElementEvent evt(ElementStructureChange,&(*itm));
					itm->fire_pool_elem_change(&evt);
				}	  			
	  		}
	  	}
	}
	else if (obj_type == ZEROD_CTRL)
	{
	  	list<ZeroDExpChannelPool>::iterator itm;
	  	for (itm = zerod_list.begin();itm != zerod_list.end();++itm)
	  	{
	  		if (itm->ctrl_id == ctrl_to_init.id)
	  		{
//
// Get the listeners
//	  			
	  			list<IPoolElementListener*> listeners = itm->pool_elem_listeners;
	  			long dist = distance(zerod_list.begin(),itm);
	  			itm->obj_proxy->command_inout("Init");  					
	  			itm = zerod_list.begin();
	  			advance(itm,dist);
//
// Restore the listeners to the new 0D Pool object
//	  			
	  			itm->pool_elem_listeners = listeners;
	  			
//
// Inform the listeners that the channel changed structure
//	
				if(itm->has_listeners())
				{
					PoolElementEvent evt(ElementStructureChange,&(*itm));
					itm->fire_pool_elem_change(&evt);
				}	  		  			
	  		}
	  	}
	}
  
//
// The object Init command clears the object proxy. Mark the
// proxy created flag as false to recreate the proxy
//
  
  	proxy_created = false;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::send_to_controller
 *
 *	description:	method to execute "SendToController"
 *	This commands sends an uninterpreted string to a controller. The first input argument is the controller
 *	name. The second input argument is the string to be sent to the controller.
 *	It returns the controller answer without any interpretation.
 *
 * @param	argin	In[0] = Controller name, In[1] = String to send
 * @return	The controller answer
 *
 */
//+------------------------------------------------------------------
Tango::DevString Pool::send_to_controller(const Tango::DevVarStringArray *argin)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter : Writing a TANGO DS / Exchanging data)
	//------------------------------------------------------------
	Tango::DevString	argout;
	DEBUG_STREAM << "Pool::send_to_controller(): entering... !" << endl;

	//	Add your own code to control device here
	
//
// Check input args
//

	if (argin->length() != 2)
	{
		Tango::Except::throw_exception((const char *)"Pool_WrongArgument",
						(const char *)"Wrong number of arguments. Two arguments required",
		        		(const char *)"Pool::send_to_controller()");
	}
	
	string ctrl_name((*argin)[0]);
	
//
// Check that we have this controller
//

	ControllerPool &ctl = get_ctrl_from_inst_name(ctrl_name);
	
//
// Check that ctrl is OK
//

	if (ctl.ctrl == NULL)
	{
		TangoSys_OMemStream o;
		o << "Controller " << ctrl_name << " is not valid" << ends;

		Tango::Except::throw_exception((const char *)"Pool_WrongArgument",o.str(),
		        	(const char *)"Pool::send_to_controller()");
	}

//
// Send string to ctrl
//

	string sent((*argin)[1]);
	string received;

	{
		AutoPoolLock lo(ctl.get_ctrl_fica_mon());
		received = ctl.ctrl->SendToCtrl(sent);
	}
	
	argout = new char[received.size() + 1];
	strcpy(argout,received.c_str());
	
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::get_controller_info
 *
 *	description:	method to execute "GetControllerInfo"
 *	Get controller class data like parameters description from the controller class name
 *
 * @param	argin	in[0] - Controller type, in[1] - Controller file name, in[2] - Controller class name, in[3] - Controller instance name (optional)
 * @return	Controller class data
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *Pool::get_controller_info(const Tango::DevVarStringArray *argin)
{
	//	Add your own code to control device here

//
// Get info from controller
//
	vector<string> info;
	get_ctrl_info(argin, info);

//
// Return info to caller
//
	Tango::DevVarStringArray *argout = new Tango::DevVarStringArray();
	argout->length(info.size());
	
	for(unsigned int i = 0;i < info.size();i++)
		(*argout)[i] = CORBA::string_dup(const_cast<char *>(info[i].c_str()));
	
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::get_controller_info_ex
 *
 *	description:	method to execute "GetControllerInfoEx"
 *	Get controller class extra data like parameters description from the controller class name
 *
 * @param	argin	in[0] - Controller type, in[1] - Controller file name, in[2] - Controller class name
 * @return	Controller class data
 *
 */
//+------------------------------------------------------------------
Tango::DevVarCharArray *Pool::get_controller_info_ex(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "Pool::get_controller_info_ex(): entering... !" << endl;

	//	Add your own code to control device here

//
// Get info from controller
//
	vector<string> info;
	Tango::DevVarCharArray *info_ex = new Tango::DevVarCharArray();
	get_ctrl_info(argin, info, info_ex);

//
// Return info to caller
//
	return info_ex;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::get_controller_info
 *
 *	description:	method to execute "GetControllerInfo"/"GetControllerInfoEx"
 *	Get controller class data like parameters description from the controller class name
 *
 * @param	argin	in[0] - Controller type, 
 * 					in[1] - Controller file name, 
 * 					in[2] - Controller class name, 
 * 					in[3] - Controller instance name (optional)
 * @return	Controller class data
 *
 */
//+------------------------------------------------------------------
void Pool::get_ctrl_info(const Tango::DevVarStringArray *argin, 
						 vector<string> &info, Tango::DevVarCharArray *info_ex)
{

//
// Check if the user request for instance info or class info
//

	string instance_name;
	bool instance_info = false;

	long nb_args = argin->length();
		
	if (nb_args == 4)
	{
		instance_name = (*argin)[3];
		instance_info = true;
	}
	else if ((nb_args < 3) || (nb_args > 4))
	{
  		Tango::Except::throw_exception(
  			(const char *)"Pool_WrongArgumentNumber",
  			(const char *)"Wrong number of argument for command GetControllerInfo."
  						  " Needs 3 or 4 strings",
  			(const char *)"Pool::get_controller_info");
	}
			
	string ctrl_type = (*argin)[0].in();
	string module_name = (*argin)[1].in();
	string class_name = (*argin)[2].in();
	string f_name;

//
// Check the module (file) name
//

	string::size_type pos;
	
  	if ((pos = module_name.find('.')) == string::npos)
  	{
  		TangoSys_OMemStream o;
  		o << module_name << " does not have file type extension.";
  		o << "Please define one (.py or .la)" << ends;
  
  		Tango::Except::throw_exception(
  			(const char *)"Pool_FileUnsupportedType",o.str(),
  			(const char *)"Pool::get_controller_info");
  	}
  	else
  	{
  		string file_ext = module_name.substr(pos);
  		
  		if ((file_ext == ".la") || (file_ext == ".py")) ;
  		else
  		{
  			TangoSys_OMemStream o;
  			o << "File " << module_name << " is from one unsupported type"; 
  			o << ends;
  
  			Tango::Except::throw_exception(
  				(const char *)"Pool_FileUnsupportedType",o.str(),
  			    (const char *)"Pool::get_controller_info");
  		}
  	}
	
	PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
	
//
// Check the controller type
//

  	transform(ctrl_type.begin(),ctrl_type.end(),ctrl_type.begin(),::tolower);
  	ctrl_type[0] = ::toupper(ctrl_type[0]);
  	CtrlType type = cl_ptr->str_2_CtrlType(ctrl_type);
		
//
// Check that we know this class
//

	Language lang;
	if(find_file_in_pool_path(module_name,f_name,PYTHON) == false)
	{
		if(find_file_in_pool_path(module_name,f_name,CPP) == false)
		{
			TangoSys_OMemStream o;
			o << "The " << module_name << " module or file could not be found.";
			o << " Please check that PoolPath property contains the correct "
				 "path." << ends;
			
			Tango::Except::throw_exception(
				(const char *)"Pool_CantLocateControllerFile",
				o.str(),
		        (const char *)"Pool::get_controller_class_info");
		}
		else
			lang = CPP;
	}
	else
		lang = PYTHON;

//
// First check if the Pool device contains the controller file
//

	bool local_ctrl_file_found = false;
	vcf_ite ctrl_f;
	
	try 
	{
		string::size_type pos = f_name.rfind('/');
		string tmp_name = f_name.substr(pos + 1);
		ctrl_f = cl_ptr->get_ctrl_file_by_name(tmp_name);
		local_ctrl_file_found = true;
	}
	catch (Tango::DevFailed &e)
	{
	}
	
//
// Trying to get instance information for a controller file that has not even 
// been loaded yet is an error
//
	if(local_ctrl_file_found == false && instance_info == true)
	{
		TangoSys_OMemStream o;
		o << "The controller instance " << (*argin)[3].in();
		o << " does not exist" << ends;
		Tango::Except::throw_exception(
			(const char *)"Pool_CantLocateController",o.str(),
			(const char *)"Pool::get_controller_info");
	}	
	
	if(instance_info == false)
	{	
//
// Build a temporary xxxCtrlFile object even if the controller already exists.
// This is done because the file may have been changed and we want to return
// always the latest information.
//
		CtrlFile *tmp_ctrl_file = NULL;
			
		if (lang == PYTHON)
		{
			switch(type)
			{
				case MOTOR_CTRL:
				tmp_ctrl_file = new PyMotCtrlFile(f_name,cl_ptr,this);
				break;

				case PSEUDO_MOTOR_CTRL:
				tmp_ctrl_file = new PyPseudoMotCtrlFile(f_name,cl_ptr,this);
				break;

				case PSEUDO_COUNTER_CTRL:
				tmp_ctrl_file = new PyPseudoCoCtrlFile(f_name,cl_ptr,this);
				break;
				
				case COTI_CTRL:
				tmp_ctrl_file = new PyCoTiCtrlFile(f_name,cl_ptr,this);
				break;
				
				case ZEROD_CTRL:
				tmp_ctrl_file = new PyZeroDCtrlFile(f_name,cl_ptr,this);
				break;
				
				case COM_CTRL:
				tmp_ctrl_file = new PyComCtrlFile(f_name,cl_ptr,this);
				break;
				
				case CONSTRAINT_CTRL:
				tmp_ctrl_file = new PyConstraintFile(f_name,cl_ptr,this);
				break;
				
				case UNDEF_CTRL:
				Tango::Except::throw_exception(
						(const char *)"Pool_BadCtrlType",
						(const char *)"Undefined controller type !!!",
						(const char *)"Pool::get_controller_info");
				break;
			}
		}
		else
		{
			switch(type)
			{
				case MOTOR_CTRL:
				tmp_ctrl_file = new CppMotCtrlFile(f_name,cl_ptr,this);
				break;

				case PSEUDO_MOTOR_CTRL:
				tmp_ctrl_file = new CppPseudoMotCtrlFile(f_name,cl_ptr,this);
				break;
				
				case PSEUDO_COUNTER_CTRL:
				tmp_ctrl_file = new CppPseudoCoCtrlFile(f_name,cl_ptr,this);
				break;

				case COTI_CTRL:
				tmp_ctrl_file = new CppCoTiCtrlFile(f_name,cl_ptr,this);
				break;
				
				case ZEROD_CTRL:
				tmp_ctrl_file = new CppZeroDCtrlFile(f_name,cl_ptr,this);
				break;
				
				case COM_CTRL:
				tmp_ctrl_file = new CppComCtrlFile(f_name,cl_ptr,this);
				break;

				case CONSTRAINT_CTRL:
				tmp_ctrl_file = new CppConstraintFile(f_name,cl_ptr,this);
				break;

				case UNDEF_CTRL:
				Tango::Except::throw_exception(
						(const char *)"Pool_BadCtrlType",
						(const char *)"Undefined controller type !!!",
						(const char *)"Pool::get_controller_info");
				break;
			}
		}
		
//
// Get the number of controller class defined in this file
//

		long nb_class;
		vector<string> tmp_ctrl_list;
		vector<string> ctrl_type_list;
		try
		{
			nb_class = tmp_ctrl_file->get_classes(tmp_ctrl_list,ctrl_type_list);
		}
		catch (Tango::DevFailed &e)
		{
			TangoSys_OMemStream o;
			o << "The class " << class_name << " does not exist ";
			o << "(or is not a valid controller) in file " << f_name << ends;
			Tango::Except::throw_exception(
				(const char *)"Pool_BadArgument",o.str(),
			    (const char *)"Pool::get_controller_info");
		}

//
// Check that the user class is one of the class found in the file
//
		
		if (nb_class == 0)
		{
			TangoSys_OMemStream o;
			o << "The class " << class_name << " does not exist ";
			o << "(or is not a valid controller) in file " << f_name << ends;
			Tango::Except::throw_exception(
				(const char *)"Pool_BadArgument",o.str(),
			    (const char *)"Pool::get_controller_info");
		}
		long cl_ind;
		for (cl_ind = 0;cl_ind < nb_class;cl_ind++)
		{
			string::size_type pos = tmp_ctrl_list[cl_ind].find('.');
			string class_wo_mod;
			if (pos != string::npos)
				class_wo_mod = tmp_ctrl_list[cl_ind].substr(pos + 1);
			else
				class_wo_mod = tmp_ctrl_list[cl_ind];
			if (class_wo_mod == class_name)
				break;
		}
		if (cl_ind == nb_class)
		{
			TangoSys_OMemStream o;
			o << "The class " << class_name << " does not exist ";
			o << "(or is not a valid controller) in file " << f_name << ends;
			Tango::Except::throw_exception(
				(const char *)"Pool_BadArgument",o.str(),
				(const char *)"Pool::get_controller_info");
		}
//
// Get class info
//		
		if(info_ex == NULL)
			tmp_ctrl_file->get_info(class_name,info);
		else
			tmp_ctrl_file->get_info_ex(class_name,info_ex);
		
		delete tmp_ctrl_file;
	}
//
// In case the information about an instance is requested and 
// the controller is already loaded
//
	else
	{			
		ControllerPool *ctrl_in_pool;
		try
		{
			ctrl_in_pool = &get_ctrl_from_inst_name(instance_name);
		}
		catch (Tango::DevFailed &e)
		{
			TangoSys_OMemStream o;
			o << "The controller instance " << instance_name;
			o << " does not exist" << ends;
			Tango::Except::throw_exception(
					(const char *)"Pool_CantLocateControllerInstance",o.str(),
					(const char *)"Pool::get_controller_info");
		}	
			
		if(ctrl_in_pool->ctrl_class_name != class_name)
		{
			TangoSys_OMemStream o;
			o << "The controller instance " << instance_name;
			o << " does not exist for the class " << module_name << ends;
			Tango::Except::throw_exception(
					(const char *)"Pool_CantLocateControllerInstance",o.str(),
		    	    (const char *)"Pool::get_controller_info");
		}
		vct_ite ite = ctrl_in_pool->ite_ctrl_fica;
		
		if(info_ex == NULL)
			(*ite)->get_ctrl_file()->get_info(class_name,instance_name,info);
		else
			(*ite)->get_ctrl_file()->get_info_ex(class_name,instance_name,info_ex);
	}
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_exp_channel
 *
 *	description:	method to execute "CreateExpChannel"
 *	This command creates a new experiment channel in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The experiment channel number within the controller
 *	3 - The experiment channel name. The experiment channel name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"ctexp/controller_instance_name/ctrl_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created experiment channels are automatically re-created at pool device startup time.
 *
 * @param	argin	long[0] = Exp Channel number in Ctrl, string[0] = Exp Channel name, string[1] = Controller instance name
 *
 */
//+------------------------------------------------------------------
void Pool::create_exp_channel(const Tango::DevVarLongStringArray *argin)
{
	DEBUG_STREAM << "Pool::create_exp_channel(): entering... !" << endl;

	//	Add your own code to control device here
	
  	if ((argin->lvalue.length() != 1) || (argin->svalue.length() != 2))
  	{
  		Tango::Except::throw_exception((const char *)"Pool_WrongArgumentNumber",
  						(const char *)"Wrong number of argument for command CreateExpChannel. Needs 1 long and 2 strings",
  		        		(const char *)"Pool::create_exp_channel");
  	}		
  	
  	Tango::DevLong channel_idx = argin->lvalue[0];
  	string ctrl_inst_name(argin->svalue[1]);
  	string channel_name(argin->svalue[0]);
  	ControllerPool &channel_ctrl = get_ctrl_from_inst_name(ctrl_inst_name);
  	long ctrl_id = channel_ctrl.id;
  	
  	DEBUG_STREAM << "Controller ID = " << ctrl_id << endl;
  	DEBUG_STREAM << "Motor index = " << channel_idx << endl;
  	DEBUG_STREAM << "Motor name = " << channel_name << endl;
  
//
// Check if the controller has been successfully constructed
//
  
  	if (channel_ctrl.ctrl == NULL)
  	{
  		Tango::Except::throw_exception((const char *)"Pool_WrongControllerId",
  						(const char *)"Can't create an experiment channel on a non-responding controller",
  				        (const char *)"Pool::create_exp_channel");
  	}

	CtrlType channel_type = channel_ctrl.get_ctrl_obj_type();
	  
//
// Check if we don't have already  enough experiment channel
//
  
  	DEBUG_STREAM << "Checking exp channel number" << endl;

	switch (channel_type)
	{
		case COTI_CTRL:
	  	if (ct_list.size() == MAX_CT)
	  	{
	  		Tango::Except::throw_exception((const char *)"Pool_TooManyCounterTimer",
	  					       (const char *)"Too many CounterTimer in your pool",
	  					       (const char *)"Pool::create_exp_channel");
	  	}
		break;
		
		case ZEROD_CTRL:
	  	if (zerod_list.size() == MAX_ZEROD)
	  	{
	  		Tango::Except::throw_exception((const char *)"Pool_TooManyZerodExpChannel",
	  					       (const char *)"Too many Zero D Experiment Channel in your pool",
	  					       (const char *)"Pool::create_exp_channel");
	  	}
		break;
		
		case UNDEF_CTRL:
		Tango::Except::throw_exception((const char *)"Pool_BadCtrlType",
									   (const char *)"Undefined controller type !!!",
						  			   (const char *)"Pool::create_exp_channel");
		break;
		
		case MOTOR_CTRL:
		Tango::Except::throw_exception((const char *)"Pool_BadCtrlType",
									   (const char *)"Cannot create an experiment channel from a motor controller !!!",
									   (const char *)"Pool::create_exp_channel");
		break;

		case CONSTRAINT_CTRL:
		Tango::Except::throw_exception((const char *)"Pool_BadCtrlType",
									   (const char *)"Cannot create an experiment channel from a constraint !!!",
									   (const char *)"Pool::create_exp_channel");
		break;
		
	}
		
//
// Check that the controller still have some experiment channel available
//
  
  	if (channel_ctrl.nb_dev == channel_ctrl.MaxDevice)
  	{
  		TangoSys_OMemStream o;
  		o << "Max number of experiment channel reached (" << channel_ctrl.MaxDevice << ")" << ends;
  		
  		Tango::Except::throw_exception((const char *)"Pool_MaxNbExpChannelInCtrl",o.str(),
  					       			   (const char *)"Pool::create_exp_channel()");
  	}
  		
//
// Build Tango device name
//
  
  	stringstream s;
  	s << channel_idx;
  
  	string tg_dev_name("expchan");
  	string tmp_str(channel_ctrl.name);
  	tmp_str.erase(0,tmp_str.find('/'));
  	tg_dev_name = tg_dev_name + tmp_str + '/' + s.str();	
  	
  	DEBUG_STREAM << "Tango device name = " << tg_dev_name << endl;
  			
//
// Check if this device is already defined in database
// Check by device alias and by Tango device name
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	Tango::Database *db = tg->get_database();
  	
  	Tango::DbDevImportInfo my_device_import;
  	bool device_exist = false;
  	bool by_alias = false;	
  
  	try
  	{
  		my_device_import = db->import_device(channel_name);
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
  			my_device_import = db->import_device(tg_dev_name);
  			device_exist = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
  			{
  				device_exist = true;
  			}
  		}
  	}
  	
  	if (device_exist == true)
  	{
  		TangoSys_OMemStream o;
  		if (by_alias == false)
  			o << "Experiment channel " << channel_idx << " on controller " << ctrl_inst_name << " already defined" << ends;
  		else
  			o << "Experiment channel " << channel_name << " is already defined" << ends;
  
  		Tango::Except::throw_exception((const char *)"Pool_WrongExpChannelName",o.str(),
  				        (const char *)"Pool::create_exp_channel");
  	}
  
//
// If the device is not defined in database, create it in database, set its alias,
// define the property used to store its ID (called motor_id) and create the default
// abs_change property for the Position attribute
//
  
  	if (device_exist == false)
  	{
  		DEBUG_STREAM << "Trying to create device entry in database" << endl;
  		
      	try
  		{
  			Tango::DbDevInfo my_device_info;
      		my_device_info.name = tg_dev_name.c_str();
      		if (channel_type == COTI_CTRL)
			{
      			my_device_info._class = "CTExpChannel";
			}
			else if (channel_type == ZEROD_CTRL)
			{      			
      			my_device_info._class = "ZeroDExpChannel";
			}
			
      		my_device_info.server = tg->get_ds_name().c_str();
  			
      		db->add_device(my_device_info);
      		db->put_device_alias(tg_dev_name,channel_name);
  
  			Tango::DbDatum id(EXP_CHANNEL_ID_PROP);
  			Tango::DbData db_data;
  			exp_channel_id++;
  			id  << exp_channel_id;
  			db_data.push_back(id);
  			db->put_device_property(tg_dev_name.c_str(),db_data);

      		if (channel_type == COTI_CTRL)
			{
				Tango::DbDatum pos("Value"),abs_ch("abs_change");
				db_data.clear();
				pos << (long)1;
				abs_ch << defaultCtVal_AbsChange;
				db_data.push_back(pos);
				db_data.push_back(abs_ch);
			}
			else if (channel_type == ZEROD_CTRL)
			{
				Tango::DbDatum pos("CumulatedValue"),abs_ch("abs_change");
				db_data.clear();
				pos << (long)1;
				abs_ch << defaultZeroDVal_AbsChange;
				db_data.push_back(pos);
				db_data.push_back(abs_ch);
			}
			db->put_device_attribute_property(tg_dev_name.c_str(),db_data);
  						
  			DEBUG_STREAM << "Device created in database (with alias)" << endl;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			DEBUG_STREAM << "Gasp an exception........" << endl;
  			TangoSys_OMemStream o;
  			o << "Can't create experiment channel " << channel_name << " in database" << ends;
  			
  			Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateExpChannel",o.str(),
  							  (const char *)"Pool::create_exp_channel");
  		}
  	
//
// Find the Tango Experiment channel class and create the device
//
  
  		string cl_name;
		if (channel_type == COTI_CTRL)
		{
			cl_name = "CTExpChannel";
		}
		else if (channel_type == ZEROD_CTRL)
		{
			cl_name = "ZeroDExpChannel";
		}
		
  		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
  		for (unsigned long i = 0;i < cl_list->size();i++)
  		{ 			
  			if ((*cl_list)[i]->get_name() == cl_name)
  			{
  				try
  				{
  					Tango::DevVarStringArray na;
  					na.length(1);
  					na[0] = tg_dev_name.c_str();
  					(*cl_list)[i]->device_factory(&na);
  					break;
  				}
  				catch (Tango::DevFailed &e)
  				{
  					DEBUG_STREAM << "Exception while trying to create Experiment Channel device" << endl;
 
//
// Check if this experiment channel has already been added into pool structures
// If yes, remove it from pool structures
//

					try
					{
						if (channel_type == COTI_CTRL)
						{
							CTExpChannelPool &failed_ct = get_ct_from_name(channel_name);
							remove_ct(failed_ct.ct_channel);
						}
						else if (channel_type == ZEROD_CTRL)	
						{
							ZeroDExpChannelPool &failed_zerod = get_zerod_from_name(channel_name);
							remove_zerod(failed_zerod.zerod_channel);
						}
					}
					catch (Tango::DevFailed &ee) {}

//
// The delete_device will also delete device property(ies)
//
  					
  					db->delete_device(tg_dev_name);
  					
  					TangoSys_OMemStream o;
  					o << "Can't create experiment channel device " << channel_name << ends;
  			
  					Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateExpChannel",o.str(),
  							  (const char *)"Pool::create_exp_channel");
  				}
  			}
  		}
  
//
// Create a Tango device proxy on the newly created experiment channel
// and set its connection to automatic re-connection
//
 
 		Tango::DeviceProxy *tmp_dev;
 		if (channel_type == COTI_CTRL)
 		{
  			tmp_dev = new Tango::DeviceProxy(ct_list.back().obj_tango_name);
  			tmp_dev->set_transparency_reconnection(true);
  			ct_list.back().obj_proxy = tmp_dev;
 		}
 		else if (channel_type == ZEROD_CTRL)
  		{	
  			tmp_dev = new Tango::DeviceProxy(zerod_list.back().obj_tango_name);
  			tmp_dev->set_transparency_reconnection(true);
  			zerod_list.back().obj_proxy = tmp_dev;
 		}
  	}
	
//
// Inform ghost group that there is a new channel
//
	
	MeasurementGroup_ns::MeasurementGroup *ghost_ptr = get_ghost_measurement_group_ptr();
	{
		Tango::AutoTangoMonitor atm(ghost_ptr);
		if (channel_type == COTI_CTRL)
			ghost_ptr->add_ct_to_ghost_group(exp_channel_id);
		else if (channel_type == ZEROD_CTRL)
			ghost_ptr->add_zerod_to_ghost_group(exp_channel_id);
		//TODO: uncomment when done
		//else if (channel_type == ONED_CTRL)
		//	ghost_ptr->add_oned_to_ghost_group(exp_channel_id);
		//else if (channel_type == TWOD_CTRL)
		//	ghost_ptr->add_twod_to_ghost_group(exp_channel_id);
	}
	
//
// Push a change event for client listenning on event
//

	Tango::Attribute &ecl = dev_attr->get_attr_by_name("ExpChannelList");
	read_ExpChannelList(ecl);
	ecl.fire_change_event();

}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_exp_channel
 *
 *	description:	method to execute "DeleteExpChannel"
 *	Delete a experiment channel from its name
 *	Once an experiment channel is deleted, it is not available any more. All its information have been
 *	removed from the database
 *
 * @param	argin	Exp Channel name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_exp_channel(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_exp_channel(): entering... !" << endl;

	//	Add your own code to control device here
	
//
// Find experiment channel in exp channel list
//

	string user_name(argin);
	PoolElement &channel_to_del =  get_exp_channel_from_name(user_name);
  		
  	DEBUG_STREAM << "Experiment Channel found" << endl;
  			
//
// Check that the exp channel is not actually used
//
  
  	if (channel_to_del.obj_proxy->state() == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete experiment channel with name " << argin;
  		o << ". It is actually used (counting/acquiring)." << ends;
  				
  		Tango::Except::throw_exception((const char *)"Pool_CantDeleteExpChannel",o.str(),
  								 (const char *)"Pool::delete_exp_channel");
  	}
  	
//
// Get experiment channel type
//

	ControllerPool &cp = get_ctrl_from_id(channel_to_del.ctrl_id);
	CtrlType channel_type = cp.get_ctrl_obj_type();

//
// If this command is used after a Stop (Abort) command, check that the pool
// internal thread has finished its job
//

  	if (channel_type == COTI_CTRL)
  	{
  		CTExpChannelPool &ct = static_cast<CTExpChannelPool &>(channel_to_del);
  		
		long wait_ctr = 0;
		struct timespec wait,rem;
		
		wait.tv_sec = 0;
		wait.tv_nsec = 10000000;
	
		while (ct.ct_channel->get_mov_th_id() != 0)
		{
			nanosleep(&wait,&rem);
			wait_ctr++;
			
			if (wait_ctr == 3)
			{
	  			TangoSys_OMemStream o;
	  			o << "Can't delete experiment channel with name " << argin;
	  			o << ". The pool internal thread still uses it" << ends;
	  				
	  			Tango::Except::throw_exception((const char *)"Pool_CantDeleteExpChannel",o.str(),
	  								 			(const char *)"Pool::delete_exp_channel");
			}
		}
	}
	  
//
// Check that the experiment channel is not member of a measurement group
//
  
  	list<MeasurementGroupPool>::iterator grp_ite;
  	for (grp_ite = measurement_group_list.begin();grp_ite != measurement_group_list.end();++grp_ite)
  	{
  		if (find(grp_ite->ch_ids.begin(),grp_ite->ch_ids.end(),channel_to_del.id) != grp_ite->ch_ids.end())
  		{
  			TangoSys_OMemStream o;
  			o << "Can't delete channel with name " << argin;
  			o << ". It is actually member of measurement group " << grp_ite->name << ends;
  					
  			Tango::Except::throw_exception((const char *)"Pool_CantDeleteChannel",o.str(),
  									 (const char *)"Pool::delete_channel");
  		}
  	}

									
//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	try
  	{
  		tg->get_database()->delete_device(channel_to_del.obj_tango_name);
  			
//
// Delete experiment channel device from server but first find its Tango xxxClass instance
//
  
  		Tango::DeviceClass *dc;
  		if (channel_type == COTI_CTRL)
  		{
  			CTExpChannelPool &ct_to_del =  get_ct_from_name(user_name);
  			dc = ct_to_del.ct_channel->get_device_class();				
  		}
  		else if (channel_type == ZEROD_CTRL)
  		{
  			ZeroDExpChannelPool &zerod_to_del =  get_zerod_from_name(user_name);
  			dc = zerod_to_del.zerod_channel->get_device_class();				
  		}
  		
  		vector<DeviceImpl *> &dev_list = dc->get_device_list();
  		dc->device_destroyer(channel_to_del.obj_tango_name);
  	}
  	catch (Tango::DevFailed &e)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete experiment channel with name " << argin << ends;
  				
  		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeleteExpChannel",o.str(),
  								 (const char *)"Pool::delete_exp_channel");
  	}
	  		
//
// Before returning, send a change event for client listenning
// on event
//
  
  	Tango::Attribute &ecl = dev_attr->get_attr_by_name("ExpChannelList");
  	read_ExpChannelList(ecl);
  	ecl.fire_change_event();

}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_measurement_group
 *
 *	description:	method to execute "CreateMeasurementGroup"
 *	This command creates a measurement group. The name of the measurement group is the first element in the input
 *	argument followed by the group element names.
 *
 * @param	argin	Measurement Group name followed by names of the elements
 *
 */
//+------------------------------------------------------------------
void Pool::create_measurement_group(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "Pool::create_measurement_group(): entering... !" << endl;

	//	Add your own code to control device here
	
//
// Basic check on input parameters
//
  	string group_name((*argin)[0]);
  	long input_nb = argin->length() - 1;
  	
  	if (input_nb <= 0)
  	{
  		TangoSys_OMemStream o;
  		o << "Cant create measurement group " << group_name << ". "; 
  		o << "You haven't defined any data aquisition member" << ends;
  
  		Tango::Except::throw_exception(
  				(const char *)"Pool_BadArgument",o.str(),
  				(const char *)"Pool::create_measurement_group");
  	}
  	
  	DEBUG_STREAM << "Measurement Group name = " << group_name;
  	
//
// Check if we don't have already enough measurement groups
//
  
  	DEBUG_STREAM << "Checking measurement group number" << endl;
  	
  	if (measurement_group_list.size() == MAX_MEASUREMENT_GROUP)
  	{
  		Tango::Except::throw_exception((const char *)"Pool_TooManyMeasurementGroup",
  					       (const char *)"Too many measurement groups in your pool",
  					       (const char *)"Pool::create_measurement_group");
  	}  	
  	
  	vector<string> group_usr_elt_name;
  	vector<string> group_phy_elt_name;
  	vector<string> group_ct_elt_name;
  	vector<string> group_0D_elt_name;
  	vector<string> group_1D_elt_name;
  	vector<string> group_2D_elt_name;
  	vector<string> group_pseudo_elt_name;
  	
  	long i;
  	for (i = 1;i < input_nb + 1;i++)
  	{
  		string elem_name((*argin)[i]);
  		group_usr_elt_name.push_back(elem_name);

//
// Check if each element is member of this pool
// as CounterTimer, 0DExpChannel, 1DExpChannel or 2DExpChannel or a pseudo counter
//
  		string elt_name((*argin)[i]);
  		
  		if (ct_channel_exist(elt_name) == true)
  			group_ct_elt_name.push_back(elt_name);
  		else if (zerod_channel_exist(elt_name) == true)
  			group_0D_elt_name.push_back(elt_name);
  		else if (oned_channel_exist(elt_name) == true)
  			group_1D_elt_name.push_back(elt_name);
  		else if (twod_channel_exist(elt_name) == true)
  			group_2D_elt_name.push_back(elt_name);
  		else if (pseudo_counter_exist(elt_name) == true)
  			group_pseudo_elt_name.push_back(elt_name);
  		else 
  		{
			TangoSys_OMemStream o;
			o << "Element " << elt_name << " is neither a counter/timer, 0D, ";
			o << "1D or 2D experiment channel or pseudo counter defined in ";
			o << "this pool. Can't create the measurement group." << ends;
	
			Tango::Except::throw_exception(
					(const char *)"Pool_BadArgument",o.str(),
			        (const char *)"Pool::create_measurement_group");
  		}
  	}

  	user_elems_to_phy_elems(group_usr_elt_name, group_phy_elt_name, true);
  	
//
// Check that each element is not used several times within the group. We don't
// trust the std::count method because it is case sensitive
//
// No longer applicable. Measurement groups support repeated channels. 
// If we go back to the previous situation ( don't support repeated groups ) then
// don't forget to change the previous method call user_elems_to_phy_elems back
// to unique = false
/*  	

	for (unsigned long idx = 0;idx < group_phy_elt_name.size();idx++)
	{
		string curr_elt(group_phy_elt_name[idx]);
		transform(curr_elt.begin(),curr_elt.end(),curr_elt.begin(),::tolower);
		for (unsigned long idy = idx+1;idy < group_phy_elt_name.size();idy++)
		{
			string comp_elt(group_phy_elt_name[idy]);
			transform(comp_elt.begin(),comp_elt.end(),comp_elt.begin(),::tolower);
			
			if(curr_elt == comp_elt)
			{
	  			TangoSys_OMemStream o;
	  			o << "Element " << group_phy_elt_name[idx] << " is used more ";
	  			o << "than once in this measurement group";
	  			o << "\nEach element should be used only once in a ";
	  			o << "measurement group" << ends;
	  
	  			Tango::Except::throw_exception(
	  					(const char *)"Pool_WrongMeasurementGroup",o.str(),
	  				    (const char *)"Pool::create_measurement_group");
			}
		}
	}  	
*/
  	
//
// Build Tango device name
//
  
  	Tango::Util	*tg = Tango::Util::instance();
  	string tg_dev_name("mntgrp/");
  	tg_dev_name += tg->get_ds_inst_name() +  '/' + group_name;
  	
  	DEBUG_STREAM << "Tango measurement group device name = " << tg_dev_name << endl;
  	
//
// Check if this device is already defined in database
// Check by device alias and by Tango device name
//
  
  	Tango::Database *db = tg->get_database();
  	
  	Tango::DbDevImportInfo my_device_import;
  	bool device_exist = false;
  	bool by_alias = false;	
  
  	try
  	{
  		my_device_import = db->import_device(group_name);
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
  			my_device_import = db->import_device(tg_dev_name);
  			device_exist = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
  			{
  				device_exist = true;
  			}
  		}
  	}
  	
  	if (device_exist == true)
  	{
  		TangoSys_OMemStream o;
  		o << "Measurement group ";
  		if (by_alias == false)
  			o << tg_dev_name;
  		else
  			o << group_name;
  		o << " already defined" << ends;
  
  		Tango::Except::throw_exception((const char *)"Pool_WrongMeasurementGroupName",o.str(),
  				        (const char *)"Pool::create_measurement_group");
  	}


			
//
// If the device is not defined in database, create it in database, set its alias
// and define its properties used to store its ID, its device pool and its element list
//
  
  	if (device_exist == false)
  	{
  		DEBUG_STREAM << "Trying to create device entry in database" << endl;
  		
      	try
  		{
  			Tango::DbDevInfo my_device_info;
      		my_device_info.name = tg_dev_name.c_str();
      		my_device_info._class = "MeasurementGroup";
      		my_device_info.server = tg->get_ds_name().c_str();
  			
      		db->add_device(my_device_info);
      		db->put_device_alias(tg_dev_name,group_name);
  
  			Tango::DbDatum id(MEASUREMENT_GROUP_ID_PROP);
  			Tango::DbDatum pool_name(POOL_DEV_NAME_PROP);
  			Tango::DbDatum elt_ct_list(MMNT_GRP_CT_LIST);
  			Tango::DbDatum elt_0D_list(MMNT_GRP_0D_LIST);
  			Tango::DbDatum elt_1D_list(MMNT_GRP_1D_LIST);
  			Tango::DbDatum elt_2D_list(MMNT_GRP_2D_LIST);
  			Tango::DbDatum elt_pc_list(MMNT_GRP_PC_LIST);
  			Tango::DbDatum user_group_elt(USER_GROUP_ELT);
  			Tango::DbDatum phys_group_elt(PHYS_GROUP_ELT);
  			Tango::DbData db_data;
  
  			measurement_group_id++;
  			id << measurement_group_id;
  			db_data.push_back(id);			
  			
  			pool_name << get_name();
  			db_data.push_back(pool_name);
  			
  			elt_ct_list << group_ct_elt_name;
  			db_data.push_back(elt_ct_list);

  			elt_0D_list << group_0D_elt_name;
  			db_data.push_back(elt_0D_list);
  			
  			elt_1D_list << group_1D_elt_name;
  			db_data.push_back(elt_1D_list);

  			elt_2D_list << group_2D_elt_name;
  			db_data.push_back(elt_2D_list);

  			elt_pc_list << group_pseudo_elt_name;
  			db_data.push_back(elt_pc_list);
  			
  			user_group_elt << group_usr_elt_name;
			db_data.push_back(user_group_elt);

			phys_group_elt << group_phy_elt_name;
			db_data.push_back(phys_group_elt);

  			db->put_device_property(tg_dev_name.c_str(),db_data);
  			  			
//
// Put default values for memorized attributes
//
			db_data.clear();
			Tango::DbDatum it("Integration_time"),it_value("__value");
			Tango::DbDatum ic("Integration_count"),ic_value("__value");
			Tango::DbDatum timer("Timer"),timer_value("__value");
			Tango::DbDatum monitor("Monitor"),monitor_value("__value");
			
			it << 1L;
			it_value << 0.0;
			db_data.push_back(it);
			db_data.push_back(it_value);
			
			ic << 1L;
			ic_value << 0L;
			db_data.push_back(ic);
			db_data.push_back(ic_value);
			
			timer << 1L;
			timer_value << "Not Initialized";
			db_data.push_back(timer);
			db_data.push_back(timer_value);
			
			monitor << 1L;
			monitor_value << "Not Initialized";
  			db_data.push_back(monitor);
  			db_data.push_back(monitor_value);
  			
//
// Register abs_change for change events on CT, 0D and pseudo attributes
// 
  			for(unsigned long idx = 0 ; idx < group_ct_elt_name.size(); idx++)
  			{
  				string attr_name = group_ct_elt_name[idx] + "_value";
				Tango::DbDatum value(attr_name),abs_ch("abs_change");
				value << 1L;
				abs_ch << defaultCtGrpVal_AbsChange;
				db_data.push_back(value);
				db_data.push_back(abs_ch);
  			}
  			for(unsigned long idx = 0 ; idx < group_0D_elt_name.size(); idx++)
  			{
  				string attr_name = group_0D_elt_name[idx] + "_value";
				Tango::DbDatum value(attr_name),abs_ch("abs_change");
				value << 1L;
				abs_ch << defaultZeroDGrpVal_AbsChange;
				db_data.push_back(value);
				db_data.push_back(abs_ch);
  			}
  			for(unsigned long idx = 0 ; idx < group_pseudo_elt_name.size(); idx++)
  			{
  				string attr_name = group_pseudo_elt_name[idx] + "_value";
				Tango::DbDatum value(attr_name),abs_ch("abs_change");
				value << 1L;
				abs_ch << defaultCtGrpVal_AbsChange;
				db_data.push_back(value);
				db_data.push_back(abs_ch);
  			}
			db->put_device_attribute_property(tg_dev_name.c_str(),db_data);  			
  			
  			DEBUG_STREAM << "Device created in database (with alias)" << endl;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			DEBUG_STREAM << "Gasp an exception........" << endl;
  			TangoSys_OMemStream o;
  			o << "Can't create measurement group " << group_name << " in database" << ends;
  			
  			Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateMeasurementGroup",o.str(),
  							  (const char *)"Pool::create_measurement_group");
  		}
  		
//
// Find the Tango MeasurementGroup class and create the measurement group
//
  
  		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
  		for (long i = 0;i < cl_list->size();i++)
  		{
  			if ((*cl_list)[i]->get_name() == "MeasurementGroup")
  			{
  				try
  				{
  					Tango::DevVarStringArray na;
  					na.length(1);
  					na[0] = tg_dev_name.c_str();
  					(*cl_list)[i]->device_factory(&na);
  					break;
  				}
  				catch (Tango::DevFailed &e)
  				{
  					DEBUG_STREAM << "Exception while trying to create MeasurementGroup device" << endl;
   
//
// The delete_device will also delete device property(ies)
//
  					
  					db->delete_device(tg_dev_name);
  					
  					TangoSys_OMemStream o;
  					o << "Can't create measurement group device " << group_name << ends;
  			
  					Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateMeasurementGroup",o.str(),
  							  (const char *)"Pool::create_measurement_group");
  				}
  			}
  		}
  
//
// Create a Tango device proxy on the newly created measurement group
// and set its connection to automatic re-connection
//
  
  		Tango::DeviceProxy *tmp_dev = new Tango::DeviceProxy(measurement_group_list.back().obj_tango_name);
  		tmp_dev->set_transparency_reconnection(true);
  		measurement_group_list.back().obj_proxy = tmp_dev;
  	}

	MeasurementGroupPool &mgp = get_measurement_group_from_id(measurement_group_id);

//
// By default, if possible set the first Counter/timer given by the user to be the timer channel
//
	if(!group_ct_elt_name.empty())
	{
		Tango::DeviceAttribute attr("Timer",group_ct_elt_name[0]);
		mgp.obj_proxy->write_attribute(attr);
	}
	
//
// subscribe to the internal events coming from each channel of the measurement group
//
	vector<PoolElement*>::iterator ite = mgp.group_elts.begin();
	for(; ite != mgp.group_elts.end(); ite++)
	{
		(*ite)->add_pool_elem_listener(&mgp);
	}
 
//
// Push a change event for clients listenning on events
//
  	Tango::Attribute &mgl = dev_attr->get_attr_by_name("MeasurementGroupList");
  	read_MeasurementGroupList(mgl);
  	mgl.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_measurement_group
 *
 *	description:	method to execute "DeleteMeasurementGroup"
 *	This command deletes a measurement group from its name
 *
 * @param	argin	The motor group name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_measurement_group(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_measurement_group(): entering... !" << endl;

	//	Add your own code to control device here
	
//
// Find measurement group in measurement group list
//

	string user_name(argin);
	
	MeasurementGroupPool &mg_to_del = get_measurement_group_from_name(user_name);  

	Tango::DevState mg_state = mg_to_del.obj_proxy->state(); 		
//
// Check that the group is not moving
//
  
  	if(mg_state == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete measurement group with name " << argin;
  		o << ". It is actually taking data." << ends;
  				
  		Tango::Except::throw_exception((const char *)"Pool_CantDeleteMeasurementGroup",o.str(),
  								 (const char *)"Pool::delete_measurement_group");
  	}

//
// Unsubscribe to the internal events for each element of the motor group
//

	vector<PoolElement*>::iterator it = mg_to_del.group_elts.begin();
	for(; it != mg_to_del.group_elts.end();++it)
	{
		(*it)->remove_pool_elem_listener(&mg_to_del);		
	}

//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	try
  	{
  		tg->get_database()->delete_device(mg_to_del.obj_tango_name);
  			
//
// Delete motor group device from server but first find its Tango xxxClass instance
//
  
  		Tango::DeviceClass *dc = mg_to_del.group->get_device_class();
  		dc->device_destroyer(mg_to_del.obj_tango_name);
  	}
  	catch (Tango::DevFailed &e)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete measurement group with name " << argin << ends;
  				
  		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeleteMeasurementGroup",o.str(),
  								 (const char *)"Pool::delete_measurement_group");
  	}

//
// Push a change event for clients listenning on events
//
  
  	Tango::Attribute &mgl = dev_attr->get_attr_by_name("MeasurementGroupList");
  	read_MeasurementGroupList(mgl);
  	mgl.fire_change_event();
  	
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::create_com_channel
 *
 *	description:	method to execute "CreateComChannel"
 *	This command creates a new communication channel in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The communication channel number within the controller
 *	3 - The communication channel name. The communication channel name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"comchan/controller_instance_name/ctrl_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created communication channels are automatically re-created at pool device startup time.
 *
 * @param	argin	long[0] = Communication Channel number in Ctrl, string[0] = communication Channel name, string[1] = Controller instance name
 *
 */
//+------------------------------------------------------------------
void Pool::create_com_channel(const Tango::DevVarLongStringArray *argin)
{
	DEBUG_STREAM << "Pool::create_com_channel(): entering... !" << endl;

	//	Add your own code to control device here

  	if ((argin->lvalue.length() != 1) || (argin->svalue.length() != 2))
  	{
		Tango::Except::throw_exception(
			(const char *)"Pool_WrongArgumentNumber",
			(const char *)"Wrong number of argument for command CreateComChannel. Needs 1 long and 2 strings",
			(const char *)"Pool::create_com_channel");
  	}

  	Tango::DevLong channel_idx = argin->lvalue[0];
  	string ctrl_inst_name(argin->svalue[1]);
  	string channel_name(argin->svalue[0]);
  	ControllerPool &com_ch_ctrl = get_ctrl_from_inst_name(ctrl_inst_name);
  	long ctrl_id = com_ch_ctrl.id;

  	DEBUG_STREAM << "Controller ID = " << ctrl_id << endl;
  	DEBUG_STREAM << "Communication channel index = " << channel_idx << endl;
  	DEBUG_STREAM << "Communication channel name = " << channel_name << endl;

//
// Check if the controller has been successfully constructed
//
  
  	if (com_ch_ctrl.ctrl == NULL)
  	{
  		Tango::Except::throw_exception((const char *)"Pool_WrongControllerId",
  						(const char *)"Can't create a communication channel on a non-responding controller",
  				        (const char *)"Pool::create_com_channel");
  	}
  
//
// Check if we don't have already  enough motor
//
  
  	DEBUG_STREAM << "Checking communication channel number" << endl;
  	
  	if (com_channel_list.size() == MAX_COM_CHANNEL)
  	{
  		Tango::Except::throw_exception((const char *)"Pool_TooManyCommunicationChannel",
  					       (const char *)"Too many Communication Channel in your pool",
  					       (const char *)"Pool::create_com_channel");
  	}
  	
//
// Check that the controller still have some communication channels available
//
  
  	if (com_ch_ctrl.nb_dev == com_ch_ctrl.MaxDevice)
  	{
  		TangoSys_OMemStream o;
  		o << "Max number of communication channel reached (" << com_ch_ctrl.MaxDevice << ")" << ends;
  		
  		Tango::Except::throw_exception((const char *)"Pool_MaxNbComChannelInCtrl",o.str(),
  					       			   (const char *)"Pool::create_com_channel()");
  	}
  		
//
// Build Tango device name
//
  
  	stringstream s;
  	s << channel_idx;
  
  	string tg_dev_name("comchan");
  	string tmp_str(com_ch_ctrl.name);
  	tmp_str.erase(0,tmp_str.find('/'));
  	tg_dev_name = tg_dev_name + tmp_str + '/' + s.str();	
  	
  	DEBUG_STREAM << "Tango device name = " << tg_dev_name << endl;
  			
//
// Check if this device is already defined in database
// Check by device alias and by Tango device name
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	Tango::Database *db = tg->get_database();
  	
  	Tango::DbDevImportInfo my_device_import;
  	bool device_exist = false;
  	bool by_alias = false;	
  
  	try
  	{
  		my_device_import = db->import_device(channel_name);
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
  			my_device_import = db->import_device(tg_dev_name);
  			device_exist = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (::strcmp(e.errors[0].reason.in(),"DB_DeviceNotDefined") != 0)
  			{
  				device_exist = true;
  			}
  		}
  	}
  	
  	if (device_exist == true)
  	{
  		TangoSys_OMemStream o;
  		if (by_alias == false)
  			o << "Communication channel " << channel_idx << " on controller " << ctrl_inst_name << " already defined" << ends;
  		else
  			o << "Communication channel " << channel_name << " is already defined" << ends;
  
  		Tango::Except::throw_exception((const char *)"Pool_WrongComChannelName",o.str(),
  				        (const char *)"Pool::create_com_channel");
  	}
  
//
// If the device is not defined in database, create it in database, set its alias,
// define the property used to store its ID (called channel_id)
//
  
  	if (device_exist == false)
  	{
  		DEBUG_STREAM << "Trying to create device entry in database" << endl;
  		
      	try
  		{
  			Tango::DbDevInfo my_device_info;
      		my_device_info.name = tg_dev_name.c_str();
      		my_device_info._class = "CommunicationChannel";
      		my_device_info.server = tg->get_ds_name().c_str();
  			
      		db->add_device(my_device_info);
  		
  			db->put_device_alias(tg_dev_name,channel_name);
  
  			Tango::DbDatum id(COM_CHANNEL_ID_PROP);
  			Tango::DbData db_data;
  			com_channel_id++;
  			id  << com_channel_id;
  			db_data.push_back(id);
  			db->put_device_property(tg_dev_name.c_str(),db_data);
  						
  			DEBUG_STREAM << "Device created in database (with alias)" << endl;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			DEBUG_STREAM << "Gasp an exception........" << endl;
  			TangoSys_OMemStream o;
  			o << "Can't create communication channel " << channel_name << " in database" << ends;
  			
  			Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateComChannel",o.str(),
  							  (const char *)"Pool::create_com_channel");
  		}
  	
//
// Find the Tango CommunicationChannel class and create the communication channel
//
  
  		const vector<Tango::DeviceClass *> *cl_list = tg->get_class_list();
  		for (unsigned long i = 0;i < cl_list->size();i++)
  		{
  			if ((*cl_list)[i]->get_name() == "CommunicationChannel")
  			{
  				try
  				{
  					DEBUG_STREAM << "Found DeviceClass CommunicationChannel. Will create the device" << endl;	
  					Tango::DevVarStringArray na;
  					na.length(1);
  					na[0] = tg_dev_name.c_str();
  					(*cl_list)[i]->device_factory(&na);
  					break;
  				}
  				catch (Tango::DevFailed &e)
  				{
  					DEBUG_STREAM << "Exception while trying to create CommunicationChannel device" << endl;
 
//
// Check if this communication channel has already been added into pool structures
// If yes, remove it from pool structures
//

					try
					{
						CommunicationChannelPool &failed_ch = get_com_channel_from_name(channel_name);
						remove_com_channel(failed_ch.com_channel);
					}
					catch (Tango::DevFailed &ee) {}

//
// The delete_device will also delete device property(ies)
//
  					
  					db->delete_device(tg_dev_name);
  					
  					TangoSys_OMemStream o;
  					o << "Can't create communication channel device " << channel_name << ends;
  			
  					Tango::Except::re_throw_exception(e,(const char *)"Pool_CantCreateComChannel",o.str(),
  							  (const char *)"Pool::create_com_channel");
  				}
  			}
  		}
  
//
// Create a Tango device proxy on the newly created communication channel
// and set its connection to automatic re-connection
//
  
  		Tango::DeviceProxy *tmp_dev = new Tango::DeviceProxy(com_channel_list.back().obj_tango_name);
  		tmp_dev->set_transparency_reconnection(true);
  		com_channel_list.back().obj_proxy = tmp_dev;
  	}

//
// Inform ghost group that there is a new communication channel
//
	//TODO: Inform ghost group that there is a new communication channel
	/*
	MotorGroup_ns::MotorGroup *ghost_ptr = get_ghost_motor_group_ptr();
	{
		Tango::AutoTangoMonitor atm(ghost_ptr);
		ghost_ptr->add_motor_to_group(mot_list.back().id);
	}
	*/
	
//
// Push a change event for client listening on event
//

	Tango::Attribute &ccl = dev_attr->get_attr_by_name("ComChannelList");
	read_ComChannelList(ccl);
	ccl.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_com_channel
 *
 *	description:	method to execute "DeleteComChannel"
 *	Delete a communication channel from its name
 *	Once an communication channel is deleted, it is not available any more. All its information have been
 *	removed from the database
 *
 * @param	argin	Communication Channel name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_com_channel(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_com_channel(): entering... !" << endl;

	//	Add your own code to control device here

//
// Find communication channel in exp channel list
//

	string user_name(argin);
	CommunicationChannelPool &channel_to_del =  get_com_channel_from_name(user_name);
  		
  	DEBUG_STREAM << "Communication Channel found" << endl;
  			
//
// Check that the com channel is not actually used
//
  	if (channel_to_del.obj_proxy->state() == Tango::MOVING)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete communication channel with name " << argin;
  		o << ". It is actually used (reading/writing)." << ends;
  				
  		Tango::Except::throw_exception((const char *)"Pool_CantDeleteComChannel",o.str(),
  								 (const char *)"Pool::delete_com_channel");
  	}
  	
//
// Get communication channel type
//

	ControllerPool &cp = get_ctrl_from_id(channel_to_del.ctrl_id);
	CtrlType channel_type = cp.get_ctrl_obj_type();

//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
  
  	Tango::Util *tg = Tango::Util::instance();
  	try
  	{
  		tg->get_database()->delete_device(channel_to_del.obj_tango_name);
  			
//
// Delete communication channel device from server but first find its Tango xxxClass instance
//
		Tango::DeviceClass *dc = channel_to_del.com_channel->get_device_class();				
  		
  		vector<DeviceImpl *> &dev_list = dc->get_device_list();
  		dc->device_destroyer(channel_to_del.obj_tango_name);
  	}
  	catch (Tango::DevFailed &e)
  	{
  		TangoSys_OMemStream o;
  		o << "Can't delete communication channel with name " << argin << ends;
  				
  		Tango::Except::re_throw_exception(e,(const char *)"Pool_CantDeleteComChannel",o.str(),
  								 (const char *)"Pool::delete_com_channel");
  	}
	  		
//
// Before returning, send a change event for client listenning
// on event
//
  
  	Tango::Attribute &ccl = dev_attr->get_attr_by_name("ComChannelList");
  	read_ComChannelList(ccl);
  	ccl.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::delete_pseudo_counter
 *
 *	description:	method to execute "DeletePseudoCounter"
 *	This command deletes a pseudo counter
 *
 * @param	argin	Pseudo Motor name
 *
 */
//+------------------------------------------------------------------
void Pool::delete_pseudo_counter(Tango::DevString argin)
{
	DEBUG_STREAM << "Pool::delete_pseudo_counter(): entering... !" << endl;

	//	Add your own code to control device here
//
// Find pseudo counter in pseudo counter list
//
	string user_name(argin);
	PseudoCounterPool &pc_to_del =  get_pseudo_counter_from_name(user_name);
  		
  	DEBUG_STREAM << "Pseudo Counter found" << endl;

//
// Check that the pseudo counter is not member of a measurement group
//
// TODO When a pseudo counter can belong to a measurement group
  	
			
	Tango::Util *util = Tango::Util::instance();
	Tango::Database *db = util->get_database();	

//
// Remove its entry in database. This will also delete any device
// properties and device attribute properties
//
	try
	{
		db->delete_device(pc_to_del.obj_tango_name);

//
// Delete pseudo motor device from server but first find its Tango xxxClass instance
//
		Tango::DeviceClass *dc = pc_to_del.pseudo_counter->get_device_class();
		dc->device_destroyer(pc_to_del.obj_tango_name);
	}
	catch (Tango::DevFailed &e)
	{
		TangoSys_OMemStream o;
		o << "Can't delete pseudo counter '" << argin << "'" << ends;
		
		Tango::Except::re_throw_exception(e,
				(const char *)"Pool_CantDeletePseudoCounter",o.str(),
				(const char *)"Pool::delete_pseudo_counter");
	}
		
//
// Push a change event for clients listenning on events
//
  
  	Tango::Attribute &pcl = dev_attr->get_attr_by_name("PseudoCounterList");
  	read_PseudoCounterList(pcl);
  	pcl.fire_change_event();
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::get_file
 *
 *	description:	method to execute "GetFile"
 *	Returns the contents of the given file as a stream of bytes
 *
 * @param	argin	complete(with absolute path) file name
 * @return	File data
 *
 */
//+------------------------------------------------------------------
Tango::DevVarCharArray *Pool::get_file(Tango::DevString argin)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter : Writing a TANGO DS / Exchanging data)
	//------------------------------------------------------------
	DEBUG_STREAM << "Pool::get_file(): entering... !" << endl;

	//	Add your own code to control device here
	string f_name(argin);
	
	// Relative path 
	if(f_name.find('/') == string::npos)
	{
		string f_path;
		if(find_file_in_pool_path(f_name,f_path) == false)
		{
			TangoSys_OMemStream o;
			o << "The " << f_name << " file could not be found. Please check ";
			o << "that PoolPath property contains the correct path." << ends;
			
			Tango::Except::throw_exception(
				(const char *)"Pool_CantLocateFile", o.str(),
		        (const char *)"Pool::get_file");
		}
		f_name = f_path;
	}
	else
	{
		if(f_name[0] != '/')
		{
			Tango::Except::throw_exception(
				(const char *)"Pool_CantLocateFile",
				(const char *)"The given path must be absolute",
		        (const char *)"Pool::get_file");
		}
		string usr_f_path = f_name.substr(0,f_name.rfind('/'));
		string usr_f_name = f_name.substr(f_name.rfind('/')+1);
		
		vector<string> &pp = get_pool_path();
		
		if( find(pp.begin(),pp.end(),usr_f_path) == pp.end())
		{
			TangoSys_OMemStream o;
			o << "The '" << usr_f_path << "' path is not part of the PoolPath " << ends;
			
			Tango::Except::throw_exception(
				(const char *)"Pool_CantLocateFile", o.str(),
		        (const char *)"Pool::get_file");
		}
		
		vector<string> files;
		get_files_with_extension(usr_f_path,".*",files);

		bool file_found = false;
		for(vector<string>::iterator ite = files.begin(); ite != files.end(); ite++)
		{	
			string curr_file = (*ite).substr((*ite).rfind("/")+1);
			if(curr_file == usr_f_name)
			{
				file_found = true;
				break;
			}
		}
		
		if( file_found == false )
		{
			TangoSys_OMemStream o;
			o << "The '" << usr_f_name << "' file could not be found in '";
			o << usr_f_path << "'" << ends;
			
			Tango::Except::throw_exception(
				(const char *)"Pool_CantLocateFile", o.str(),
		        (const char *)"Pool::get_file");
		}
	}
	
	ifstream infile(f_name.c_str(), ios::in | ios::binary);
	
	// Checks that the file is ok for reading
	if(infile.fail())
	{
		infile.close();
		TangoSys_OMemStream o;
		o << "Error trying to read '" << f_name << "'" << ends;
		
		Tango::Except::throw_exception(
			(const char *)"Pool_CantReadFile", o.str(),
	        (const char *)"Pool::get_file");
	}
	
	// Determine the size of the file 
	infile.seekg (0, ios::end);
	unsigned long file_size = infile.tellg();
	infile.seekg (0, ios::beg);
	
	Tango::DevVarCharArray *ret = new Tango::DevVarCharArray();
	ret->length(file_size);
	
	// Read the data into the buffer
	if(file_size > 0)
	{
		char *dest = (char*)(&(*ret)[0]);
		infile.read(dest,file_size);
	}
	infile.close();
	return ret;
}

//+------------------------------------------------------------------
/**
 *	method:	Pool::put_file
 *
 *	description:	method to execute "PutFile"
 *	Adds/updates a new file into the Pool.
 *	Files can only be placed in directories which are in the PoolPath property.
 *
 * @param	argin	complete filename (inc. absolute path)+'\0'+\nusername+'\0'+\ndescription of the change+'\0'+\nfile data
 *
 */
//+------------------------------------------------------------------
void Pool::put_file(const Tango::DevVarCharArray *argin)
{
	DEBUG_STREAM << "Pool::put_file(): entering... !" << endl;

	//	Add your own code to control device here
	unsigned long idx = 0;
	unsigned long len = argin->length();

//
// Determine Filename
//
	string f_name;
	while(idx < len && (*argin)[idx] != '\0')
		f_name += (*argin)[idx++];
	
	TangoSys_OMemStream gen_desc;
	gen_desc << "Invalid format: Argument must contain:\n";
	gen_desc << " - complete filename (inc. absolute path) + '\0' +\n";
	gen_desc << " - username + '\0' +\n";
	gen_desc << " - description of the change + '\0' +\n";
	gen_desc << " - file data";
	
	DEBUG_STREAM << "Filename is " << f_name << endl;
	
	if(idx == len)
	{
		Tango::Except::throw_exception(
			(const char *)"Pool_CantDetermineFilename", gen_desc.str(),
	        (const char *)"Pool::put_file");		
	}
	
	// Make sure it is an absolute path 
	if(f_name.size() == 0 || f_name[0] != '/')
	{
		Tango::Except::throw_exception(
			(const char *)"Pool_CantDetermineFilename", 
			(const char *)"The filename must contain an absolute path",
	        (const char *)"Pool::put_file");
	}

	string usr_f_path = f_name.substr(0,f_name.rfind('/'));
	string usr_f_name = f_name.substr(f_name.rfind('/')+1);
		
	vector<string> &pp = get_pool_path();
	
	// Don't check against PoolPath.
	// It should be possible to add files first to a new directory and only
	// after add this new directory to the PoolPath.
	/*
	if( find(pp.begin(),pp.end(),usr_f_path) == pp.end() )
	{
		TangoSys_OMemStream o;
		o << "The '" << usr_f_path << "' path is not part of the PoolPath.\n"
		     "New files can only be inserted in directories inside the "
		     "PoolPath" << ends;
		
		Tango::Except::throw_exception(
			(const char *)"Pool_CantDetermineFilename", o.str(),
	        (const char *)"Pool::put_file");
	}
	*/
	
	
	// No need to check if the file already exists
	/*
	vector<string> files;
	get_files_with_extension(usr_f_path,".*",files);

	bool file_found = false;
	for(vector<string>::iterator ite = files.begin(); ite != files.end(); ite++)
	{	
		string curr_file = (*ite).substr((*ite).rfind("/")+1);
		if(curr_file == usr_f_name)
		{
			file_found = true;
			break;
		}
	}
	*/
	
//
// Determine username
//
	
	// skip the NULL character
	idx++; 
	
	string user_name;
	while(idx < len && (*argin)[idx] != '\0')
		user_name += (*argin)[idx++];
	
	if(idx == len)
	{
		Tango::Except::throw_exception(
			(const char *)"Pool_CantDetermineUsername", gen_desc.str(),
	        (const char *)"Pool::put_file");		
	}

//
// Determine commit description
//

	// skip the NULL character
	idx++; 
	
	string descr;
	while(idx < len && (*argin)[idx] != '\0')
		descr += (*argin)[idx++];
	
	if(idx == len)
	{	
		Tango::Except::throw_exception(
			(const char *)"Pool_CantDetermineDescription", gen_desc.str(),
	        (const char *)"Pool::put_file");		
	}

	
//
// Determine file data
//

	// skip the NULL character
	idx++; 
	stringstream strm_data;
	
//
// Determine if the file should contain the metadata. For now this is only 
// enabled for files with extension: .py, .h and .cpp and Makefile
//
	string f_ext = usr_f_name.substr(usr_f_name.rfind('.'));
	if(f_ext == ".py" || f_ext == ".h" || f_ext == ".cpp" || usr_f_name == "Makefile")
	{
		DEBUG_STREAM << "File is writable." << endl;
	
		bool has_meta = false;
		string meta;
		
		string cmt = "// ";
		
		if(usr_f_name == "Makefile")
			cmt = "# ";
		else if(f_ext == ".py")
		{
			cmt = "# ";
			// Avoid writing before the '#!' line if it exists
			if(idx+2<len)
			{
				unsigned char c1 = (*argin)[idx], c2 = (*argin)[idx+1];
				if(c1 == '#' && c2 == '!')
				{
					has_meta = true;
					while(idx < len && (*argin)[idx] != '\n')
						meta += (*argin)[idx++];
					
					// Skip the new line
					idx++;
				}
			}
		}
		
		time_t rawtime;
		struct tm * timeinfo;
		char buffer [80];

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );		
		strftime (buffer,80,"%c",timeinfo);
		if (has_meta)
			strm_data << meta << endl;
		strm_data << cmt << buffer << " by " << user_name << endl;
		strm_data << cmt << descr << endl;
	}
	
	
	while(idx < len)
	{
		strm_data << (*argin)[idx++];
	}
	
	ofstream outfile(f_name.c_str(), ios::out | ios::binary);
	
	// Checks that the file is ok for reading
	if(outfile.fail())
	{
		outfile.close();
		TangoSys_OMemStream o;
		o << "Error trying to create '" << f_name << "'" << ends;
		
		Tango::Except::throw_exception(
			(const char *)"Pool_CantCreateFile", o.str(),
	        (const char *)"Pool::put_file");
	}
	
	outfile << strm_data.str();
	outfile.close();
}

}	//	namespace
