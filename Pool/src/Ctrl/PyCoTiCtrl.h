#ifndef _PYCOTICTRL_H
#define _PYCOTICTRL_H

#include <Python.h>
#include <PyCtrl.h>
#include <tango.h>
#include <pool/CoTiCtrl.h>

extern "C"
{
	Controller *_create_PyCoTiController(const char *,const char *,PyObject *,PyObject *);
}

/**
 * The Python counter/timer controller base class
 */
class PyCoTiController:public PyController, public CoTiController
{
public:
	PyCoTiController(const char *,const char *,PyObject *,PyObject *);
	~PyCoTiController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);
		
	virtual void PreReadAll();
	virtual void PreReadOne(long);
	virtual void ReadAll();
	virtual double ReadOne(long);
	
	virtual void PreLoadAll();
	virtual bool PreLoadOne(long,double);
	virtual void LoadOne(long,double);
	virtual void LoadAll();

	virtual void PreStateAll();
	virtual void PreStateOne(long);
	virtual void StateAll();
	virtual void StateOne(long, Controller::CtrlState *);

	virtual void PreStartAllCT();
	virtual bool PreStartOneCT(long);
	virtual void StartOneCT(long);
	virtual void StartAllCT();
	
	virtual void AbortOne(long);
	virtual string SendToCtrl(string &);
	
	virtual void SetExtraAttributePar(long,string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	PyObject 	*mod;
	string		py_class_name;

	bool		pre_start_all_ct;
	bool		pre_start_one_ct;
	bool		start_one_ct;
	bool		start_all_ct;
	
	bool		pre_load_all;
	bool		pre_load_one;
	bool		load_one;
	bool		load_all;
	
	bool		pre_read_all;
	bool		pre_read_one;
	bool		read_all;
			
	bool		abort_one;
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif /* _PYCOTICTRL_H */
