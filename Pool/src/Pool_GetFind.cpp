//+=============================================================================
//
// file :         Pool_GetFind.cpp
//
// description :  C++ source for Pool methods related to getting & finding
//                elements
//
// project :      TANGO Device Server
//
// $Author: tcoutinho $
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//+=============================================================================

#include <dirent.h>

#include <Motor.h>
#include <MotorGroup.h>
#include <PseudoMotor.h>
#include <CTExpChannel.h>
#include <ZeroDExpChannel.h>
#include <PseudoCounter.h>
#include <MeasurementGroup.h>
#include <CommunicationChannel.h>

#include <pool/Ctrl.h>
#include <pool/ComCtrl.h>
#include <pool/MotCtrl.h>
#include <pool/CoTiCtrl.h>
#include <pool/ZeroDCtrl.h>
#include <pool/PseudoCoCtrl.h>

namespace Pool_ns
{

//------------------------------------------------------------------------------
// Pool::get_full_name
// 
string &Pool::get_full_name(string &mot_tg_name)
{
	list<MotorPool>::iterator ite;
	for (ite = mot_list.begin();ite != mot_list.end();++ite)
	{
		if (ite->obj_tango_name == mot_tg_name)
		{
			break;
		}
	}
	return ite->user_full_name;
}

//------------------------------------------------------------------------------
// Pool::get_path_subdirs
//
void Pool::get_path_subdirs(string &dir, vector<string> &paths)
{
	DIR *pdir = opendir(dir.c_str());
	
	if(pdir != NULL)
	{
		// If dir is already in the list of known directories, skip it
		if(find(paths.begin(),paths.end(),dir) != paths.end())
			return;
		paths.push_back(dir);
	
		// this is done to avoid having many file descriptors opened since this
		// is a recursive method
		vector<string> pending_dirs;
		
		struct dirent *pent;
		struct stat buf;
		while((pent = readdir(pdir)))
		{
			if(pent->d_name[0] == '.')
				continue;

			string element = dir + "/";
			element += pent->d_name;	
			
			stat(element.c_str(), &buf);
			
			if(!S_ISDIR(buf.st_mode))
				continue;
			
			pending_dirs.push_back(element);
		}
		closedir(pdir);
		
		vector<string>::iterator pd_it = pending_dirs.begin();
		for(;pd_it != pending_dirs.end(); ++pd_it)
		{
			get_path_subdirs(*pd_it, paths);
		}
	}
}

//------------------------------------------------------------------------------
// Pool::get_pool_path
//
vector<string> &Pool::get_pool_path()
{
	
//
// Immediately returns, if the PoolPath has already been splitted
//

	if (poolPath_splitted == false)
	{
		pool_path.clear();

//
// Throws exception if PoolPath property is not defined
//
		
		if (poolPath.size() == 0 || poolPath[0] == "NotDefined")
		{
			TangoSys_OMemStream o;
			o << "The PoolPath device property is not defined. "
			     "Please, define one.";
		
			Tango::Except::throw_exception(
					(const char *)"Pool_PoolPathNotDefined",o.str(),
					(const char *)"Pool::get_pool_path");
		}
				
		const char delims = ':';
		string::size_type beg_pos, end_pos;
		
		vector<string>::iterator pp_it = poolPath.begin();
		for(;pp_it != poolPath.end(); ++pp_it)
		{
			string &poolPathElem = *pp_it;
			
			// Empty string: skip it
			if (poolPathElem.size() == 0)
				continue;
			
			beg_pos = poolPathElem.find_first_not_of(delims);
			
			// Empty string: skip it
			if (beg_pos == string::npos)
				continue;
			
			while(true)
			{
				end_pos = poolPathElem.find(delims,beg_pos);
	
				string path_elem;
				if(end_pos == string::npos)
				{
					if(beg_pos == string::npos)
						break;
					path_elem = poolPathElem.substr(beg_pos);
				}
	    		else
	    			path_elem = poolPathElem.substr(beg_pos,end_pos - beg_pos);
	    		
	    		if(path_elem[0] != '/') 
				{
					TangoSys_OMemStream o;
					o << path_elem << " is not an absolute path. Please give "
					     "an absolute path in the PoolPath property" << ends;
			
					Tango::Except::throw_exception(
							(const char *)"Pool_DirUnsupportedType",o.str(),
					        (const char *)"Pool::get_pool_path");
				}
				
	    		get_path_subdirs(path_elem, pool_path);
	    		
				if(end_pos == string::npos)
					break;   		
	    		beg_pos = poolPathElem.find_first_not_of(delims, end_pos);
			}
		}
		poolPath_splitted = true;
//		DEBUG_STREAM << "PP IS:"<<endl;
//		for(unsigned long ul = 0; ul < pool_path.size(); ++ul)
//			DEBUG_STREAM << pool_path[ul] << endl;
	}
	
	return pool_path;
}

	
//------------------------------------------------------------------------------
// Pool::get_files_with_extension
//
void Pool::get_files_with_extension(string &dir, const char *f_ext,
                                    vector<string> &lst)
{
	DEBUG_STREAM << "Entering get_files_with_extension for " << f_ext 
	             << " files in directory " << dir << endl;
	errno = 0;
	
	DIR *pdir = opendir(dir.c_str());
	
	string ext(f_ext);
	
	bool any_ext;
	{
		string ext(f_ext);
		any_ext = ext == ".*" || ext == "*";
	}
	
	
	if(pdir != NULL)
	{
		struct dirent *pent;
		while((pent = readdir(pdir)))
		{
			if(pent->d_name[0] == '.')
				continue;
				
			string element = dir + "/";
			element += pent->d_name;	
			
			if(pent->d_type == DT_DIR)
			{
				// ignore directories
			}
			else if(any_ext ||(element.rfind(f_ext, element.size()-1) == element.size() - 3))
			{
				bool new_file = true;
				for(vector<string>::iterator ite = lst.begin(); ite != lst.end(); ite++)
				{
					string curr_name = ite->substr(ite->rfind('/')+1);
					if( curr_name == pent->d_name )
					{
						new_file = false;
						break;
					}
				}
				if(new_file == true)
				{
					lst.push_back(element);
				}	
			}
		}
		
		closedir(pdir);
	}
	
	if(errno) 
	{
		if(EACCES == errno)
			INFO_STREAM << "Permission denied on directory " << dir.c_str();
		else if(EMFILE == errno)
			INFO_STREAM << "Too many file descriptors in use by process";
		else if(ENFILE == errno)
			INFO_STREAM << "Too many files are currently open in the system";
		else if(ENOENT == errno)
			INFO_STREAM << "Directory does not exist: " << dir.c_str();
		else if(ENOTDIR == errno)
			INFO_STREAM << dir.c_str() << "is not a directory";
	}
}

//------------------------------------------------------------------------------
// Pool::find_file_in_pool_path
//
bool Pool::find_file_in_pool_path(string &f_name, string &f_path,Language lang)
{
	vector<string> &path = get_pool_path();
	string name;
	
	if (lang == PYTHON)	
		name = f_name.rfind(".py") == string::npos ? f_name + ".py" : f_name;
	else
		name = f_name.rfind(".la") == string::npos ? f_name + ".la" : f_name;
	
	for(vector<string>::iterator path_ite = path.begin(); path_ite != path.end(); path_ite++)
	{
		vector<string> files;
		
		if (lang == PYTHON)
			get_files_with_extension(*path_ite,".py",files);
		else
			get_files_with_extension(*path_ite,".la",files);
			
		for(vector<string>::iterator ite = files.begin(); ite != files.end(); ite++)
		{	
			string curr_file = (*ite).substr((*ite).rfind("/")+1);
			if(curr_file == name)
			{
				f_path = (*ite);
				return true;	
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::find_file_in_pool_path
//
bool Pool::find_file_in_pool_path(string &f_name, string &f_path)
{
	vector<string> &path = get_pool_path();
	
	for(vector<string>::iterator path_ite = path.begin(); 
	    path_ite != path.end(); path_ite++)
	{
		vector<string> files;
		
		get_files_with_extension(*path_ite,".*",files);
			
		for(vector<string>::iterator ite = files.begin(); 
		    ite != files.end(); ite++)
		{	
			string curr_file = (*ite).substr((*ite).rfind("/")+1);
			if(curr_file == f_name)
			{
				f_path = (*ite);
				return true;	
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_counter_from_name
// 
PseudoCounterPool &Pool::get_pseudo_counter_from_name(string &pc_name)
{
	list<PseudoCounterPool>::iterator pseudo_ite;
	for (pseudo_ite = pseudo_co_list.begin();pseudo_ite != pseudo_co_list.end();++pseudo_ite)
	{
		if (pseudo_ite->name == pc_name)
			break;
	}

	if (pseudo_ite == pseudo_co_list.end())
	{
		TangoSys_OMemStream o;
		o << "No pseudo counter with name " << pc_name << 
		     " found in pseudo counter list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoCounterNotFound",o.str(),
				(const char *)"Pool::get_pseudo_counter_from_id");	
	}	
	return *pseudo_ite;
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_counter_from_id
// 
PseudoCounterPool &Pool::get_pseudo_counter_from_id(long pc_id)
{	
	list<PseudoCounterPool>::iterator ite;
	for (ite = pseudo_co_list.begin();ite != pseudo_co_list.end();++ite)
	{
		if (ite->id == pc_id)
		{
			break;
		}
	}

	if (ite == pseudo_co_list.end())
	{
		TangoSys_OMemStream o;
		o << "Pseudo counter " << pc_id 
		  << " not found in pseudo counter list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoCounterNotFound",o.str(),
				(const char *)"Pool::get_pseudo_counter_from_id");
	}
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_motor_from_name
//
PseudoMotorPool &Pool::get_pseudo_motor_from_name(string &pm_name)
{
	list<PseudoMotorPool>::iterator pseudo_ite;
	for(pseudo_ite = pseudo_mot_list.begin();
	    pseudo_ite != pseudo_mot_list.end();++pseudo_ite)
	{
		if (pseudo_ite->name == pm_name)
			break;
	}

	if (pseudo_ite == pseudo_mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "No pseudo motor with name " << pm_name 
		  << " found in pseudo motor list" << ends;
		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoMotorNotFound",o.str(),
				(const char *)"Pool::get_pseudo_motor_from_id");	
	}	
	return *pseudo_ite;
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_motor_from_id
//
PseudoMotorPool &Pool::get_pseudo_motor_from_id(long pm_id)
{	
	list<PseudoMotorPool>::iterator ite;
	for (ite = pseudo_mot_list.begin();ite != pseudo_mot_list.end();++ite)
	{
		if (ite->id == pm_id)
		{
			break;
		}
	}

	if (ite == pseudo_mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "Pseudo motor " << pm_id << " not found in pseudo motor list" 
		  << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_PseudoMotorNotFound",o.str(),
				(const char *)"Pool::get_pseudo_motor_from_id");
	}
	return *ite;
}


//------------------------------------------------------------------------------
// Pool::get_motor_from_id
//	
MotorPool &Pool::get_motor_from_id(long id)
{
	list<MotorPool>::iterator mot_ite;
	for (mot_ite = mot_list.begin();mot_ite != mot_list.end();++mot_ite)
	{
		if (mot_ite->id == id)
			break;
	}

	if (mot_ite == mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "No motor with ID " << id << " found in motor list" << ends;

		Tango::Except::throw_exception((const char *)"Pool_BadArgument",o.str(),
					  				   (const char *)"Pool::get_motor_from_id");	
	}	
	return *mot_ite;
}

//------------------------------------------------------------------------------
// Pool::get_exp_channel_from_id
//	
PoolElement &Pool::get_exp_channel_from_id(long id)
{
	list<CTExpChannelPool>::iterator exp_ite;
	for (exp_ite = ct_list.begin();exp_ite != ct_list.end();++exp_ite)
	{
		if (exp_ite->id == id)
			return *exp_ite;
	}

	list<ZeroDExpChannelPool>::iterator zero_ite;
	for (zero_ite = zerod_list.begin();zero_ite != zerod_list.end();++zero_ite)
	{
		if (zero_ite->id == id)
			return *zero_ite;
	}
		
	list<PseudoCounterPool>::iterator pc_ite;
	for (pc_ite = pseudo_co_list.begin();pc_ite != pseudo_co_list.end();++pc_ite)
	{
		if (pc_ite->id == id)
			return *pc_ite;
	}

	TangoSys_OMemStream o;
	o << "No experiment channel with ID " << id;
	o << " found in exp channel lists" << ends;

	Tango::Except::throw_exception(
			(const char *)"Pool_BadArgument",o.str(),
			(const char *)"Pool::get_exp_channel_from_id");
}

//------------------------------------------------------------------------------
// Pool::get_exp_channel_from_id
//	
PoolElement &Pool::get_exp_channel_from_id(long id, MntGrpEltType &type)
{
	list<CTExpChannelPool>::iterator exp_ite;
	for (exp_ite = ct_list.begin();exp_ite != ct_list.end();++exp_ite)
	{
		if (exp_ite->id == id)
		{
			type = CT_EXP_CHANNEL;
			return *exp_ite;
		}
	}

	list<ZeroDExpChannelPool>::iterator zero_ite;
	for (zero_ite = zerod_list.begin();zero_ite != zerod_list.end();++zero_ite)
	{
		if (zero_ite->id == id)
		{
			type = ZEROD_EXP_CHANNEL;
			return *zero_ite;
		}
	}
		
	list<PseudoCounterPool>::iterator pc_ite;
	for (pc_ite = pseudo_co_list.begin();pc_ite != pseudo_co_list.end();++pc_ite)
	{
		if (pc_ite->id == id)
		{
			type = PSEUDO_EXP_CHANNEL;
			return *pc_ite;
		}
	}

	TangoSys_OMemStream o;
	o << "No experiment channel with ID " << id;
	o << " found in exp channel lists" << ends;

	Tango::Except::throw_exception(
			(const char *)"Pool_BadArgument",o.str(),
			(const char *)"Pool::get_exp_channel_from_id");
}

//------------------------------------------------------------------------------
// Pool::get_com_channel_from_id
//
CommunicationChannelPool &Pool::get_com_channel_from_id(long id)
{
	list<CommunicationChannelPool>::iterator comch_ite;
	for (comch_ite = com_channel_list.begin();
	     comch_ite != com_channel_list.end();++comch_ite)
	{
		if (comch_ite->id == id)
			break;
	}

	if (comch_ite == com_channel_list.end())
	{

		TangoSys_OMemStream o;
		o << "No communication channel with ID " << id 
		  << " found in communication channel list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_BadArgument",o.str(),
				(const char *)"Pool::get_com_channel_from_id");

	}	
	return *comch_ite;
}

//------------------------------------------------------------------------------
// Pool::get_motor_from_name
//
MotorPool &Pool::get_motor_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	list<MotorPool>::iterator mot_ite;
	for (mot_ite = mot_list.begin();mot_ite != mot_list.end();++mot_ite)
	{
		if (mot_ite->obj_alias_lower == name_lower)
			break;
	}

	if (mot_ite == mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "No motor with name " << name << " found in motor list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorNotFound",o.str(),
				(const char *)"Pool::get_motor_from_name");	
	}	
	return *mot_ite;
}

//------------------------------------------------------------------------------
// Pool::get_ct_from_name
//
CTExpChannelPool &Pool::get_ct_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	list<CTExpChannelPool>::iterator ite;
	for (ite = ct_list.begin();ite != ct_list.end();++ite)
	{
		if (ite->obj_alias_lower == name_lower)
			break;
	}

	if (ite == ct_list.end())
	{
		TangoSys_OMemStream o;
		o << "No experiment channel with name " << name 
		  << " found in Counter Timer list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_CoTiNotFound",o.str(),
				(const char *)"Pool::get_ct_from_name");	
	}	
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_zerod_from_name
//
ZeroDExpChannelPool &Pool::get_zerod_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	list<ZeroDExpChannelPool>::iterator ite;
	for (ite = zerod_list.begin();ite != zerod_list.end();++ite)
	{
		if (ite->obj_alias_lower == name_lower)
			break;
	}

	if (ite == zerod_list.end())
	{
		TangoSys_OMemStream o;
		o << "No experiment channel with name " << name 
		  << " found in Zero D Exp Channel list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_ZeroDNotFound",o.str(),
				(const char *)"Pool::get_zerod_from_name");	
	}	
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_exp_channel_from_name
//
PoolElement &Pool::get_exp_channel_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	list<CTExpChannelPool>::iterator exp_ite;
	for (exp_ite = ct_list.begin(); exp_ite != ct_list.end(); ++exp_ite)
	{
		if (exp_ite->obj_alias_lower == name_lower)
			return *exp_ite;
	}

	list<ZeroDExpChannelPool>::iterator zero_ite;
	for (zero_ite = zerod_list.begin(); zero_ite != zerod_list.end(); ++zero_ite)
	{
		if (zero_ite->obj_alias_lower == name_lower)
			return *zero_ite;
	}
		
	list<PseudoCounterPool>::iterator pc_ite;
	for (pc_ite = pseudo_co_list.begin(); pc_ite != pseudo_co_list.end(); ++pc_ite)
	{
		if (pc_ite->obj_alias_lower == name_lower)
			return *pc_ite;
	}

	TangoSys_OMemStream o;
	o << "No experiment channel with name " << name;
	o << " found in exp channel lists" << ends;

	Tango::Except::throw_exception(
			(const char *)"Pool_BadArgument",o.str(),
			(const char *)"Pool::get_exp_channel_from_name");	
}

//------------------------------------------------------------------------------
// Pool::get_exp_channel_from_name
//
PoolElement &Pool::get_exp_channel_from_name(string &name, MntGrpEltType &type)
{
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	list<CTExpChannelPool>::iterator exp_ite;
	for (exp_ite = ct_list.begin(); exp_ite != ct_list.end(); ++exp_ite)
	{
		if (exp_ite->obj_alias_lower == name_lower)
		{
			type = CT_EXP_CHANNEL;
			return *exp_ite;
		}
	}

	list<ZeroDExpChannelPool>::iterator zero_ite;
	for (zero_ite = zerod_list.begin(); zero_ite != zerod_list.end(); ++zero_ite)
	{
		if (zero_ite->obj_alias_lower == name_lower)
		{
			type = ZEROD_EXP_CHANNEL;
			return *zero_ite;
		}
	}
		
	list<PseudoCounterPool>::iterator pc_ite;
	for (pc_ite = pseudo_co_list.begin(); pc_ite != pseudo_co_list.end(); ++pc_ite)
	{
		if (pc_ite->obj_alias_lower == name_lower)
		{
			type = PSEUDO_EXP_CHANNEL;
			return *pc_ite;
		}
	}

	TangoSys_OMemStream o;
	o << "No experiment channel with name " << name;
	o << " found in exp channel lists" << ends;

	Tango::Except::throw_exception(
			(const char *)"Pool_BadArgument",o.str(),
			(const char *)"Pool::get_exp_channel_from_name");	

}

//------------------------------------------------------------------------------
// Pool::get_com_channel_from_name
//
CommunicationChannelPool &Pool::get_com_channel_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(),name_lower.end(),name_lower.begin(),::tolower);
	
