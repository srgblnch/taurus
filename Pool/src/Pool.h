//
// file :        Pool.h
//
// description : Include for the Pool class.
//
// project :	Device pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.87  2007/09/07 15:00:07  tcoutinho
// safety commit
//
// Revision 1.86  2007/08/30 12:40:39  tcoutinho
// - changes to support Pseudo counters.
//
// Revision 1.85  2007/08/24 15:56:29  tcoutinho
// safety weekend commit
//
// Revision 1.84  2007/08/20 06:37:32  tcoutinho
// development commit
//
// Revision 1.83  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.82  2007/08/08 08:47:55  tcoutinho
// Fix bug 18 : CreateController should be a one step operation
//
// Revision 1.81  2007/07/30 11:01:00  tcoutinho
// Fix bug 5 : Additional information for controllers
//
// Revision 1.80  2007/07/26 10:25:15  tcoutinho
// - Fix bug 1 :  Automatic temporary MotorGroup/MeasurementGroup deletion
//
// Revision 1.79  2007/07/17 11:41:57  tcoutinho
// replaced comunication with communication
//
// Revision 1.78  2007/07/12 13:13:33  tcoutinho
// - added poolapi features
//
// Revision 1.77  2007/07/04 15:06:38  tcoutinho
// first commit with stable Pool API
//
// Revision 1.76  2007/06/28 07:15:34  tcoutinho
// safety commit during comunication channels development
//
// Revision 1.75  2007/06/27 10:24:45  tcoutinho
// scond commit for comuncation channels. Added command/attribute skeleton with pogo
//
// Revision 1.74  2007/06/27 08:56:28  tcoutinho
// first commit for comuncation channels
//
// Revision 1.73  2007/05/25 12:48:10  tcoutinho
// fix the same dead locks found on motor system to the acquisition system since release labeled for Josep Ribas
//
// Revision 1.72  2007/05/18 09:04:06  tcoutinho
// - first try to fix dead lock bug
//
// Revision 1.71  2007/05/11 08:07:48  tcoutinho
// - added new propertie to allow different sleep time in CounterTimer
// - added new property to allow sleep time in 0D experiment channel
//
// Revision 1.70  2007/04/30 15:47:05  tcoutinho
// - new attribute "Channels"
// - new device property "Channel_List"
// - when add/remove channel, pool sends a change event on the MeasurementGroupList
//
// Revision 1.69  2007/04/30 14:51:20  tcoutinho
// - make possible to Add/Remove elements on motorgroup that are part of other motor group(s)
//
// Revision 1.68  2007/04/23 15:23:06  tcoutinho
// - changes according to Sardana metting 26-03-2007: ActiveMeasurementGroup attribute became obsolete
//
// Revision 1.67  2007/02/28 16:21:52  tcoutinho
// - support for 0D channels
// - basic fixes after running first battery of tests on measurement group
//
// Revision 1.66  2007/02/26 09:53:00  tcoutinho
// - Introduction of properties for defult abs_change values in measurement group with fix
//
// Revision 1.65  2007/02/26 09:46:04  tcoutinho
// - Introduction of properties for defult abs_change values in measurement group
//
// Revision 1.64  2007/02/26 09:40:07  tcoutinho
// - changed enum name
//
// Revision 1.63  2007/02/22 12:02:00  tcoutinho
// - added support for ghost measurement group
// - added support for measurement group in init/reload controller operations
// - added support of internal events on measurement group
//
// Revision 1.62  2007/02/16 09:59:57  tcoutinho
// - fix memory leak related with Active measurement group attribute
//
// Revision 1.61  2007/02/13 14:39:43  tcoutinho
// - fix bug in motor group when a motor or controller are recreated due to an InitController command
//
// Revision 1.60  2007/02/08 16:18:14  tcoutinho
// - controller safety on PoolGroupBaseDev
//
// Revision 1.59  2007/02/08 10:49:29  etaurel
// - Some small changes after the merge
//
// Revision 1.58  2007/02/08 08:51:15  etaurel
// - Many changes. I don't remember the list
//
// Revision 1.57  2007/02/07 16:53:06  tcoutinho
// safe guard commit
//
// Revision 1.56  2007/02/06 09:41:03  tcoutinho
// - added MeasurementGroup
//
// Revision 1.55  2007/01/30 16:00:47  etaurel
// - The CT Value attribute is now a Double
// WARNING, this change has made us discouvering the GCC BUG  2834...
// Some code is now between ifdef/endif statement to compile using gcc 3.3
//
// Revision 1.54  2007/01/26 08:36:48  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.53  2007/01/23 08:27:22  tcoutinho
// - fix some pm bugs found with the test procedure
// - added internal event for MotionEnded
//
// Revision 1.52  2007/01/16 14:32:21  etaurel
// - Coomit after a first release with CT
//
// Revision 1.51  2007/01/09 07:52:10  tcoutinho
// - changes to make it gcc 4.1 compatible
//
// Revision 1.50  2007/01/05 15:02:38  etaurel
// - First implementation of the Counter Timer class
//
// Revision 1.49  2007/01/05 13:03:36  tcoutinho
// -changes to internal event mechanism
// -support for gcc 4.1.1 compilation without errors
//
// Revision 1.48  2006/12/28 15:36:58  etaurel
// - Fire events also on the motor limit_switches attribute
// - Throw events even in simulation mode
// - Mange motor position limit switches dependant of the offset attribute
//
// Revision 1.47  2006/12/20 10:25:36  tcoutinho
// - changes to support internal event propagation
// - bug fix in motor groups containing other motor groups or pseudo motors
//
// Revision 1.46  2006/12/18 11:37:10  etaurel
// - Features are only boolean values invisible from the external world
// - ExtraFeature becomes ExtraAttribute with data type of the old features
//
// Revision 1.45  2006/12/12 11:09:17  tcoutinho
// - support for pseudo motors and motor groups in a motor group
//
// Revision 1.44  2006/11/29 07:59:21  etaurel
// - Which changes ?
//
// Revision 1.43  2006/11/24 08:49:03  tcoutinho
// - changes to support pseudo motors in motor groups
//
// Revision 1.42  2006/11/20 14:32:43  etaurel
// - Add ghost group and event on motor group position attribute
//
// Revision 1.41  2006/11/07 14:57:09  etaurel
// - Now, the pool really supports different kind of controllers (cpp and py)
//
// Revision 1.40  2006/11/03 15:48:59  etaurel
// - Miscellaneous changes that I don't remember
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================
#ifndef _POOL_H
#define _POOL_H

#include <CtrlFiCa.h>
#include <tango.h>

#include <PoolDef.h>
#include <pool/PoolAPI.h>
#include <PoolFactory.h>

#include <PyMotCtrl.h>

#define		MAX_FICA					256
#define		MAX_FILE					128
#define		MAX_CTRL_CLASS				256
#define		MAX_POOL_ELEM				1024
#define		MAX_CTRL					MAX_POOL_ELEM
#define		MAX_MOTOR					MAX_POOL_ELEM
#define		MAX_CT						MAX_POOL_ELEM
#define		MAX_ZEROD					MAX_POOL_ELEM
#define		MAX_MOTOR_GROUP		   		MAX_POOL_ELEM
#define		MAX_MEASUREMENT_GROUP		MAX_POOL_ELEM
#define		MAX_PSEUDO_MOTOR       		MAX_POOL_ELEM
#define		MAX_PSEUDO_COUNTER			MAX_POOL_ELEM
#define		MAX_COM_CHANNEL				MAX_POOL_ELEM
#define		CTRL_PROP					"Controller"
#define		PROP_BY_CTRL		   		5
#define		PROP_BY_PSEUDO	       		4
#define		MOTOR_ID_PROP		   		"motor_id"
#define		MOTOR_GROUP_ID_PROP	   		"motor_group_id"
#define		PSEUDO_MOTOR_ID_PROP   		"pseudo_motor_id"
#define    	PSEUDO_MOTOR_MG    	    	"motor_group"
#define   	PSEUDO_MOTOR_LIST   	   	"motor_list"
#define   	PSEUDO_MOTOR_ROLE    	  	"role"
#define   	PSEUDO_MOTOR_ROLE_IDX  	  	"role_idx"
#define		PSEUDO_MOTOR_MG_PREFIX		"_pm"
#define		PSEUDO_COUNTER_ID_PROP   	"channel_id"
#define		PSEUDO_COUNTER_LIST			"channel_list"
#define		EXP_CHANNEL_ID_PROP			"channel_id"
#define		COM_CHANNEL_ID_PROP			"channel_id"
#define		MEASUREMENT_GROUP_ID_PROP	"measurement_group_id"
#define		POOL_DEV_NAME_PROP	   		"pool_device"
#define		MMNT_GRP_CT_LIST			"ct_list"
#define		MMNT_GRP_0D_LIST			"zerodexpchannel_list"
#define		MMNT_GRP_1D_LIST			"onedexpchannel_list"
#define		MMNT_GRP_2D_LIST			"twodexpchannel_list"
#define		MMNT_GRP_PC_LIST			"pseudocounter_list"
#define		MOTOR_GROUP_LIST		   	"motor_list"
#define		MOTOR_GROUP_GROUP_LIST	   	"motor_group_list"
#define		PSEUDO_MOTOR_GROUP_LIST	   	"pseudo_motor_list"
#define		USER_GROUP_ELT				"user_group_elt"
#define		PHYS_GROUP_ELT				"phys_group_elt"
#define		POS_SPECTRUM_DIM_X			"pos_spectrum_dim_x"
#define		NB_STATE_PER_READ			10
#define		MIN_TANGO_VERSION			600

#define		NOT_INITIALIZED				"Not Initialized"
//
// Some error reason fields
//

#define			PY_CONTROLLER_NOT_VALID	"Pool_PyControllerNotValid"
#define			PROP_NOT_DEFINED		"Pool_PropNotDefined"
#define			PYTHON_ERROR			"Pool_PythonError"
#define			PYTHON_CONTROLLER_ERROR	"Pool_PythonControllerError"

//
// Some macros definition
//

namespace Motor_ns
{
class Motor;
}

namespace MotorGroup_ns
{
class MotorGroup;
}

namespace PseudoMotor_ns
{
class PseudoMotor;
}

namespace CTExpChannel_ns
{
class CTExpChannel;
}

namespace ZeroDExpChannel_ns
{
class ZeroDExpChannel;
}

namespace PseudoCounter_ns
{
class PseudoCounter;
}

namespace MeasurementGroup_ns
{
class MeasurementGroup;
}

namespace CommunicationChannel_ns
{
class CommunicationChannel;
} 

//using namespace Tango;

/**
 * @author	$Author$
 * @version	$Revision$
 */

 //	Add your own constants definitions here.
 //-----------------------------------------------

/**
 * @namespace Pool_ns
 * 
 * @brief The namespace for the Pool tango class.
 * 
 * @author	tcoutinho 
 */
