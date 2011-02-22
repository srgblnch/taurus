//+=============================================================================
//
// file :         Pool_AddRemove.cpp
//
// description :  C++ source for Pool methods related to adding / removing
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

#include <PoolClass.h>
#include <PoolUtil.h>
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

struct PoolElementComp: public binary_function<PoolElement,PoolElement,bool>
{
	bool operator() (PoolElement &mp1,PoolElement &mp2)
	{
		return mp1.id < mp2.id;
	}
};

struct PseudoMotorComp: public binary_function<PseudoMotorPool,PseudoMotorPool,bool>
{
	bool operator() (PseudoMotorPool &pmp1,PseudoMotorPool &pmp2)
	{
		return pmp1.id < pmp2.id;
	}
};

//------------------------------------------------------------------------------
// Pool::add_ctrl
//
void Pool::add_ctrl(const Tango::DevVarStringArray *argin,
                    bool internal /*= false*/)
{ 
	Tango::DevLong argout;

//
// Check if we are called from a client or from init_device
//
  	long ctrl_id_db = internal ? atol((*argin)[4]) : 0;

  	if (argin->length() <= 3)
  	{
  		Tango::Except::throw_exception(
  			(const char *)"Pool_WrongArgumentNumber",
  			(const char *)"Wrong number of argument for command "
  						  "CreateController. Needs at least 4 strings",
  		    (const char *)"Pool::create_controller");
  	}		

//
// Create strings to manage name
//
  
  	string f_name;
  	string f_name_we((*argin)[1].in());
  	string::size_type pos;
  
  	if ((pos = f_name_we.find('/')) != string::npos)
  	{
  		TangoSys_OMemStream o;
  		o << f_name_we << " is not simply a file name. Just give the file name";
  		o << ", not the path";
  		o << "\nPath will be taken from env. variables LD_LIBRARY_PATH or ";
  		o << "device property PoolPath" << ends;
  
  		Tango::Except::throw_exception(
  				(const char *)"Pool_FileUnsupportedType",o.str(),
  		        (const char *)"Pool::create_controller");
  	}
  		
  	if ((pos = f_name_we.find('.')) == string::npos)
  	{
  		TangoSys_OMemStream o;
  		o << f_name_we << " does not have file type extension. Please define ";
  		o << "one (.py or .la)" << ends;
  
  		Tango::Except::throw_exception(
  				(const char *)"Pool_FileUnsupportedType",o.str(),
  		        (const char *)"Pool::create_controller");
  	}
  	else
  	{
  		if ((pos = f_name_we.find(".la")) != string::npos)
  			f_name = f_name_we.substr(0,pos);
  		else if ((pos = f_name_we.find(".py")) != string::npos)
  			f_name = f_name_we.substr(0,pos);
  		else
  		{
  			TangoSys_OMemStream o;
  			o << "File " << f_name_we << " is from one unsupported type"
  			  << ends;
  
  			Tango::Except::throw_exception(
  					(const char *)"Pool_FileUnsupportedType",o.str(),
  			        (const char *)"Pool::create_controller");
  		}
  	}
  		
  	string f_name_lower(f_name);
  	transform(f_name_lower.begin(),f_name_lower.end(),f_name_lower.begin(),
  			  ::tolower);
  
  	string ctrl_class_name((*argin)[2].in());	
  	string ctrl_class_name_lower = ctrl_class_name;
  	transform(ctrl_class_name_lower.begin(),
  			  ctrl_class_name_lower.end(),
  			  ctrl_class_name_lower.begin(),
  			  ::tolower);
  	
  	string inst_name((*argin)[3].in());	
  	string inst_name_lower = inst_name;
  	transform(inst_name_lower.begin(),
  			  inst_name_lower.end(),
  			  inst_name_lower.begin(),
  			  ::tolower);
  			  
  	string ctrl_name = f_name_lower + '.' + ctrl_class_name_lower + '/' + 
  					   inst_name_lower;
 
//
// Check controller type
//

  	PoolClass *cl_ptr = static_cast<PoolClass *>(this->get_device_class());
  	
  	string ob_type((*argin)[0].in());
  	transform(ob_type.begin(),ob_type.end(),ob_type.begin(),::tolower);
  	ob_type[0] = ::toupper(ob_type[0]);
  	CtrlType type = cl_ptr->str_2_CtrlType(ob_type);
  	
  	if (type == COTI_CTRL)
  	{
  		ob_type[7] = ::toupper(ob_type[7]);
  	}
  	else if (type == ZEROD_CTRL)
  	{
  		ob_type[4] = ::toupper(ob_type[4]);
  		ob_type[5] = ::toupper(ob_type[5]);
  		ob_type[8] = ::toupper(ob_type[8]);
	}
  	else if (type == PSEUDO_MOTOR_CTRL || type == PSEUDO_COUNTER_CTRL)
  	{
  		ob_type[6] = ::toupper(ob_type[6]);
  	}

//
// Check that we do not have already the max number of controllers
//
  		
  	if (ctrl_list.size() == MAX_CTRL)
  	{
  		Tango::Except::throw_exception(
  				(const char *)"Pool_TooManyController",
  				(const char *)"Too many controller in your pool",
  				(const char *)"Pool::create_controller");
  	}
  	
//
// Check that the instance name is not already used
//

	if (ctrl_list.empty() != true)
	{
  		list<ControllerPool>::iterator ctrl_ite;		
  		for (ctrl_ite = ctrl_list.begin();ctrl_ite != ctrl_list.end();++ctrl_ite)
  		{
  			string ctrl_instance_name = ctrl_ite->name;
  			ctrl_instance_name.erase(0,ctrl_instance_name.find('/') + 1);
  			if (ctrl_instance_name == inst_name_lower)
  			{
  				TangoSys_OMemStream o;
  				o << "Instance name " << inst_name << " already used for ";
  				o << "controller " << ctrl_class_name << "/" << inst_name;
  				o << ends;
  
  				Tango::Except::throw_exception(
  						(const char *)"Pool_ControllerAlreadyCreated",o.str(),
  				        (const char *)"Pool::create_controller");
  			}
  		}
	}
  
//
// Check if the same controller is not already defined in the control system
//
  
  	bool throw_ex = false;
  	TangoSys_OMemStream ex_mess;
  	
  	if (internal == false)
  	{
  		try
  		{
  			Tango::Util *tg = Tango::Util::instance();
  			Tango::Database *db = tg->get_database();
 
 //
 // Ask Db for the list of Pool device server defined
 //
  			
  			Tango::DeviceData din,dout;
  			Tango::DevString cl_name = "Pool/*";
  			din << cl_name;
  			dout = db->command_inout("DbGetServerList",din);
  			
  			vector<string> pool_serv_list;
  			dout >> pool_serv_list;
  			
  			vector<string>::iterator str_ite;
  			vector<string> pool_list;

//
// Ask db for the pool device of each pool DS
//
  			
  			string cla_name("Pool");
  			for (str_ite = pool_serv_list.begin();str_ite != pool_serv_list.end();++str_ite)
  			{
  				Tango::DbDatum db_res = db->get_device_name(*str_ite,cla_name);
  				vector<string> tmp_vect;
  				db_res >> tmp_vect;
  				vector<string>::iterator ite_dev_name;
  				for (ite_dev_name = tmp_vect.begin();
  					 ite_dev_name != tmp_vect.end();++ite_dev_name)
  					pool_list.push_back(*ite_dev_name);
  			}
  
//
// All device names in lower case
//
  		
  			for (str_ite = pool_list.begin();str_ite != pool_list.end();++str_ite)
  				transform((*str_ite).begin(),(*str_ite).end(),(*str_ite).begin(),::tolower);
  			
//
// Remove ourself from list
//
  
  			string & low_dev_name = this->get_name_lower();
  			pool_list.erase(remove(pool_list.begin(),pool_list.end(),
  							low_dev_name),pool_list.end());
  
//
// Search for the new controller in pool devices controller list
//
  
  			if (pool_list.size() != 0)
  			{
  				for (unsigned long loop = 0;loop < pool_list.size();loop++)
  				{
  					Tango::DbData db_data;
  					db_data.push_back(Tango::DbDatum(CTRL_PROP));
  					db->get_device_property(pool_list[loop],db_data);
  					vector<string> ct_list;
  					if (db_data[0].is_empty() == false)
  					{
  						db_data[0] >> ct_list;
  						long nb_ctrl = ct_list.size() / PROP_BY_CTRL;
  			 
  						for (long l = 0;l < nb_ctrl;l++)
  						{
  							if (ct_list[(l * PROP_BY_CTRL) + 3] == inst_name)
  							{
  								throw_ex = true;
  								ex_mess << "Controller " << (*argin)[2].in() 
  										<< "/";
  								ex_mess << (*argin)[3].in() << " already "
  										   "created in your control system.";
  								ex_mess << "\n. It belongs to pool device " 
  										<< pool_list[loop] << "."  << ends;
  								
  								break;
  							}
  						}
  						
  						if (throw_ex == true)
  							break;
  					}
  				}
  			}
  		}
  		catch (Tango::DevFailed &e)
  		{
  			Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateController",
  					(const char *)"Can't create controller!!!",
  					(const char *)"Pool::create_controller");
  		}
  	}
  	
  	if (throw_ex == true)
  	{
  		Tango::Except::throw_exception(
  				(const char *)"Pool_ControllerAlreadyCreated",ex_mess.str(),
  				(const char *)"Pool::create_controller");
  	}
  
