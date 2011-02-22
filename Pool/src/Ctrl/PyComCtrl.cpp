#include <PyComCtrl.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

PyComController::PyComController(const char *inst,const char *cl_name,
								 PyObject *module,PyObject *prop_dict):
PyController(),ComController(inst),mod(module),py_class_name(cl_name)
{
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
		o << "Can't build argument to create Python controller ";
		o << cl_name << ends;

		Tango::Except::throw_exception(
					(const char *)"Pool_CantCreatePyController",o.str(),
					(const char *)"PyComController::PyComController");
	}
	
	int insert_res = PyTuple_SetItem(arg_tuple,0,arg_str);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller ";
		o << cl_name << ends;

		Tango::Except::throw_exception(
						(const char *)"Pool_CantCreatePyController",o.str(),
						(const char *)"PyComController::PyComController");
	}
	
	insert_res = PyTuple_SetItem(arg_tuple,1,prop_dict);
	if (insert_res != 0)
	{
		TangoSys_OMemStream o;
		o << "Can't build argument to create Python controller ";
		o << cl_name << ends;

		Tango::Except::throw_exception(
						(const char *)"Pool_CantCreatePyController",o.str(),
						(const char *)"PyComController::PyComController");
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

        err_list[1].origin = 
        	CORBA::string_dup("PyComController::PyComController");
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
// method : 		PyComController::~PyComController
// 
// description : 	This Python CommunicationChannel Controller class dtor
//
//--------------------------------------------------------------------------

PyComController::~PyComController()
{
	//cout << "[PyComController] class dtor" << endl;
	Py_DECREF(py_obj);
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::AddDevice
// 
// description : 	Creates a new CommunicationChannel for the controller 
//
// argin : - ind : The CommunicationChannel index within the controller
//
//--------------------------------------------------------------------------

void PyComController::AddDevice(long ind)
{
	//cout << "[PyComController] Creating a new CommunicationChannel with index " << ind << " on controller PyComController/" << inst_name  << endl;

	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"AddDevice", (char*)"l", ind);
	check_void_return(res,"Error reported from the controller AddDevice method",
					  "AddDevice");
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::DeleteDevice
// 
// description : 	Delete a CommunicationChannel from the controller 
//
// argin : - ind : The CommunicationChannel index within the controller
//
//--------------------------------------------------------------------------

void PyComController::DeleteDevice(long ind)
{
	//cout << "[PyComController] Deleting CommunicationChannel with index " << ind << " on controller PyComController/" << inst_name  << endl;

	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"DeleteDevice", (char*)"l", ind);
	check_void_return(res,
					  "Error reported from the controller DeleteDevice method",
					  "DeleteDevice");
}

void PyComController::OpenOne(long ind)
{
	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"OpenOne", (char*)"l", ind);
	
    if (res == NULL)
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't open Communication Channel");

        err_list[1].origin = CORBA::string_dup("PyComController::Open");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantOpen");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }		
}

void PyComController::CloseOne(long ind)
{
	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"CloseOne", (char*)"l", ind);
	
    if (res == NULL)
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't close Communication Channel");

        err_list[1].origin = CORBA::string_dup("PyComController::Close");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantClose");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }	
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::ReadOne
// 
// description : 	Read CommunicationChannel data from the Python controller 
//
// argin : - ind : The CommunicationChannel index within the controller
//
//--------------------------------------------------------------------------

string &PyComController::ReadOne(long ind, long max_read_len /*= -1*/)
{
	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"ReadOne", (char*)"(ll)", 
    		ind, max_read_len);
    
    if (res != NULL)
    {
		if (PyString_Check(res) == 1)
		{
			read_buff.assign(PyString_AsString(res), PyString_Size(res));
		}
		else
		{
			Py_DECREF(res);
			Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
		            (const char *)"The value returned by the controller code is"
		            			  " not a Python string as expected",
		            (const char *)"PyComController::ReadOne()");
		}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't read the Communication Channel value");

        err_list[1].origin = CORBA::string_dup("PyComController::ReadOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantReadValue");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
    Py_DECREF(res);
    return read_buff;
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::ReadLineOne
// 
// description : 	Read CommunicationChannel data up to a new line character
//                  from the Python controller 
//
// argin : - ind : The CommunicationChannel index within the controller
//
//--------------------------------------------------------------------------

