#ifndef _CTRLFILE_
#define _CTRLFILE_

#include <Python.h>
#include <tango.h>
#include <ExternalFile.h>
#include <PropertyData.h>
#include <PoolDef.h>
#include <PoolExtraAttrTypes.h>

namespace Pool_ns
{
	
class PoolClass;
class PythonLock;
class Pool;

#define DEFAULT_GENDER			"Generic"
#define DEFAULT_MODEL			"Generic"
#define DEFAULT_ORGANIZATION	"Tango community"
/**
 * The CtrlFile class declaration
 */
class CtrlFile:public ExternalFile
{
public:
	CtrlFile(string &,PoolClass *,Pool *);
	virtual ~CtrlFile();
	
	virtual long get_classes(vector<string>&,vector<string> &) = 0;	
	
	virtual void get_info(string &, vector<string> &) = 0;
	virtual void get_info(string &, string &, vector<string> &) = 0;

	virtual void get_info_ex(string &, Tango::DevVarCharArray *) = 0;
	virtual void get_info_ex(string &, string &, Tango::DevVarCharArray *) = 0;

	virtual void get_prop_info(string &, vector<string> &);
	virtual void get_prop_info(string &, vector<PropertyData*> &) = 0;
		
	virtual void get_sequence_value(string &,const char *,vector<string> &) = 0;
	virtual void get_extra_attr_dec(string &,const char *,vector<PoolExtraAttr> &) {}
	
	virtual long get_MaxDevice(string &cl) {return -1;}

	virtual void reload() = 0;

	CtrlType get_ctrl_type() {return ctrl_obj_type;}		
			
protected:
	virtual void vecinfo_to_chararray(vector<string> &,Tango::DevVarCharArray *);
	
	CtrlType			ctrl_obj_type;
};

typedef vector<CtrlFile *>::iterator vcf_ite;

} // End of Pool_ns namespace

#endif /*_CTRLFILE_*/