	list<CommunicationChannelPool>::iterator ite;
	for (ite = com_channel_list.begin();ite != com_channel_list.end();++ite)
	{
		if (ite->obj_alias_lower == name_lower)
			break;
	}

	if (ite == com_channel_list.end())
	{
		TangoSys_OMemStream o;
		o << "No communication channel with name " << name 
		  << " found in communication channel list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_ComChNotFound",o.str(),
				(const char *)"Pool::get_com_channel_from_name");	
	}	
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_motor_id_from_name
//
long Pool::get_motor_id_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);

	long argout;	
	list<MotorPool>::iterator mot_ite;
	for (mot_ite = mot_list.begin(); mot_ite != mot_list.end(); ++mot_ite)
	{
		if (mot_ite->obj_alias_lower == name_lower)
		{
			argout = mot_ite->id;
			break;
		}
	}

	if (mot_ite == mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "No motor with name " << name << " found in motor list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorNotFound",o.str(),
				(const char *)"Pool::get_motor_id_from_name");	
	}	
	return argout;
}

//------------------------------------------------------------------------------
// Pool::get_motor_ctrl_idx
//
long Pool::get_motor_ctrl_idx(const char *mot_alias)
{
	long	argout ;

	string wanted_mot_name(mot_alias);
	transform(wanted_mot_name.begin(), wanted_mot_name.end(),
	          wanted_mot_name.begin(), ::tolower);
	
	list<MotorPool>::iterator ite;
	for (ite = mot_list.begin(); ite != mot_list.end(); ++ite)
	{
		string alias_lower(ite->name);
		transform(alias_lower.begin(), alias_lower.end(),
		          alias_lower.begin(), ::tolower);
		
		if (alias_lower == wanted_mot_name)
		{
			argout = ite->obj_idx;
			break;
		}
	}

	if (ite == mot_list.end())
	{
		TangoSys_OMemStream o;
		o << "Motor " << mot_alias << " not found in motor list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorNotFound",o.str(),
				(const char *)"Pool::get_motor_id");
	}
	
	return argout;
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_id
//
ControllerPool &Pool::get_ctrl_from_id(long id)
{
	list<ControllerPool>::iterator ctrl_ite;
	for (ctrl_ite = ctrl_list.begin(); ctrl_ite != ctrl_list.end(); ++ctrl_ite)
	{
		if (ctrl_ite->id == id)
			break;
	}

	if (ctrl_ite == ctrl_list.end())
	{
		TangoSys_OMemStream o;
		o << "No controller with ID " << id 
		  << " found in controller list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_BadArgument",o.str(),
				(const char *)"Pool::get_ctrl_from_id");	
	}	
	return *ctrl_ite;
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_motor_id
//
ControllerPool &Pool::get_ctrl_from_motor_id(long id)
{
	MotorPool &mp = get_motor_from_id(id);
	return get_ctrl_from_motor(mp);
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_exp_channel_id
//
ControllerPool &Pool::get_ctrl_from_exp_channel_id(long id)
{
	PoolElement &ecp = get_exp_channel_from_id(id);
	return get_ctrl_from_pool_elem(ecp);
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_name
//
ControllerPool &Pool::get_ctrl_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	list<ControllerPool>::iterator ctrl_ite;
	for (ctrl_ite = ctrl_list.begin(); ctrl_ite != ctrl_list.end(); ++ctrl_ite)
	{
		if (ctrl_ite->name == name_lower)
			break;
	}

	if (ctrl_ite == ctrl_list.end())
	{
		TangoSys_OMemStream o;
		o << "No controller with name " << name 
		  << " found in controller list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_ControllerNotFound",o.str(),
				(const char *)"Pool::get_ctrl_from_name");	
	}	
	return *ctrl_ite;
}


