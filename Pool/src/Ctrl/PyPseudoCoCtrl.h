#ifndef PYPSEUDOCOCTRL_H_
#define PYPSEUDOCOCTRL_H_

#include <Python.h>
#include <PyCtrl.h>
#include <tango.h>
#include <pool/PseudoCoCtrl.h>

extern "C"
{
	Controller *_create_PyPseudoCounterController(const char *,const char *,PyObject *,PyObject *);
}


#define CALC_METHOD					"calc"
#define GET_PSEUDO_CLASS_INFO		"get_pseudo_class_info"
#define COUNTER_ROLES_ATTR			"counter_roles"

/**
 * The Python pseudo counter controller base class
 */
class PyPseudoCounterController : public PyController, public PseudoCounterController
{
public:
	PyPseudoCounterController(const char *,const char *,PyObject *,PyObject *);
	~PyPseudoCounterController();

	virtual double Calc(std::vector<double> &);

	virtual std::string SendToCtrl(std::string &);
	
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	long get_counter_nb_from_py();
	
	PyObject 	*mod;
	std::string	py_class_name;

	bool		calc;	
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif /*PYPSEUDOCOCTRL_H_*/