string &PyComController::ReadLineOne(long ind)
{
	PyObject *res;
	
    res = PyObject_CallMethod(py_obj, (char*)"ReadLineOne", (char*)"l", ind);
    
    if (res != NULL)
    {
		if (PyString_Check(res) == 1)
		{
			read_buff.assign(PyString_AsString(res), PyString_Size(res));
		}
		else
		{
			Py_DECREF(res);
			Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
					(const char *)"The value returned by the controller code is"
								  " not a Python string as expected",
					(const char *)"PyComController::ReadLineOne()");
		}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't read the Communication Channel value");

        err_list[1].origin = CORBA::string_dup("PyComController::ReadLineOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantReadValue");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
    Py_DECREF(res);
    return read_buff;
}

long PyComController::WriteOne(long ind, string &istr, long write_len /*= -1*/) 
{
	PyObject *res;
	long lres;
	
    res = PyObject_CallMethod(py_obj, (char*)"WriteOne", (char*)"(ls#l)", 
    		ind, istr.c_str(), istr.size(), write_len);
    if (res != NULL)
    {
    	if (PyNumber_Check(res) == 1)
    	{
    		if (PyInt_Check(res) == true)
   				lres = PyInt_AsLong(res);
    		else if (PyLong_Check(res) == true)
    			lres = PyLong_AsLong(res);
	    	else
	    	{
	    		Py_DECREF(res);
				Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
					(const char *)"The value returned by the controller code is"
								  " an invalid numeric type. It must be an integer",
					(const char *)"PyComController::WriteOne()");
	    	}
    	}	
    	else
    	{
    		Py_DECREF(res);
			Tango::Except::throw_exception(
				(const char *)"PyCtrl_BadType",
				(const char *)"The value returned by the controller code is not"
							  " a Python int/long as expected",
				(const char *)"PyComController::WriteOne()");
    	}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't write the Communication Channel value");

        err_list[1].origin = CORBA::string_dup("PyComController::WriteOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantWriteValue");
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
    Py_DECREF(res);
	
	return lres;
}

