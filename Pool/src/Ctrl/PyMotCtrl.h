#ifndef _PYMOTCTRL_H
#define _PYMOTCTRL_H

#include <Python.h>
#include <PyCtrl.h>
#include <tango.h>
#include <pool/MotCtrl.h>

extern "C"
{
	Controller *_create_PyMotorController(const char *,const char *,PyObject *,PyObject *);
}


class PyMotorController:public PyController, public MotorController
{
public:
	PyMotorController(const char *,const char *,PyObject *,PyObject *);
	virtual ~PyMotorController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	virtual void PreStartAll();
	virtual bool PreStartOne(long,double);
	virtual void StartOne(long,double);
	virtual void StartAll();
		
	virtual void PreReadAll();
	virtual void PreReadOne(long);
	virtual void ReadAll();
	virtual double ReadOne(long);

	virtual void PreStateAll();
	virtual void PreStateOne(long);
	virtual void StateAll();
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual void AbortOne(long);
	virtual void DefinePosition(long,double);
	virtual Controller::CtrlData GetPar(long,std::string &);
	virtual void SetPar(long,std::string &,Controller::CtrlData &);
	virtual std::string SendToCtrl(std::string &);
	
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	PyObject 	*mod;

	std::string		py_class_name;
	
	bool		pre_start_all;
	bool		pre_start_one;
	bool		start_one;
	bool		start_all;
	
	bool		pre_read_all;
	bool		pre_read_one;
	bool		read_all;
			
	bool		abort_one;
	bool		def_position;
	bool		set_par;
	bool		get_par;
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif /* _PYMOTCTRL_H */