//
// Starting from this point, we might change the device state
//
  
  	PoolStateEvent pse(this);
  	
//
// Check if the controller fica is not already defined
//
  
  	string ctrl_type = f_name_lower + '/' + ctrl_class_name_lower;
  	bool ctrl_fica_constructed = false;
  	bool new_ctrl_fica = false;
  	Tango::DevFailed save_exception;
  	Controller *ct;
  		
  	vector<CtrlFiCa *>::iterator ctrlFiCa_ite;
  	try
  	{
  		ctrlFiCa_ite = cl_ptr->get_ctrl_fica_by_name(ctrl_type,ob_type);
  		ctrl_fica_constructed = true;
  	}
  	catch (Tango::DevFailed &e)
  	{
  		new_ctrl_fica = true;
  	}

//
// Check that the file is not already known for another
// type of controller
//

	if (new_ctrl_fica == false)
	{
		CtrlType ty;
		try
		{
			vector<CtrlFile *>::iterator CtrlFile_it = 
				cl_ptr->get_ctrl_file_by_name(f_name_we);
			ty = (*CtrlFile_it)->get_ctrl_type();
		}
		catch (Tango::DevFailed &e) {}
	
		if (ty != type)
		{
			TangoSys_OMemStream o;
	  		o << "file << " << f_name_we << " already used as ";
	  		o << cl_ptr->CtrlType_2_str(ty) << " controller" << ends;
	  
	  		Tango::Except::throw_exception(
	  				(const char *)"Pool_ControllerAlreadyCreated",o.str(),
	  				(const char *)"Pool::create_controller");
		}
	}
	
