#include <iostream>
#include <PyCtrl.h>
#include <pool/PoolAPI.h>

using namespace std;

//+-------------------------------------------------------------------------
//
// method : 		PyController::PyController
// 
// description : 	The Python Controller base class ctor
//
//--------------------------------------------------------------------------

PyController::PyController()
{
	//cout << "[PyController] class ctor" << endl;
	
	base_clear_method_flag();	
}

PyController::~PyController()
{
}

string PyController::get_name()
{
	if(py_obj != NULL)
	{
		PyObject *py_inst_name_str = PyObject_GetAttrString(py_obj, "inst_name");
		if(py_inst_name_str != NULL)
		{
			string ret(PyString_AsString(py_inst_name_str));
			Py_DECREF(py_inst_name_str);
			return ret;
		}
	}
	return "";
}

//+-------------------------------------------------------------------------
//
// method : 		PyController::check_void_return
// 
// description : 	Check if a Python method throws an exception for method
//					returning void data type. In case the method returned an
//					exception, translate it to a Tango exception
//
// argin : - res: The method return value
//		   - mesg: The Tango exception message
//		   - met: The name of the checked method
//
//--------------------------------------------------------------------------

void PyController::check_void_return(PyObject *res,const char *mesg,const char *met)
{
	if (res != NULL)
    {
    	Py_DECREF(res);
    }
    else
    {
 		PyObject *ex_exec,*ex_value,*ex_tb;
		Tango::DevErrorList err_list;
		err_list.length(2);
		
		PyErr_Fetch(&ex_exec,&ex_value,&ex_tb);
		this->Py_init_dev_error(ex_exec,ex_value,ex_tb,err_list);

		string met_str("PyController::");
		met_str += met;
		
		string reas_str("PyCtrl_Cant");
		reas_str += met; 
		
        err_list[1].origin = CORBA::string_dup(met_str.c_str());
        err_list[1].desc = CORBA::string_dup(mesg);
		err_list[1].reason = CORBA::string_dup(reas_str.c_str());
		err_list[1].severity = Tango::ERR;

		throw Tango::DevFailed(err_list); 
    }
}

//+-------------------------------------------------------------------------
//
// method : 		PyController::throw_simple_exception
// 
// description : 	Throws a tango exception
//
// argin : - mesg: The Tango exception message
//		   - met: The name of the checked method
//
//--------------------------------------------------------------------------

void PyController::throw_simple_exception(const char *mesg,const char *met)
{	
	Tango::DevErrorList err_list;
	err_list.length(1);	

	string met_str("PyController::");
	met_str += met;
	
	string reas_str("PyCtrl_Cant");
	reas_str += met; 
	
    err_list[0].origin = CORBA::string_dup(met_str.c_str());
    err_list[0].desc = CORBA::string_dup(mesg);
	err_list[0].reason = CORBA::string_dup(reas_str.c_str());
	err_list[0].severity = Tango::ERR;

	throw Tango::DevFailed(err_list);
}
	
	
	
//+-------------------------------------------------------------------------
//
// method : 		PyController::Py_init_dev_error
// 
// description : 	This method creates a Tango DevError
//					from a Python exception
//
// argin : - exec_ptr : The python exception type pbject
//	   	   - value_ptr : The ipython exception value object
//	  	   - tb_ptr ; The python exception traceback object
//	       - dev_err : Reference to the DevError list used by the
//					   Tango exception. This list is supposed to have
//					   at least a size of 1
//
//--------------------------------------------------------------------------


void PyController::Py_init_dev_error(PyObject *exec_ptr,PyObject *value_ptr,PyObject *tb_ptr,Tango::DevErrorList &dev_err)
{
//
// Send a default exception in case Python does not send us infornation
//

	if (value_ptr == NULL)
	{
		Py_XDECREF(exec_ptr);
		Py_XDECREF(value_ptr);
		Py_XDECREF(tb_ptr);

       	dev_err[0].origin = CORBA::string_dup("PyController::Py_init_dev_error");
        dev_err[0].desc = CORBA::string_dup("A badly formed exception has been received");
		dev_err[0].reason = CORBA::string_dup("Pool_BadPythonException");
		dev_err[0].severity = Tango::ERR;

		return;
	}

//
// Populate a one level DevFailed exception
//

	PyObject *tracebackModule = PyImport_ImportModule("traceback");
	if (tracebackModule != NULL)
    {
		PyObject *tbList, *emptyString, *strRetval;

//
// Format the traceback part of the Python exception
// and store it in the origin part of the Tango exception
//

		tbList = PyObject_CallMethod(tracebackModule, 
				(char *)"format_tb", 
				(char *)"O",
				tb_ptr == NULL ? Py_None : tb_ptr);
			
		emptyString = PyString_FromString("");
		strRetval = PyObject_CallMethod(emptyString, 
				(char*)"join", 
				(char*)"O", tbList);

		dev_err[0].origin = CORBA::string_dup(PyString_AsString(strRetval));

		Py_DECREF(tbList);
		Py_DECREF(emptyString);
		Py_DECREF(strRetval);

//
// Format the exec and value part of the Python exception
// and store it in the desc part of the Tango exception
//
		
		tbList = PyObject_CallMethod(tracebackModule, 
				(char*)"format_exception_only", 
				(char*)"OO",
				exec_ptr,value_ptr == NULL ? Py_None : value_ptr);
			
		emptyString = PyString_FromString("");
		strRetval = PyObject_CallMethod(emptyString, 
				(char*)"join", 
				(char*)"O", tbList);

		dev_err[0].desc = CORBA::string_dup(PyString_AsString(strRetval));

		Py_DECREF(tbList);
		Py_DECREF(emptyString);
		Py_DECREF(strRetval);
		Py_DECREF(tracebackModule);
		
		dev_err[0].reason = CORBA::string_dup("Pool_PythonControllerError");
		dev_err[0].severity = Tango::ERR;
    }
	else
	{
	
//
// Send a default exception because we can't format the
// different parts of the Python's one !
//

        dev_err[0].origin = CORBA::string_dup("PyController::Py_init_dev_error");
        dev_err[0].desc = CORBA::string_dup("Can't import Python traceback module. Can't extract info from Python exception");
		dev_err[0].reason = CORBA::string_dup("Pool_PythonControllerError");
		dev_err[0].severity = Tango::ERR;
	}
	    
	Py_XDECREF(exec_ptr);
	Py_XDECREF(value_ptr);
	Py_XDECREF(tb_ptr);			
}	

