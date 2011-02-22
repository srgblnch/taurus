#include <PyMotCtrl.h>
#include <iostream>

#include <stdlib.h>

using namespace std;

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PyMotorController
// 
// description : 	This Python Motor Controller class ctor
//
// argin : - inst : The controller instance name
//	   	   - cl_name : The controller class name
//	  	   - module : The python module object
//		   - prop_dict : The python properties dictionnary
//
//--------------------------------------------------------------------------

PyMotorController::PyMotorController(const char *inst,const char *cl_name,PyObject *module,PyObject *prop_dict)
:PyController(),MotorController(inst),mod(module),py_class_name(cl_name)
{
	//cout << "[PyMotorController] class ctor with instance = " << inst << endl;

	clear_method_flag();

//
// Build the string to create the Python controller object
//

	string py_cmd(cl_name);
	py_cmd = py_cmd + "('" + inst_name + "')";
	
//
// Create the controller object
//

	PyObject *ctrl_class = PyObject_GetAttrString(mod,(char *)cl_name);
	PyObject *arg_tuple = PyTuple_New(2);
	PyObject *arg_str = PyString_FromString(inst);
	
	if ((ctrl_class == NULL) || (arg_tuple == NULL) || (arg_str == NULL))
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller " << cl_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
			        	(const char *)"PyMotorController::PyMotorController");
	}
	
	int insert_res = PyTuple_SetItem(arg_tuple,0,arg_str);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller " << cl_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
			        	(const char *)"PyMotorController::PyMotorController");
	}
	
	insert_res = PyTuple_SetItem(arg_tuple,1,prop_dict);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller " << cl_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
			        	(const char *)"PyMotorController::PyMotorController");
	}
		
	py_obj = PyObject_Call(ctrl_class,arg_tuple,NULL);
	if (py_obj == NULL)
	{
		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't create Python controller ");
		tmp_str += cl_name;

        err_list[1].origin = CORBA::string_dup("PyMotorController::PyMotorController");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantCreatePyController");
		err_list[1].severity = Tango::ERR;
		
		Py_DECREF(ctrl_class);
		Py_DECREF(arg_tuple);

		throw Tango::DevFailed(err_list);
	}
	
	Py_DECREF(ctrl_class);
	Py_DECREF(arg_tuple);

//
// Check which methods are defined within this class
//

	check_existing_methods(py_obj);

}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::~PyMotorController
// 
// description : 	This Python Motor Controller class dtor
//
//--------------------------------------------------------------------------