string &PyComController::WriteReadOne(long ind, string &istr, 
									  long write_len /*= -1*/, 
									  long max_read_len /*= -1*/)
{
	PyObject *res;

    res = PyObject_CallMethod(py_obj, (char*)"WriteReadOne", (char*)"(ls#ll)", 
    		ind, istr.c_str(), istr.size(), write_len, max_read_len);

    if (res != NULL)
    {
		if (PyString_Check(res) == 1)
		{
			read_buff.assign(PyString_AsString(res), PyString_Size(res));
		}
		else
		{
			Py_DECREF(res);
			Tango::Except::throw_exception(
					(const char *)"PyCtrl_BadType",
		            (const char *)"The value returned by the controller code is"
		            			  " not a Python string as expected",
		            (const char *)"PyComController::WriteReadOne()");
		}
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string tmp_str("Can't write the Communication Channel value");

        err_list[1].origin = CORBA::string_dup("PyComController::WriteReadOne");
        err_list[1].desc = CORBA::string_dup(tmp_str.c_str());
		err_list[1].reason = CORBA::string_dup("PyCtrl_CantWriteValue");
		err_list[1].severity = Tango::ERR;
		
		throw Tango::DevFailed(err_list); 
    }
    Py_DECREF(res);
    
	return read_buff;
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::PreStateAll
// 
// description : 	Call the Python controller "PreReadAll" method
//
//--------------------------------------------------------------------------

void PyComController::PreStateAll()
{
	if (pre_state_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStateAll", NULL);
		check_void_return(res,
				"Error reported from the controller PreStateAll method",
				"PreStateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::PreStateOne
// 
// description : 	Call the Python controller "PreStateOne" method
//
// argin : - axis: The motor index within the controller
//
//--------------------------------------------------------------------------

void PyComController::PreStateOne(long axis)
{
	if (pre_state_one == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"PreStateOne", (char*)"l", axis);
		check_void_return(res,
				"Error reported from the controller PreStateOne method",
				"PreStateOne");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::StateAll
// 
// description : 	Call the Python controller "StateAll" method
//
//--------------------------------------------------------------------------

void PyComController::StateAll()
{
	if (state_all == true)
	{
		PyObject *res;
    	res = PyObject_CallMethod(py_obj, (char*)"StateAll", NULL);
		check_void_return(res,
				"Error reported from the controller StateAll method",
				"StateAll");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::StateOne
// 
// description : 	Call the Python controller "StateOne" method
//
// argin : - ind: The CommunicationChannel index within the controller
//		   - ptr: Pointer to the structure used to returned a 
//                CommunicationChannel state
//
//--------------------------------------------------------------------------

void PyComController::StateOne(long ind, Controller::CtrlState *ptr)
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
					string me("The first data in the tuple returned by the "
							  "controller ");
					me = me + py_class_name;
					me = me + " StateOne method does not have the correct type"
							  " (Integer wanted)";
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
							string me("Can't decode the string in the tuple "
									  "returned by the controller ");
							me = me + py_class_name;
							throw_simple_exception(me.c_str(),"StateOne");						
						}
					}					
					else
					{
						Py_DECREF(res);
						string me("The second data in the tuple returned by"
								  " the controller ");
						me = me + py_class_name;
						me = me + " StateOne method does not have the correct "
								  "type (String wanted)";
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
				throw_simple_exception("Tuple returned by the controller "
						"StateOne method does not have the correct size",
						"StateOne");
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
		check_void_return(res,"Error reported from the controller "
				"StateOne method",
				"StateOne");
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::SetExtraAttributePar
// 
// description : 	Call the Python controller "SetExtraAttribute" method
//
// argin : - axis: The Communication channel	 index within the controller
//		   - par_name: The parameter name
//		   - par_value: The parameter value
//
//--------------------------------------------------------------------------

void PyComController::SetExtraAttributePar(long axis,string &par_name,Controller::CtrlData &par_value)
{
	if (set_extra_attribute == true)
	{
		AutoPythonCtrlLock lo;
		
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
   	 				(char*)"lsd", axis, par_name.c_str(), par_value.db_data);
			break;
			
			default:
   	 		res = PyObject_CallMethod(py_obj, (char*)"SetExtraAttributePar",
   	 				(char*)"lss", axis, par_name.c_str(), par_value.str_data.c_str());
			break;
		}
		check_void_return(res,"Error reported from the controller SetExtraAttributePar method","SetExtraAttributePar");
	}
	else
		throw_simple_exception("Method SetExtraAttributePar is not implemented in controller","SetExtraAttributePar");
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::GetExtraAttributePar
// 
// description : 	Call the Python controller "GetExtraAttributePar" method
//
// argin : - axis: The Communication channel index within the controller
//		   - par_name: The parameter name
//
//--------------------------------------------------------------------------

Controller::CtrlData PyComController::GetExtraAttributePar(long axis,string &extra_par_name)
{
    Controller::CtrlData dres;
 
 	if (get_extra_attribute == true)
 	{   
 		AutoPythonCtrlLock lo;
 		
		PyObject *res;
		
	    res = PyObject_CallMethod(py_obj, (char*)"GetExtraAttributePar", 
	    		(char*)"ls", axis,extra_par_name.c_str());
	    
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
// method : 		PyComController::SendToCtrl
// 
// description : 	Call the Python controller "SendToCtrl" method
//
// argin : - in_str: The input string to pass to the controller
//
//--------------------------------------------------------------------------

string PyComController::SendToCtrl(string &in_str)
{
	string returned_str("Default string: The controller returns nothing or an invalid data type");
	
	if (send_to_ctrl == true)
	{
		AutoPythonCtrlLock lo;
		
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
// method : 		PyComController::clear_method_flag
// 
// description : 	Clear all the boolean flags used to memorize which
//					pre-defined method are implemented in this controller
//
//--------------------------------------------------------------------------

void PyComController::clear_method_flag()
{
	read_one = false;
	read_line_one = false;
	write_one = false;
	write_read_one = false; 
	open_one = false;
	close_one = false;
}

//+-------------------------------------------------------------------------
//
// method : 		PyComController::check_existing_method
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

void PyComController::check_existing_methods(PyObject *obj)
{
	base_check_existing_methods(obj);
	
	PyObject *met;

	if ((met = PyObject_GetAttrString(obj,"ReadOne")) != NULL)
	{
		read_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"ReadLineOne")) != NULL)
	{
		read_line_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"WriteOne")) != NULL)
	{
		write_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"WriteReadOne")) != NULL)
	{
		write_read_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"OpenOne")) != NULL)
	{
		open_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"CloseOne")) != NULL)
	{
		close_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
}

extern "C"
{
	Controller *_create_PyCommunicationController(const char *inst,const char *cl_name,PyObject *mo,PyObject *prop)
	{
		return new PyComController(inst,cl_name,mo,prop);
	}
}
