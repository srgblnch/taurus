#ifndef _PYEXTERNALFILE_
#define _PYEXTERNALFILE_

#include <Python.h>
#include <tango.h>
#include <CtrlFile.h>
#include <PoolDef.h>

namespace Pool_ns
{
	
class PoolClass;
class PythonLock;
class Pool;

#define CLASS_PROP_ATTR		"class_prop"

#define INST_NAME_ATTR		"inst_name"

#define TYPE_KEY		"Type"
#define DESCR_KEY		"Description"
#define DFT_VALUE_KEY		"DefaultValue"

/**
 * The PyExternalFile class declaration
 */
class PyExternalFile:public CtrlFile
{
public:
	PyExternalFile(string &,PoolClass *,Pool *);
	PyExternalFile(PyExternalFile &,PoolClass *,Pool *);
	~PyExternalFile();

	long get_classes(vector<string>&,vector<string> &);
	void get_prop_info(string &, vector<PropertyData*> &);
	
	virtual void get_prop_info(PyObject *, vector<string> &);
	virtual void get_prop_info(PyObject *, vector<PropertyData*> &);
	
	void append_sequence_value(string &,const char *,vector<string> &);
	void get_sequence_value(string &,const char *,vector<string> &);
	
	void check_py(const char *);	
	void reload();
	PyObject *get_py_module() {return module;}
	void check_py_method(PyObject *,const char *);
	
	void get_py_doc(string &,vector<string> &);
	
	bool reload_allowed(string &);
	
protected:	
	virtual void check_py_methods(PyObject *class_obj) = 0;
	virtual const char *get_super_class() = 0;
	
	void init_py_env();
	PyObject *load_py_module(string &);
	void update_py_path();
	
	void throw_dev_failed_from_py(PythonLock *);
	void Py_init_dev_error(PyObject *,PyObject *,PyObject *,Tango::DevErrorList &);
	
	PyObject *get_py_class(const char *);
	void check_py_class(PyObject *, PyObject *);

	PyObject *get_py_valid_class(const char *);	
	void check_py(PyObject *);
	
	PyObject			*module;
	bool				mod_dec_ref;
};


} // End of Pool_ns namespace

#endif /*_EXTERNALFILE_*/