//------------------------------------------------------------------------------
// Pool::get_ctrl_from_inst_name
//
ControllerPool &Pool::get_ctrl_from_inst_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	list<ControllerPool>::iterator ctrl_ite;
	for (ctrl_ite = ctrl_list.begin(); ctrl_ite != ctrl_list.end(); ++ctrl_ite)
	{
		string tmp_str(ctrl_ite->name);
		tmp_str.erase(0,tmp_str.find('/') + 1);
		if (tmp_str == name_lower)
			break;
	}

	if (ctrl_ite == ctrl_list.end())
	{
		TangoSys_OMemStream o;
		o << "No controller with instance name " << name
		  << " found in controller list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_ControllerNotFound",o.str(),
				(const char *)"Pool::get_ctrl_from_inst_name");	
	}	
	return *ctrl_ite;
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_motor
//
ControllerPool &Pool::get_ctrl_from_motor(MotorPool &mot)
{
	list<ControllerPool>::iterator ite;
	for (ite = ctrl_list.begin(); ite != ctrl_list.end(); ++ite)
	{
		if (ite->id == mot.ctrl_id)
			return *ite;
	}

	if (ite == ctrl_list.end())
	{	
		TangoSys_OMemStream o;
		o << "Can't find controller for motor " 
		  << mot.name << "(" << mot.obj_tango_name << ")" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_CantFindController",o.str(),
			  	(const char *)"Pool::get_ctrl_from_motor");
	}
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_pool_elem
//
ControllerPool &Pool::get_ctrl_from_pool_elem(PoolElement &elem)
{
	list<ControllerPool>::iterator ite;
	for (ite = ctrl_list.begin(); ite != ctrl_list.end(); ++ite)
	{
		if (ite->id == elem.ctrl_id)
			return *ite;
	}

	if (ite == ctrl_list.end())
	{	
		TangoSys_OMemStream o;
		o << "Can't find controller for exp channel " 
		  << elem.name << "(" << elem.obj_tango_name << ")" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_CantFindController",o.str(),
			  	(const char *)"Pool::get_ctrl_from_pool_elem");
	}
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_from_com_channel_id
//
ControllerPool &Pool::get_ctrl_from_com_channel_id(long id)
{
	PoolElement &ecp = get_com_channel_from_id(id);
	return get_ctrl_from_pool_elem(ecp);
}

