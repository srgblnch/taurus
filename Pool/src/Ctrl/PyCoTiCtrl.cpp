#include <PyCoTiCtrl.h>
#include <iostream>

#include <stdlib.h>

using namespace std;

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PyCoTiController
// 
// description : 	This Python CounterTimer Controller class ctor
//
// argin : - inst : The controller instance name
//	   	   - cl_name : The controller class name
//	  	   - module : The python module object
//		   - prop_dict : The python properties dictionnary
//
//--------------------------------------------------------------------------

PyCoTiController::PyCoTiController(const char *inst,const char *cl_name,PyObject *module,PyObject *prop_dict)
:PyController(),CoTiController(inst),mod(module),py_class_name(cl_name)
{
	//cout << "[PyCoTiController] class ctor with instance = " << inst << endl;

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
			        	(const char *)"PyCoTiController::PyCoTiController");
	}
	
	int insert_res = PyTuple_SetItem(arg_tuple,0,arg_str);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller " << cl_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
			        	(const char *)"PyCoTiController::PyCoTiController");
	}
	
	insert_res = PyTuple_SetItem(arg_tuple,1,prop_dict);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller " << cl_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
			        	(const char *)"PyCoTiController::PyCoTiController");
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

        err_list[1].origin = CORBA::string_dup("PyCoTiController::PyCoTiController");
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
// method : 		PyCoTiController::~PyCoTiController
// 
// description : 	This Python CounterTimer Controller class dtor
//
//--------------------------------------------------------------------------