//
// Create a new controller fica if it is not defined
// Get its properties and check that they have a defined value
//
	vector<pair<string,string> > value_pairs;
	
  	Language lang;
  	vector<Controller::Properties> *prop = NULL; 	
  	
  	if (new_ctrl_fica == true)
  	{
  		try
  		{
  			ctrlFiCa_ite = cl_ptr->add_ctrl_fica(ctrl_type,f_name_we,
  												 ctrl_class_name,
  										         ob_type,this);
  			if(!internal)
  			{
  				int start = 4;
  				if(type == PSEUDO_MOTOR_CTRL)
  				{
  					PseudoMotCtrlFiCa* pm_fica = 
  						static_cast<PseudoMotCtrlFiCa*>(*ctrlFiCa_ite);
  					int m_nb = pm_fica->get_motor_role_nb();
  					int pm_nb = pm_fica->get_pseudo_motor_role_nb();
  					 
  					start += m_nb + pm_nb;
					
					int len = argin->length();
					int min = 4 + m_nb + pm_nb;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Pseudo motor controller '";
						o << ctrl_class_name << "' requires:\n";
						o << "- controller instance name\n";
						o << "- " << m_nb << " motor name(s) and\n";
						o << "- " << pm_nb << " pseudo motor name(s)";

						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}
  				else if(type == PSEUDO_COUNTER_CTRL)
  				{
  					PseudoCoCtrlFiCa* co_fica = 
  						static_cast<PseudoCoCtrlFiCa*>(*ctrlFiCa_ite);
  					int co_nb = co_fica->get_counter_role_nb(); 
  					start += co_nb + 1;  // +1 for the pseudo counter instance name
					
					int len = argin->length();
					int min = 4 + co_nb + 1;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Pseudo counter controller '";
						o << ctrl_class_name << "' requires:\n";
						o << "- controller instance name\n";
						o << "- " << co_nb << " counter name(s) and\n";
						o << "- pseudo counter name";
				
						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}
  				else if(type == CONSTRAINT_CTRL)
  				{
  					ConstraintFiCa* co_fica = 
  						static_cast<ConstraintFiCa*>(*ctrlFiCa_ite);
  					int co_nb = co_fica->get_role_nb(); 
  					start += co_nb;
					
					int len = argin->length();
					int min = 4 + co_nb;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Constraint '";
						o << ctrl_class_name << "' requires:\n";
						o << "- instance name and\n";
						o << "- " << co_nb << " role name(s)";
				
						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}
  				
  				build_property_params(argin, value_pairs, start);
  			}
			build_property_data(inst_name,ctrl_class_name, value_pairs,
								(*ctrlFiCa_ite)->get_ctrl_prop_list());
			check_property_data((*ctrlFiCa_ite)->get_ctrl_prop_list());
			
  			ctrl_fica_constructed = true;
  		}
  		catch (Tango::DevFailed &e)
  		{
  			if (internal == true)
  			{
  				ct = NULL;
  				save_exception = e;
  				
  				string err_reason = e.errors[0].reason.in();
  				if (err_reason.find("Python") != string::npos)
  					lang = PYTHON;
  				else
  					lang = CPP;
  			}
  			else
  			{
  				TangoSys_OMemStream o;
  				o << "Impossible to create controller " << ctrl_class_name;
  				o << " from file " << f_name_we << ends;
  	
  				Tango::Except::re_throw_exception(e,
  						(const char *)"Pool_ControllerNotFound",o.str(),
  			        	(const char *)"Pool::create_controller");
  			}
  		}
  	}
  	else
  	{
//
// Even if the FiCa is already there, we need to check the property of this 
// instance
//
		try
		{
  			if(!internal)
  			{
  				int start = 4;
  				if(type == PSEUDO_MOTOR_CTRL)
  				{
  					PseudoMotCtrlFiCa* pm_fica = 
  						static_cast<PseudoMotCtrlFiCa*>(*ctrlFiCa_ite);
  					int m_nb = pm_fica->get_motor_role_nb();
  					int pm_nb = pm_fica->get_pseudo_motor_role_nb();
  					start += m_nb + pm_nb;
					
					int len = argin->length();
					int min = 4 + m_nb + pm_nb;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Pseudo motor controller '";
						o << ctrl_class_name << "' requires:\n";
						o << "- controller instance name\n";
						o << "- " << m_nb << " motor name(s) and\n";
						o << "- " << pm_nb << " pseudo motor name(s)";
				
						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}
  				else if(type == PSEUDO_COUNTER_CTRL)
  				{
  					PseudoCoCtrlFiCa* co_fica = 
  						static_cast<PseudoCoCtrlFiCa*>(*ctrlFiCa_ite);
  					int co_nb = co_fica->get_counter_role_nb(); 
  					start += co_nb + 1;  // +1 for the pseudo counter instance name
					
					int len = argin->length();
					int min = 4 + co_nb + 1;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Pseudo counter controller '";
						o << ctrl_class_name << "' requires:\n";
						o << "- controller instance name\n";
						o << "- " << co_nb << " counter name(s) and\n";
						o << "- pseudo counter name";
				
						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}
  				else if(type == CONSTRAINT_CTRL)
  				{
  					ConstraintFiCa* co_fica = 
  						static_cast<ConstraintFiCa*>(*ctrlFiCa_ite);
  					int co_nb = co_fica->get_role_nb(); 
  					start += co_nb;
					
					int len = argin->length();
					int min = 4 + co_nb;
					if( (len < min) || ( ((len-min) % 2) != 0 ) )
					{
						TangoSys_OMemStream o;
						o << "Insuficient arguments: Constraint '";
						o << ctrl_class_name << "' requires:\n";
						o << "- instance name and\n";
						o << "- " << co_nb << " role name(s)";
				
						Tango::Except::throw_exception(
								(const char *)"Pool_InsuficientArgs",o.str(),
								(const char *)"Pool::create_controller");
					}
  				}

  				build_property_params(argin, value_pairs, start);
  			}
			
			build_property_data(inst_name,ctrl_class_name,value_pairs,
								(*ctrlFiCa_ite)->get_ctrl_prop_list());
			check_property_data((*ctrlFiCa_ite)->get_ctrl_prop_list());
		}
		catch (Tango::DevFailed &e)
		{
  			if (internal == true)
  			{
  				ct = NULL;
  				ctrl_fica_constructed = false;
  				save_exception = e;
  				
  				string err_reason = e.errors[0].reason.in();
  				if (err_reason.find("Python") != string::npos)
  					lang = PYTHON;
  				else
  					lang = CPP;
  			}
  			else
  			{
  				TangoSys_OMemStream o;
  				o << "Impossible to create controller " << ctrl_class_name;
  				o << " from file " << f_name_we << ends;
  	
  				Tango::Except::re_throw_exception(e,
  						(const char *)"Pool_ControllerNotFound",o.str(),
  			        	(const char *)"Pool::create_controller");
  			}
		}
  	}
	  	
  	if (ctrl_fica_constructed == true)
  	{		
  		lang = (*ctrlFiCa_ite)->get_language();
  		if (lang == CPP)
  		{		
//
// Retrieve the controller object creator C function
//
  			lt_ptr sym;
  		
  			string sym_name("_create_");
  			sym_name = sym_name + ctrl_class_name;
  			
  			DEBUG_STREAM << "Symbol name = " << sym_name << endl;
  				
  			sym = lt_dlsym((*ctrlFiCa_ite)->get_lib_ptr(),sym_name.c_str());
  			if (sym == NULL)
  			{
  				TangoSys_OMemStream o;
  				o << "Controller library " << (*argin)[1].in(); 
  				o << " does not have the C creator function ";
  				o << "(_create_<Controller name>)" << ends;
  		
  				Tango::Except::throw_exception(
  						(const char *)"Pool_CCreatorFunctionNotFound",o.str(),
  						(const char *)"Pool::create_controller");
  			}
  			
  			DEBUG_STREAM << "lt_dlsym is a success" << endl;
  	
//
// Create the controller
//
 	
  			Ctrl_creator_ptr ct_ptr = (Ctrl_creator_ptr)sym;
  			try
  			{
  				AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  				ct = NULL;
  				prop = properties_2_cpp_vect((*ctrlFiCa_ite)->get_ctrl_prop_list());
  				ct = (*ct_ptr)(inst_name_lower.c_str(),*prop);
  			}
  			catch (Tango::DevFailed &e)
  			{
  				if (prop != NULL)
  					delete prop;
  					
  				if (internal == true)
  				{
  					if (ct != NULL)
  					{
  						AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  						delete ct;
  					}
  					ct = NULL;
  					save_exception = e;
  				}
  				else
  				{
  					Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateController",
  					(const char *)"Can't create controller class instance!!!",
  					(const char *)"Pool::create_controller");
  				}
  			}
  		}
  		else
  		{
  			
//
// Retrieve the controller object creator C function
//
  		
  			lt_ptr sym;
  		
  			string sym_name("_create_Py");
  			sym_name = sym_name + ob_type;
  			
  			if(type != CONSTRAINT_CTRL)
  				sym_name += "Controller";
  			
  			DEBUG_STREAM << "Symbol name = " << sym_name << endl;

			lt_dlhandle tmp_lib_ptr = (*ctrlFiCa_ite)->get_py_inter_lib_ptr(); 
			sym = lt_dlsym(tmp_lib_ptr,sym_name.c_str());				
  			if (sym == NULL)
  			{
  				TangoSys_OMemStream o;
  				o << "Controller library " << (*argin)[1].in(); 
  				o << " does not have the C creator function ";
  				o << "(_create_<Controller name>)" << ends;
  		
  				Tango::Except::throw_exception(
  						(const char *)"Pool_CCreatorFunctionNotFound",o.str(),
  						(const char *)"Pool::create_controller");
  			}
  			
  			DEBUG_STREAM << "lt_dlsym is a success" << endl;
  			
//
// Create the Python controller object
//
  		
  			PyCtrl_creator_ptr ct_ptr = (PyCtrl_creator_ptr)sym;
  			try
  			{
  				AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  				PyObject *prop_dict = 
  					properties_2_py_dict((*ctrlFiCa_ite)->get_ctrl_prop_list());
  				ct = (*ct_ptr)(inst_name_lower.c_str(),
  							   ctrl_class_name.c_str(),
  							   (*ctrlFiCa_ite)->get_py_module(),prop_dict);
  			}
  			catch (Tango::DevFailed &e)
  			{
  				if (internal == true)
  				{
  					ct = NULL;
  					save_exception = e;
  				}
  				else
  				{
  					Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantCreateController",
  					(const char *)"Can't create controller class instance!!!",
  					(const char *)"Pool::create_controller");
  				}
  			}
  		}
  	}
  	else
  	{
  		
//
// The FiCa is not constructed. Deduce controller language
// from file name extension
//

  		if ((pos = f_name_we.find(".la")) != string::npos)
  			lang = CPP;
  		else
  			lang = PYTHON;
  	}
  	