//------------------------------------------------------------------------------
// Pool::get_ctrl_nb_by_fica_name
//
 long Pool::get_ctrl_nb_by_fica_name(string &fica_name)
 {
 	long ctr = 0;
 	list<ControllerPool>::iterator ctrl_ite;

	for (ctrl_ite = ctrl_list.begin(); ctrl_ite != ctrl_list.end(); ++ctrl_ite)
	{
		if (ctrl_ite->ctrl_fica_name == fica_name)
			ctr++;
	}
	return ctr;
 }
 
//------------------------------------------------------------------------------
// Pool::get_motor_group_ptr_from_id
//
MotorGroup_ns::MotorGroup *Pool::get_motor_group_ptr_from_id(long group_id)
{	
	list<MotorGroupPool>::iterator ite;
	for (ite = mot_group_list.begin(); ite != mot_group_list.end(); ++ite)
	{
		if (ite->id == group_id)
		{
			break;
		}
	}

	if (ite == mot_group_list.end())
	{
		TangoSys_OMemStream o;
		o << "Motor group " << group_id << " not found in motor group list" 
		  << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorGroupNotFound",o.str(),
				(const char *)"Pool::get_motor_group_ptr_from_id");
	}
	return ite->group;
}

//------------------------------------------------------------------------------
// Pool::get_motor_group_from_id
//
MotorGroupPool &Pool::get_motor_group_from_id(long group_id)
{	
	list<MotorGroupPool>::iterator ite;
	for (ite = mot_group_list.begin(); ite != mot_group_list.end(); ++ite)
	{
		if (ite->id == group_id)
		{
			break;
		}
	}

	if (ite == mot_group_list.end())
	{
		TangoSys_OMemStream o;
		o << "Motor group " << group_id << " not found in motor group list" 
		  << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorGroupNotFound",o.str(),
				(const char *)"Pool::get_motor_group_from_id");
	}
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_motor_group_from_name
//
MotorGroupPool &Pool::get_motor_group_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	list<MotorGroupPool>::iterator mot_ite;
	for (mot_ite = mot_group_list.begin();
	     mot_ite != mot_group_list.end();++mot_ite)
	{
		if (mot_ite->obj_alias_lower == name_lower)
			break;
	}

	if (mot_ite == mot_group_list.end())
	{
		TangoSys_OMemStream o;
		o << "No motor group with name " << name << " found in motor list" 
		  << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorGroupNotFound",o.str(),
				(const char *)"Pool::get_motor_group_from_name");	
	}	
	return *mot_ite;
}

