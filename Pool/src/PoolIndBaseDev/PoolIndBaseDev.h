//=============================================================================
//
// file :        PoolIndBaseDev.h
//
// description : Include for the PoolIndBaseDev class.
//
// project :	Sardana Device Pool
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
// Revision 1.8  2007/05/25 12:48:10  tcoutinho
// fix the same dead locks found on motor system to the acquisition system since release labeled for Josep Ribas
//
// Revision 1.7  2007/05/22 13:43:09  tcoutinho
// - added new method
//
// Revision 1.6  2007/02/28 16:22:21  tcoutinho
// - added get_alias method
//
// Revision 1.5  2007/02/22 12:03:04  tcoutinho
// - additional "get extra attribute"  needed by the measurement group
//
// Revision 1.4  2007/02/08 07:55:54  etaurel
// - Changes after compilation -Wall. Handle case of different ctrl for the
// same class of device but with same extra attribute name
//
// Revision 1.3  2007/01/30 15:57:41  etaurel
// - Add a missing data member init
// - Add code to manage case with different controller of the same Tango class
// with extra attribute of the same name but with different data type
//
// Revision 1.2  2007/01/26 08:34:32  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.1  2007/01/16 14:22:25  etaurel
// - Initial revision
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

#ifndef _POOLINDBASEDEV_H
#define _POOLINDBASEDEV_H

#define		MAX_EXTRA_ATTRIBUTES		128

#include <PoolBaseDev.h>
#include <PoolExtraAttrTypes.h>