//
// Retrieve MaxDevice property
//
	long MaxDevice = ctrl_fica_constructed == true ? (*ctrlFiCa_ite)->get_MaxDevice(): -1; 
  	
//
// Save controller info
//
  
  	ControllerPool cp;
  	cp.ctrl = ct;
  	cp.name = f_name_lower + '.' + ctrl_class_name_lower + '/' + 
  			  inst_name_lower;
  	cp.user_full_name = inst_name + " (" + f_name + '.' + ctrl_class_name + 
  						'/' + inst_name + ')';
  	cp.ite_ctrl_fica = ctrlFiCa_ite;
  	cp.ctrl_fica_built = ctrl_fica_constructed;
  	cp.ctrl_class_name = ctrl_class_name;
  	cp.ctrl_fica_name = ctrl_type;
  	cp.MaxDevice = MaxDevice;
  				
  	if (lang == PYTHON)
  	{		
  		cp.user_full_name += " - ";
  		cp.user_full_name += ob_type;
  		cp.user_full_name += " Python ctrl (";
  		cp.user_full_name += f_name_we;
  		
  		if (f_name_we.find('.') == string::npos)
  			cp.user_full_name = cp.user_full_name + ".py";
  		cp.user_full_name = cp.user_full_name + ")";
  	}
  	else
  	{
  		cp.cpp_ctrl_prop = prop;
  		cp.user_full_name = cp.user_full_name + " - ";
  		cp.user_full_name += ob_type;
  		cp.user_full_name += " Cpp ctrl (";
  		cp.user_full_name += f_name_we;
  		
  		if (f_name_we.find('.') == string::npos)
  			cp.user_full_name = cp.user_full_name + ".la";
  		cp.user_full_name = cp.user_full_name + ")";
  	}
  	
