//=============================================================================
//
// file :        PoolBaseDev.h
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
// Revision 1.10  2007/08/30 12:40:39  tcoutinho
// - changes to support Pseudo counters.
//
// Revision 1.9  2007/08/17 13:07:30  tcoutinho
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

#ifndef _POOLBASEDEV_H
#define _POOLBASEDEV_H

#include <Python.h>
#include <tango.h>
#include <pool/Constraint.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

namespace Pool_ns
{

class CtrlFiCa;
class Pool;
class PoolBaseUtil;
struct PoolElement;

/**
 * Interface for a constraint able Pool device. By constraint able it is meant
 * that the constraint can check this object for its value in order to calculate
 * its own contraint algorithm.
 */ 
class IConstraintable
{
public:
	
	/**
	 * Adds a new constraint to this object. If the constraint is already there
	 * nothing happens.
	 * 
	 * @param	c			[in]	the constraint to be added
	 * @param	start_idx	[in]	the index of this constraintable in the constraint
	 */
	void add_constraint(Constraint *c, unsigned long start_idx)
	{
		pair<Constraint*, unsigned long> p(c,start_idx);
		if (find(constraints.begin(),constraints.end(),p) == constraints.end())
			constraints.push_back(p);
	}
	
	/**
	 * Remove the constraint from this object. If the constraint is not 
	 * registered nothing happens.
	 * 
	 * @param	c			[in]	the constraint to be removed
	 */
	void remove_constraint(Constraint *c)
	{
		list<pair<Constraint*,unsigned long> >::iterator it;
		for(it = constraints.begin(); it != constraints.end(); ++it)
		{
			if (it->first == c)
			{
				constraints.erase(it);
				break;
			}
		}
	}
	
	/**
	 * Returns a reference to the list of constraints assigned to this object.
	 * 
	 * @return the list of constraints.
	 */
	list<pair<Constraint*, unsigned long> > &get_constraints() 
	{ return constraints; }
	
protected:
	
	/**
	 * list of constraints assigned to this constraintable.
	 * - The first element of the pair is the constraint
	 * - The second element of the pair is the index of this constraintable in 
	 *   the corresponding constraint
	 */
	list<pair<Constraint*,unsigned long> >	constraints;
};


/**
 * A generic Device for the Pool
 */
class PoolBaseDev: public Tango::Device_3Impl
{
public:
	
	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 */ 
	PoolBaseDev(Tango::DeviceClass *cl,string &s);

	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 */ 
	PoolBaseDev(Tango::DeviceClass *cl,const char *s);
	
	/** 
	 * Constructor
	 * 
	 * @param cl the pointer to the DeviceClasss
	 * @param s the name
	 * @param d
	 */ 	
	PoolBaseDev(Tango::DeviceClass *cl, const char *c,const char *d);
	
	/// Destructor
	virtual ~PoolBaseDev() {}
	
	/**
	 * Get the device alias
	 * @return the device alias
	 */
	inline const string &get_alias()          { return alias; }
	
	/**
	 * Gets the current movement thread id
	 * @return the current movement thread id
	 */
	inline int get_mov_th_id()                { return mov_th_id; }
	
	/**
	 * Activates shutdown mode
	 */
	inline void pool_shutdown()		          { pool_sd = true; }
	
	/**
	 * Set the movement thread id
	 * 
	 * @param[in] t the thread id
	 */
	inline void set_mov_th_id(int t)           { mov_th_id = t; }
	
	/**
	 * Sets the Utils object
	 * 
	 * @param ptr pointer to the utils object
	 */
	inline void set_utils(PoolBaseUtil *ptr)  { utils = ptr; }
	
	/**
	 * Deletes the Utils object
	 */
	void delete_utils();
	
	/**
	 * Gets the current object id
	 * @return the current object id
	 */
	virtual long get_id() = 0;
	
	/** 
	 * Base abort 
	 */
	virtual void base_abort(bool) = 0;
	
	/**
	 * Delete this object from the pool
	 */
	virtual void delete_from_pool();
	
	/**
	 * Initializes data of the PoolElement
	 * @param[out] elem the pool element to be initialized
	 */
	virtual void init_pool_element(PoolElement *);
	
	/** <code>true</code> if the abort command */
	bool				abort_cmd_executed;

	/**
	 * debugging output
	 */
	void dbg_dvector(vector<double> &, const char *optcstr = "");
	
protected :
	
	/** Pool device */
	Pool 				*pool_dev;
	
	/** Utils pointer */
    PoolBaseUtil		*utils;
    
    /** device alias */
	string				alias;
	
	/** temporary status string */
	string				tmp_status;
	
	/** 
	 * <code>true</code> if init_device or <code>false</code> if during 
	 * object construction
	 */
	bool				init_cmd;
	
	/** 
	 * <code>true</code> if pool in shutdown mode or <code>false</code> 
	 * otherwise
	 */
	bool				pool_sd;
	
	/** movement thread id */
	int					mov_th_id;

	/**
	 * Base Init
	 */
	virtual void base_init();
	
	/** 
	 * Gets the corresponding pool object
	 * @return the pool object
	 */
	virtual PoolElement &get_pool_obj() = 0;
};

}	// namespace_ns

#endif	// _POOLBASEDEV_H