class Controller;

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Pool_ns
{
	
class Pool;
class CtrlFiCa;

/**
 * A generic individual Device for the Pool
 */
class PoolIndBaseDev: public PoolBaseDev
{
public:
	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 */ 
	PoolIndBaseDev(Tango::DeviceClass *cl,string &s);

	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 */ 
	PoolIndBaseDev(Tango::DeviceClass *cl,const char *s);
	
	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 * @param d
	 */ 	
	PoolIndBaseDev(Tango::DeviceClass *cl, const char *c,const char *d);
	
	/// Destructor
	virtual ~PoolIndBaseDev() {}

	/**
	 * Returns if the controller is built
	 * @return <code>true</code> if controller is built or
	 *         <code>false</code> otherwise
	 */ 
	inline bool is_my_ctrl_built()               { return ctrl_dev_built; }
	
	/**
	 * Returns if the controller is built
	 * @return <code>true</code> if controller is built or
	 *         <code>false</code> otherwise
	 */ 	
	inline bool is_fica_built()	                { return fica_built; }
	
	/**
	 * Gets the controller
	 * @return pointer to the controller
	 */
	inline Controller *get_ctrl()               { return my_ctrl; }
	
	/**
	 * Gets the controller FiCa
	 * @return pointer to the controller FiCa
	 */
	inline CtrlFiCa *get_fica_ptr()             { return fica_ptr; }
	
	/**
	 * Gets the controller index
	 * @return the controller index
	 */
	inline long get_controller_idx()            { return ct_idx; }
	
	/**
	 * Informs that the controller is offline
	 */
	inline void ctrl_offline()                  { ctrl_code_online = false; }

	/**
	 * Informs that the controller is online
	 */
	inline void ctrl_online()                   { ctrl_code_online = true; }
	
	/**
	 * Determines if in simulation mode
	 * @return <code>true</code> if in simulation mode or
	 *         <code>false</code> otherwise
	 */
	inline bool simulation()                    { return simu; }
	
	/** 
	 * Sets/Unsets simulation mode
	 * @param[in] sim <code>true</code> to set simulation or <code>false</code>
	 *            to switch it off
	 */ 
	inline void simulation(bool sim)            { simu = sim; }
	
	/**
	 * Sets the controller for this element
	 * @param[in] ptr controller pointer
	 */
	inline void set_ctrl(Controller *ptr)       { my_ctrl = ptr; fica_built = ptr != NULL; }
	
	/** 
	 * Inform the device controller that we are dying
	 */
	void suicide();
	
	/**
	 * Inform the device controller that we now exist
	 * param[in] ctrl_id controller id
	 */
	void a_new_child(long);
	
	/** 
	 * The basic code to be executed in the always_executed_hook method of 
	 * Pool based device
	 * @param[in] motor boolean set to true if the device is a motor for which 
	 *                  the limit switches (with ALARM state) has to be taken 
	 *                  into account
	 * @param[in] propagate if set to true the ghost group is informed of the
	 *            state change
	 */
	void base_always_executed_hook(bool motor,bool propagate = true);
	
	/** 
	 * Read a single element state from the controller.
	 * 
	 * @param[in] sta  The structure used to pass object state
	 * @param[in] lock A boolean set to true if the method must lock the
	 *                 controller
	 */
	void read_state_from_ctrl(Controller::CtrlState &,bool);
	
	/**
	 * The base dev_status
	 */
	void base_dev_status(Tango::ConstDevString);
	
	/**
	 * Delete this object from the pool
	 */
	virtual void delete_from_pool();
	
	/**
	 * Initializes data of the PoolElement
	 * @param[out] elem the pool element to be initialized
	 */
	virtual void init_pool_element(PoolElement *);
	
	/** 
	 * Determines if this object should be in fault
	 */
	bool should_be_in_fault() 
	{
		return fica_built == false || my_ctrl == NULL || 
			   this->ctrl_code_online == false || 
			   ctrl_dev_built == false;
	}
	

	/**
	 * @name Dynamic attributes
	 */
	//@{
	
	/**
	 * Create dynamic attributes
	 */
	void create_dyn_attr();
	
	/**
	 * Create one extra attribute
	 * 
	 * @param extra_attr Extra attribute information
	 */
	void create_one_extra_attr(PoolExtraAttr &);
	
	/**
	 * Remove unwanted dynamic attributes from the device
	 */
	void remove_unwanted_dyn_attr_from_device();
	
	long ExtraAttrDataType_2_Tango(ExtraAttrDataType &);
	Tango::AttrWriteType ExtraAttrWriteType_2_Tango(ExtraAttrDataWrite &);
		
	map<string,bool>	bool_extra_data;
	map<string,long>	long_extra_data;
	map<string,double>	double_extra_data;
	map<string,long>	string_extra_index;
	vector<string>		string_extra_data;
	Tango::DevString	string_extra_attr[MAX_EXTRA_ATTRIBUTES];
	long				sf_index;
	
	bool is_ExtraAttr_allowed(Tango::AttReqType type);
	
	void read_Boo_R_Attr(Tango::Attribute &attr);	
	void read_Boo_RW_Attr(Tango::Attribute &attr);
	void write_Boo_RW_Attr(Tango::WAttribute &attr);

	void read_Dou_R_Attr(Tango::Attribute &attr);		
	void read_Dou_RW_Attr(Tango::Attribute &attr);
	void write_Dou_RW_Attr(Tango::WAttribute &attr);

	void read_Lo_R_Attr(Tango::Attribute &attr);	
	void read_Lo_RW_Attr(Tango::Attribute &attr);
	void write_Lo_RW_Attr(Tango::WAttribute &attr);
	
	void read_Str_R_Attr(Tango::Attribute &attr);	
	void read_Str_RW_Attr(Tango::Attribute &attr);
	void write_Str_RW_Attr(Tango::WAttribute &attr);
	
	vector<PoolExtraAttr> &get_extra_attributes();

	//@}
protected :	

	Controller 			*my_ctrl;	
	CtrlFiCa 			*fica_ptr;
	    
	string 				ctrl_inst_name;
	bool				fica_built;
	long				ct_idx;
	bool				ctrl_dev_built;
	string				ctrl_add_dev_error_str;
	bool				ctrl_code_online;
	bool				simu;
	bool				unknown_state;

	virtual void base_init();	
	void analyse_name(string &,string &,long &);
	
//
// For commands, attribute
//
	string				ctrl_error_str;
	
	virtual void inform_ghost(Tango::DevState,Tango::DevState) = 0;
};

}	// namespace_ns

#endif	// _POOLINDBASEDEV_H