//
// Add  this controller to the controller list
//
  
  	ctrl_list.push_back(cp);
  
//
// Manage controller id (from db or a new one)
//
  
  	if (internal == true)
  	{
  		ctrl_list.back().id = ctrl_id_db;
  		if (ctrl_id_db > last_ctrl_id)
  			last_ctrl_id = ctrl_id_db;
  	}
  	else
  	{		
  		last_ctrl_id++;
  		ctrl_list.back().id = last_ctrl_id;
  	}
  		
//
// Update info in DB
//
  
  	ctrl_info.push_back(ob_type.c_str());
  	ctrl_info.push_back(f_name_we.c_str());
  	ctrl_info.push_back(ctrl_class_name.c_str());
  	ctrl_info.push_back(inst_name.c_str());
  
  	if (internal == false)
  	{
  		stringstream s;
  		s << last_ctrl_id;
  		ctrl_info.push_back(s.str().c_str());
  	}
  	else
  		ctrl_info.push_back((*argin)[4].in());
  	
  	if (upd_db == true)
  	{
  		Tango::DbDatum ctrl(CTRL_PROP);
  		Tango::DbData db_data;
  
  		ctrl << ctrl_info;
  		db_data.push_back(ctrl);		
  		
  		try
  		{	
  			get_db_device()->put_property(db_data);
  		}
  		catch (Tango::DevFailed &e)
  		{
  			for (long loop = 0;loop < PROP_BY_CTRL;loop++)
  				ctrl_info.pop_back();
  			
  			{
  				AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  				delete ct;
  			}
  			ctrl_list.pop_back();
  			
  			TangoSys_OMemStream o;
  			o << "Can't update Db for controller " << (*argin)[2].in();
  			o << "/" << (*argin)[3].in() << ends;
  
  			Tango::Except::re_throw_exception(e,
  					(const char *)"Pool_CantUpdateDb",o.str(),
  				    (const char *)"Pool::create_controller");
  		}
  	}
  	