//------------------------------------------------------------------------------
// Pool::get_measurement_group_from_name
//
MeasurementGroupPool &Pool::get_measurement_group_from_name(string &name)
{
	string name_lower(name);
	transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
	
	list<MeasurementGroupPool>::iterator mg_ite;
	for (mg_ite = measurement_group_list.begin();
	     mg_ite != measurement_group_list.end(); ++mg_ite)
	{
		if (mg_ite->obj_alias_lower == name_lower)
			break;
	}

	if (mg_ite == measurement_group_list.end())
	{
		TangoSys_OMemStream o;
		o << "No measurement group with name " << name 
		  << " found in measurement group list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MeasurementGroupNotFound",o.str(),
				(const char *)"Pool::get_measurement_group_from_name");	
	}	
	return *mg_ite;
}

//------------------------------------------------------------------------------
// Pool::get_measurement_group_from_id
//
MeasurementGroupPool &Pool::get_measurement_group_from_id(long group_id)
{	
	list<MeasurementGroupPool>::iterator ite;
	for (ite = measurement_group_list.begin();
	     ite != measurement_group_list.end(); ++ite)
	{
		if (ite->id == group_id)
		{
			break;
		}
	}

	if (ite == measurement_group_list.end())
	{
		TangoSys_OMemStream o;
		o << "Measurement group " << group_id 
		  << " not found in moeasurement group list" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MeasurementGroupNotFound",o.str(),
				(const char *)"Pool::get_measurement_group_from_id");
	}
	return *ite;
}

