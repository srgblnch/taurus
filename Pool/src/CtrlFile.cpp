#include <CtrlFile.h>
#include <PoolClass.h>

namespace Pool_ns
{

//-----------------------------------------------------------------------------
//
//					The CtrlFile class
//
//-----------------------------------------------------------------------------

//+----------------------------------------------------------------------------
//
// method : 		CtrlFile::CtrlFile()
// 
// description : 	constructor for CtrlFile class
//
// in : - f_name : The class file name (cpp lib or python module)
//		- class_name : The class name (case dependant)
//		- pool_class : Pointer to the PoolClass object
//
//-----------------------------------------------------------------------------

CtrlFile::CtrlFile(string &f_name,PoolClass *pool_class,Pool *dev)
:ExternalFile(f_name,pool_class,dev)
{
//	cout << "In the CtrlFile ctor" << endl;
}

//+----------------------------------------------------------------------------
//
// method : 		CtrlFile::~CtrlFile()
// 
// description : 	destructor for CtrlFile class
//
//-----------------------------------------------------------------------------

CtrlFile::~CtrlFile()
{
//	cout << "In the CtrlFile dtor" << endl;
}


//+----------------------------------------------------------------------------
//
// method : 		CtrlFile::get_prop_info()
// 
// description : 	Retrieves the information about properties for a given class.
//                  
// arg(s) : - class_name [in]: The class for which to retrieve the information
//          - info [out]: The properties information related to the given class
//
// Returns the properties class information.
//
//-----------------------------------------------------------------------------

void CtrlFile::get_prop_info(string &class_name, vector<string> &info)
{
	vector<PropertyData*> prop_data;
	get_prop_info(class_name,prop_data);
	
//
// First place the number of properties as a first element in the string
//	

	stringstream prop_nb;
	prop_nb << prop_data.size();
	info.push_back(prop_nb.str());
	
//
// Split each property into four strings: name, type, description and default value
//

	vector<PropertyData*>::iterator ite;
	for(ite = prop_data.begin(); ite != prop_data.end(); ite++)
	{
		(*ite)->append_to_property_vec(info);
		delete (*ite);
	}
	prop_data.clear();
}

//+----------------------------------------------------------------------------
//
// method : 	CtrlFile::vecinfo_to_chararray()
// 
// description : Generic method to translate from a vector<string> containning
//               information about a class. Override when necessary.  
//                  
// arg(s) : - info [in]: Vector containning the information
//          - info [out]: char array containning the new information
//
//-----------------------------------------------------------------------------

void CtrlFile::vecinfo_to_chararray(vector<string> &info,
									   Tango::DevVarCharArray *info_ex)
{
	unsigned long image_size;
	unsigned long logo_size;
	unsigned long icon_size;
	
	unsigned long image_name_idx = info.size() - 3;
	unsigned long logo_name_idx = info.size() - 2;
	unsigned long icon_name_idx = info.size() - 1;
	
	string &image_filename = info[image_name_idx];
	string &logo_filename = info[logo_name_idx];
	string &icon_filename = info[icon_name_idx];
	
	ifstream image_infile;
	ifstream logo_infile;
	ifstream icon_infile;
	
	bool image_file_opened = false;
	bool logo_file_opened = false;
	bool icon_file_opened = false;

	if(image_filename.empty())
	{
		image_size = 0;
	}
	else 
	{
		if(image_filename[0] != '/')
			image_filename = get_path() + '/' + image_filename;
		
		image_infile.open(image_filename.c_str(), ios::in | ios::binary);
		image_file_opened = true;	
		
		// Check that the file exists
		if(!image_infile.good())
		{
			image_filename += " (invalid)";
			image_size = 0;
			DEBUG_STREAM << "Invalid image file name (" << image_filename 
						 << ") given for " << get_name() <<endl;
		}
		else 
		{
			// Determine the size of the file 
			image_infile.seekg (0, ios::end);
			image_size = image_infile.tellg();
			image_infile.seekg (0, ios::beg);
		}
	}
	
	if(logo_filename.empty())
	{
		logo_size = 0;
	}
	else 
	{
		if(logo_filename[0] != '/')
			logo_filename = get_path() + '/' + logo_filename;
		
		logo_infile.open(logo_filename.c_str(), ios::in | ios::binary);
		logo_file_opened = true;
		
		// Check that the file exists
		if(!logo_infile.good())
		{
			logo_filename += " (invalid)";
			logo_size = 0;
			DEBUG_STREAM << "Invalid logo file name (" << logo_filename 
						 << ") given for " << get_name() <<endl;
		}
		else
		{
			// Determine the size of the file 
			logo_infile.seekg (0, ios::end);
			logo_size = logo_infile.tellg();
			logo_infile.seekg (0, ios::beg);
		}
	}
	
	if(icon_filename.empty())
	{
		icon_size = 0;
	}
	else 
	{
		if(icon_filename[0] != '/')
			icon_filename = get_path() + '/' + icon_filename;
		
		icon_infile.open(icon_filename.c_str(), ios::in | ios::binary);
		icon_file_opened = true;
		
		// Check that the file exists
		if(!icon_infile.good())
		{
			icon_filename += " (invalid)";
			icon_size = 0;
			DEBUG_STREAM << "Invalid icon file name (" << icon_filename 
						 << ") given for " << get_name() <<endl;
		}
		else
		{
			// Determine the size of the file 
			icon_infile.seekg (0, ios::end);
			icon_size = icon_infile.tellg();
			icon_infile.seekg (0, ios::beg);
		}
	}	

	stringstream image_size_stream;
	image_size_stream << image_size;
	string image_size_str = image_size_stream.str();
	
	stringstream logo_size_stream;
	logo_size_stream << logo_size;
	string logo_size_str = logo_size_stream.str();

	stringstream icon_size_stream;
	icon_size_stream << icon_size;
	string icon_size_str = icon_size_stream.str();

//
// Now we calculate the size of everything
//
	// Each string will be added a '\0' so we start by adding the number of 
	// strings to the count
	unsigned long global_length = info.size();
	
	global_length += image_size_str.size() + 1;
	global_length += logo_size_str.size() + 1;
	global_length += icon_size_str.size() + 1;
	
	// add the size of each string
	for(unsigned long l = 0; l < info.size(); l++)
		global_length += info[l].size();
	
	// add the size of the image logo and icon
	global_length += image_size + logo_size + icon_size;
	
	info_ex->length(global_length);
	
	unsigned long n = 0;
	for(unsigned long l = 0; l < info.size()-3; l++)
	{
		unsigned long s = info[l].size();
		char *dest = (char*)(&(*info_ex)[n]); 
		strncpy(dest,info[l].c_str(),s);
		n += s;
		(*info_ex)[n++] = '\0';
	}
	
	// add image name and size
	unsigned long s = image_filename.size();
	char *dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,image_filename.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';

	s = image_size_str.size();
	dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,image_size_str.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';
	
	// add logo name and size
	s = logo_filename.size();
	dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,logo_filename.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';

	s = logo_size_str.size();
	dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,logo_size_str.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';

	// add icon name and size
	s = icon_filename.size();
	dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,icon_filename.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';

	s = icon_size_str.size();
	dest = (char*)(&(*info_ex)[n]); 
	strncpy(dest,icon_size_str.c_str(),s);
	n += s;
	(*info_ex)[n++] = '\0';
	
//
// Finally read data from file to buffer
//
	if(image_size > 0 && image_file_opened)
	{
		char *dest = (char*)(&(*info_ex)[n]);
		image_infile.read(dest,image_size);
		n += image_size;
	}

	if(logo_size > 0 && logo_file_opened)
	{
		char *dest = (char*)(&(*info_ex)[n]);
		logo_infile.read(dest,logo_size);
		n += logo_size;
	}
	
	if(icon_size > 0 && icon_file_opened)
	{
		char *dest = (char*)(&(*info_ex)[n]);
		icon_infile.read(dest,icon_size);
		n += icon_size;
	}	
	
	if(image_file_opened)
		image_infile.close();
	if(logo_file_opened)
		logo_infile.close();
	if(icon_file_opened)
		icon_infile.close();
	
	assert(n == global_length);
}

} // End of Pool_ns namespacce
