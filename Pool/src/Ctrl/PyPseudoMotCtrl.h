#ifndef PYPSEUDOMOTCTRL_H_
#define PYPSEUDOMOTCTRL_H_

#include <Python.h>
#include <PyCtrl.h>
#include <tango.h>
#include <pool/PseudoMotCtrl.h>

extern "C"
{
	Controller *_create_PyPseudoMotorController(const char *,const char *,PyObject *,PyObject *);
}


#define CALC_PHYSICAL_METHOD			"calc_physical"
#define CALC_PSEUDO_METHOD				"calc_pseudo"
#define CALC_ALL_PHYSICAL_METHOD		"calc_all_physical"
#define CALC_ALL_PSEUDO_METHOD			"calc_all_pseudo"
#define GET_PSEUDO_CLASS_INFO			"get_pseudo_class_info"

#define MOTOR_ROLES_ATTR				"motor_roles"
#define GET_PSEUDO_MOTOR_ROLES			"get_pseudo_motor_roles"
#define GET_PSEUDO_MOTOR_NB				"get_pseudo_motor_nb"

/**
 * The Python motor controller base class
 */
class PyPseudoMotorController : public PyController, public PseudoMotorController
{
public:
	PyPseudoMotorController(const char *,const char *,PyObject *,PyObject *);
	~PyPseudoMotorController();

	virtual double CalcPhysical(long,std::vector<double> &);
	virtual double CalcPseudo(long,std::vector<double> &);
	
	virtual void CalcAllPhysical(std::vector<double> &,std::vector<double> &);
	virtual void CalcAllPseudo(std::vector<double> &,std::vector<double> &);

	virtual std::string SendToCtrl(std::string &);
	
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	long get_motor_nb_from_py();
	long get_pseudo_motor_nb_from_py();
	
	PyObject 	*mod;
	std::string	py_class_name;

	bool		calc_pseudo;
	bool		calc_physical;
	bool		calc_all_pseudo;
	bool		calc_all_physical;
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif /*PYPSEUDOMOTCTRL_H_*/