namespace Pool_ns
{
	
class PoolThread;
class CTPoolThread;
class AquisitionInfo;
class PropertyData;
class MotInMove;
class CtrlInMove;
class CtrlInCount;
class GrpInMove;
class CtInCount;
class GrpInCount;
struct DelayedEvt;
class IConstraintable;

/**
 * @brief Types of internal events
 * 
 * StateChange - the state has changed
 * ElementStructureChange - the internal structure of an element changed. 
 *                          Ex.: the controller code of a motor was reloaded.
 * ElementsChange - the list of elements changed. Ex.: a new motor was added
 *                  to a motor group  
 * PositionChange - the position has changed
 * PositionArrayChange - the positions have changed
 * CTValueChange - the value of a counter timer changed
 * ZeroDValueChange - the value of a 0D channel changed
 * PseudoCoValueChange - the value of a pseudo counter changed
 * MotionEnded - the position has changed for the last time. An event of 
 *               this type is sent usually at the end of a motor, 
 *                motor group, or pseudo motor motion
**/ 
typedef enum PoolEventType
{
	StateChange = 0,
	ElementStructureChange,
	ElementListChange,
	PositionChange,
	PositionArrayChange,
	CTValueChange,
	ZeroDValueChange,
	PseudoCoValueChange,
	MotionEnded
};

struct PoolElement;

/**
 * The data related to an internal event inside the pool
 */
typedef struct PoolElementEvent
{
	/** Type of event */
	PoolEventType		type;
	
	/** Element which originated the event */
	PoolElement*		src;
	
	/** number of elements (in case of a PositionArrayChange type of event) */
	long				dim;
	
	/** set to <code>true</code> if it is a priority event */
	bool				priority;
	
	/** Union for the old value */
	union {
		Tango::DevDouble	old_value;
		Tango::DevDouble	old_position;
		Tango::DevDouble	*old_position_array;
		Tango::DevState		old_state;
	};
	
	/** union for the new value */
	union {
		Tango::DevDouble	new_value;
		Tango::DevDouble	new_position;
		Tango::DevDouble	*new_position_array;
		Tango::DevState		new_state;
	};
	
	/**
	 * Constructor for the PoolElementEvent class
	 *
	 * @param[in] evt_type type of event
	 * @param[in] src_elem element which triggered the event
	 */	
	PoolElementEvent(PoolEventType evt_type,PoolElement* src_elem);
	
	/**
	 * Copy constructor
	 * 
	 * @param[in] rhs the original object
	 */
	PoolElementEvent(const PoolElementEvent &rhs);
};
typedef list<PoolElementEvent*> PoolElemEventList;

/**
 * The Pool Listener interface. Any object interested in receiving  events must 
 * implement this interface and register itself in the event suplier object.
 */
typedef struct IPoolElementListener
{
	/**
	 * Called when an event occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */
	virtual void pool_elem_changed(PoolElemEventList &evt) = 0;
};

/** 
 * A generic element of the Pool. It implements a IPoolElementListener.
 *
 */
typedef struct PoolElement: public IPoolElementListener
{
	/** Element identifier */
	long						id;
	
	/** Name of the element */
	string						name;
	
	/**
	 * the full name
	 * Generic string for elements which are tango devices is:
	 * [alias] ([tango_device_name]) [class-of_device] [extra_info]  
	 */
	string						user_full_name;
	
	/** the lower case name (alias) */
	string 						obj_alias_lower;
	
	/** tango name .../.../... */
	string 						obj_tango_name;
	
	/** the index in the controller */
	long						obj_idx;
	
	/** the controller identifier for which this element belongs to */
	long						ctrl_id;
	
	/** the proxy object (in case the element is a tango device) */
	Tango::DeviceProxy			*obj_proxy;
			
	/** list of listeners for this element */ 
	list<IPoolElementListener*>	pool_elem_listeners;	
	
	/**
	 * Appends the given listener to the list of listeners
	 * 
	 * @param[in] listener the listener to be added.
	 */ 
	void add_pool_elem_listener(IPoolElementListener *);
	
	/**
	 * Removes the given listener from the list of listeners
	 * 
	 * @param[in] listener the listener to be removed. 
	 */
	void remove_pool_elem_listener(IPoolElementListener *);
	
	/**
	 * Determines if this element has any listeners.
	 * 
	 * @return <code>true</code> if there are listeners or <code>false</code>
	 *         otherwise
	 */
	bool has_listeners() { return !pool_elem_listeners.empty(); }
	
	/**
	 * Propagates the given event to all listeners of this element.
	 * 
	 * @param[in] evt_data the event to be forwarded to the listeners.
	 */
	void propagate_event(PoolElemEventList &);
	
	/**
	 * Determines if the given element name is part of this element. Default
	 * implementation always returns false.
	 * 
	 * @param[in] elt the element name to be checked.
	 * 
	 * @return <code>true</code> if the given element is part of this element
	 *         or <code>false</code> otherwise
	 */
	virtual bool is_member(string &elt)			{ return false; }
	
	/**
	 * Returns the Tango server Device_3Impl object correspoding to this pool
	 * element (if there is one).
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device()	{ return NULL; }
	
	/**
	 * Returns the list of elements that are part of this element or NULL if this
	 * element does not have any elements.
	 * 
	 * @return list of elements that are part of this element or NULL.
	 */
	virtual vector<PoolElement *> *get_elems()	{ return NULL; }
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */
	virtual void pool_elem_changed(PoolElemEventList &evt);
	
	/**
	 * Fires an event to all listeners.
	 * 
	 * @param[in] evt     the event data
	 * @param[in] exclude optional element to be exclude from the event.
	 * 				      (default is NULL - meaning no listener is excluded)
	 */
	void fire_pool_elem_change(PoolElementEvent *evt,IPoolElementListener *exclude = NULL);
};

class CtrlFiCa;
class PoolLock;

/** 
 * Controller object
 */
typedef struct ControllerPool: public PoolElement
{
	/** pointer to the controller */
	Controller *					ctrl;
	
	/** pointer to the list of controller properties */
	vector<Controller::Properties>	*cpp_ctrl_prop;
	
	/** 
	 * <code>true</code> if the ctrl class has been built or <code>false</code>
	 * otherwise 
	 */
	bool							ctrl_fica_built;
	
	/** the Controller class reference */ 
	vector<CtrlFiCa *>::iterator	ite_ctrl_fica;
	
	/** controller class name */
	string							ctrl_class_name;
	
	/** controller fica name */
	string							ctrl_fica_name;
	
	/** maximum number of devices */
	long							MaxDevice;
	
	/** current number of devices */
	long							nb_dev;
	
	/** current error description */
	string							error_status;

	/// Constructor
	ControllerPool():cpp_ctrl_prop(NULL),MaxDevice(CTRL_MAXDEVICE_NOTDEF),nb_dev(0) {}
	
	/**
	 * Obtains a pointer to the CtrlFile
	 * @return the CtrlFile pointer
	 */
	CtrlFile	*get_ctrl_file_ptr()	{ return (*ite_ctrl_fica)->get_ctrl_file(); }

	/**
	 * Obtains a reference to the CtrlFile
	 * @return the CtrlFile reference
	 */
	CtrlFile	&get_ctrl_file()		{ return *get_ctrl_file_ptr(); }
	
	/**
	 * Obtains a reference to the monitor
	 * @return the PoolLock reference
	 */	
	PoolLock 	&get_ctrl_fica_mon()	{ return (*ite_ctrl_fica)->get_mon(); }
	
	/**
	 * Gets the controller type
	 * @return the controller type
	 */	
	CtrlType	get_ctrl_obj_type()		{ return (*ite_ctrl_fica)->get_obj_type(); }

	/**
	 * Obtains fica name
	 * @return a reference to the fica
	 */
	string		&get_fica_name()		{ return (*ite_ctrl_fica)->get_name(); }
	
	/**
	 * Gets the controller language
	 * @return the controller language
	 */	
	Language	get_language()			{ return (*ite_ctrl_fica)->get_language(); }
	
	/**
	 * Gets the file name
	 * @return a reference to the file name
	 */	
	string		&get_f_name()			{ return get_ctrl_file_ptr()->get_name(); }
	
	/**
	 * Gets the absolute path name
	 * @return a reference to the absolute path name
	 */	
	string		&get_path()				{ return get_ctrl_file_ptr()->get_path(); }
	
	/**
	 * Obtains the library handler
	 * @return the controller library handler
	 */	
	lt_dlhandle	get_lib_ptr()			{ return (*ite_ctrl_fica)->get_lib_ptr(); }

	/**
	 * Obtains the intermidiate python library handler (if applicable)
	 * @return the intermidiate python library handler
	 */	
	lt_dlhandle	get_py_inter_lib_ptr()	{ return (*ite_ctrl_fica)->get_py_inter_lib_ptr(); }
	
	/**
	 * Obtains the python module object (if applicable)
	 * @return the python module object
	 */		
	PyObject 	*get_py_module() 		{ return (*ite_ctrl_fica)->get_py_module(); }
	
	/**
	 * Reloads the controller code
	 */
	void		reload()				{ get_ctrl_file().reload(); }
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */	
	virtual void pool_elem_changed(PoolElemEventList &);
};

/**
 * The constraint pool object
 */
typedef struct ConstraintPool : public ControllerPool
{
	/** List of elements connected to the constraint. */
	vector<IConstraintable*>			elements; 
};

/**
 * The motor object
 */
typedef struct MotorPool: public PoolElement
{
	/** motor DeviceImpl object pointer */
	Motor_ns::Motor 	*motor;

	/**
	 * Returns the Tango server Device_3Impl object corresponding to this motor
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */	
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &);
};

struct PseudoMotorPool;
struct PseudoCounterPool;

/**
 * Motor group element
 */
typedef struct MotorGroupPool: public PoolElement
{
	/** motor group DeviceImpl object pointer */
	MotorGroup_ns::MotorGroup	*group;
	
	/** list of motor ids */
	vector<long>				mot_ids;
	
	/** list of USER motors */
	vector<MotorPool*>			mot_elts;
	
	/** list of USER pseudo motors */ 
	vector<PseudoMotorPool*>	pm_elts;
	
	/** list of USER motor groups */
	vector<MotorGroupPool*>		mg_elts;
	
	/** list of all USER elements */
	vector<PoolElement*>		group_elts; 

	/**
	 * Returns the Tango server Device_3Impl object corresponding to this motor
	 * group
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */			
	virtual void pool_elem_changed(PoolElemEventList &);
	
	/** 
	 * Determines if the given element name is a member of this group
	 * @param[in] name element name
	 * @return <code>true</code> if the element is a member or <code>false</code>
	 *         otherwise
	 */
	virtual bool is_member(string &);
	
	/** 
	 * Determines if the given element name is a member of this group at the 
	 * USER level
	 * @param[in] name element name
	 * @return <code>true</code> if the element is a member or <code>false</code>
	 *         otherwise
	 */
	bool is_user_member(string &);
	
	/** 
	 * Returns a pointer to the list of elements in this group
	 * @return the list of elements pointer
	 */
	virtual vector<PoolElement *> *get_elems()	{ return &group_elts; }
	
};
	