PyCoTiController::~PyCoTiController()
{
	//cout << "[PyCoTiController] class dtor" << endl;
	Py_DECREF(py_obj);
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::AddDevice
// 
// description : 	Creates a new CounterTimer for the controller 
//
// argin : - ind : The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::AddDevice(long ind)
{
	//cout << "[PyCoTiController] Creating a new C/T with index " << ind << " on controller PyCoTiController/" << inst_name  << endl;

	PyObject *res;
    res = PyObject_CallMethod(py_obj, (char*)"AddDevice", (char*)"l", ind);
	check_void_return(res, "Error reported from the controller AddDevice method","AddDevice");
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::DeleteDevice
// 
// description : 	Delete a CounterTimer from the controller 
//
// argin : - ind : The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::DeleteDevice(long ind)
{
	//cout << "[PyCoTiController] Deleting C/T with index " << ind << " on controller PyCoTiController/" << inst_name  << endl;

	PyObject *res;
    res = PyObject_CallMethod(py_obj, (char*)"DeleteDevice", (char*)"l",ind);
	check_void_return(res, "Error reported from the controller DeleteDevice method","DeleteDevice");
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreReadAll
// 
// description : 	Call the Python controller "PreReadAll" method
//
//--------------------------------------------------------------------------

void PyCoTiController::PreReadAll()
{
	if (pre_read_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreReadAll", NULL);
		check_void_return(res, "Error reported from the controller PreReadAll method","PreReadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreReadOne
// 
// description : 	Call the Python controller "PreReadOne" method
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::PreReadOne(long ind)
{
	if (pre_read_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreReadOne", (char*)"l", ind);
		check_void_return(res, "Error reported from the controller PreReadOne method","PreReadOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::ReadAll
// 
// description : 	Call the Python controller "ReadAll" method
//
//--------------------------------------------------------------------------

void PyCoTiController::ReadAll()
{
	if (read_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"ReadAll", NULL);
		check_void_return(res,"Error reported from the controller ReadAll method","ReadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::ReadOne
// 
// description : 	Read a C/T value from the Python controller 
//
// argin : - ind : The C/T index within the controller
//
//--------------------------------------------------------------------------

double PyCoTiController::ReadOne(long ind)
{
	PyObject *res;
    double dres;
	
    res = PyObject_CallMethod(py_obj, (char*)"ReadOne", (char*)"l", ind);
    if (res != NULL)
    {
    	if (PyNumber_Check(res) == 1)
    	{
    		if (PyFloat_Check(res) == true)
   				dres = PyFloat_AsDouble(res);
    		else
    			dres = (double)PyInt_AsLong(res);
    	}
    	else
    	{
    		Py_DECREF(res);
			Tango::Except::throw_exception((const char *)"PyCtrl_BadType",
		                               	(const char *)"The value returned by the controller code is not a Python float as expected",
					       				(const char *)"PyCoTiController::ReadOne()");
    	}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't read the Counter/Timer value");

        err_list[1].origin = CORBA::string_dup("PyCoTiController::ReadOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantReadValue");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
      
    Py_DECREF(res);
    return dres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreStateAll
// 
// description : 	Call the Python controller "PreReadAll" method
//
//--------------------------------------------------------------------------

void PyCoTiController::PreStateAll()
{
	if (pre_state_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStateAll", NULL);
		check_void_return(res,"Error reported from the controller PreStateAll method","PreStateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreStateOne
// 
// description : 	Call the Python controller "PreStateOne" method
//
// argin : - axis: The motor index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::PreStateOne(long axis)
{
	if (pre_state_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStateOne", (char*)"l", axis);
		check_void_return(res,"Error reported from the controller PreStateOne method","PreStateOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::StateAll
// 
// description : 	Call the Python controller "StateAll" method
//
//--------------------------------------------------------------------------

void PyCoTiController::StateAll()
{
	if (state_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StateAll", NULL);
		check_void_return(res,"Error reported from the controller StateAll method","StateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::StateOne
// 
// description : 	Call the Python controller "StateOne" method
//
// argin : - ind: The C/T index within the controller
//		   - ptr: Pointer to the structure used to returned a CounterTimer state
//
//--------------------------------------------------------------------------

void PyCoTiController::StateOne(long ind, Controller::CtrlState *ptr)
{
	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"StateOne", (char*)"l", ind);
    
//
// Check that the return value is a tuple with two int values
// and get them
//

    if (res != NULL)
	{
		if (PyTuple_Check(res) == true)
		{
			long size = PyTuple_Size(res);
			if ((size == 1) || (size == 2))
			{
				PyObject *py_state = PyTuple_GET_ITEM(res,0);
				PyObject *py_status;
				if (size == 2)
					py_status = PyTuple_GET_ITEM(res,1);
	
				long state;
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
							
				if (size == 2)
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
						string me("The second data in the tuple returned by the controller ");
						me = me + py_class_name;
						me = me + " StateOne method does not have the correct type (String wanted)";
						throw_simple_exception(me.c_str(),"StateOne");
					}
					ptr->status = tmp_status;
				}
					
				ptr->state = state;
			
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
// method : 		PyCoTiController::PreStartAllCT
// 
// description : 	Call the Python controller "PreStartAllCT" method
//
//--------------------------------------------------------------------------

void PyCoTiController::PreStartAllCT()
{
	if (pre_start_all_ct == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStartAllCT", NULL);
		check_void_return(res,"Error reported from the controller PreStartAllCT method","PreStartAllCT");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreStartOneCT
// 
// description : 	Call the Python controller "PreStartOneCT" method
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

bool PyCoTiController::PreStartOneCT(long ind)
{
	bool bres = true;
	
	if (pre_start_one_ct == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStartOneCT", (char*)"l", ind);

    	if (res != NULL)
    	{
    		if (PyBool_Check(res) == 1)
    		{
    			if (res == Py_False)
    				bres = false;
    			else
    				bres = true;
    		}
    		else
    		{
    			Py_DECREF(res);
				Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
		            (const char *)"The value returned by the controller code is not a Python boolean",
					(const char *)"PyCoTiController::PreStartOneCT()");
    		}
   	 	}
    	else
    	{
 			PyObject *ex_exec,*ex_value,*ex_tb;
			Tango::DevErrorList err_list;
			err_list.length(2);
		
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

			string tmp_str("Can't execute PreStartOne method");

        	err_list[1].origin = CORBA::string_dup("PyCoTiController::PreStartOneCT");
        	err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
			err_list[1].reason = CORBA::string_dup("PyCtrl_CantRunPreStartOneCT");
			err_list[1].severity = Tango::ERR;

			throw Tango::DevFailed(err_list); 
    	}		
	}
	
	return bres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::StartOneCT
// 
// description : 	Call the Python controller "StartOneCT" method
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::StartOneCT(long ind)
{
	if (start_one_ct == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StartOneCT", (char*)"l", ind);
		check_void_return(res, "Error reported from the controller StartOneCT method","StartOneCT");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::StartAllCT
// 
// description : 	Call the Python controller "StartAllCT" method
//
//--------------------------------------------------------------------------

void PyCoTiController::StartAllCT()
{
	if (start_all_ct == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StartAllCT", NULL);
		check_void_return(res, "Error reported from the controller StartAllCT method","StartAllCT");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreLoadAll
// 
// description : 	Call the Python controller "PreStartAllCT" method
//
//--------------------------------------------------------------------------

void PyCoTiController::PreLoadAll()
{
	if (pre_load_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreLoadAll", NULL);
		check_void_return(res, "Error reported from the controller PreLoadAll method","PreLoadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::PreLoadOne
// 
// description : 	Call the Python controller "PreLoadOne" method
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

bool PyCoTiController::PreLoadOne(long ind,double value)
{
	bool bres = true;
	
	if (pre_load_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, 
    			(char*)"PreLoadOne", 
    			(char*)"ld", ind, value);

    	if (res != NULL)
    	{
    		if (PyBool_Check(res) == 1)
    		{
    			if (res == Py_False)
    				bres = false;
    			else
    				bres = true;
    		}
    		else
    		{
    			Py_DECREF(res);
				Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
		            (const char *)"The value returned by the controller code is not a Python boolean",
					(const char *)"PyCoTiController::PreLoadOne()");
    		}
   	 	}
    	else
    	{
 			PyObject *ex_exec,*ex_value,*ex_tb;
			Tango::DevErrorList err_list;
			err_list.length(2);
		
			PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
			this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

			string tmp_str("Can't execute PreLoadOne method");

        	err_list[1].origin = CORBA::string_dup("PyCoTiController::PreLoadOne");
        	err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
			err_list[1].reason = CORBA::string_dup("PyCtrl_CantRunPreLoadOne");
			err_list[1].severity = Tango::ERR;

			throw Tango::DevFailed(err_list); 
    	}		
	}
	
	return bres;
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::LoadOne
// 
// description : 	Call the Python controller "LoadOne" method
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::LoadOne(long ind,double value)
{
	if (load_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"LoadOne", (char*)"ld", ind, value);
		check_void_return(res, "Error reported from the controller LoadOne method","LoadOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::LoadAll
// 
// description : 	Call the Python controller "LoadAll" method
//
//--------------------------------------------------------------------------

void PyCoTiController::LoadAll()
{
	if (load_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"LoadAll", NULL);
		check_void_return(res, "Error reported from the controller LoadAll method","LoadAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::AbortOne
// 
// description : 	Call the Python controller "AbortOne" method
//					which aborts a counting counter
//
// argin : - ind: The C/T index within the controller
//
//--------------------------------------------------------------------------

void PyCoTiController::AbortOne(long ind)
{
	if (abort_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"AbortOne", (char*)"l", ind);
   	 	check_void_return(res, "Error reported from the controller AbortOne method","AbortOne");
	}
	else
		throw_simple_exception("Method AbortOne is not implemented in controller","AbortOne");
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::SetExtraAttributePar
// 
// description : 	Call the Python controller "SetExtraAttribute" method
//
// argin : - axis: The counter/timer index within the controller
//		   - par_name: The parameter name
//		   - par_value: The parameter value
//
//--------------------------------------------------------------------------

void PyCoTiController::SetExtraAttributePar(long axis,string &par_name,Controller::CtrlData &par_value)
{
	if (set_extra_attribute == true)
	{
		PyObject *res;
		switch (par_value.data_type)
		{
			case Controller::BOOLEAN:
			res = PySetExtraAttributeBool(py_obj,axis,par_name,par_value.bo_data);
			break;
			
			case Controller::LONG:
   	 		res = PyObject_CallMethod(py_obj, (char*)"SetExtraAttributePar", 
   	 				(char*)"lsl", axis, par_name.c_str(), par_value.lo_data);
			break;
			
			case Controller::DOUBLE:
   	 		res = PyObject_CallMethod(py_obj, (char*)"SetExtraAttributePar",
   	 				(char*)"lsd",axis,par_name.c_str(),par_value.db_data);
			break;
			
			default:
   	 		res = PyObject_CallMethod(py_obj, (char*)"SetExtraAttributePar",
   	 				(char*)"lss",axis,par_name.c_str(),par_value.str_data.c_str());
			break;
		}
		check_void_return(res, "Error reported from the controller SetExtraAttributePar method","SetExtraAttributePar");
	}
	else
		throw_simple_exception("Method SetExtraAttributePar is not implemented in controller","SetExtraAttributePar");
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::GetExtraAttributePar
// 
// description : 	Call the Python controller "GetExtraAttributePar" method
//
// argin : - axis: The counter/timer index within the controller
//		   - par_name: The parameter name
//
//--------------------------------------------------------------------------

Controller::CtrlData PyCoTiController::GetExtraAttributePar(long axis,string &extra_par_name)
{
    Controller::CtrlData dres;
 
 	if (get_extra_attribute == true)
 	{   
		PyObject *res;
		
	    res = PyObject_CallMethod(py_obj, (char*)"GetExtraAttributePar",
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
// method : 		PyCoTiController::SendToCtrl
// 
// description : 	Call the Python controller "SendToCtrl" method
//
// argin : - in_str: The input string to pass to the controller
//
//--------------------------------------------------------------------------

string PyCoTiController::SendToCtrl(string &in_str)
{
	string returned_str("Default string: The controller returns nothing or an invalid data type");
	
	if (send_to_ctrl == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, 
    			(char*)"SendToCtrl", (char*)"s",
    			in_str.c_str());
    	
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
// method : 		PyCoTiController::clear_method_flag
// 
// description : 	Clear all the boolean flags used to memorize which
//					pre-defined method are implemented in this controller
//
//--------------------------------------------------------------------------

void PyCoTiController::clear_method_flag()
{
	pre_read_all = false;
	pre_read_one = false;
	read_all = false;	
	abort_one = false;
	pre_start_all_ct = false;
	pre_start_one_ct = false;
	start_one_ct = false;
	start_all_ct = false;
	pre_load_all = false;
	pre_load_one = false;
	load_one = false;
	load_all = false;
}

//+-------------------------------------------------------------------------
//
// method : 		PyCoTiController::check_existing_method
// 
// description : 	Check which pre-defined methods are implemented in this
//					controller and set the method flag according to the
//					check result
//					It is not necesseray to check for 
//						GetState()
//						ReadOne()
//					because the pool refuses to load controller code if these
//					methods are not defined
//
// argin : - obj : The python controller object
//
//--------------------------------------------------------------------------

void PyCoTiController::check_existing_methods(PyObject *obj)
{
	base_check_existing_methods(obj);
	
	PyObject *met;

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

	if ((met = PyObject_GetAttrString(obj,"PreStartAllCT")) != NULL)
	{
		pre_start_all_ct = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"PreStartOneCT")) != NULL)
	{
		pre_start_one_ct = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"StartOneCT")) != NULL)
	{
		start_one_ct = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"StartAllCT")) != NULL)
	{
		start_all_ct = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"PreLoadAll")) != NULL)
	{
		pre_load_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"PreLoadOne")) != NULL)
	{
		pre_load_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"LoadOne")) != NULL)
	{
		load_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"LoadAll")) != NULL)
	{
		load_all = true;
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
}

extern "C"
{
Controller *_create_PyCounterTimerController(const char *inst,const char *cl_name,PyObject *mo,PyObject *prop)
{
	return new PyCoTiController(inst,cl_name,mo,prop);
}
}
