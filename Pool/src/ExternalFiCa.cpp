static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         ExternalFiCa.cpp
//
// description :  C++ source for the Pool and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Pool are implemented in this file.
//
// project :      TANGO Device Server
//
// copyleft :     Alba synchrotron
//				  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
// 				  08193 Bellaterra, Barcelona
//                Spain
//
//-=============================================================================

#include <ExternalFiCa.h>
#include <ltdl.h>
#include <PoolClass.h>

namespace Pool_ns
{

//-----------------------------------------------------------------------------
//
//					The ExternalFiCa class
//
//-----------------------------------------------------------------------------

//+----------------------------------------------------------------------------
//
// method : 		ExternalFiCa::ExternalFiCa()
// 
// description : 	constructor for ExternalFiCa class
//
// in : - type : The generic file_class name (class_name/inst name in lowercase letter)
//		- pool_class : Pointer to the PoolClass object
//
//-----------------------------------------------------------------------------
ExternalFiCa::ExternalFiCa(string &type, PoolClass *pool_class,Pool *dev)
:Tango::LogAdapter(dev),po_class(pool_class),my_file(NULL),name(type),monitor(type.c_str(),this)
{
//	cout << "In the ExternalFiCa ctor" << endl;
}

//+----------------------------------------------------------------------------
//
// method : 		ExternalFiCa::ExternalFiCa()
// 
// description : 	constructor for ExternalFiCa class
//
// in : - type : The generic file_class name (class_name/inst name in lowercase letter)
//      - f_name : The class file name (cpp lib or python module)
//		- class_name : The class name (case dependant)
//		- pool_class : Pointer to the PoolClass object
//
//-----------------------------------------------------------------------------

ExternalFiCa::ExternalFiCa(string &type, string &f_name, string &class_name, PoolClass *pool_class,Pool *dev)
:Tango::LogAdapter(dev),po_class(pool_class),my_file(NULL),name(type),monitor(type.c_str(),this)
{
//	cout << "In the ExternalFiCa ctor" << endl;
}

//+----------------------------------------------------------------------------
//
// method : 		ExternalFiCa::~ExternalFiCa()
// 
// description : 	destructor for ExternalFiCa class
//
//-----------------------------------------------------------------------------

ExternalFiCa::~ExternalFiCa()
{
//	cout << "In the ExternalFiCa dtor" << endl;
}

Language ExternalFiCa::get_language()
{
	return my_file->get_language();
}

//+----------------------------------------------------------------------------
//
// method : 		ExternalFiCa::check_python()
// 
// description : 	check if a file name is defined within all the
//					directories defined in the PYTHONPATH env.
//					variable. It throws an exception if the file does
//					not exist
//
// in : - class_name : The Python class name
//		- f_name : The module file name
// 
//-----------------------------------------------------------------------------

void ExternalFiCa::check_python(string &class_name)
{
	static_cast<PyExternalFile *>(my_file)->check_py(class_name.c_str());
}

//-----------------------------------------------------------------------------
//
//					The PythonLock class methods
//
// Ctor, Dtor, Get and Release methods
//
//-----------------------------------------------------------------------------


PythonLock::PythonLock(int init_th_id,PyInterpreterState *inter):locking_ctr(0)
{
	int th_id = omni_thread::self()->id();

//	cout << "\tCreating Py lock (" << omni_thread::self()->id() << ")... ";
	
	PyEval_AcquireLock();
	if (th_id == init_th_id)
		tstate = PyThreadState_Get();
	else
		tstate = PyThreadState_New(inter);
	PyEval_ReleaseLock();
//	cout << "[DONE]" << endl;
}

PythonLock::~PythonLock()
{
//	cout << "\tThread (" << omni_thread::self()->id() << ") is deleting Py lock of other threads... ";
	
//
// If the thread calling this dtor is the thread which has already done the Py_Finalise, do nothing here
//

	int th_id = omni_thread::self()->id();
	
	PoolClass *pool_cl = PoolClass::instance();
	
	if (th_id != pool_cl->finalyse_python_th_id)
	{
		PyEval_AcquireLock();
		PyThreadState_Swap(NULL);
		PyThreadState_Clear(tstate);
		PyThreadState_Delete(tstate);
		PyEval_ReleaseLock();
	}
//	cout << "[DONE]" << endl;
}

void PythonLock::Get()
{
	int th_id = omni_thread::self()->id();
	cout5 << "\t\t[START] Get Py lock (" << th_id << "), ctr = " << locking_ctr << "... " << endl;

	if (locking_ctr == 0)
	{	
		PyEval_AcquireLock();
		PyThreadState_Swap(tstate);
		cout5 << "\t\t[DONE] Get Py lock (" << th_id << "), ctr = " << locking_ctr+1 << endl;
	}
	else
	{
//
// Do we really have the lock ? Lets check it!
//
		PyThreadState *curr_state = PyThreadState_GET();
		if(curr_state == NULL)
		{
//
// PyTango must have stolen our thread state (bad boy)! So we try to put it again
//
			cout5 << "\t\t\t[START] Thread State Stolen! Trying to put it again (" << th_id << ")..." << endl;
			
			PyThreadState_Swap(tstate);	
			
			cout5 << "\t\t\t[DONE] Putting stolen thread state (" << th_id << ")"<< endl;
		}
		cout5 << "\t\t[DONE] Get Py lock (" << th_id << "), ctr = " << locking_ctr+1 << " - already have lock" << endl;
	}
	locking_ctr++;
}

void PythonLock::Release()
{
	int th_id = omni_thread::self()->id();
	cout5 << "\t\t[START] Release Py lock (" << th_id << "), ctr = " << locking_ctr << "..." << endl;

	if (locking_ctr == 0)
	{
		cout5 << "\t\t[DONE] Release Py lock (" << th_id << "), ctr = " << locking_ctr << " - nothing done" << endl;
		return;
	}
	
	locking_ctr--;
	if (locking_ctr == 0)
	{		
		PyThreadState_Swap(NULL);
		PyEval_ReleaseLock();
	}
	cout5 << "\t\t[DONE] Release Py lock (" << th_id << "), ctr = " << locking_ctr << endl;
}
	
//-----------------------------------------------------------------------------
//
//					The PoolLock class
//
//-----------------------------------------------------------------------------

void PoolLock::get_monitor()
{
	cout5 << "\t[START] PoolLock::get_monitor()..." << endl;
	if (lock_wanted == true)		
		mon.get_monitor(); 
//
// For Python, we also need to take the Python GIL
// This is done using an instance of the PythonLock class
// We have one instance of PythonLock class per thread
// Create the instance if it is not already done for this thread
//

	if (my_external_fica->get_language() == PYTHON)
	{
		int th_id = omni_thread::self()->id();
		PoolClass *po_class = my_external_fica->get_pool_class();
		map<int,PythonLock *> *map_ptr = &(po_class->py_locks);
		map<int,PythonLock *>::iterator map_ite = map_ptr->find(th_id);

		if (map_ite == map_ptr->end())
		{
			pair<map<int,PythonLock *>::iterator,bool> status;
			PythonLock *lock = new PythonLock(
						po_class->init_python_th_id,
						po_class->py_inter);
			status = map_ptr->insert(make_pair(th_id, lock));
			if (status.second == false)
			{
				if (lock_wanted == true)
					mon.rel_monitor();
				TangoSys_OMemStream o;
				o << "Can't create the Python lock for thread " << th_id << ends;
				Tango::Except::throw_exception((const char *)"Pool_CantCreatePythonLock",o.str(),
        									   (const char *)"PoolLock::get_monitor()");	
			}
			map_ite = status.first;
		}
//cout << " ( getting python lock (" << th_id << ")...";cout.flush();
		map_ite->second->Get();
//cout << "[DONE] ) ";cout.flush();
	}
	cout5 << "\t[DONE] PoolLock::get_monitor()"<<endl;	
}

void PoolLock::rel_monitor()
{
	cout5 << "\t[START] PoolLock::rel_monitor()..."<<endl;

//
// For Python controller, it is also necessary to release
// the Python GIL.
//

	if (my_external_fica->get_language() == PYTHON)
	{
		int th_id = omni_thread::self()->id();
		map<int,PythonLock *> *map_ptr = &(my_external_fica->get_pool_class()->py_locks);
		map<int,PythonLock *>::iterator map_ite = map_ptr->find(th_id);
		if (map_ite == map_ptr->end())
		{
			if (lock_wanted == true)
				mon.rel_monitor();
				
			TangoSys_OMemStream o;
			o << "Can't find the Python lock for thread " << th_id << ends;
			Tango::Except::throw_exception((const char *)"Pool_CantFindPythonLock",o.str(),
    									   (const char *)"PoolLock::get_monitor()");
		}
//cout << " ( releasing python lock (" << th_id << ")...";cout.flush();
		map_ite->second->Release();	
//cout << "[DONE] ) ";cout.flush();
		}
	
	if (lock_wanted == true)
		mon.rel_monitor();
	cout5 << "\t[DONE] PoolLock::rel_monitor()"<<endl;	
}
	
//-----------------------------------------------------------------------------
//
//					The AutoPythonLock class
//
//-----------------------------------------------------------------------------

AutoPythonLock::AutoPythonLock()
{
	th_id = omni_thread::self()->id();
	cl_ptr = PoolClass::instance();
	map<int,PythonLock *> *map_ptr = &cl_ptr->py_locks;
	map<int,PythonLock *>::iterator map_ite = map_ptr->find(th_id);
	
	{
		omni_mutex_lock lo(cl_ptr->py_locks_mutex);
		map_ite = map_ptr->find(th_id);

		if (map_ite == map_ptr->end())
		{
			pair<map<int,PythonLock *>::iterator,bool> status;
			status = map_ptr->insert(make_pair(th_id, new PythonLock(cl_ptr->init_python_th_id, cl_ptr->py_inter)));
			if (status.second == false)
			{
			TangoSys_OMemStream o;
				o << "Can't create the Python lock for thread " << th_id << ends;
				Tango::Except::throw_exception((const char *)"Pool_CantCreatePythonLock",o.str(),
    									 	  (const char *)"AutoPythonLock::AutoPythonLock()");	
			}
			map_ite = status.first;
		}
		map_ite->second->Get();	
	}
}

AutoPythonLock::~AutoPythonLock()
{
	map<int,PythonLock *> *map_ptr = &cl_ptr->py_locks;
	map<int,PythonLock *>::iterator map_ite = map_ptr->find(th_id);
	{
		omni_mutex_lock lo(cl_ptr->py_locks_mutex);
		map_ite = map_ptr->find(th_id);
		map_ite->second->Release();	
	}
}

//-----------------------------------------------------------------------------
//
//					The AutoCleanPythonLock class
//
//-----------------------------------------------------------------------------

AutoCleanPythonLock::~AutoCleanPythonLock()
{
	int th_id = omni_thread::self()->id();
	PoolClass *cl_ptr = PoolClass::instance();
	map<int,PythonLock *> *map_ptr = &cl_ptr->py_locks;
	map<int,PythonLock *>::iterator map_ite;
	{
		omni_mutex_lock lo(cl_ptr->py_locks_mutex);
		map_ite = map_ptr->find(th_id);

		if (map_ite != map_ptr->end())
		{
			delete map_ite->second;
			map_ptr->erase(map_ite);
		}
	}
}
	
} // End of Pool_ns namespacce