/**
 * the counter/timer experiment channel element
 */
typedef struct CTExpChannelPool: public PoolElement
{
	/** counter/timer experiment channel DeviceImpl object pointer */
	CTExpChannel_ns::CTExpChannel	*ct_channel;
	
	/**
	 * Returns the Tango server Device_3Impl object corresponding to this
	 * counter/timer experiment channel
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */	
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &);
};

/**
 * 0D experiment channel element
 */
typedef struct ZeroDExpChannelPool: public PoolElement
{
	/** 0D experiment channel DeviceImpl object pointer */
	ZeroDExpChannel_ns::ZeroDExpChannel	*zerod_channel;

	/**
	 * Returns the Tango server Device_3Impl object corresponding to this 0D 
	 * experiment channel
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &);
};

/** 
 * Measurement group element
 */
typedef struct MeasurementGroupPool : public PoolElement
{
	/** measurement group DeviceImpl object pointer */
	MeasurementGroup_ns::MeasurementGroup	*group;
	
	/** list of physical channels */
	vector<long>							ch_ids; 
	
	/** list of user elements */
	vector<PoolElement*>					group_elts;
	
	/**
	 * Returns the Tango server Device_3Impl object corresponding to this 
	 * measurement group
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &);
	
	/** 
	 * Determines if the given element name is a member of this group
	 * @param[in] name element name
	 * @return <code>true</code> if the element is a member or <code>false</code>
	 *         otherwise
	 */
	virtual bool is_member(string &elt);
	
	/** 
	 * Returns a pointer to the list of elements in this group
	 * @return the list of elements pointer
	 */	
	virtual vector<PoolElement *> *get_elems()	{ return &group_elts; }
};

/**
 * Pseudo motor element
 */
typedef struct PseudoMotorPool: public PoolElement
{
	/** pseudo motor DeviceImpl object pointer */
	PseudoMotor_ns::PseudoMotor *pseudo_motor;
	
	/** list of motor elements */
	vector<PoolElement*>		mot_elts;
	
	/** hidden motor group name */
	string						mot_grp_name;
	
	/** extra full name data */
	string						user_full_name_extra;
	
	/** motor group pointer */
	MotorGroupPool*				mot_grp;
	
	/** role index */
	long						role_idx;
	
	/**
	 * Returns the Tango server Device_3Impl object corresponding to this pseudo
	 * motor
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &evt);
	
	/** 
	 * Determines if the given motor is a member of this group
	 * @param[in] name element name
	 * @return <code>true</code> if the element is a member or <code>false</code>
	 *         otherwise
	 */	
	virtual bool is_member(string &elt);
	
	/** 
	 * Returns a pointer to the list of elements in this group
	 * @return the list of elements pointer
	 */		
	virtual vector<PoolElement *> *get_elems()	{ return &mot_elts; }
};

/**
 * Pseudo counter element
 */
typedef struct PseudoCounterPool: public PoolElement
{
	/** pseudo counter experiment channel DeviceImpl object pointer */
	PseudoCounter_ns::PseudoCounter *pseudo_counter;
	
	/** list of user elements */
	vector<PoolElement*>			ch_elts;			
	
	/** extra full name data */
	string							user_full_name_extra;
	
	/**
	 * Returns the Tango server Device_3Impl object correspoding to this pseudo
	 * counter experiment channel
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &);
	
	/** 
	 * Determines if the given motor is a member of this group
	 * @param[in] name element name
	 * @return <code>true</code> if the element is a member or <code>false</code>
	 *         otherwise
	 */	
	virtual bool is_member(string &elt);
	
	/** 
	 * Returns a pointer to the list of elements in this group
	 * @return the list of elements pointer
	 */		
	virtual vector<PoolElement *> *get_elems()	{ return &ch_elts; }
};

/**
 * Communication channel element
 */
typedef struct CommunicationChannelPool: public PoolElement
{
	/** communication channel DeviceImpl object pointer */
	CommunicationChannel_ns::CommunicationChannel *com_channel;
	
	/**
	 * Returns the Tango server Device_3Impl object corresponding to this
	 * communication channel
	 * 
	 * @return the Tango server Device_3Impl device object for this pool element
	 */
	virtual Tango::Device_3Impl *get_device();
	
	/**
	 * IPoolElementListener interface implementation. Called when an event 
	 * occurs.
	 * 
	 * @param[in] evt stack of event data elements.
	 */		
	virtual void pool_elem_changed(PoolElemEventList &evt);
};

/**
 * @class Pool
 * 
 * @brief Handle a generic client device pool.
 * 
 * Device States Description:
 * Tango::ON :      Pool ready
 * Tango::ALARM :   No communication with at least one of the controller
 * Tango::MOVING :  This state means that the pool device is shuting down and is waiting for motor(s)
 *                  to finish their movement
 */
class Pool: public Tango::Device_3Impl
{
public :
	//	Add your own data members here
	//-----------------------------------------


	//	Here is the Start of the automatic code generation part
	//-------------------------------------------------------------	
/**
 *	@name attributes
 *	Attributs member data.
 */
//@{
		Tango::DevBoolean	*attr_SimulationMode_read;
		Tango::DevBoolean	attr_SimulationMode_write;
		Tango::DevString	*attr_ControllerClassList_read;
		Tango::DevString	*attr_ControllerList_read;
		Tango::DevString	*attr_ExpChannelList_read;
		Tango::DevString	*attr_MeasurementGroupList_read;
		Tango::DevString	*attr_MotorGroupList_read;
		Tango::DevString	*attr_MotorList_read;
		Tango::DevString	*attr_PseudoMotorList_read;
		Tango::DevString	*attr_ComChannelList_read;
		Tango::DevString	*attr_PseudoCounterList_read;
//@}

/**
 *	@name Device properties
 *	Device properties member data.
 */
//@{
/**
 *	Path for pool external stuff (C++ shared libraries or Python modules)
 */
	vector<string>	poolPath;
/**
 *	Default value for motor position attribute abs_change property
 */
	Tango::DevDouble	defaultMotPos_AbsChange;
/**
 *	Default value for motor group position attribute abs_change property
 */
	Tango::DevLong	defaultMotGrpPos_AbsChange;
/**
 *	Polling period for the ghost motor group
 */
	string	ghostGroup_PollingPeriod;
/**
 *	Sleep time in the motion thread loop in mS
 */
	Tango::DevLong	motThreadLoop_SleepTime;
/**
 *	Number of State to be done before doing a read in the motion thread
 */
	Tango::DevLong	nbStatePerRead;
/**
 *	Default value for counter value attribute abs_change property
 */
	Tango::DevDouble	defaultCtVal_AbsChange;
/**
 *	Number of read between firing event during the Zero D Exp Channel
 *	acquisition loop
 */
	Tango::DevLong	zeroDNbReadPerEvent;
/**
 *	Default value for Zero D Cumulated value attribute abs_change property
 */
	Tango::DevDouble	defaultZeroDVal_AbsChange;
/**
 *	The default abs_change value for Counter/Timer value attributes in a Measurement Group
 */
	Tango::DevDouble	defaultCtGrpVal_AbsChange;
/**
 *	The default abs_change value for 0D Channel value attributes in a Measurement Group
 */
	Tango::DevDouble	defaultZeroDGrpVal_AbsChange;
/**
 *	Sleep time in the motion thread loop in mS
 */
	Tango::DevLong	cTThreadLoop_SleepTime;
/**
 *	Sleep time in the motion thread loop in mS
 */
	Tango::DevLong	zeroDThreadLoop_SleepTime;
/**
 *	Maximum inactivity time for temporary objects.
 *	Units are multiples of GhostGroup_PollingPeriod.
 *	If a temporary object (motor/measurement group created with a '__'
 *	prefix in its name) does not receive a command/attribute request for
 *	this amount of time, it is marked for deletion.
 *	
 */
	Tango::DevLong	tmpElement_MaxInactTime;
//@}

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Pool(Tango::DeviceClass *cl,string &s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Pool(Tango::DeviceClass *cl,const char *s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device name
 *	@param d	Device description.
 */
	Pool(Tango::DeviceClass *cl,const char *s,const char *d);
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The object desctructor.
 */	
	~Pool() {delete_device();};
/**
 *	will be called at device destruction or at init command.
 */
	void delete_device();
//@}

	
/**@name Miscellaneous methods */
//@{
/**
 *	Initialize the device
 */
	virtual void init_device();
/**
 *	Always executed method befor execution command method.
 */
	virtual void always_executed_hook();

//@}

/**
 * @name Pool methods prototypes
 */

//@{
/**
 *	Hardware acquisition for attributes.
 */
	virtual void read_attr_hardware(vector<long> &attr_list);
/**
 *	Extract real attribute values for SimulationMode acquisition result.
 */
	virtual void read_SimulationMode(Tango::Attribute &attr);
/**
 *	Write SimulationMode attribute values to hardware.
 */
	virtual void write_SimulationMode(Tango::WAttribute &attr);
/**
 *	Extract real attribute values for ControllerClassList acquisition result.
 */
	virtual void read_ControllerClassList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for ControllerList acquisition result.
 */
	virtual void read_ControllerList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for ExpChannelList acquisition result.
 */
	virtual void read_ExpChannelList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for MeasurementGroupList acquisition result.
 */
	virtual void read_MeasurementGroupList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for MotorGroupList acquisition result.
 */
	virtual void read_MotorGroupList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for MotorList acquisition result.
 */
	virtual void read_MotorList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for PseudoMotorList acquisition result.
 */
	virtual void read_PseudoMotorList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for ComChannelList acquisition result.
 */
	virtual void read_ComChannelList(Tango::Attribute &attr);
/**
 *	Extract real attribute values for PseudoCounterList acquisition result.
 */
	virtual void read_PseudoCounterList(Tango::Attribute &attr);
/**
 *	Read/Write allowed for SimulationMode attribute.
 */
	virtual bool is_SimulationMode_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for ControllerClassList attribute.
 */
	virtual bool is_ControllerClassList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for ControllerList attribute.
 */
	virtual bool is_ControllerList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for ExpChannelList attribute.
 */
	virtual bool is_ExpChannelList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for MeasurementGroupList attribute.
 */
	virtual bool is_MeasurementGroupList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for MotorGroupList attribute.
 */
	virtual bool is_MotorGroupList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for MotorList attribute.
 */
	virtual bool is_MotorList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for PseudoMotorList attribute.
 */
	virtual bool is_PseudoMotorList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for ComChannelList attribute.
 */
	virtual bool is_ComChannelList_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for PseudoCounterList attribute.
 */
	virtual bool is_PseudoCounterList_allowed(Tango::AttReqType type);
/**
 *	Execution allowed for CreateController command.
 */
	virtual bool is_CreateController_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CreateExpChannel command.
 */
	virtual bool is_CreateExpChannel_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CreateMotor command.
 */
	virtual bool is_CreateMotor_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CreateMotorGroup command.
 */
	virtual bool is_CreateMotorGroup_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteController command.
 */
	virtual bool is_DeleteController_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteExpChannel command.
 */
	virtual bool is_DeleteExpChannel_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteMotor command.
 */
	virtual bool is_DeleteMotor_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteMotorGroup command.
 */
	virtual bool is_DeleteMotorGroup_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeletePseudoMotor command.
 */
	virtual bool is_DeletePseudoMotor_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetControllerInfo command.
 */
	virtual bool is_GetControllerInfo_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for InitController command.
 */
	virtual bool is_InitController_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for ReloadControllerCode command.
 */
	virtual bool is_ReloadControllerCode_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for SendToController command.
 */
	virtual bool is_SendToController_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CreateMeasurementGroup command.
 */
	virtual bool is_CreateMeasurementGroup_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteMeasurementGroup command.
 */
	virtual bool is_DeleteMeasurementGroup_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for CreateComChannel command.
 */
	virtual bool is_CreateComChannel_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeleteComChannel command.
 */
	virtual bool is_DeleteComChannel_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetControllerInfoEx command.
 */
	virtual bool is_GetControllerInfoEx_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DeletePseudoCounter command.
 */
	virtual bool is_DeletePseudoCounter_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for GetFile command.
 */
	virtual bool is_GetFile_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for PutFile command.
 */
	virtual bool is_PutFile_allowed(const CORBA::Any &any);
/**
 * This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *	@return	State Code
 *	@exception DevFailed
 */
	virtual Tango::DevState	dev_state();
/**
 * This command gets the device status (stored in its <i>device_status</i> data member) and returns it to the caller.
 *	@return	Status description
 *	@exception DevFailed
 */
	virtual Tango::ConstDevString	dev_status();
/**
 * This command creates a controller object in the pool. Each controller code has to be defined as a shared library
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
 *	@param	argin	in[0] = Controller dev type, in[1] = Controller lib, in[2] = Controller name, in[3] = Instance name
 *	@exception DevFailed
 */
	void	create_controller(const Tango::DevVarStringArray *);
/**
 * This command creates a new experiment channel in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The experiment channel number within the controller
 *	3 - The experiment channel name. The experiment channel name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"ctexp/controller_instance_name/ctrl_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created experiment channels are automatically re-created at pool device startup time.
 *	@param	argin	long[0] = Exp Channel number in Ctrl, string[0] = Exp Channel name, string[1] = Controller instance name
 *	@exception DevFailed
 */
	void	create_exp_channel(const Tango::DevVarLongStringArray *);
/**
 * This command creates a new motor in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The motor number within the controller
 *	3 - The motor name. The motor name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"motor/controller_instance_name/motor_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created motors are automatically re-created at pool device startup time.
 *	@param	argin	long[0] = Motor number in Ctrl, string[0] = Motor name, string[1] = Controller instance name
 *	@exception DevFailed
 */
	void	create_motor(const Tango::DevVarLongStringArray *);
/**
 * This command creates a motor group. The name of the group is the first element in the input
 *	argument followed by the group element name.
 *	@param	argin	Group name followed by motor's name
 *	@exception DevFailed
 */
	void	create_motor_group(const Tango::DevVarStringArray *);
/**
 * Delete controller from its name.
 *	Before executing this command, you must first delete all the elements associated
 *	with this controller
 *	@param	argin	The controller name
 *	@exception DevFailed
 */
	void	delete_controller(Tango::DevString);
/**
 * Delete a experiment channel from its name
 *	Once an experiment channel is deleted, it is not available any more. All its information have been
 *	removed from the database
 *	@param	argin	Exp Channel name
 *	@exception DevFailed
 */
	void	delete_exp_channel(Tango::DevString);
/**
 * Delete a motor from its name
 *	Once a motor is deleted, it is not available any more. All its information have been
 *	removed from the database
 *	@param	argin	Motor name
 *	@exception DevFailed
 */
	void	delete_motor(Tango::DevString);
/**
 * This command delete a motor group from its name
 *	@param	argin	The motor group name
 *	@exception DevFailed
 */
	void	delete_motor_group(Tango::DevString);
/**
 * This command deletes a pseudo motor
 *	@param	argin	Pseudo Motor name
 *	@exception DevFailed
 */
	void	delete_pseudo_motor(Tango::DevString);
/**
 * Get controller class data like parameters description from the controller class name
 *	@param	argin	in[0] - Controller type, in[1] - Controller file name, in[2] - Controller class name, in[3] - Controller instance name (optional)
 *	@return	Controller class data
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*get_controller_info(const Tango::DevVarStringArray *);
/**
 * Initialiaze a controller. Needed if the pool has been started when a controller was down in order to
 *	connect the controller to the pool without needs to restart the complete pool once it has
 *	been switched up.
 *	@param	argin	Controller name
 *	@exception DevFailed
 */
	void	init_controller(Tango::DevString);
/**
 * This command reloads controller code in the device server process. For C++ controller, it will
 *	unload and load the controller shared library and for Python controller, it will re-load the controller module.
 *	@param	argin	Controller file name
 *	@exception DevFailed
 */
	void	reload_controller_code(Tango::DevString);
/**
 * This commands sends an uninterpreted string to a controller. The first input argument is the controller
 *	name. The second input argument is the string to be sent to the controller.
 *	It returns the controller answer without any interpretation.
 *	@param	argin	In[0] = Controller name, In[1] = String to send
 *	@return	The controller answer
 *	@exception DevFailed
 */
	Tango::DevString	send_to_controller(const Tango::DevVarStringArray *);
/**
 * This command creates a measurement group. The name of the measurement group is the first element in the input
 *	argument followed by the group element names.
 *	@param	argin	Measurement Group name followed by names of the elements
 *	@exception DevFailed
 */
	void	create_measurement_group(const Tango::DevVarStringArray *);
/**
 * This command deletes a measurement group from its name
 *	@param	argin	The motor group name
 *	@exception DevFailed
 */
	void	delete_measurement_group(Tango::DevString);
/**
 * This command creates a new communication channel in the pool. It has three arguments which are
 *	1 - The controller name (its instance name)
 *	2 - The communication channel number within the controller
 *	3 - The communication channel name. The communication channel name is a Tango alias and does not have any '/' characters.
 *	This command creates a Tango device with a Tango name set to
 *	"comchan/controller_instance_name/ctrl_number"
 *	and with an alias as given by the user in the command parameter.
 *	All the created communication channels are automatically re-created at pool device startup time.
 *	@param	argin	long[0] = Communication Channel number in Ctrl, string[0] = communication Channel name, string[1] = Controller instance name
 *	@exception DevFailed
 */
	void	create_com_channel(const Tango::DevVarLongStringArray *);
/**
 * Delete a communication channel from its name
 *	Once an communication channel is deleted, it is not available any more. All its information have been
 *	removed from the database
 *	@param	argin	Communication Channel name
 *	@exception DevFailed
 */
	void	delete_com_channel(Tango::DevString);
/**
 * Get controller class extra data like parameters description from the controller class name
 *	@param	argin	in[0] - Controller type, in[1] - Controller file name, in[2] - Controller class name
 *	@return	Controller class data
 *	@exception DevFailed
 */
	Tango::DevVarCharArray	*get_controller_info_ex(const Tango::DevVarStringArray *);
/**
 * This command deletes a pseudo counter
 *	@param	argin	Pseudo Motor name
 *	@exception DevFailed
 */
	void	delete_pseudo_counter(Tango::DevString);
/**
 * Returns the contents of the given file as a stream of bytes
 *	@param	argin	complete(with absolute path) file name
 *	@return	File data
 *	@exception DevFailed
 */
	Tango::DevVarCharArray	*get_file(Tango::DevString);
/**
 * Adds/updates a new file into the Pool.
 *	Files can only be placed in directories which are in the PoolPath property.
 *	@param	argin	complete filename (inc. absolute path)+'\0'+\nusername+'\0'+\ndescription of the change+'\0'+\nfile data
 *	@exception DevFailed
 */
	void	put_file(const Tango::DevVarCharArray *);

/**
 *	Read the device properties from database
 */
	 void get_device_property();
//@}

	//	Here is the end of the automatic code generation part
	//-------------------------------------------------------------	


protected :	
	//	Add your own data members here
	//-----------------------------------------
	
	/** last controller id */
	long							last_ctrl_id;
	
	/** list of controllers */
	list<ControllerPool>			ctrl_list;
	
	/** Controller information (as in the controllers property */
	vector<string>					ctrl_info;
	
	/**
	 * flag. set to true means that it is necessary to write new values to the 
	 * tango database
	 */
	bool							upd_db;
	
	/** 
	 * <code>true</code> if in init_device or <code>false</code> during 
	 * startup 
	 */
	bool							init_cmd;
	
	/**
	 * <code>true</code> if the necessary proxies have been created or 
	 * <code>false</code> otherwise
	 */
	bool							proxy_created;

	/** Python Pool API object pointer */
	PyObject						*pool_util;
	
	/** DeviceFactory pointer */
	IDeviceFactory					*factory;

	/** last motor id */
	long							mot_id;
	
	/** List of motors */
	list<MotorPool>					mot_list;
	
	/** last motor group id */
	long							mot_group_id;
	
	/** List of motor groups */
	list<MotorGroupPool>			mot_group_list;
		
	/** last pseudo motor id */
	long                    		pseudo_mot_id;
	
	/** List of pseudo motors */
	list<PseudoMotorPool>   		pseudo_mot_list;
	
	/** last experiment channel id */
	long							exp_channel_id;
	
	/** List of counter/timer */
	list<CTExpChannelPool>			ct_list;
	
	/** List of 0D experiment channels */
	list<ZeroDExpChannelPool> 		zerod_list;
	
	/** List of pseudo counter channels */
	list<PseudoCounterPool>			pseudo_co_list;
	
	/** last measurement group id */
	long							measurement_group_id;
	
	/* List of measurement groups */
	list<MeasurementGroupPool>		measurement_group_list;

	/** last communication channel id */
	long							com_channel_id;
	
	/** List of communication channels */
	list<CommunicationChannelPool>	com_channel_list;
	
	/** List of pseudo motor classes */
	vector<string>					pseudo_mot_classes;
	
	/** List of C++ controller classes */
	vector<string>					cpp_ctrl_classes;
	
	/** List of Python controller classes */
	vector<string>					py_ctrl_classes;
	
	/** List of paths that can contain controllers */
	vector<string>        	 	 	pool_path;
	
	/** 
	 * <code>true</code> if the poolPath has been splitted <code>false</code>
	 * otherwise
	 */
	bool							poolPath_splitted;
	
	/** Buffer for the controller list */
	Tango::DevString				ctrl_name_list[MAX_CTRL];
	
	/** Buffer for the motor list */
	Tango::DevString				motor_name_list[MAX_MOTOR + MAX_PSEUDO_MOTOR];
	
	/** Buffer for the motor group list */
	Tango::DevString				motor_group_name_list[MAX_MOTOR_GROUP];
	
	/** Buffer for the pseudo motor list */
	Tango::DevString				pseudo_motor_name_list[MAX_PSEUDO_MOTOR];
	
	/** Buffer for the pseudo counter list */
	Tango::DevString				pseudo_counter_name_list[MAX_PSEUDO_COUNTER];
	
	/** Buffer for the experiment channel list */
	Tango::DevString				expch_name_list[MAX_CT + MAX_ZEROD + MAX_PSEUDO_COUNTER];
	
	/** Buffer for the controller class list */
	Tango::DevString				ctrl_class_list[MAX_CTRL_CLASS];
	
	/** Buffer for the measurement group list */
	Tango::DevString				measurement_group_name_list[MAX_MEASUREMENT_GROUP];
	
	/** Buffer for the communication channel list */
	Tango::DevString				com_channel_name_list[MAX_COM_CHANNEL];
	
	/** temporary storage of the pool status */
	string							tmp_status;
	
	/** <code>true</code> if during a motion or <code>false</code> otherwise */
	bool							moving_state_requested;
	
	/** Pool Element backup structure. Used during Reload/Init */
	typedef struct ObjBkp
	{
		Controller 					*old_ctrl;
		long						idx;
		long						dist;
		CtrlType					type;
		list<IPoolElementListener*>	listeners;
	};
	
	/** Controller backup structure. Used during Reload/Init */
	typedef struct CtrlBkp
	{
        Controller          		*good_old_ctrl;
		long 						dist;
		long						idx;
		bool						wrong_fica;
		vct_ite						fica_ite;
		bool						old_fica_built;
		bool						fica_already_reloaded;
	};
	
	/**
	 *	@name Property handling
	 *	Device properties handling methods
	 */
	//@{	
	/**
	 * Retrieve a controller code file name from controller name and from the 
	 * info stored in db.
	 * 
	 * @param ctrl_name [in] The controller name (controller/instance)
	 * 
	 * @return file name
	 */
	string &f_name_from_db_prop(string &);
	
	/*
	 * Retrieve a controller device type from controller name and from the info
	 * stored in db.
	 * 
	 * @param ctrl_name [in] The controller name (controller/instance)
	 * 
	 * @return controller type
	 */
	CtrlType dev_type_from_db_prop(string &);
	
	/**
	 * checks for consistency of property values. It accesses
	 *  the Database server
	 *
	 * @param inst_name  [in] the instance name
	 * @param class_name [in] the class name
	 * @param info       [in] a vector of properties containning information 
	 *                        about all properties at the class level. It will 
	 *                        be changed to contain updated information at the 
	 *                        instance level	
	 * @param start      [in] the index from which to start analysing the info 
	 *                        vector (default value is 0)
	 */
	void check_property_data(string &,string &,vector<string>&,int start = 0);
	
	/**
	 * checks for consistency of property values. It is assumed that the given 
	 * vector contains a complete up to date description of the property 
	 * including the existence of the property in the Tango Database server
	 *
	 * @param prop_list [in] a vector of properties containning information 
	 *                       about all properties at the class level. It will be
	 *                       changed to contain updated information at the 
	 *                       instance level				  
	 */	
	void check_property_data(vector<PropertyData *> &);
	
	/**
	 * Build a Python dictionary from the list of given properties
	 * 
	 * @param properties [in] a vector of properties containning information
	 *                        about all properties.
	 *  
	 * @return a ptr to the Python dictionary with one item for each property. 
	 *         The key is the property name and the value is the property value
	 */
	PyObject *properties_2_py_dict(vector<PropertyData*> &);
	
	/**
	 * Build a list of properties from the list of given property data
	 * 
	 * @param properties [in] a vector of properties containning information
	 *                        about all properties.
	 *  
	 * @return a vector containning property data
	 */	
	vector<Controller::Properties> *properties_2_cpp_vect(vector<PropertyData*> &);

	//@}
	
	/** 
	 * auxiliary method to execute "GetControllerInfo"/"GetControllerInfoEx"
	 * Get controller class data like parameters description from the given
	 * controller class name
	 *
	 * @param[in] argin in[0] - Controller type, 
	 * 			        in[1] - Controller file name, 
	 * 					in[2] - Controller class name, 
	 *                  in[3] - Controller instance name (optional)
	 * 
	 * @param[out] info vector to be filled with the controller information
	 * @param[out] info_ex extra information (if requested)
	 */
	void get_ctrl_info(const Tango::DevVarStringArray *, vector<string> &, Tango::DevVarCharArray *info_ex = NULL);

	/**
	 * Restore memorized controller ptr and object ptrs in their controller and
	 * object list.	This method is used in som error cases to restore the device
	 * pool state.
	 * 
	 * @param[in] ctrls The memorized controller(s)
	 * @param[in] objs  The memorized elements(s)
	 */
	void restore_ctrl_obj(vector<Pool::CtrlBkp> *,vector<Pool::ObjBkp> *);
 
	/**
	 * Finds and invalidates the motors inside the given controllers
	 * 
	 * @param[in]  ctrls motor controllers that contain motors to be invalidated
	 * @param[out] objs  motor backup information
	 */
	void find_and_invalidate_motor(vector<CtrlBkp> &,vector<ObjBkp> &);
	
	/**
	 * Finds and invalidates the pseudo motors inside the given controllers
	 * 
	 * @param[in]  ctrls pseudo motor controllers that contain pseudo motors to
	 *                   be invalidated
	 * @param[out] objs  pseudo motor backup information
	 */
	void find_and_invalidate_pseudo_motor(vector<CtrlBkp> &,vector<ObjBkp> &);

	/**
	 * Finds and invalidates the pseudo counters inside the given controllers
	 * 
	 * @param[in]  ctrls pseudo counters controllers that contain motors to be 
	 *                   invalidated
	 * @param[out] objs  pseudo counter backup information
	 */
	void find_and_invalidate_pseudo_counter(vector<CtrlBkp> &,vector<ObjBkp> &);
	
	/**
	 * Finds and invalidates the counters inside the given controllers
	 * 
	 * @param[in]  ctrls counters controllers that contain pseudo counters to be 
	 *                   invalidated
	 * @param[out] objs  counter backup information
	 */	
	void find_and_invalidate_ct(vector<CtrlBkp> &,vector<ObjBkp> &);
	
	/**
	 * Finds and invalidates the 0D channels inside the given controllers
	 * 
	 * @param[in]  ctrls 0D channel controllers that contain 0D channels to be 
	 *                   invalidated
	 * @param[out] objs  0D channel backup information
	 */	
	void find_and_invalidate_zerod(vector<CtrlBkp> &,vector<ObjBkp> &);
	
	/**
	 * Finds and invalidates the communication channels inside the given 
	 * controllers
	 * 
	 * @param[in]  ctrls communication channel controllers that contain 
	 *                   communication channels to be invalidated
	 * @param[out] objs  communication channel backup information
	 */	
	void find_and_invalidate_comch(vector<CtrlBkp> &,vector<ObjBkp> &);
	
	/**
	 * restore the given motors
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_motor(vector<ObjBkp> &,
					   list<ControllerPool>::iterator &,
					   vector<Pool::CtrlBkp>::iterator &);

	/**
	 * restore the given pseudo motors
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_pseudo_motor(vector<ObjBkp> &,
							  list<ControllerPool>::iterator &,
							  vector<Pool::CtrlBkp>::iterator &);
	
	/**
	 * restore the given pseudo counters
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_pseudo_counter(vector<ObjBkp> &,
								list<ControllerPool>::iterator &,
								vector<Pool::CtrlBkp>::iterator &);

	/**
	 * restore the given counter/timers
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_ct(vector<ObjBkp> &,list<ControllerPool>::iterator &,
					vector<Pool::CtrlBkp>::iterator &);
	
	/**
	 * restore the given 0D experiment channels
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_zerod(vector<ObjBkp> &,list<ControllerPool>::iterator &,
					   vector<Pool::CtrlBkp>::iterator &);
	
	/**
	 * restore the given communication channels
	 * 
	 * @param[in]  objs list of elements to be restored 
	 * @param[in]  ctrl controller for the list of elements
	 * @param[int] ite  corresponding controller backup info 
	 */	
	void restore_comch(vector<ObjBkp> &,list<ControllerPool>::iterator &,
					   vector<Pool::CtrlBkp>::iterator &);

	/** 
	 * Determines if a motor for the given name exists
	 * @param[in] name motor name
	 * 
	 * @return <code>true</code> if the motor exists or <code>false</code>
	 *         otherwise
	 */
 	bool motor_exist(string &);
 	
 	/**
 	 * Fills given string with the contents of the first (up to 2) desc fields 
 	 * of the exception
 	 * 
 	 * @param[in]  e           exception
 	 * @param[out] ctrl_status the string to be filled with the exception info
 	 */
	void except_2_ctrl_status(Tango::DevFailed &,string &);
	
	/**
	 * Get how many controller used the Fica with name passed as parameter
	 * 
	 * @param[in] fica_name controller class name
	 * 
	 * @return number of controller instances for the given class name
	 */
	long get_ctrl_nb_by_fica_name(string &);
    
	/**
	 * from the user parameters build a list of pairs
	 * ([param name], [param value]).
	 * @param[in]  argin       the user parameters from the Tango command
	 * @param[out] value_pairs the data structure which will contain the 
	 *                         parameter information
	 * @param[in]  start       index from which to start interpreting arguments
	 *                         as parameter data
	 */
    void build_property_params(const Tango::DevVarStringArray *,
			 				   vector<pair<string,string> > &,int );
    
    /**
     * Create elements for the given controller
     * 
     * @param[in] ctrl_inst_name   controller instance name
     * @param[in] fica             controller Class
     * @param[in] pseudo_mot_names pseudo motor names
     * @param[in] mot_names        physical motor names
     */
    void create_pseudo_motor_ctrl_elems(string &, PseudoMotCtrlFiCa*, 
    									vector<string> &, vector<string> &);

    /**
     * Create elements for the given controller
     * 
     * @param[in] ctrl_inst_name   controller instance name
     * @param[in] fica             controller Class
     * @param[in] counter_name     pseudo counter name
     * @param[in] mot_names        physical counter names
     */
    void create_pseudo_counter_ctrl_elems(string &, PseudoCoCtrlFiCa*,
    									  string &, vector<string> &);
public:

	/**
	 * Create proxies (Tango sense) to all pool elements
	 */
	void create_proxies();
	
	/** 
	 * this method should be called in pool startup AFTER all the devices have 
	 * been created. In other words, it should be called in the device_factory()
	 * method of the last Class to be registered in the PoolFactory. It should 
	 * fill internal Pool data that can only be determined after all devices are
	 * created.
	 */
	void post_init_device();

	/**
	 * Retrieve the full motor name from its tango name. The full motor name is
	 * "alias (tango name)"
	 * 
	 * @param[in] mot_tg_name motor name (tango name)
	 * 
	 * @return a string reference to the full motor name
	 */
	string &get_full_name(string &);
	
	/**
	 * Check if a motor group with the given elements exists. 
	 * 
	 * @param[in]  elems list of motor ids
	 * @param[out] name  name of the motor group that matches the description.
	 *                   If no motor group exists, the string is unchanged
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool group_exist(vector<long> &,string &);

	/**
	 * Check if a motor group with the given elements exists. 
	 * 
	 * @param[in]  elems list of motor names
	 * @param[out] name  name of the motor group that matches the description.
	 *                   If no motor group exists, the string is unchanged
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */		
	bool group_exist(vector<string> &,string &);

	/**
	 * Check if a motor group is defined in this pool from its name. The group 
	 * name could be the group alias or its tango name. The check is done first 
	 * by group alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool group_exist(string &);

	/**
	 * Check if a pseudo motor is defined in this pool from its name. The  
	 * name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool pseudo_motor_exist(string &);

	/**
	 * Check if a pseudo counter is defined in this pool from its name. The  
	 * name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool pseudo_counter_exist(string &);
	
	/**
	 * Check if a counter/timer channel is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool ct_channel_exist(string &);

	/**
	 * Check if a 0D channel is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */		
	bool zerod_channel_exist(string &);
	
	/**
	 * Check if a 1D channel is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	bool oned_channel_exist(string &);
	
	/**
	 * Check if a 2D channel is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */		
	bool twod_channel_exist(string &);

	/**
	 * Check if a measurement group is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */		
	bool measurement_group_exist(string &);
	bool measurement_group_exist(vector<string> &,string &);
	
	/**
	 * Check if a communication channel is defined in this pool from its name. 
	 * The name could be the alias or its tango name. The check is done first 
	 * by alias
	 * 
	 * @param[in] name element name
	 * 
	 * @return <code>true</code> in case the element exists or 
	 *         <code>false</code> otherwise
	 */	
	
	bool com_channel_exist(string &);
	
	/**
	 * Returns complete list of absolute paths that the pool uses to search for
	 * controllers
	 * 
	 * @return a vector where each element is a string containning an absolute
	 *         path
	 */
	vector<string>& get_pool_path();
	
	/**
	 * Helper method. Gets the list of subdirectories of a directory
	 * 
	 * @param[in]  dir   the absolute path of the directory to be inspected
	 * @param[out] paths a vector which will contain absolute paths to 
	 *                   directories inside the given directory
	 */
	void get_path_subdirs(string &, vector<string> &);
	
	/**
	 * Helper method. Determines if the given file name exists inside a directory
	 * given by its absolute path
	 * 
	 * @param[in] f_name the file name
	 * @param[in] f_path the absolute path 
	 * 
	 * @return <code>true</code> if the file exists inside the given directory or 
	 *         <code>false</code> otherwise
	 */
	bool find_file_in_pool_path(string &, string &);

	/**
	 * Helper method. Determines if the given file name exists inside a directory
	 * given by its absolute path. It filters the file by language
	 * 
	 * @param[in] f_name the file name
	 * @param[in] f_path the absolute path 
	 * @param[in] lang   the language
	 * 
	 * @return <code>true</code> if the file exists inside the given directory or 
	 *         <code>false</code> otherwise
	 */
	bool find_file_in_pool_path(string &, string &, Language);
	
	/**
	 * Helper method. Builds a list of files with the given extension that are
	 * present inside the given directory
	 *
	 * @param[in]  dir   the directory to be inspected
	 * @param[in]  f_ext the file extension
	 * @param[out] lst   the vector which will contain the list of files that 
	 *                   match the given extension in the given directory
	 */
	void get_files_with_extension(string &, const char *, vector<string> &);	
	
	/**
	 * Return a reference to the PseudoMotorPool structure for the motor with 
	 * the given name or throws an exception if no pseudo motor is found
	 * 
	 * @param[in] name pseudo motor name
	 * 
	 * @return the corresponding pseudo motor reference
	 */
	PseudoMotorPool &get_pseudo_motor_from_name(string &);

	/**
	 * Return a reference to the PseudoMotorPool structure for the motor with 
	 * the given ID or throws an exception if no pseudo motor is found
	 * 
	 * @param[in] id pseudo motor ID
	 * 
	 * @return the corresponding pseudo motor reference
	 */
	PseudoMotorPool &get_pseudo_motor_from_id(long );
	
	/**
	 * Return a reference to the PseudoCounterPool structure for the counter 
	 * with the given name or throws an exception if no pseudo counter is found
	 * 
	 * @param[in] name pseudo counter name
	 * 
	 * @return the corresponding pseudo counter reference
	 */	
	PseudoCounterPool &get_pseudo_counter_from_name(string &);

	/**
	 * Return a reference to the PseudoCounterPool structure for the counter 
	 * with the given ID or throws an exception if no pseudo counter is found
	 * 
	 * @param[in] id pseudo counter id
	 * 
	 * @return the corresponding pseudo counter reference
	 */	
	PseudoCounterPool &get_pseudo_counter_from_id(long );
	
	/** 
	 * @name Add/Remove pool elements
	 * Set of Add/Remove methods 
	 * @see Pool_AddRemove.cpp
	 */
	//@{
	
	/**
	 * Creates a controller object in the pool. Each controller code has to be
	 * defined as a shared library and the user LD_LIBRARY_PATH environment 
	 * variable has to be correctly set to find the controller shared library.
	 * Each controller shared library has to have a C function called 
	 * "_create_<Controller name>" to create a controller object.
	 *
	 * @param[in] argin in[0] = Controller dev type, 
	 *                  in[1] = Controller lib, 
	 *                  in[2] = Controller name,
	 *                  in[3] = Instance name or id (if internal == true)
	 * @param[in] internal False (default) means that this method was called 
	 *                     from a command_inout request. True means this method
	 *                     was called internally be the pool
	 */
	void add_ctrl(const Tango::DevVarStringArray *, bool internal = false);	
	
	/**
	 * Add a motor to the pool
	 * 
	 * @param[in] mp reference to the motor element
	 */
	void add_motor(MotorPool &);
	
	/**
	 * Add a motor group to the pool
	 * 
	 * @param[in] mgp reference to the motor group element
	 */
	void add_motor_group(MotorGroupPool &);
	
	/**
	 * Add a pseudo motor to the pool
	 * 
	 * @param[in] reference to the pseudo motor element
	 */
	void add_pseudo_motor(PseudoMotorPool &);
	
	/**
	 * Add a pseudo counter to the pool
	 * 
	 * @param[in] pm reference to the pseudo counter element
	 */
	void add_pseudo_counter(PseudoCounterPool &);
	
	/**
	 * Add a counter/timer to the pool
	 * 
	 * @param[in] ct reference to the counter/timer element
	 */
	void add_counter_timer(CTExpChannelPool &);
	
	/**
	 * Add a 0D experiment channel to the pool
	 * 
	 * @param[in] zerod reference to the 0D experiment channel element
	 */
	void add_zerod(ZeroDExpChannelPool &);
	
	/**
	 * Add a measurement group to the pool
	 * 
	 * @param[in] mntgrp reference to the measurement group element
	 */
	void add_measurement_group(MeasurementGroupPool &);
	
	/**
	 * Add a communication channel to the pool
	 * 
	 * @param[in] com_ch reference to the communication channel element
	 */
	void add_com_channel(CommunicationChannelPool &);
	
	/**
	 * Remove a motor from the pool
	 * 
	 * @param[in] mot pointer to the motor element to be removed
	 */
	void remove_motor(Motor_ns::Motor *);
	
	/**
	 * Remove a motor group from the pool
	 * 
	 * @param[in] motgrp pointer to the motor group element to be removed
	 */
	void remove_motor_group(MotorGroup_ns::MotorGroup *);
	
	/**
	 * Remove a pseudo motor from the pool
	 * 
	 * @param[in] pm pointer to the pseudo motor element to be removed
	 */
	void remove_pseudo_motor(PseudoMotor_ns::PseudoMotor *);
	
	/**
	 * Remove a pseudo counter from the pool
	 * 
	 * @param[in] pc pointer to the pseudo counter element to be removed
	 */
	void remove_pseudo_counter(PseudoCounter_ns::PseudoCounter *);
	
	/**
	 * Remove a counter/timer experiment channel from the pool
	 * 
	 * @param[in] ct pointer to the counter/timer element to be removed
	 */
	void remove_ct(CTExpChannel_ns::CTExpChannel *);
	
	/**
	 * Remove a 0D experiment channel from the pool
	 * 
	 * @param[in] zerod pointer to the 0D element to be removed
	 */
	void remove_zerod(ZeroDExpChannel_ns::ZeroDExpChannel *);
	
	/**
	 * Remove a measurement group from the pool
	 * 
	 * @param[in] mntgrp pointer to the measurement group element to be removed
	 */
	void remove_measurement_group(MeasurementGroup_ns::MeasurementGroup *);
	
	/**
	 * Remove a communication channel from the pool
	 * 
	 * @param[in] com_ch pointer to the communication channel element to be removed
	 */
	void remove_com_channel(CommunicationChannel_ns::CommunicationChannel *);
	//@}
	
	/**
	 * This method should be invoked by a motor group when its structure has 
	 * changed by a direct user request. This method will send a change event 
	 * on the MotorGroupList attribute informing all clients of the change.
	 * 
	 * @param[in] mg_id Id of the motor group that has changed
	 */
	void motor_group_elts_changed(long );

	/**
	 * This method should be invoked by a measuremnt group when its structure  
	 * has changed by a direct user request. This method will send a change
	 * event on the MotorGroupList attribute informing all clients of the 
	 * change.
	 * 
	 * @param[in] mg_id Id of the measurement group that has changed
	 */
	void measurement_group_elts_changed(long );
	
	/** 
	 * @name Get/Find pool elements
	 * Set of Get/Find methods 
	 * @see Pool_GetFind.cpp
	 */
	//@{
		
	/**
	 * Gets the controller containning the given motor. Throws exception if
	 * no proper controller is found
	 * 
	 * @param[in] mot motor
	 * 
	 * @return a reference to the controller
	 */
	ControllerPool &get_ctrl_from_motor(MotorPool &);

	/**
	 * Gets the controller with the given ID. Throws exception if
	 * no proper controller is found
	 * 
	 * @param[in] ctrl_id controller ID
	 * 
	 * @return a reference to the controller
	 */
	ControllerPool &get_ctrl_from_id(long );
	
	/**
	 * Gets the controller containning the given motor. Throws exception if
	 * no proper controller is found
	 * 
	 * @param[in] mot_id motor ID
	 * 
	 * @return a reference to the controller
	 */	
	ControllerPool &get_ctrl_from_motor_id(long );
	
	/**
	 * Gets the controller from its name. Throws exception if
	 * no proper controller is found
	 * 
	 * @param[in] name controller name
	 * 
	 * @return a reference to the controller
	 */
	ControllerPool &get_ctrl_from_name(string &);
	
	/**
	 * Gets the controller from its instance name. Throws exception if
	 * no proper controller is found
	 * 
	 * @param[in] inst_name controller instance name
	 * 
	 * @return a reference to the controller
	 */	
	ControllerPool &get_ctrl_from_inst_name(string &);
	
	/**
	 * Gets the controller containning the given pool element. Throws exception
	 * if no proper controller is found
	 * 
	 * @param[in] elem reference to the pool element
	 * 
	 * @return a reference to the controller
	 */
	ControllerPool &get_ctrl_from_pool_elem(PoolElement &);
	
	/**
	 * Gets the controller containning the given experiment channel. Throws 
	 * exception if no proper controller is found
	 * 
	 * @param[in] ch_id experiment channel id
	 * 
	 * @return a reference to the controller
	 */
	ControllerPool &get_ctrl_from_exp_channel_id(long );

	/**
	 * Gets the controller containning the given communication channel. Throws 
	 * exception if no proper controller is found
	 * 
	 * @param[in] comch_id communication channel id
	 * 
	 * @return a reference to the controller
	 */	
	ControllerPool &get_ctrl_from_com_channel_id(long );
	
	/**
	 * Gets the motor index in controller from the motor name. Throws exception
	 * if no proper motor is found
	 * 
	 * @param[in] mot_alias motor name
	 * 
	 * @return motor index in the controller
	 */	
	long get_motor_ctrl_idx(const char *);

	/**
	 * Gets the motor object from its ID. Throws exception if no matching motor
	 * is found
	 * 
	 * @param[in] mot_id motor ID
	 * 
	 * @return a reference to the motor
	 */
	MotorPool &get_motor_from_id(long id);

	/**
	 * Gets the motor object from its name. Throws exception if no matching 
	 * motor is found
	 * 
	 * @param[in] name motor name
	 * 
	 * @return a reference to the motor
	 */
	MotorPool &get_motor_from_name(string &);
	
	/**
	 * Gets the motor ID from its name. Throws exception if no matching motor
	 * is found
	 * 
	 * @param[in] name motor name
	 * 
	 * @return a reference to the motor
	 */
	long get_motor_id_from_name(string &);

	/**
	 * Gets the list of motors.
	 * 
	 * @return the internal motor list
	 */
	list<MotorPool>	&get_mot_list() {return mot_list;}

	/**
	 * Gets the list of pseudo motors.
	 * 
	 * @return the internal pseudo motor list
	 */
	list<PseudoMotorPool> &get_psm_list() {return pseudo_mot_list;}
	
	/**
	 * Gets the list of experiment channels.
	 * 
	 * @return the internal experiment channel list
	 */	
	list<CTExpChannelPool> &get_ct_list() {return ct_list;}
	
	/**
	 * Gets the list of 0D experiment channels.
	 * 
	 * @return the internal 0D experiment channel list
	 */
	list<ZeroDExpChannelPool> &get_zerod_list() {return zerod_list;}
	
	/**
	 * Gets the list of communication channels.
	 * 
	 * @return the internal communication channel list
	 */	
	list<CommunicationChannelPool> &get_com_channel_list() {return com_channel_list;}
	
	/**
	 * Gets the motor group from the given ID. Throws exception if no matching
	 * motor group is found
	 * 
	 * @param[in] mg_id motor group id
	 * 
	 * @return a pointer to the matching motor group
	 */
	MotorGroup_ns::MotorGroup *get_motor_group_ptr_from_id(long );

	/**
	 * Gets the ghost motor group 
	 * @return a pointer to the ghost motor group
	 */
	MotorGroup_ns::MotorGroup *get_ghost_motor_group_ptr();
	
	/**
	 * Gets the motor group from the given name. Throws exception if no matching
	 * motor group is found
	 * 
	 * @param[in] mg_name motor group name
	 * 
	 * @return a reference to the matching motor group
	 */	
	MotorGroupPool &get_motor_group_from_name(string &);

	/**
	 * Gets the motor group from the given id. Throws exception if no matching
	 * motor group is found
	 * 
	 * @param[in] mg_id motor group id
	 * 
	 * @return a reference to the matching motor group
	 */	
	MotorGroupPool &get_motor_group_from_id(long );
	
	/**
	 * Gets the motor groups which contain the given element. 
	 * 
	 * @param[in]  elt_name element name
	 * @param[out] mgs      list of motor group names that contain the given 
	 *                      element
	 * @return <code>true</code> if there is at least one matching motor group or
	 *         <code>false</code> otherwise
	 */		
	bool get_motor_groups_containing_elt(string &, vector<string> &);

	/**
	 * Gets the motor groups which contain the given element. 
	 * 
	 * @param[in]  elt_name element name
	 * @param[out] mgs      list of motor groups that contain the given 
	 *                      element
	 * @return <code>true</code> if there is at least one matching motor group or
	 *         <code>false</code> otherwise
	 */		
	bool get_motor_groups_containing_elt(string &, vector<MotorGroupPool*> &);
	
	/**
	 * Determines the list of motor groups in a motor group hierarchy that 
	 * contain a certain element as a member.
	 * 
	 * @param[in]  mg       motor group in the hierarchy from which to start the
	 *                      search
	 * @param[in]  elt_name name of the element to search for
	 * @param[out] mgs      list of motor group names that contain the element 
	 *
	 * @return <code>true</code> if at least one of the motor groups in the 
	 *         hierarchy contain the element or <code>false</code> otherwise
	 */     
	bool get_motor_groups_in_hierarchy_containing_elt(MotorGroupPool &, string &,
	                                                  vector<string> &);

	/**
	 * Determines which pseudo motors use the given motor group
	 * 
	 * @param[in]  mg_id  motor group id
	 * @param[out] pm_lst list of matching pseudo motors
	 * 
	 * @return <code>true</code> if there is at least one matching pseudo motor or
	 *         <code>false</code> otherwise
	 */
	 bool get_pseudo_motors_that_use_mg(long, vector<string> &);
	
	/**
	 * Get counter/timer experiment channel from the given name. Throws 
	 * exception if no matching counter/timer experiment channel is found
	 * 
	 * @param[in] ch_name counter/timer experiment channel name
	 * 
	 * @return the matching counter/timer experiment channel
	 */
	CTExpChannelPool &get_ct_from_name(string &);

	/**
	 * Get 0D experiment channel from the given name. Throws 
	 * exception if no matching 0D experiment channel is found
	 * 
	 * @param[in] ch_name 0D experiment channel name
	 * 
	 * @return the matching 0D experiment channel
	 */
	ZeroDExpChannelPool &get_zerod_from_name(string &);
	
	/**
	 * Get experiment channel from the given name. Throws 
	 * exception if no matching experiment channel is found
	 * 
	 * @param[in] ch_name experiment channel name
	 * 
	 * @return the matching experiment channel
	 */	
	PoolElement &get_exp_channel_from_name(string &);

	/**
	 * Get experiment channel from the given id. Throws 
	 * exception if no matching experiment channel is found
	 * 
	 * @param[in] ch_id experiment channel id
	 * 
	 * @return the matching experiment channel
	 */
	PoolElement &get_exp_channel_from_id(long );
	
	/**
	 * Get experiment channel from the given name and type. Throws 
	 * exception if no matching experiment channel is found
	 * 
	 * @param[in] ch_name experiment channel name
	 * @param[in] ch_type experiment channel type
	 * 
	 * @return the matching experiment channel
	 */
	PoolElement &get_exp_channel_from_name(string &, MntGrpEltType &);
	
	/**
	 * Get experiment channel from the given id and type. Throws 
	 * exception if no matching experiment channel is found
	 * 
	 * @param[in] ch_name experiment channel name
	 * @param[in] ch_type experiment channel type
	 * 
	 * @return the matching experiment channel
	 */
	PoolElement &get_exp_channel_from_id(long, MntGrpEltType &);
	
	/**
	 * Gets the ghost measurement group 
	 * @return a pointer to the ghost measurement group
	 */	
	MeasurementGroup_ns::MeasurementGroup *get_ghost_measurement_group_ptr();
	
	/**
	 * Gets the measurement group from the given name. Throws exception if no 
	 * matching measurement group is found
	 * 
	 * @param[in] mg_name measurement group name
	 * 
	 * @return a reference to the matching measurement group
	 */	
	MeasurementGroupPool &get_measurement_group_from_name(string &);

	/**
	 * Gets the measurement group from the given id. Throws exception if no 
	 * matching measurement group is found
	 * 
	 * @param[in] mg_id measurement group id
	 * 
	 * @return a reference to the matching measurement group
	 */	
	MeasurementGroupPool &get_measurement_group_from_id(long );
	
	/**
	 * Gets the measurement groups that contains the given experiment channel. 
	 * 
	 * @param[in]  ch_name experiment channel name
	 * @param[out] mgs     list of matching measurement group names
	 * 
	 * @return <code>true</code> if there is at least one matching measurement 
	 *         group or <code>false</code> otherwise
	 */	
	bool get_measurement_groups_containing_channel(string &, vector<string> &);

	/**
	 * Get communication channel from the given name. Throws 
	 * exception if no matching communication channel is found
	 * 
	 * @param[in] ch_name communication channel name
	 * 
	 * @return the matching communication channel
	 */	
	CommunicationChannelPool &get_com_channel_from_name(string &);
	
	/**
	 * Get communication channel from the given id. Throws 
	 * exception if no matching communication channel is found
	 * 
	 * @param[in] ch_id communication channel id
	 * 
	 * @return the matching communication channel
	 */		
	CommunicationChannelPool &get_com_channel_from_id(long );
	
	/**
	 * Get a pool element from its name. Throws 
	 * exception if no matching element is found
	 * 
	 * @param[in] name pool element name
	 * 
	 * @return a reference to the matching pool element
	 */
	PoolElement &get_pool_element_from_name(string &);

	/**
	 * Get a physical pool element from its name. Excluded from the search
	 * are groups and pseudo elements. Throws exception if no matching element 
	 * is found
	 * 
	 * @param[in] name pool element name
	 * 
	 * @return a reference to the matching pool element
	 */
	PoolElement &get_phy_pool_element_from_name(string &);

	/**
	 * Get a group pool element from its name. Throws 
	 * exception if no matching element is found
	 * 
	 * @param[in] name pool element name
	 * 
	 * @return a reference to the matching pool element
	 */
	PoolElement &get_group_pool_element_from_name(string &);
	
	/**
	 * Get a pseudo pool element from its name. Throws 
	 * exception if no matching element is found
	 * 
	 * @param[in] name pool element name
	 * 
	 * @return a reference to the matching pool element
	 */
	PoolElement &get_pseudo_pool_element_from_name(string &);

	/**
	 * Get a the current Device factory for this pool
	 * 
	 * @return a pointer to the device factory
	 */
	inline IDeviceFactory *get_factory() { return factory; }
	
	//@}
	
	/** 
	 * @name Motion
	 * Set of motion related methods 
	 * @see PoolThread.cpp
	 */
	//@{

	/**
	 * Move motor(s) to the wanted position(s)
	 * This method as run by a separate thread
	 *
	 * @param[in] mot_ids   The vector of individual motor elements
	 * @param[in] positions The corresponding positions to move to
	 * @param[in] th        Pointer to the thread object
	 * @param[in] wait_flag Either to wait or not for the end of the movement.
	 *                      (default is <code>true</code>)
	 */
	//+------------------------------------------------------------------	
	void forward_move(vector<long> &, vector<double> &, PoolThread *, 
	                  bool wait_flag = true);
	
	/**
	 *	Sends position to controller(s)
	 *
	 * @param[in] mot_ids       The individual motor ids
	 * @param[in] positions     The corresponding positions to move to
	 * @param[in] th            The thread object
	 * @param[in] wait_flag     Either to wait or not for the end of the movement
	 * @param[in] implied_ctrls List of implied controllers
	 * @param[in] implied_mots: List of implied motors
	 * @param[in] implied_group Motor group
	 * @param[in] back_pos      The final position if a backlash has to be 
	 *                          applied. 0 otherwise
	 */	
	void send_to_ctrl(vector<long> &, vector<double> &, PoolThread *, 
	                  bool, vector<CtrlInMove> &, vector<MotInMove> &, 
	                  auto_ptr<GrpInMove> &, vector<double> &);
	
	/**
	 * Wait for end of moving and send appropriate event(s) to the right device
	 * at the right moment
	 *
	 * @param[in] implied_ctrls List of implied controllers
	 * @param[in] implied_mots  List of implied motors
	 * @param[in] implied_group Motor group
	 * @param[in] th            The thread object
	 * @param[in] back_pos      The final position if a backlash has to be 
	 *                          applied. 0 otherwise
	 * @param[in] obj_trigg     A vector of flag to memorize which objects are
	 *                          externally triggered     
	 */	
	void moving_loop(vector<CtrlInMove> &, vector<MotInMove> &, 
	                 auto_ptr<GrpInMove> &, PoolThread *, vector<double> &,
	                 vector<bool> &);
	
	/**
	 * Read motor positions during a movement
	 * 
	 * @param[in]  implied_mots    List of implied motors
	 * @param[in]  implied_group   Motor group
	 * @param[out] internal_events List of delayed events generated
	 * @param[in]  th              The thread object
	 * @param[in]  obj_trigg       A vector of flag to memorize which objects
	 *                             are externally triggered
	 * @param[in]  last_call       <code>true</code> means last call for the
	 *                             current motion.
	 * @param[in]  send_event      <code>true</code> to send event 
	 */     
	void read_pos_while_moving(vector<MotInMove> &, auto_ptr<GrpInMove> &,
	                           vector<DelayedEvt> *, PoolThread *,
	                           vector<bool> &, bool, bool);
	
	/** 
	 * Add all object implied in this movement to the list of externally 
	 * triggered polled objects
	 *
	 * @param[in]  implied_mots    List of implied motors
	 * @param[in]  implied_group   Motor group
	 * @param[in]  th              The thread object
	 * @param[in]  obj_trigg       A vector of flag to memorize which objects
	 *                             are externally triggered
	 */	
	void set_ext_trigg(vector<MotInMove> &, auto_ptr<GrpInMove> &,
	                   PoolThread *, vector<bool> &);
	
	/**
	 * Remove all object implied in this movement from the list of externally 
	 * triggered polled objects

	 * @param[in]  implied_mots    List of implied motors
	 * @param[in]  implied_group   Motor group
	 * @param[in]  th              The thread object
	 * @param[in]  obj_trigg       A vector of flag to memorize which objects
	 *                             are externally triggered
	 */
	void reset_ext_trigg(vector<MotInMove> &, auto_ptr<GrpInMove> &, 
	                     PoolThread *, vector<bool> &);

	//@}
	
	/** 
	 * Set the pool in moving state because it is actually shutting down.
	 * Also propagates this info to each motor that it is not possible to move 
	 * them and to change something in their tunning
	 */	
	void set_moving_state();

	/** 
	 * @name Data acquisition
	 * Set of data acquisition related methods 
	 * @see CTPoolThread.cpp
	 */
	//@{
	
	/**
	 * Starts a data acquisition.
	 * 
	 * @param[in] aq_info acquisition information
	 * @param[in] th      the pointer to the thread object
	 */
	void forward_count(AquisitionInfo &,CTPoolThread *);

	/**
	 * Sends values to the controller(s)
	 * 
	 * @param[in] aq_info       acquisition information
	 * @param[in] th            the pointer to the thread object
	 * @param[in] implied_ctrls list of implied controllers
	 * @param[in] implied_cts   list of implied channels
	 * @param[in] implied_group measurement group
	 */
	void send_to_ctrl(AquisitionInfo &, CTPoolThread *, vector<CtrlInCount> &,
	                  vector<CtInCount> &, auto_ptr<GrpInCount> &);
	
	/**
	 * Wait for end of data acquisition and send appropriate event(s) to the 
	 * right device at the right moment
	 * 
	 * @param[in] aq_info       acquisition information
	 * @param[in] implied_ctrls list of implied controllers
	 * @param[in] implied_cts   list of implied channels
	 * @param[in] implied_group measurement group
	 * @param[in] th            the pointer to the thread object
	 * @param[in] obj_trigg     A vector of flag to memorize which objects are
	 *                          externally triggered
	 */
	void counting_loop(AquisitionInfo &, vector<CtrlInCount> &, 
	                   vector<CtInCount> &, auto_ptr<GrpInCount> &, 
	                   CTPoolThread *, vector<bool> &);
	
	/**
	 * Read counter(s) value while they are counting
	 * 
	 * @param[in]  aq_info         acquisition information
	 * @param[in]  implied_cts     list of implied channels
	 * @param[in]  implied_group   measurement group
	 * @param[out] internal_events list of delayed events generated
	 * @param[in]  th              the pointer to the thread object
	 * @param[in]  obj_trigg       A vector of flag to memorize which objects
	 *                             are externally triggered
	 * @param[in]  last_call       <code>true</code> means last call for the
	 *                             current motion.
	 * @param[in]  send_event      <code>true</code> to send event 
	 */
	void read_val_while_counting(AquisitionInfo &, vector<CtInCount> &,
	                             auto_ptr<GrpInCount> &, vector<DelayedEvt> *,
	                             CTPoolThread *, vector<bool> &, bool, bool);

	/** 
	 * Add all object implied in this data acquisition to the list of externally 
	 * triggered polled objects
	 *
	 * @param[in]  implied_cts   List of implied channels
	 * @param[in]  implied_group Measurement group
	 * @param[in]  th            The thread object
	 * @param[in]  obj_trigg     A vector of flag to memorize which objects
	 *                           are externally triggered
	 */	
	void set_ext_trigg(vector<CtInCount> &, auto_ptr<GrpInCount> &, 
	                   CTPoolThread *, vector<bool> &);

	/**
	 * Remove all object implied in this data acquisition from the list of 
	 * externally triggered polled objects
	 * 
	 * @param[in]  implied_cts   List of implied channels
	 * @param[in]  implied_group Measurement group
	 * @param[in]  th            The thread object
	 * @param[in]  obj_trigg     A vector of flag to memorize which objects
	 *                           are externally triggered
	 */	
	void reset_ext_trigg(vector<CtInCount> &, auto_ptr<GrpInCount> &,
	                     CTPoolThread *, vector<bool> &);
	
	/**
	 * Finds the PropertyData object given by its name in the given vector of
	 * properties. Throws exception if no mathing property is found.
	 * 
	 * @param[in] prop_data vector of property data objects
	 * @param[in] prop_name property name
	 * 
	 * @return the matching property
	 */
	PropertyData *find_property_data(vector<PropertyData*>&, string &);
	
	/**
	 * Receives a list of class properties. It returns the same list filled with
	 * updated data at the instance level.
	 *
	 * @param[in]     inst_name the instance name
	 * @param[in]     class_name the class name
	 * @param[in]     prop_pairs pairs of (property, value)
	 * @param[in,out] prop_data	 a vector of properties containning information
	 *                           about all properties at the class level. It
	 *                           will be changed to contain updated information
	 *                           at the instance level				  
	 */
    void build_property_data(string &, string &, vector<pair<string, string> >&,
    						 vector<PropertyData*>&);

    /**
     * manages temporary motor groups. Should be called periodicaly by the 
     * ghost motor group.
     */ 
	void handle_tmp_motor_groups();

    /**
     * manages temporary measurement groups. Should be called periodicaly by the 
     * ghost measurement group.
     */ 
	void handle_tmp_measurement_groups();

	/**
	 * Translates the given user element into the physical elements it uses.
	 * 
	 * @param[in]  user_elem user element name
	 * @param[out] phy_elems vector of physical elements to be filled in
	 * @param[in]  unique <code>true</code> means do not put repeated elements.
	 */ 
    void user_elem_to_phy_elems(string &, vector<string> &, bool unique = true);

	/**
	 * Translates the given user elements into the physical elements it uses.
	 * 
	 * @param[in]  user_elems user element names
	 * @param[out] phy_elems vector of physical elements to be filled in
	 * @param[in]  unique <code>true</code> means do not put repeated elements.
	 */ 
    void user_elems_to_phy_elems(vector<string> &, vector<string> &, 
                                 bool unique = true);
    
	/**
	 * Translates the given user elements into the physical elements it uses.
	 * 
	 * @param[in]  user_elems user elements
	 * @param[out] phy_elems vector of physical elements to be filled in
	 * @param[in]  unique <code>true</code> means do not put repeated elements.
	 */ 
    void user_elems_to_phy_elems(vector<PoolElement*> &, vector<string> &, 
                                 bool unique = true);

};

/**
 * Compare struct for ControllerPool objects
 */
struct CtrlComp: public binary_function<ControllerPool,ControllerPool,bool>
{
	bool operator() (ControllerPool &cp1,ControllerPool &cp2)
	{
		return cp1.id < cp2.id;
	}
};

}	// namespace_ns

#endif	// _POOL_H
