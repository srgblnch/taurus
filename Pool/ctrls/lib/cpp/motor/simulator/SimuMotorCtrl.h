#ifndef _MAXECTRL_H
#define _MAXECTRL_H

#include <pool/MotCtrl.h>
#include <tango.h>

extern "C"
{
	Controller *_create_SimuMotorController(const char *,vector<Controller::Properties> &);
}

/**
 * The motor controller class that talks to a SimuMotorCtrl device server
 */
class SimuMotorController:public MotorController
{
public:
	SimuMotorController(const char *,vector<Controller::Properties> &);
	virtual ~SimuMotorController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	virtual void PreStartAll();
	virtual void StartOne(long,double);
	virtual	void StartAll();
	virtual void AbortOne(long);

	virtual double ReadOne(long);
	
	virtual void DefinePosition(long,double);
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual Controller::CtrlData GetPar(long,string &);
	virtual void SetPar(long,string &,Controller::CtrlData &);
	
	virtual Controller::CtrlData GetExtraAttributePar(long,string &);
	virtual void SetExtraAttributePar(long,string &,Controller::CtrlData &);
	
	virtual string SendToCtrl(string &);
					
protected:
	void bad_data_type(string &);
	
	string				DevName;
	Tango::DeviceProxy	*simu_ctrl;
	stringstream		convert_stream;
	
	vector<double> 		wanted_mot_pos;
	vector<long>		wanted_mot;
	
	long				ctr_11;
	bool				mot_10_fault;
	
	double				home_acc;
};

#endif /* _MAXECTRL_H */