PyMotorController::~PyMotorController()
{
	//cout << "[PyMotorController] class dtor" << endl;
	Py_DECREF(py_obj);
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::AddDevice
// 
// description : 	Creates a new motor for the controller 
//
// argin : - axis : The motor index within the controller
//
//--------------------------------------------------------------------------

void PyMotorController::AddDevice(long axis)
{
	//cout << "[PyMotorController] Creating a new motor with index " << axis << " on controller PyMotorController/" << inst_name  << endl;

	AutoPythonCtrlLock lo;
	
	PyObject *res;
    res = PyObject_CallMethod(py_obj,
    		(char*)"AddDevice",
    		(char*)"l", axis);
	check_void_return(res, "Error reported from the controller AddDevice method","AddDevice");
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::DeleteDevice
// 
// description : 	Delete a motor from the controller 
//
// argin : - axis : The motor index within the controller
//
//--------------------------------------------------------------------------

void PyMotorController::DeleteDevice(long axis)
{
	//cout << "[PyMotorController] Deleting motor with index " << axis << " on controller PyMotorController/" << inst_name  << endl;

	AutoPythonCtrlLock lo;
	
	PyObject *res;
    res = PyObject_CallMethod(py_obj,
    		(char*)"DeleteDevice",
    		(char*)"l",axis);
	check_void_return(res, "Error reported from the controller DeleteDevice method","DeleteDevice");
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreStartAll
// 
// description : 	Call the Python controller "PreStartAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::PreStartAll()
{
	if (pre_start_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStartAll", NULL);
		check_void_return(res, "Error reported from the controller PreStartAll method","PreStartAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreStartOne
// 
// description : 	Call the Python controller "PreStartOne" method
//
// argin : - axis: The motor index within the controller
//		   - pos : The new motor position
//
//--------------------------------------------------------------------------

bool PyMotorController::PreStartOne(long axis,double pos)
{
	bool bres = true;

	if (pre_start_one == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
   	 	res = PyObject_CallMethod(py_obj, 
   	 			(char*)"PreStartOne",
   	 			(char*)"ld", axis, pos);
   	 	
    	if (res == NULL)
    	{
 			PyObject *ex_exec,*ex_value,*ex_tb;
			Tango::DevErrorList err_list;
			err_list.length(2);
		
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

			string tmp_str("Can't execute PreStartOne method");

        	err_list[1].origin = CORBA::string_dup("PyMotorController::PreStartOne");
        	err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
			err_list[1].reason = CORBA::string_dup("PyCtrl_CantRunPreStartOne");
			err_list[1].severity = Tango::ERR;

			throw Tango::DevFailed(err_list); 
    	}
    	
		if (PyBool_Check(res) == 1)
		{
			bres = res == Py_True; 
		}
		else
		{
			Py_DECREF(res);
			Tango::Except::throw_exception(
				(const char *)"PyCtrl_BadType",
	            (const char *)"The value returned by the controller code is not a Python boolean",
				(const char *)"PyMotorController::PreStartOne()");
		}
      
    	Py_DECREF(res);
	}

	return bres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::StartOne
// 
// description : 	Call the Python controller "StartOne" method
//					which is part of the calls sequence to move a motor
//
// argin : - axis: The motor index within the controller
//		   - pos: The new motor position
//
//--------------------------------------------------------------------------

void PyMotorController::StartOne(long axis,double pos)
{
	if (start_one == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, 
    			(char*)"StartOne", 
    			(char*)"ld", axis, pos);
   	 	check_void_return(res,"Error reported from the controller StartOne method","StartOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::StartAll
// 
// description : 	Call the Python controller "StartAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::StartAll()
{
	if (start_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StartAll", NULL);
		check_void_return(res, "Error reported from the controller StartAll method","StartAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreReadAll
// 
// description : 	Call the Python controller "PreReadAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::PreReadAll()
{
	if (pre_read_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreReadAll", NULL);
		check_void_return(res, "Error reported from the controller PreReadAll method","PreReadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreReadOne
// 
// description : 	Call the Python controller "PreReadOne" method
//
// argin : - axis: The motor index within the controller
//
//--------------------------------------------------------------------------

void PyMotorController::PreReadOne(long axis)
{
	if (pre_read_one == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, 
    			(char*)"PreReadOne", 
    			(char*)"l", axis);
		check_void_return(res, "Error reported from the controller PreReadOne method","PreReadOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::ReadAll
// 
// description : 	Call the Python controller "ReadAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::ReadAll()
{
	if (read_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"ReadAll", NULL);
		check_void_return(res, "Error reported from the controller ReadAll method","ReadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::ReadOne
// 
// description : 	Read a motor position from the Python controller 
//
// argin : - axis : The motor index within the controller
//
//--------------------------------------------------------------------------

double PyMotorController::ReadOne(long axis)
{
	PyObject *res;
    double dres;
	
    AutoPythonCtrlLock lo;
    
    res = PyObject_CallMethod(py_obj,
    		(char*)"ReadOne", 
    		(char*)"l", axis);
    
    if (res != NULL)
    {
    	if (!get_py_number(res,dres))
    	{
    		Py_DECREF(res);
			Tango::Except::throw_exception( (const char *)"PyCtrl_BadType",
		        (const char *)"The value returned by the controller code is not a Python number as expected",
		        (const char *)"PyMotorController::ReadOne()");
    	}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't read the position");

        err_list[1].origin = CORBA::string_dup("PyMotorController::ReadOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantReadPosition");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
      
    Py_DECREF(res);
    return dres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreStateAll
// 
// description : 	Call the Python controller "PreReadAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::PreStateAll()
{
	if (pre_state_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStateAll", NULL);
		check_void_return(res, "Error reported from the controller PreStateAll method","PreStateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::PreStateOne
// 
// description : 	Call the Python controller "PreStateOne" method
//
// argin : - axis: The motor index within the controller
//
//--------------------------------------------------------------------------

void PyMotorController::PreStateOne(long axis)
{
	if (pre_state_one == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj,
    			(char*)"PreStateOne",
    			(char*)"l", axis);
		check_void_return(res,"Error reported from the controller PreStateOne method","PreStateOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::StateAll
// 
// description : 	Call the Python controller "StateAll" method
//
//--------------------------------------------------------------------------

void PyMotorController::StateAll()
{
	if (state_all == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StateAll", NULL);
		check_void_return(res, "Error reported from the controller StateAll method","StateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::StateOne
// 
// description : 	Call the Python controller "StateOne" method
//
// argin : - axis: The motor index within the controller
//		   - base_ptr: Pointer to the structure used to returned a motor state
//
//--------------------------------------------------------------------------

void PyMotorController::StateOne(long axis, Controller::CtrlState *base_ptr)
{
	PyObject *res;
	
	AutoPythonCtrlLock lo;
	
    res = PyObject_CallMethod(py_obj, 
    		(char*)"StateOne", 
    		(char*)"l", axis);
    
//
// Check that the return value is a tuple with two int values
// and get them
//

    if (res != NULL)
	{
		MotorController::MotorState *ptr = static_cast<MotorController::MotorState *>(base_ptr);

		if (PyTuple_Check(res) == true)
		{
			long size = PyTuple_Size(res);
			if ((size == 2) || (size == 3))
			{
				PyObject *py_state = PyTuple_GET_ITEM(res,0);
				PyObject *py_limit = PyTuple_GET_ITEM(res,1);
				PyObject *py_status;
				if (size == 3)
					py_status = PyTuple_GET_ITEM(res,2);
	
				long state,limit;
				char *tmp_status;
				
				if (PyInt_Check(py_state) == true)			
					state = PyInt_AsLong(py_state);
				else
				{
					Py_DECREF(res);
					string me("The first data in the tuple returned by the controller ");
					me = me + py_class_name;
					me = me + " StateOne method does not have the correct type (Integer wanted)";
					throw_simple_exception(me.c_str(),"StateOne");
				}
							
				if (PyInt_Check(py_limit) == true)
					limit = PyInt_AsLong(py_limit);
				else
				{
					Py_DECREF(res);
					string me("The second data in the tuple returned by the controller ");
					me = me + py_class_name;
					me = me + " StateOne method does not have the correct type (Integer wanted)";
					throw_simple_exception(me.c_str(),"StateOne");
				}

				if (size == 3)
				{				
					if (PyString_Check(py_status) == true)
					{
						tmp_status = PyString_AsString(py_status);
						if (tmp_status == NULL)
						{
							Py_DECREF(res);
							string me("Can't decode the string in the tuple returned by the controller ");
							me = me + py_class_name;
							throw_simple_exception(me.c_str(),"StateOne");						
						}
					}					
					else
					{
						Py_DECREF(res);
						string me("The third data in the tuple returned by the controller ");
						me = me + py_class_name;
						me = me + " StateOne method does not have the correct type (String wanted)";
						throw_simple_exception(me.c_str(),"StateOne");
					}
					ptr->status = tmp_status;
				}
					
				ptr->state = state;
				ptr->switches = limit;
			
				Py_DECREF(res);
			}
			else
			{
				Py_DECREF(res);
					string me("Tuple returned by the controller ");
					me = me + py_class_name;
					me = me + " StateOne method does not have the correct size";
				throw_simple_exception("Tuple returned by the controller StateOne method does not have the correct size","StateOne");
			}
		}
		else
		{
			Py_DECREF(res);
			string me("Data returned by the controller ");
			me = me + py_class_name;
			me = me + " StateOne method does not have the correct type";
			throw_simple_exception(me.c_str(),"StateOne");
		}
	}
	else
		check_void_return(res,"Error reported from the controller StateOne method","StateOne");
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::AbortOne
// 
// description : 	Call the Python controller "AbortOne" method
//					which aborts a motor movement
//
// argin : - axis: The motor index within the controller
//
//--------------------------------------------------------------------------

void PyMotorController::AbortOne(long axis)
{
	if (abort_one == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj,
    			(char*)"AbortOne", 
    			(char*)"l", axis);
   	 	check_void_return(res, "Error reported from the controller AbortOne method","AbortOne");
	}
	else
		throw_simple_exception("Method AbortOne is not implemented in controller","AbortOne");
}
	
//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::DefinePosition
// 
// description : 	Call the Python controller "DefinePosition" method
//
// argin : - axis: The motor index within the controller
//		   - pos: The new motor position to be written in the controller
//
//--------------------------------------------------------------------------

void PyMotorController::DefinePosition(long axis,double pos)
{
	if (def_position == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj,
    			(char*)"DefinePosition",
    			(char*)"ld", axis, pos);
		check_void_return(res, "Error reported from the controller DefinePosition method","DefinePosition");
	}
	else
		throw_simple_exception("Method DefinePosition is not implemented in controller","DefinePosition");
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::GetPar
// 
// description : 	Call the Python controller "GetPar" method
//
// argin : - axis: The motor index within the controller
//		   - par_name: The parameter name
//
//--------------------------------------------------------------------------

Controller::CtrlData PyMotorController::GetPar(long axis,string &par_name)
{
    Controller::CtrlData dres;
    
	if (get_par == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
	
    	res = PyObject_CallMethod(py_obj,
    			(char*)"GetPar",
    			(char*)"ls", axis, par_name.c_str());
    	
   	 	if (res == NULL)
    	{
 			PyObject *ex_exec,*ex_value,*ex_tb;
			Tango::DevErrorList err_list;
			err_list.length(2);
		
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

			string tmp_str("Can't get motor parameter");

        	err_list[1].origin = CORBA::string_dup("PyMotorController::GetPar");
        	err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
			err_list[1].reason = 
				CORBA::string_dup("PyCtrl_CantGetMotorParameter");
			err_list[1].severity = Tango::ERR;

			throw Tango::DevFailed(err_list); 
    	}    		
   	 	
   	 	if (PyNumber_Check(res) == 1)
		{
			if (par_name == "backlash")
			{
				if (PyInt_Check(res) == true)
				{
					dres.lo_data = PyInt_AsLong(res);
					dres.data_type = Controller::LONG;
				}
				else
				{
					Py_DECREF(res);
					Tango::Except::throw_exception(
						(const char *)"PyCtrl_BadType",
						(const char *)"The value returned by the controller"
								" code is not a Python integer as expected",
				       	(const char *)"PyMotorController::GetPar()");
				}
			}
			else
			{
				if (PyFloat_Check(res) == true)
					dres.db_data = PyFloat_AsDouble(res);
				else
					dres.db_data = (double)PyInt_AsLong(res);
				dres.data_type = Controller::DOUBLE;
			}
		}
		else
		{
			Py_DECREF(res);
			Tango::Except::throw_exception(
				(const char *)"PyCtrl_BadType",
				(const char *)"The value returned by the controller code is"
							  " not a Python number as expected",
				(const char *)"PyMotorController::GetPar()");
		}

       	Py_DECREF(res);
	}
	else
	{
		cout << "Default GetPar method returning NaN" << endl;
		dres.db_data = strtod("NaN",NULL);
	}
	
    return dres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::SetPar
// 
// description : 	Call the Python controller "SetPar" method
//
// argin : - axis: The motor index within the controller
//		   - par_name: The parameter name
//		   - par_value: The parameter value
//
//--------------------------------------------------------------------------

void PyMotorController::SetPar(long axis,string &par_name,
							   Controller::CtrlData &par_value)
{
	if (set_par == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
		if (par_name == "backlash")
		{
			if (par_value.data_type == Controller::LONG)
				res = PyObject_CallMethod(py_obj,
						(char*)"SetPar", (char*)"lsl", axis,
						par_name.c_str(),par_value.lo_data);
			else
				throw_simple_exception("Bad data type used to set backlash "
									   "parameter: Long wanted","SetPar");
		}
		else
		{
			if (par_value.data_type == Controller::DOUBLE)
    			res = PyObject_CallMethod(py_obj,
    					(char*)"SetPar",
    					(char*)"lsd", axis,
    					par_name.c_str(),par_value.db_data);
    		else
    			throw_simple_exception("Bad data type used to set motor "
    								   "parameters: Double wanted","SetPar");
		}
		check_void_return(res,"Error reported from the controller SetPar method",
						  "SetPar");
	}
	else
		throw_simple_exception("Method SetPar is not implemented in controller",
							   "SetPar");
}


//+-------------------------------------------------------------------------
//
// method : 		PymotorController::SetExtraAttributePar
// 
// description : 	Call the Python controller "SetExtraAttribute" method
//
// argin : - axis: The motor index within the controller
//		   - par_name: The parameter name
//		   - par_value: The parameter value
//
//--------------------------------------------------------------------------

void PyMotorController::SetExtraAttributePar(long axis,string &par_name,
											 Controller::CtrlData &par_value)
{
	if (set_extra_attribute == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
		switch (par_value.data_type)
		{
			case Controller::BOOLEAN:
			res = PySetExtraAttributeBool(py_obj,axis,par_name,
										  par_value.bo_data);
			break;
			
			case Controller::LONG:
   	 		res = PyObject_CallMethod(py_obj,
   	 				(char*)"SetExtraAttributePar",
   	 				(char*)"lsl", axis,
   	 				par_name.c_str(), par_value.lo_data);
			break;
			
			case Controller::DOUBLE:
   	 		res = PyObject_CallMethod(py_obj,
   	 				(char*)"SetExtraAttributePar",
   	 				(char*)"lsd", axis,
   	 				par_name.c_str(),par_value.db_data);
			break;
			
			default:
   	 		res = PyObject_CallMethod(py_obj,
   	 				(char*)"SetExtraAttributePar",
   	 				(char*)"lss", axis,
   	 				par_name.c_str(), par_value.str_data.c_str());
			break;
		}
		check_void_return(res,"Error reported from the controller "
						  "SetExtraAttributePar method","SetExtraAttributePar");
	}
	else
		throw_simple_exception("Method SetExtraAttributePar is not implemented "
							   "in controller","SetExtraAttributePar");
}

//+-------------------------------------------------------------------------
//
// method : 		PymotorController::GetExtraAttributePar
// 
// description : 	Call the Python controller "GetExtraAttributePar" method
//
// argin : - axis: The motor index within the controller
//		   - par_name: The parameter name
//
//--------------------------------------------------------------------------

Controller::CtrlData PyMotorController::GetExtraAttributePar(long axis,string &extra_par_name)
{
    Controller::CtrlData dres;
 
 	if (get_extra_attribute == true)
 	{   
 		AutoPythonCtrlLock lo;
 		
		PyObject *res;
		
	    res = PyObject_CallMethod(py_obj,
	    		(char*)"GetExtraAttributePar",
	    		(char*)"ls", axis, extra_par_name.c_str());
	    
	   	if (res != NULL)
	    {
	    	if (PyString_Check(res) == 1)
	    	{
	    		dres.str_data = PyString_AsString(res);
	    		dres.data_type = Controller::STRING;
	    	}
	    	else if (PyFloat_Check(res) == 1)
	    	{
	    		dres.db_data = PyFloat_AsDouble(res);
	    		dres.data_type = Controller::DOUBLE;
	    	}	    	
	    	else if (PyInt_Check(res) == 1)
	    	{
				if (PyBool_Check(res) == 1)
	    		{
	    			if (res == Py_False)
	    				dres.bo_data = false;
	    			else
	    				dres.bo_data = true;
	    			dres.data_type = Controller::BOOLEAN;
	    		}
	    		else
	    		{
	    			dres.lo_data = PyInt_AsLong(res);
	    			dres.data_type = Controller::LONG;
	    		}
	    	}
	    	else
	    	{
	    		Py_DECREF(res);
				Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
	                (const char *)"The value returned by the controller code is neither a Python string, a Python float or a Python int as expected",
					(const char *)"PyController::GetExtraAttributePar()");
	    	}
	    }
		else
		{
			PyObject *ex_exec,*ex_value,*ex_tb;
			Tango::DevErrorList err_list;
			err_list.length(2);
		
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);
	
			string tmp_str("Can't get extra attribute parameter");
	
	    	err_list[1].origin = CORBA::string_dup("PyController::GetExtraAttributePar");
	    	err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
			err_list[1].reason = CORBA::string_dup("PyCtrl_CantGetExtraAttrParameter");
			err_list[1].severity = Tango::ERR;
	
			throw Tango::DevFailed(err_list); 
		}
	   	Py_DECREF(res);
 	}
 	else
 		throw_simple_exception("Method GetExtraAttributePar is not implemented in controller","GetExtraAttributePar");
	
    return dres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::SendToCtrl
// 
// description : 	Call the Python controller "SendToCtrl" method
//
// argin : - in_str: The input string to pass to the controller
//
//--------------------------------------------------------------------------

string PyMotorController::SendToCtrl(string &in_str)
{
	string returned_str("Default string: The controller returns nothing or an invalid data type");
	
	if (send_to_ctrl == true)
	{
		AutoPythonCtrlLock lo;
		
		PyObject *res;
    	res = PyObject_CallMethod(py_obj,
    			(char*)"SendToCtrl",
    			(char*)"s", in_str.c_str());
    	
	   	if (res != NULL)
	    {
	    	if (PyString_Check(res) == 1)
	    	{
	    		returned_str = PyString_AsString(res);
	    	}
	    	Py_DECREF(res);
	    }
	}
	else
		throw_simple_exception("Method SendToCtrl is not implemented in controller","SendToCtrl");
		
	return returned_str;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::clear_method_flag
// 
// description : 	Clear all the boolean flags used to memorize which
//					pre-defined method are implemented in this controller
//
//--------------------------------------------------------------------------

void PyMotorController::clear_method_flag()
{
	pre_start_all = false;
	pre_start_one = false;
	start_one = false;
	start_all = false;	
	pre_read_all = false;
	pre_read_one = false;
	read_all = false;	
	abort_one = false;
	def_position = false;
	set_par = false;
	get_par = false;
}

//+-------------------------------------------------------------------------
//
// method : 		PyMotorController::check_existing_methods
// 
// description : 	Check which pre-defined methods are implemented in this
//					controller and set the method flag according to the
//					check result
//					It is not ncesseray to check for 
//						GetState()
//						ReadOne()
//					because the pool refuses to load controller code if these
//					methods are not defined
//
// argin : - obj : The python controller object
//
//--------------------------------------------------------------------------

void PyMotorController::check_existing_methods(PyObject *obj)
{
	base_check_existing_methods(obj);
	
	PyObject *met;

	if ((met = PyObject_GetAttrString(obj,"PreStartAll")) != NULL)
	{
		pre_start_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"PreStartOne")) != NULL)
	{
		pre_start_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
			
	if ((met = PyObject_GetAttrString(obj,"StartOne")) != NULL)
	{
		start_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"StartAll")) != NULL)
	{
		start_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"PreReadAll")) != NULL)
	{
		pre_read_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"PreReadOne")) != NULL)
	{
		pre_read_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
			
	if ((met = PyObject_GetAttrString(obj,"ReadAll")) != NULL)
	{
		read_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
	
	if ((met = PyObject_GetAttrString(obj,"AbortOne")) != NULL)
	{
		abort_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
	
	if ((met = PyObject_GetAttrString(obj,"DefinePosition")) != NULL)
	{
		def_position = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"GetPar")) != NULL)
	{
		get_par = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"SetPar")) != NULL)
	{
		set_par = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
}

extern "C"
{
Controller *_create_PyMotorController(const char *inst,const char *cl_name,PyObject *mo,PyObject *prop)
{
	return new PyMotorController(inst,cl_name,mo,prop);
}
}