//------------------------------------------------------------------------------
// Pool::get_ghost_motor_group_ptr
//
MotorGroup_ns::MotorGroup *Pool::get_ghost_motor_group_ptr()
{	
	vector<Tango::DeviceImpl *> &dev_list = 
		Tango::Util::instance()->get_device_list_by_class("MotorGroup");
	
	unsigned long loop;
	MotorGroup_ns::MotorGroup *grp_ptr;

	for (loop = 0; loop < dev_list.size(); loop++)
	{
		string &dev_name = dev_list[loop]->get_name();
		string::size_type pos = dev_name.find("ghost");
		if (pos != string::npos)
		{
			grp_ptr = (MotorGroup_ns::MotorGroup *)(dev_list[loop]);
			break;
		}
	}
	
	if (loop == dev_list.size())
	{
		TangoSys_OMemStream o;
		o << "No ghost group found in motor group devices" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MotorGroupNotFound",o.str(),
				(const char *)"Pool::get_ghost_motor_group_ptr");	
	}
	return grp_ptr;
}

//------------------------------------------------------------------------------
// Pool::get_ghost_measurement_group_ptr
//
MeasurementGroup_ns::MeasurementGroup *Pool::get_ghost_measurement_group_ptr()
{
	vector<Tango::DeviceImpl *> &dev_list = 
		Tango::Util::instance()->get_device_list_by_class("MeasurementGroup");

	unsigned long loop;
	MeasurementGroup_ns::MeasurementGroup *grp_ptr;

	for (loop = 0;loop < dev_list.size();loop++)
	{
		string &dev_name = dev_list[loop]->get_name();
		string::size_type pos = dev_name.find("ghost");
		if (pos != string::npos)
		{
			grp_ptr = (MeasurementGroup_ns::MeasurementGroup *)(dev_list[loop]);
			break;
		}
	}
	
	if (loop == dev_list.size())
	{
		TangoSys_OMemStream o;
		o << "No ghost group found in measurement group devices" << ends;

		Tango::Except::throw_exception(
				(const char *)"Pool_MeasurementGroupNotFound",o.str(),
				(const char *)"Pool::get_ghost_measurement_group_ptr");	
	}
	return grp_ptr;
}

