#ifndef _SimuZEROD_H
#define _SimuZEROD_H

#include <pool/ZeroDCtrl.h>
#include <tango.h>

extern "C"
{
	Controller *_create_SimuZeroDController(const char *,vector<Controller::Properties> &);
}

/**
 * The simulated 0D experiment channel controller class
 */
class SimuZeroDController:public ZeroDController
{
public:
	SimuZeroDController(const char *,vector<Controller::Properties> &);
	virtual ~SimuZeroDController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	virtual double ReadOne(long);	
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual Controller::CtrlData GetExtraAttributePar(long,string &);
	virtual void SetExtraAttributePar(long,string &,Controller::CtrlData &);
	
	virtual string SendToCtrl(string &);
					
protected:
	void bad_data_type(string &);
	
	string				DevName;
	Tango::DeviceProxy	*simu_ctrl;
	stringstream		convert_stream;
};

#endif /* _SimuZEROD_H */