//
// If needed create controller elements
//
  	if(internal == false)
  	{
  		if(type == PSEUDO_MOTOR_CTRL)
  		{
  			vector<string> pseudo_mot_names, mot_names;

			PseudoMotCtrlFiCa* pm_fica = 
				static_cast<PseudoMotCtrlFiCa*>(*ctrlFiCa_ite);
			int m_nb = pm_fica->get_motor_role_nb();
			int pm_nb = pm_fica->get_pseudo_motor_role_nb();
  			
  			for(int i = 4; i < m_nb + 4; i++)
  				mot_names.push_back((*argin)[i].in());

  			for(int i = 4 + m_nb; i < m_nb + pm_nb + 4; i++)
  				pseudo_mot_names.push_back((*argin)[i].in());
  			
  			try
  			{
  				create_pseudo_motor_ctrl_elems(inst_name, pm_fica, 
  											   pseudo_mot_names, mot_names);
  			}
  	  		catch (Tango::DevFailed &e)
  	  		{
  	  			// restore 'Controller' property internally and in the DB
  	  			for (long loop = 0;loop < PROP_BY_CTRL;loop++)
  	  				ctrl_info.pop_back();
  	  		
  	    		Tango::DbDatum ctrl(CTRL_PROP);
  	    		Tango::DbData db_data;
  	    
  	    		ctrl << ctrl_info;
  	    		db_data.push_back(ctrl);		

  	    		get_db_device()->put_property(db_data);
 	  			
  	    		// Restore controller info
  	    		{
  	    			AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  	    			delete ct;
  	    		}

  	  			ctrl_list.pop_back();
  	  			
  	  			TangoSys_OMemStream o;
  	  			o << "Can't update Db for controller " << (*argin)[2].in();
  	  			o << "/" << (*argin)[3].in() << ends;
  	  
  	  			Tango::Except::re_throw_exception(e,
  	  				(const char *)"Pool_CantCreateControllerElements",o.str(),
  	  				(const char *)"Pool::create_controller");
  	  		}  	  			
  		}
  		else if(type == PSEUDO_COUNTER_CTRL)
  		{
  			vector<string> counter_names;

			PseudoCoCtrlFiCa* co_fica = 
				static_cast<PseudoCoCtrlFiCa*>(*ctrlFiCa_ite);
			int co_nb = co_fica->get_counter_role_nb(); 
  			
  			for(int i = 4; i < co_nb + 4; i++)
  				counter_names.push_back((*argin)[i].in());
  			
  			string pc_inst_name = (*argin)[co_nb + 4].in();
  			
  			try
  			{
  				create_pseudo_counter_ctrl_elems(inst_name, co_fica,
  												 pc_inst_name, counter_names);
  			}
  	  		catch (Tango::DevFailed &e)
  	  		{
  	  			// restore 'Controller' property internally and in the DB
  	  			for (long loop = 0;loop < PROP_BY_CTRL;loop++)
  	  				ctrl_info.pop_back();
  	  		
  	    		Tango::DbDatum ctrl(CTRL_PROP);
  	    		Tango::DbData db_data;
  	    
  	    		ctrl << ctrl_info;
  	    		db_data.push_back(ctrl);		

  	    		get_db_device()->put_property(db_data);
 	  			
  	    		// Restore controller info
  	    		{
  	    			AutoPoolLock lo((*ctrlFiCa_ite)->get_mon());
  	    			delete ct;
  	    		}

  	  			ctrl_list.pop_back();
  	  			
  	  			TangoSys_OMemStream o;
  	  			o << "Can't update Db for controller " << (*argin)[2].in();
  	  			o << "/" << (*argin)[3].in() << ends;
  	  
  	  			Tango::Except::re_throw_exception(e,
  	  				(const char *)"Pool_CantCreateControllerElements",o.str(),
  	  				(const char *)"Pool::create_controller");
  	  		}  	  			
  		}
  	}
  	