//------------------------------------------------------------------------------
// Pool::get_motor_groups_containing_elt
//
bool Pool::get_motor_groups_containing_elt(string &elt_name, 
										   vector<string> &mgs)
{
	bool any = false;
	list<MotorGroupPool>::iterator grp_ite;
	
	for (grp_ite = mot_group_list.begin();
		 grp_ite != mot_group_list.end();++grp_ite)
	{
		if (grp_ite->is_member(elt_name) == true)
		{
			mgs.push_back(grp_ite->name);
			any = true;
		}
	}
	return any;
}

//------------------------------------------------------------------------------
// Pool::get_motor_groups_containing_elt
//
bool Pool::get_motor_groups_containing_elt(string &elt_name, 
										   vector<MotorGroupPool*> &mgs)
{
	bool any = false;
	list<MotorGroupPool>::iterator grp_ite;
	
	for (grp_ite = mot_group_list.begin();
		 grp_ite != mot_group_list.end();++grp_ite)
	{
//
// If the element 
//		
		if (grp_ite->is_member(elt_name) == true)
		{
			mgs.push_back(&(*grp_ite));
			any = true;
		}
	}
	return any;
}

//------------------------------------------------------------------------------
// Pool::get_measurement_groups_containing_channel
//
bool Pool::get_measurement_groups_containing_channel(string &elt_name, 
													 vector<string> &mgs)
{
	bool any = false;
	list<MeasurementGroupPool>::iterator grp_ite;
	
	for (grp_ite = measurement_group_list.begin();
	     grp_ite != measurement_group_list.end();++grp_ite)
	{
		if (grp_ite->is_member(elt_name) == true)
		{
			mgs.push_back(grp_ite->name);
			any = true;
		}
	}
	return any;	
}

