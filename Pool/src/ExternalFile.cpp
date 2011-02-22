#include <ExternalFile.h>
#include <PoolClass.h>

namespace Pool_ns
{

//-----------------------------------------------------------------------------
//
//					The ExternalFile class
//
//-----------------------------------------------------------------------------

//+----------------------------------------------------------------------------
//
// method : 		ExternalFile::ExternalFile()
// 
// description : 	constructor for ExternalFile class
//
// in : - f_name : The class file name (cpp lib or python module)
//		- class_name : The class name (case dependant)
//		- pool_class : Pointer to the PoolClass object
//
//-----------------------------------------------------------------------------
 
ExternalFile::ExternalFile(string &f_name,PoolClass *pool_class,Pool *dev)
:Tango::LogAdapter(dev),full_name(f_name),po_class(pool_class),the_device(dev)
{
//	cout << "In the ExternalFile ctor" << endl;

	string::size_type pos = f_name.rfind("/");
	if (pos != string::npos)
	{
		path = f_name.substr(0,pos);
		name = f_name.substr(pos+1);
	}
	else
	{
		name = f_name;
	}
		
	lang = get_language(name);
	
	name_lower = name;
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	name_lower.erase(name_lower.find('.'));
}

//+----------------------------------------------------------------------------
//
// method : 		ExternalFile::~ExternalFile()
// 
// description : 	destructor for ExternalFile class
//
//-----------------------------------------------------------------------------

ExternalFile::~ExternalFile()
{
//	cout << "In the ExternalFile dtor" << endl;
}

//+----------------------------------------------------------------------------
//
// method : 		ExternalFile::get_language(string &f_name)
// 
// description : 	Determines the language of the binary file based on its  
//                  extension.
// in : - f_name : The class file name (cpp lib or python module)
//
//-----------------------------------------------------------------------------
Language ExternalFile::get_language(string &f_name)
{
	Language language;
	string::size_type pos;
	
	if ((pos = f_name.find(".py")) != string::npos)
		language = PYTHON;
	else
		language = CPP;
	
	return language;
}

}
