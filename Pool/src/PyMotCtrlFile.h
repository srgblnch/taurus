#ifndef _PYMOTCTRLFILE_
#define _PYMOTCTRLFILE_

#include <Python.h>
#include <tango.h>
#include <PyCtrlFile.h>

namespace Pool_ns
{

/**
 * The PyUndefCtrlFile class declaration
 */
class PyUndefCtrlFile: public PyCtrlFile 
{
public:
	PyUndefCtrlFile(string &,PoolClass *,Pool *);
	~PyUndefCtrlFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};

/**
 * The PyMotCtrlFile class declaration
 */
class PyMotCtrlFile: public PyCtrlFile 
{
public:
	PyMotCtrlFile(string &,PoolClass *,Pool *);
	PyMotCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyMotCtrlFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};

#define CALC_PHYSICAL_METHOD			"calc_physical"
#define CALC_PSEUDO_METHOD				"calc_pseudo"
#define CALC_ALL_PHYSICAL_METHOD		"calc_all_physical"
#define CALC_ALL_PSEUDO_METHOD			"calc_all_pseudo"

#define MOTOR_ROLES_ATTR				"motor_roles"
#define PSEUDO_MOTOR_ROLES_ATTR			"pseudo_motor_roles"

/**
 * The PyPseudoMotCtrlFile class declaration
 */
class PyPseudoMotCtrlFile: public PyCtrlFile
{
public:
	
	PyPseudoMotCtrlFile(string &,PoolClass *,Pool *);
	PyPseudoMotCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	virtual ~PyPseudoMotCtrlFile();

	virtual void get_info(string &, vector<string> &);
	virtual void get_info(string &, string &, vector<string> &);
	
protected:
	
	virtual void check_py_methods(PyObject *);
	virtual const char *get_super_class();
	
	void get_pseudo_info(string &,vector<string> &);
};

/**
 * The PyCoTiCtrlFile class declaration
 */
class PyCoTiCtrlFile: public PyCtrlFile 
{
public:
	PyCoTiCtrlFile(string &,PoolClass *,Pool *);
	PyCoTiCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyCoTiCtrlFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};

/**
 * The PyZeroDCtrlFile class declaration
 */
class PyZeroDCtrlFile: public PyCtrlFile 
{
public:
	PyZeroDCtrlFile(string &,PoolClass *,Pool *);
	PyZeroDCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyZeroDCtrlFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};


#define CALC_METHOD			"calc"
#define COUNTER_ROLES_ATTR	"counter_roles"

/**
 * The PyPseudoCoCtrlFile class declaration
 */
class PyPseudoCoCtrlFile: public PyCtrlFile 
{
public:
	PyPseudoCoCtrlFile(string &,PoolClass *,Pool *);
	PyPseudoCoCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyPseudoCoCtrlFile();
			
	virtual void get_info(string &, vector<string> &);
	virtual void get_info(string &, string &, vector<string> &);
	
protected:
	
	virtual void check_py_methods(PyObject *);
	virtual const char *get_super_class();
	
	void get_pseudo_info(string &, vector<string> &);
};

/**
 * The PyComCtrlFile class declaration
 */
class PyComCtrlFile: public PyCtrlFile 
{
public:
	PyComCtrlFile(string &,PoolClass *,Pool *);
	PyComCtrlFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyComCtrlFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};

/**
 * The PyConstraintFile class declaration
 */
class PyConstraintFile: public PyCtrlFile 
{
public:
	PyConstraintFile(string &,PoolClass *,Pool *);
	PyConstraintFile(PyUndefCtrlFile &,PoolClass *,Pool *);
	~PyConstraintFile();
			
protected:
	virtual void check_py_methods(PyObject *class_obj);
	virtual const char *get_super_class();
};

} // End of Pool_ns namespace

#endif /*_PYMOTCTRLFILE_*/
