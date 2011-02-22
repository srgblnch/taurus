//=============================================================================
//
// file :         PoolClass.h
//
// description :  Include for the PoolClass root class.
//                This class is represents the singleton class for
//                the Pool device class.
//                It contains all properties and methods which the 
//                Pool requires only once e.g. the commands.
//			
// project :      TANGO Device Server
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//=============================================================================

#ifndef _EXTERNALFICA_H_
#define _EXTERNALFICA_H_

#include <Python.h>
#include <tango.h>
#include <PoolDef.h>

namespace Pool_ns
{

class ExternalFile;
class Pool;
class PoolClass;

class ExternalFiCa;

/**
 *  The PoolLock class declaration
 */
class PoolLock
{
public:
  
	/**
	 * Constructor
	 * 
	 * @param str name
	 * @param external_fica pointer to the external Fica
	 */ 
  	PoolLock(const char *str,ExternalFiCa *external_fica):mon(str),my_external_fica(external_fica),lock_wanted(true) {}
  	
  	/// Destructor
  	~PoolLock() {}
    
  	/**
  	 * Acquire the monitor
  	 */
  	void get_monitor();
  	
  	/**
  	 * Release the monitor
  	 */
  	void rel_monitor();
  
  	/**
  	 * Enable/Disavle lock
  	 * 
  	 * @param val enable/disavle
  	 */
  	void lock_enable(bool val) {lock_wanted = val;}
  	
  	/** 
  	 * Determines if the lock is enabled
  	 * 
  	 * @return <code>true</code> if enabled or <code>false</code> otherwise
  	 */
  	bool is_lock_enabled() {return lock_wanted;}
  
protected:

	Tango::TangoMonitor		mon;                   ///< tango monitor
  	ExternalFiCa  	      	*my_external_fica;     ///< pointer to the external Fica 
  	bool  		      		lock_wanted;           ///< lock wanted
};

/**
 *  The PythonLock class declaration
 */
class PythonLock
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param init_th_id thread id
	 * @param inter      pointer to the python interpreter state
	 */
  	PythonLock(int, PyInterpreterState *);
  	
  	/// Destructor
  	~PythonLock();
  
  	/**
  	 * Acquire lock
  	 */
  	void Get();
  	
  	/**
  	 * Release lock
  	 */
  	void Release();
  
protected:    
  	PyThreadState		*tstate;      ///< Python thread state
  	long  		      	locking_ctr;  ///< lock counter
};

/**
 * The ExternalFiCa class declaration
 */
class ExternalFiCa:public Tango::LogAdapter
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type       the FiCa type
	 * @param pool_class pointer to the PoolClass
	 * @param dev        pointer to the Pool device
	 */
	ExternalFiCa(string &, PoolClass *, Pool *);

	/**
	 * Constructor
	 * 
	 * @param type       the FiCa type
	 * @param f_name     file name
	 * @param class_name class name
	 * @param pool_class pointer to the PoolClass
	 * @param dev        pointer to the Pool device
	 */
	ExternalFiCa(string &, string &, string &, PoolClass *, Pool *);
	
	/// Destructor
	virtual ~ExternalFiCa();

	/**
	 * Gets the FiCa name
	 * @return reference to the string with the FiCa name
	 */
	string &get_name()				{ return name; }
	
	/**
	 * Gets a reference to the monitor of the FiCa
	 * @return a reference to the monitor
	 */
	PoolLock &get_mon()				{ return monitor; }
	
	/**
	 * Gets the PoolClass
	 * @return a pointer to the PoolClass
	 */
	PoolClass *get_pool_class()		{ return po_class; }
	
	/**
	 * Gets the ExternalFile
	 * @return a pointer to the ExternalFile
	 */
	ExternalFile *get_file()		{ return my_file; }

	/**
	 * Gets the language
	 * @return the language
	 */
	Language get_language();
	
	/**
	 * check if a file name is defined within all the directories defined in 
	 * the PYTHONPATH env. variable. It throws an exception if the file does
	 * not exist
	 * 
	 * @param class_name python class name to be checked
	 */
	void check_python(string &);
 
protected:

	string				name;            ///< FiCa name
	PoolLock			monitor;         ///< the monitor
	PoolClass			*po_class;       ///< pointer to the PoolClass
	ExternalFile		*my_file;        ///< pointer to the ExternalFile to which this FiCa belongs
};

typedef vector<ExternalFiCa *>::iterator vex_ite;

/**
 * The AutoPoolLock class declaration
 */
class AutoPoolLock
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param po_lock reference to the PoolLock
	 */
  	AutoPoolLock(PoolLock &po_lock): mon(po_lock) {mon.get_monitor();}
  	
  	/// Destructor
    ~AutoPoolLock() {mon.rel_monitor();}
    
protected:
    PoolLock &mon;	///< reference to the PoolLock
};

  

/**
 * The AutoPoolLock class declaration
 */
class AutoPythonLock
{
public:
	/// Constructor
  	AutoPythonLock();
  	
  	/// Destructor
    ~AutoPythonLock();

protected:
	int			th_id;    ///< thread id
	PoolClass 	*cl_ptr;  ///< pointer to the PoolClass
};

/**
 * The AutoCleanPythonLock class declaration
 */
class AutoCleanPythonLock
{
public:
	/// Constructor
  	AutoCleanPythonLock() {}
  	
  	/// Destructor
    ~AutoCleanPythonLock();
};

}

#endif // EXTERNALFICA_H_