//
// Sort controller list if needed
//
  
  	if (internal == false)
  	{
  		if (ctrl_list.size() >= 2)
  			ctrl_list.sort(CtrlComp());
  	}
  	
//
// Throw exception if the controller creation failed during the device
// init phase
//
  
  	if ((internal == true) && (ct == NULL))
  	{
  		Tango::Except::re_throw_exception(save_exception,
  				(const char *)"Pool_CantCreateController",
  				(const char *)"Can't create controller class instance!!!",
  				(const char *)"Pool::create_controller");
	}
  	
//
// Inform client listennig on event but only if the controler are not
// created due to init command. In this case, the event is sent at the end
// of the init_device code
//
  
  	if (internal != true)
  	{
  		Tango::Attribute &cl = dev_attr->get_attr_by_name("ControllerList");
  		read_ControllerList(cl);
  		cl.fire_change_event();
  		
  		if(type == PSEUDO_MOTOR_CTRL)
  		{
		  	Tango::Attribute &pml = dev_attr->get_attr_by_name("PseudoMotorList");
		  	read_PseudoMotorList(pml);
		  	pml.fire_change_event();
  		}
  		else if(type == PSEUDO_COUNTER_CTRL)
  		{
		  	Tango::Attribute &pcl = dev_attr->get_attr_by_name("PseudoCounterList");
		  	read_PseudoCounterList(pcl);
		  	pcl.fire_change_event();
  		}
  	}		
}

