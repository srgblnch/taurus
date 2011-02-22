//=============================================================================
//
// file :        PoolGroupBaseDev.h
//
// description : Include for the PoolGroupBaseDev class.
//
// project :	Sardana Device Pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.20  2007/08/30 12:40:39  tcoutinho
// - changes to support Pseudo counters.
//
// Revision 1.19  2007/08/24 15:55:26  tcoutinho
// safety weekend commit
//
// Revision 1.18  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.17  2007/07/26 10:25:15  tcoutinho
// - Fix bug 1 :  Automatic temporary MotorGroup/MeasurementGroup deletion
//
// Revision 1.16  2007/05/31 09:53:20  etaurel
// - Fix some memory leaks
//
// Revision 1.15  2007/05/30 14:48:40  etaurel
// - Add a boolean used when the pool device is executing an Init command
//
// Revision 1.14  2007/05/25 09:34:55  tcoutinho
// - fix internal event propagation to ghost motor group when client asks for state and the new calculated state is different from the one previously stored
//
// Revision 1.13  2007/05/22 13:43:09  tcoutinho
// - added new method
//
// Revision 1.12  2007/05/17 13:05:44  etaurel
// - Remove some print messages
//
// Revision 1.11  2007/05/16 16:26:22  tcoutinho
// - fix dead lock
//
// Revision 1.10  2007/05/16 07:56:35  tcoutinho
// - added some logging to find bug
//
// Revision 1.9  2007/02/22 12:04:13  tcoutinho
// - support for pool_elem_changed
//
// Revision 1.8  2007/02/16 10:01:40  tcoutinho
// - development checkin
//
// Revision 1.7  2007/02/13 14:39:43  tcoutinho
// - fix bug in motor group when a motor or controller are recreated due to an InitController command
//
// Revision 1.6  2007/02/08 16:18:14  tcoutinho
// - controller safety on PoolGroupBaseDev
//
// Revision 1.5  2007/02/07 16:53:06  tcoutinho
// safe guard commit
//
// Revision 1.4  2007/02/06 19:36:51  tcoutinho
// safe guard commit
//
// Revision 1.3  2007/02/06 19:11:23  tcoutinho
// safe guard commit
//
// Revision 1.2  2007/02/06 09:37:45  tcoutinho
// - Motor Group now uses PoolGroupBaseDev
//
// Revision 1.1  2007/02/03 15:19:41  tcoutinho
// new base tango group device
//
// Revision 1.1  2007/01/16 14:22:25  tcoutinho
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

#ifndef _POOLGROUPBASEDEV_H
#define _POOLGROUPBASEDEV_H

#include <Utils.h>
#include <PoolBaseDev.h>
#include <Pool.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

class Controller;

namespace Pool_ns
{

class AutoPoolLock;
struct ControllerPool;
struct PoolElement;
struct PoolElementEvent;

struct IndEltGrp;

struct CtrlGrp: public Tango::LogAdapter
{
	long					ctrl_id;
	ControllerPool			*ct;
	AutoPoolLock			*lock_ptr;

	map<long,IndEltGrp*>	channels;

	CtrlGrp(ControllerPool &ref, Tango::Device_3Impl *dev = NULL);
	
	virtual ~CtrlGrp() { Unlock(); }
	
	long get_ctrl_id()	{ return ctrl_id; }
	
	void Lock(vector<Controller *> *failed);
	void Unlock();
	void PreStateAll(vector<Controller *> *failed);
	void StateAll(vector<Controller *> *failed);
	
	void add_channel(IndEltGrp *);
	void remove_channel(IndEltGrp *);
};

struct IndEltGrp: public Tango::LogAdapter
{
	Pool_ns::PoolElement	*pe;
	Pool_ns::CtrlGrp		*ctrl_grp;
	Tango::Attribute		&state_att;
	Tango::AutoTangoMonitor	*atm_ptr;
	Tango::DeviceProxy		*obj_proxy;
	