//+-------------------------------------------------------------------------
//
// method : 		PyController::base_clear_method
// 
// description : 	Clear all the boolean flags used to memorize which
//					pre-defined method are implemented in this controller
//
//--------------------------------------------------------------------------

void PyController::base_clear_method_flag()
{
	pre_state_all = false;
	pre_state_one = false;
	state_all = false;
	get_extra_attribute = false;
	set_extra_attribute = false;
	send_to_ctrl = false;
}

//+-------------------------------------------------------------------------
//
// method : 		PyController::base_check_existing_method
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

void PyController::base_check_existing_methods(PyObject *obj)
{
	PyObject *met;
		
	if ((met = PyObject_GetAttrString(obj,"PreStateAll")) != NULL)
	{
		pre_state_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"PreStateOne")) != NULL)
	{
		pre_state_one = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
			
	if ((met = PyObject_GetAttrString(obj,"StateAll")) != NULL)
	{
		state_all = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"GetExtraAttributePar")) != NULL)
	{
		get_extra_attribute = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();
		
	if ((met = PyObject_GetAttrString(obj,"SetExtraAttributePar")) != NULL)
	{
		set_extra_attribute = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();

	if ((met = PyObject_GetAttrString(obj,"SendToCtrl")) != NULL)
	{
		send_to_ctrl = true;
		Py_DECREF(met);
	}
	else
		PyErr_Clear();	
}

//+-------------------------------------------------------------------------
//
// method : 		PyController::PySetExtraAttributeBool
// 
// description : 	Call the controller SetExtraAttributePar method when the
//					input value is a boolean. There is no way to do this with
//					a simple call to the PyObject_CallMethod() function of the
//					Python C API. Boolean type is not supported in the data type
//
// argin : - obj : The python controller object
//		   - axis : The axis number within the controller
//		   - par_name : The extra attribute name
//		   - val : The new value
//
//--------------------------------------------------------------------------

PyObject *PyController::PySetExtraAttributeBool(PyObject *obj,long axis,string &par_name,bool val)
{
	PyObject *meth_name = PyString_FromString("SetExtraAttributePar");
	PyObject *py_par_name = PyString_FromString(par_name.c_str());
	PyObject *py_axis = PyInt_FromLong(axis);
	
	PyObject *ret_value;
	if (val == true)
		ret_value = PyObject_CallMethodObjArgs(obj,meth_name,py_axis,py_par_name,Py_True,NULL);
	else
		ret_value = PyObject_CallMethodObjArgs(obj,meth_name,py_axis,py_par_name,Py_False,NULL);
	
	Py_DECREF(meth_name);
	Py_DECREF(py_par_name);
	Py_DECREF(py_axis);
	
	return ret_value;
}

/*
 * Helper method to get a double value from a python object.
 * Returns false if the given python object is not a valid number.
 */
bool PyController::get_py_number(PyObject *py_ptr, double &number)
{
	if ((!py_ptr) || (PyNumber_Check(py_ptr) != 1))
		return false;
		
	if (PyFloat_Check(py_ptr) == true)
		number = PyFloat_AsDouble(py_ptr);
	else if(PyInt_Check(py_ptr) == true)
		number = (double)PyInt_AsLong(py_ptr);
	else if(PyLong_Check(py_ptr) == true)
		number = (double)PyLong_AsLong(py_ptr);
	return true;
}

bool PyController::get_py_bool(PyObject *py_ptr, bool &b)
{
	if ((!py_ptr) || (!PyBool_Check(py_ptr)))
		return false;
	
	b = py_ptr == Py_True;
	return true;
} 
