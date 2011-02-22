#ifndef PSEUDOCTRL_H_
#define PSEUDOCTRL_H_

#include <vector>
#include <map>
#include <tango.h>
#include <pool/Ctrl.h>

class PseudoController:public Controller
{
public:
	PseudoController(const char *inst):
	Controller(inst),meth_not_impl("Pool_meth_not_implemented") 
	{ val_NaN = strtod("NAN",NULL);	}
	
	virtual ~PseudoController() {}

	virtual void AddDevice(long idx) {}
	virtual void DeleteDevice(long idx) {}
	
	virtual void PreStateAll()
	{
		assert(false);
		Tango::Except::throw_exception(
			(const char *)"Pool_WrongTangoRelease",
			(const char *)"Pool should never call PseudoController::PreStateAll()",
			(const char *)"PseudoController::PreStateAll");
	}

	virtual void PreStateOne(long)
	{
		assert(false);
		Tango::Except::throw_exception(
			(const char *)"Pool_WrongTangoRelease",
			(const char *)"Pool should never call PseudoController::PreStateOne()",
			(const char *)"PseudoController::PreStateOne");
	}

	virtual void StateAll()
	{
		assert(false);
		Tango::Except::throw_exception(
			(const char *)"Pool_WrongTangoRelease",
			(const char *)"Pool should never call PseudoController::StateAll()",
			(const char *)"PseudoController::StateAll");	
	}

	virtual void StateOne(long, CtrlState *) 
	{
		assert(false);
		Tango::Except::throw_exception(
			(const char *)"Pool_WrongTangoRelease",
			(const char *)"Pool should never call PseudoController::StateOne()",
			(const char *)"PseudoController::StateOne");
	}

	//
	// Methods to Get/Set pseudo motor parameters
	//
	virtual Controller::CtrlData GetPar(long,std::string &)
	{
		Controller::CtrlData cd;
		cd.db_data = val_NaN;
		cd.data_type = Controller::DOUBLE;
		return cd;
	}	
	virtual void SetPar(long,std::string &,Controller::CtrlData &) {}
	
protected:
	double			val_NaN;
	std::string		meth_not_impl;
};

#endif /*PSEUDOCTRL_H_*/
