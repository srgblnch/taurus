#ifndef _PYCTRLFILE_
#define _PYCTRLFILE_

#include <Python.h>
#include <tango.h>
#include <ltdl.h>
#include <PyExternalFile.h>

namespace Pool_ns
{

#define EXT_ATTR_TYPE_KEY		"Type"
#define EXT_ATTR_RWTYPE_KEY		"R/W Type"
	
class PoolClass;
class PythonLock;
class Pool;

/**
 * The PyCtrlFile class declaration
 */
class PyCtrlFile:public PyExternalFile 
{
public:
	PyCtrlFile(string &,const char *,PoolClass *,Pool *);
	PyCtrlFile(PyCtrlFile &,const char *,PoolClass *,Pool *);
	~PyCtrlFile();
	
	virtual void get_info(string &, vector<string> &);
	virtual void get_info(string &, string &, vector<string> &);
	virtual void get_info_ex(string &, Tango::DevVarCharArray *);
	virtual void get_info_ex(string &, string &, Tango::DevVarCharArray *);
	
	virtual long get_MaxDevice(string &);
	virtual void get_prop_info(string &,vector<PropertyData*> &);
	virtual void get_extra_attr_dec(string &,const char *,vector<PoolExtraAttr> &);
	
	lt_dlhandle get_py_inter_lib_ptr() {return py_inter_lib_ptr;}
			
protected:
	void load_code();	
	void load_py_inter_lib(string &);
	string get_py_inter_lib_name(string &);
	
	void get_py_elem(string &,const char *,vector<string> &, const char *dft);
	void get_py_gender(string &,vector<string> &);
	void get_py_model(string &,vector<string> &);
	void get_py_image(string &,vector<string> &);
	void get_py_organization(string &,vector<string> &);
	void get_py_logo(string &,vector<string> &);
	void get_py_icon(string &,vector<string> &);
	
	lt_dlhandle		py_inter_lib_ptr;
	bool			include_MaxDevice;
};


} // End of Pool_ns namespace

#endif /*_EXTERNALFILE_*/