//------------------------------------------------------------------------------
// Pool::get_pseudo_motors_that_use_mg
//
bool Pool::get_pseudo_motors_that_use_mg(long mg_id, vector<string> &mgs)
{
	bool any = false;
	list<PseudoMotorPool>::iterator pm_ite = pseudo_mot_list.begin();
	for (;pm_ite != pseudo_mot_list.end();++pm_ite)
	{
		if(pm_ite->mot_grp != NULL && pm_ite->mot_grp->id == mg_id)
		{
  			mgs.push_back(pm_ite->name);
  			any = true;
		}
	}
	return any;
}

//------------------------------------------------------------------------------
// Pool::get_motor_group_in_hierarchy_containing_elt
//
bool Pool::get_motor_groups_in_hierarchy_containing_elt(MotorGroupPool &mg, 
														string &elt_name, 
														vector<string> &mgs)
{
	if(mg.is_member(elt_name))
	{
		mgs.push_back(mg.name);
		return true;
	}
	
	vector<MotorGroupPool*> mg_mgs;
	bool res = get_motor_groups_containing_elt(mg.name,mg_mgs);
	
	if(res == false)
		return false;
	
	res = false;
	vector<MotorGroupPool*>::iterator ite = mg_mgs.begin();
	for(;ite != mg_mgs.end(); ite++)
	{
		res |= get_motor_groups_in_hierarchy_containing_elt(*(*ite), elt_name, mgs);
	}
	return res;
}
}
