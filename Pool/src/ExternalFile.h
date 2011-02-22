#ifndef _EXTERNALFILE_
#define _EXTERNALFILE_

#include <Python.h>
#include <tango.h>
#include <PoolDef.h>

namespace Pool_ns
{
	
class PoolClass;
class PythonLock;
class Pool;

/**
 * The ExternalFile class declaration
 */
class ExternalFile:public Tango::LogAdapter
{
public:
	ExternalFile(string &,PoolClass *,Pool *);
	virtual ~ExternalFile();
	
	string &get_name() {return name;}
	string &get_name_lower() {return name_lower;}
	string &get_full_name() {return full_name;}
	Language get_language() {return lang;}
	static Language get_language(string &);
	string &get_path() { return path; }
			
protected:
	virtual void load_code() = 0;
	
	Language			lang;
	string				name;				///< filename
	string				name_lower;			///< filename in lower case
	string 				path;				///< absolute path
	string				full_name;			///< absolute path + filename
	PoolClass			*po_class;
	Pool				*the_device;
};

typedef vector<ExternalFile *>::iterator vef_ite;

} // End of Pool_ns namespace

#endif /*_EXTERNALFILE_*/
