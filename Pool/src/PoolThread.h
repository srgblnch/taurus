//=============================================================================
//
// file :        PoolThread.h
//
// description : Include for the PoolThread class.
//
// project :	Sardana Device pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.16  2007/08/23 10:32:44  tcoutinho
// - basic pseudo counter check
// - some fixes regarding pseudo motors
//
// Revision 1.15  2007/05/30 14:58:44  etaurel
// - Remove some printed messages
//
// Revision 1.14  2007/05/22 13:42:04  tcoutinho
// - fix dead lock on internal event propagation in case of an abort command
//
// Revision 1.13  2007/05/17 13:05:13  etaurel
// - Remove some print messages
//
// Revision 1.12  2007/05/16 16:26:22  tcoutinho
// - fix dead lock
//
// Revision 1.11  2007/02/08 08:51:16  etaurel
// - Many changes. I don't remember the list
//
// Revision 1.10  2007/02/07 16:53:06  tcoutinho
// safe guard commit
//
// Revision 1.9  2007/01/16 14:32:22  etaurel
// - Coomit after a first release with CT
//
// Revision 1.8  2006/12/20 10:25:36  tcoutinho
// - changes to support internal event propagation
// - bug fix in motor groups containing other motor groups or pseudo motors
//
// Revision 1.7  2006/10/20 15:37:30  etaurel
// - First release with GetControllerInfo command supported and with
// controller properties
//
// Revision 1.6  2006/08/08 12:17:19  etaurel
// - It now uses the multi-motor controller interface
//
// Revision 1.5  2006/07/07 12:38:43  etaurel
// - Some changes in file header
// - Commit after implementing the group multi motor read
//
// Revision 1.4  2006/06/28 15:56:46  etaurel
// - Commit after first series of tests
//
// Revision 1.3  2006/05/26 09:12:52  etaurel
// - Add some exception checking between the thread used to move motor and the
// write_Position method
//
// Revision 1.2  2006/05/15 10:54:48  etaurel
// - Change the event used to report on motor movement from USER event to CHANGE event
//
// Revision 1.1  2006/04/27 07:29:43  etaurel
// - Many changes after the travel to Boston
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//----------------------------------------------------------------------------------

#ifndef _POOLTHREAD_H
#define _POOLTHREAD_H

#include <tango.h>

namespace MotorGroup_ns
{
	class MotorGroup;
}

namespace Pool_ns
{

class Pool;
class PoolElement;
class ControllerPool;
class MotorGroupPool;

struct CtrlInMove
{
	CtrlInMove(long id,ControllerPool &ref):ctrl_id(id),ct(ref),lock_ptr(NULL) {}
	CtrlInMove &operator=(const CtrlInMove &rhs) {ctrl_id=rhs.ctrl_id;ct=rhs.ct;return *this;}
	
	long					ctrl_id;
	ControllerPool	&ct;
	Pool_ns::AutoPoolLock	*lock_ptr;
	
	void lock()		{ lock_ptr = new AutoPoolLock(ct.get_ctrl_fica_mon()); }
	void unlock()	{ if(lock_ptr != NULL) { delete lock_ptr; lock_ptr = NULL; } }
};

struct ElemInMove
{
	ElemInMove(PoolElement &ref):
		state_att(ref.get_device()->get_device_attr()->get_attr_by_name("state")),
		pos_att(ref.get_device()->get_device_attr()->get_attr_by_name("Position")),
		atm_ptr(NULL) {}

	Tango::Attribute		&state_att;
	Tango::Attribute		&pos_att;

	Tango::AutoTangoMonitor	*atm_ptr;
		
};

struct MotInMove: public ElemInMove
{
	MotInMove(long id,MotorPool &ref,ControllerPool &ct_ref):
		ElemInMove(ref),
		mot_id(id),mot(ref),motor(mot.motor),ct(ct_ref)
	{}
	
	MotInMove &operator=(const MotInMove &rhs) 
	{ 
		mot_id=rhs.mot_id ; mot=rhs.mot ; ct=rhs.ct ; state_att=rhs.state_att; 
		pos_att=rhs.pos_att;
		return *this;
	}
	
	long				mot_id;
	MotorPool			&mot;
	Motor_ns::Motor		*motor;
	ControllerPool		&ct;
	
	void Lock();
	void Unlock();
};

struct GrpInMove: public ElemInMove
{
	GrpInMove(MotorGroupPool &ref);
	
	GrpInMove &operator=(const GrpInMove &rhs);
	
	~GrpInMove() { delete grp_proxy; }
	
	MotorGroupPool				&mgp;
	MotorGroup_ns::MotorGroup	*grp;
	Tango::DeviceProxy			*grp_proxy;
};

struct DelayedEvt
{
	PoolElement			*src;
	PoolElementEvent	evt;
	PoolElement			*exception;

	DelayedEvt(PoolEventType evt_type,PoolElement* src_elem):
		src(src_elem), evt(evt_type,src_elem), exception(NULL) 
	{}
	
	DelayedEvt(const DelayedEvt &rhs):
		src(rhs.src),evt(rhs.evt),exception(rhs.exception)
	{}
};

class PoolThread: public omni_thread
{
public :
	PoolThread(vector<long> &m_ids,vector<double> &pos,Pool *p_dev,Tango::TangoMonitor *mon,long gr=-1)
	:mon_ptr(mon),group_id(gr),mot_ids(m_ids),positions(pos),pool_dev(p_dev) {}
	
	void run(void *);
	void manage_thread_exception(Tango::DevFailed &,vector<CtrlInMove> &,vector<MotInMove> &,auto_ptr<GrpInMove> &,string &,bool,long);
	
	Tango::TangoMonitor			*mon_ptr;
	Motor_ns::Motor				*failed_mot;
	MotorGroup_ns::MotorGroup	*failed_group;
	long						group_id;
	
protected:
	vector<long> 				mot_ids;
	vector<double> 				positions;
	Pool 						*pool_dev;
	
};

}	// namespace_ns

#endif	// _POOLTHREAD_H
