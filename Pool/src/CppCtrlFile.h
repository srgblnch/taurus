#ifndef CPPCTRLFILE_H_
#define CPPCTRLFILE_H_

#include <Python.h>
#include <tango.h>
#include <ltdl.h>
#include <CtrlFile.h>

namespace Pool_ns
{

class PoolClass;
class Pool;
class PythonLock;

/**
 * @brief The CtrlFile class declaration
 */
class CppCtrlFile : public CtrlFile
{
public:
	CppCtrlFile(string &,const char *,PoolClass *,Pool *);
	CppCtrlFile(CppCtrlFile &,const char *,PoolClass *,Pool *);
	~CppCtrlFile();

	virtual long get_classes(vector<string> &,vector<string> &);	
	
	virtual void get_info(string &, vector<string> &);
	virtual void get_info(string &, string &, vector<string> &);
	
	virtual void get_info_ex(string &, Tango::DevVarCharArray *);
	virtual void get_info_ex(string &, string &, Tango::DevVarCharArray *);
	
	virtual void get_prop_info(string &, vector<PropertyData*> &);

	lt_dlhandle get_lib_ptr() {return lib_ptr;}
	
	void reload();
	
	virtual long get_MaxDevice(string &);
	
	virtual void get_sequence_value(string &,const char *,vector<string> &);
	virtual void get_extra_attr_dec(string &,const char *,vector<PoolExtraAttr> &);

protected:
	virtual void load_code();
	void get_cpp_elem(string &,const char *,vector<string> &, const char *dft="");
	void get_cpp_doc(string &,vector<string> &);
	void get_cpp_gender(string &,vector<string> &);
	void get_cpp_model(string &,vector<string> &);
	void get_cpp_image(string &,vector<string> &);
	void get_cpp_organization(string &,vector<string> &);
	void get_cpp_logo(string &,vector<string> &);
	void get_cpp_icon(string &,vector<string> &);
	
	bool get_cpp_str_array(string &,const char *,vector<string> &);
	
	virtual void get_particular_info(string &, vector<string> &) {}
	virtual void get_particular_info(string &, string &, vector<string> &) {}
	
	/** a handler to the library */
	lt_dlhandle			lib_ptr;
	bool				close_lib;
	bool				include_MaxDevice;
};

typedef vector<CppCtrlFile *>::iterator v_cpp_ctrl_ite;

} // End of Pool_ns namespace

#endif /*CTRLFILE_H_*/
