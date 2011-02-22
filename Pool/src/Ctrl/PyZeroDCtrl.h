#ifndef _PYZERODCTRL_H
#define _PYZERODCTRL_H

#include <Python.h>
#include <PyCtrl.h>
#include <tango.h>
#include <pool/ZeroDCtrl.h>

extern "C"
{
	Controller *_create_PyZeroDExpChannelController(const char *,const char *,PyObject *,PyObject *);
}

/**
 * The Python 0D experiment channel controller base class
 */
class PyZeroDController:public PyController,public ZeroDController
{
public:
	PyZeroDController(const char *,const char *,PyObject *,PyObject *);
	~PyZeroDController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);
		
	virtual void PreReadAll();
	virtual void PreReadOne(long);
	virtual void ReadAll();
	virtual double ReadOne(long);
	
	virtual void PreStateAll();
	virtual void PreStateOne(long);
	virtual void StateAll();
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual std::string SendToCtrl(std::string &);
	
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	PyObject 	*mod;
	std::string	py_class_name;
	
	bool		pre_read_all;
	bool		pre_read_one;
	bool		read_all;
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif /* _PYZERODCTRL_H */