	string					name;			///< use this instead of pe->name to avoid concurrent access to pool data
	long					id;				///< the element ID in the pool
	long					idx_in_ctrlgrp; ///< CtrlGrp index
	long					idx_in_ctrl;    ///< index in the controller
	long					grp_id;			///< Motor group to which this element belongs
	
	IndEltGrp(Pool_ns::PoolElement &ref,CtrlGrp *ct, 
			  long grp, Tango::Device_3Impl *dev = NULL);
	virtual ~IndEltGrp(); 
	
	virtual const char *get_family()	{ return "Element"; }
	PoolBaseDev *get_base_device()		{ return static_cast<PoolBaseDev*>(get_device()); }
	long get_ctrl_idx_in_grp()			{ return idx_in_ctrlgrp; }

	string &get_alias();
	Tango::Device_3Impl *get_device();
	
	void Lock();
	void Unlock();
	
	void PreStateOne(vector<Controller *> *failed);
	
	void abort(bool send_state_evt);
	void abort_evt(vector<Tango::DevFailed> *v_exp);
	void abort_no_evt(vector<Tango::DevFailed> *v_exp);
};

class PoolGroupBaseDev: public PoolBaseDev
{
	
protected :	

	Tango::TangoMonitor				*pos_mon;

	long							age;
	long							age_bckp;
	
	long							ind_elt_nb;				
	long 							usr_elt_nb;				
	vector<CtrlGrp*> 				implied_ctrls;
	vector<IndEltGrp*>				ind_elts;				 
	vector<Tango::DevState>			state_array;			
	vector<bool>					event_fired_array;		

	virtual void base_init();
	virtual void delete_from_pool();
	void base_delete_device();
	void base_dev_status(Tango::ConstDevString);
	void read_state_from_ctrls();
	
	virtual void init_pool_element(PoolElement *);
	virtual void State_all_ind(vector<Controller *> &) = 0;
	virtual void handle_temporary_siblings() = 0;
	
public:
	
	bool							pool_init_cmd;
	bool							th_failed;
	Tango::DevErrorList				th_except;

	PoolGroupBaseDev(Tango::DeviceClass *cl,string &s);
	PoolGroupBaseDev(Tango::DeviceClass *cl,const char *s);
	PoolGroupBaseDev(Tango::DeviceClass *cl, const char *c,const char *d);
	~PoolGroupBaseDev() {}
	
	vector<Tango::DevState> &get_state_array()	{ return state_array; }
	vector<bool> &get_event_fired_array()		{ return event_fired_array; }
	void clear_event_fired_array()				{ event_fired_array.reserve(ind_elt_nb);event_fired_array.assign(ind_elt_nb,false); }
	void get_implied_ctrls(vector<long>& v)		{ transform(implied_ctrls.begin(),implied_ctrls.end(),back_inserter(v),mem_fun(&Pool_ns::CtrlGrp::get_ctrl_id)); }
	
	bool is_ghost()								{ return get_id() == 0; }
	bool is_temporary()							{ return age >= 0; }
	long get_age()								{ return age; }
	void aging(long amount = 1)					{ age += is_temporary() ? amount : 0; }
	void reset_age()							{ if(is_temporary()) { age_bckp = age; age = 0; } }
	void restore_age()							{ if(is_temporary()) { age = age_bckp; } }
	
	bool is_ctrl_member(long ctrl_id);

	void send_state_event(vector<Tango::DevState> &,vector<Tango::DevState> &);
	
	long get_ind_elt_idx_from_id(long );
	IndEltGrp &get_ind_elt_from_name(string &);
	IndEltGrp &get_ind_elt_from_id(long );
	CtrlGrp	&get_ctrl_grp_from_id(long, long &);
	
	virtual void always_executed_hook();
	virtual Tango::DevState dev_state();
	virtual Tango::ConstDevString dev_status();
	
	virtual void build_grp() = 0;

	virtual int get_polling_th_id() = 0;
	
	virtual void pool_elem_changed(PoolElemEventList &,PoolElementEvent &) {}
};

}	// namespace_ns

#endif	// _POOLGROUPBASEDEV_H