//------------------------------------------------------------------------------
// Pool::add_motor
//
void Pool::add_motor(MotorPool &mp)
{
	mot_list.push_back(mp);
	if (mot_list.size() >=2 )
		mot_list.sort(PoolElementComp());
			
	if (mp.id > mot_id)
		mot_id = mp.id;
	
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::add_motor_group
//
void Pool::add_motor_group(MotorGroupPool &mgp)
{
	mot_group_list.push_back(mgp);
	if (mot_group_list.size() >=2 )
		mot_group_list.sort(PoolElementComp());
		
	if (mgp.id > mot_group_id)
		mot_group_id = mgp.id;
		
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::add_measurement_group
//
void Pool::add_measurement_group(MeasurementGroupPool &mgp)
{
	measurement_group_list.push_back(mgp);
	if (measurement_group_list.size() >=2 )
		measurement_group_list.sort(PoolElementComp());
		
	if (mgp.id > measurement_group_id)
		measurement_group_id = mgp.id;
		
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::add_pseudo_motor
//
void Pool::add_pseudo_motor(PseudoMotorPool &pmp)
{
	pseudo_mot_list.push_back(pmp);
	if (pseudo_mot_list.size() >=2 )
		pseudo_mot_list.sort(PoolElementComp());
			
	if (pmp.id > pseudo_mot_id)
		pseudo_mot_id = pmp.id;
	
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;						
}

//------------------------------------------------------------------------------
// Pool::add_pseudo_counter
//
void Pool::add_pseudo_counter(PseudoCounterPool &pcp)
{
	pseudo_co_list.push_back(pcp);
	if (pseudo_co_list.size() >=2 )
		pseudo_co_list.sort(PoolElementComp());
			
	if (pcp.id > exp_channel_id)
		exp_channel_id = pcp.id;
	
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;						
}

//------------------------------------------------------------------------------
// Pool::add_counter_timer
//
void Pool::add_counter_timer(CTExpChannelPool &mp)
{
	ct_list.push_back(mp);
	if (ct_list.size() >=2 )
		ct_list.sort(PoolElementComp());
			
	if (mp.id > exp_channel_id)
		exp_channel_id = mp.id;
		
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::add_zerod
//
void Pool::add_zerod(ZeroDExpChannelPool &mp)
{
	zerod_list.push_back(mp);
	if (zerod_list.size() >=2 )
		zerod_list.sort(PoolElementComp());
			
	if (mp.id > exp_channel_id)
		exp_channel_id = mp.id;
		
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a motor (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the motor. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::add_com_channel
//
void Pool::add_com_channel(CommunicationChannelPool &comchp)
{
	com_channel_list.push_back(comchp);
	if (com_channel_list.size() >=2 )
		com_channel_list.sort(PoolElementComp());
			
	if (comchp.id > exp_channel_id)
		com_channel_id = comchp.id;
		
//
// It is necessary to force proxy creation after this method.
// This method can be called when creating a communication channel (where the 
// proxy will be created by the Pool afterwards) but also in case of Init
// command on the communication channel. If a write of the Position attribute
// is executed just after, we need to create the proxy
//

	proxy_created = false;		
}

//------------------------------------------------------------------------------
// Pool::remove_motor
//
void Pool::remove_motor(Motor_ns::Motor *mot_ptr)
{
	list<MotorPool>::iterator ite;
	for (ite = mot_list.begin();ite != mot_list.end();++ite)
	{
		if (ite->motor == mot_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			mot_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_motor_group
//
void Pool::remove_motor_group(MotorGroup_ns::MotorGroup *grp_ptr)
{
	list<MotorGroupPool>::iterator ite;
	for (ite = mot_group_list.begin();ite != mot_group_list.end();++ite)
	{
		if (ite->group == grp_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			mot_group_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_measurement_group
//
void Pool::remove_measurement_group(MeasurementGroup_ns::MeasurementGroup *grp_ptr)
{
	list<MeasurementGroupPool>::iterator ite;
	for (ite = measurement_group_list.begin();
	     ite != measurement_group_list.end();++ite)
	{
		if (ite->group == grp_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			measurement_group_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_ct
//
void Pool::remove_ct(CTExpChannel_ns::CTExpChannel *ct_ptr)
{
	list<CTExpChannelPool>::iterator ite;
	for (ite = ct_list.begin();ite != ct_list.end();++ite)
	{
		if (ite->ct_channel == ct_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			ct_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_zerod
//
void Pool::remove_zerod(ZeroDExpChannel_ns::ZeroDExpChannel *zerod_ptr)
{
	list<ZeroDExpChannelPool>::iterator ite;
	for (ite = zerod_list.begin();ite != zerod_list.end();++ite)
	{
		if (ite->zerod_channel == zerod_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			zerod_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_pseudo_motor
//
void Pool::remove_pseudo_motor(PseudoMotor_ns::PseudoMotor *pseudo_ptr)
{	
	list<PseudoMotorPool>::iterator ite;
	for (ite = pseudo_mot_list.begin();ite != pseudo_mot_list.end();++ite)
	{
		if (ite->pseudo_motor == pseudo_ptr)
		{
			if(ite->obj_proxy != NULL)
				delete ite->obj_proxy; 
			pseudo_mot_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_pseudo_counter
//
void Pool::remove_pseudo_counter(PseudoCounter_ns::PseudoCounter *pseudo_ptr)
{	
	list<PseudoCounterPool>::iterator ite;
	for (ite = pseudo_co_list.begin();ite != pseudo_co_list.end();++ite)
	{
		if (ite->pseudo_counter == pseudo_ptr)
		{
			if(ite->obj_proxy != NULL)
				delete ite->obj_proxy; 
			pseudo_co_list.erase(ite);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Pool::remove_com_channel
//
void Pool::remove_com_channel(CommunicationChannel_ns::CommunicationChannel *comch_ptr)
{
	list<CommunicationChannelPool>::iterator ite;
	for (ite = com_channel_list.begin();ite != com_channel_list.end();++ite)
	{
		if (ite->com_channel == comch_ptr)
		{
			if (ite->obj_proxy != NULL)
				delete ite->obj_proxy;
			com_channel_list.erase(ite);
			break;
		}
	}
}

}
