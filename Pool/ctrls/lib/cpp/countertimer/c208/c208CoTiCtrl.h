#ifndef _C208COTI_H
#define _C208COTI_H

#include <CoTiCtrl.h>
#include <tango.h>

#define MAX_CHANNELS 12

extern "C"
{
	Controller *_create_c208CoTiController(const char *,vector<Controller::Properties> &);
}


class c208CoTiController:public CoTiController
{
public:
	c208CoTiController(const char *,vector<Controller::Properties> &);
	virtual ~c208CoTiController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	virtual void LoadOne(long idx, double new_value);
	virtual void StartOneCT(long idx);
	virtual void StartAllCT();
	virtual void ReadAll();
	virtual double ReadOne(long idx);
	virtual void AbortOne(long idx);
	
	virtual void StateAll();
	virtual void StateOne(long, Controller::CtrlState *);
	
	/*	virtual Controller::CtrlData GetExtraAttributePar(long,string &);
		virtual void SetExtraAttributePar(long,string &,Controller::CtrlData &);    */
	
protected:
	void bad_data_type(string &);
	
	Tango::DeviceProxy *ctr_dev;
	Controller::CtrlState ctr_state;
	string DevName;
	long master_idx;
	int ghostchan, master_started, channels;
	vector<double> values;
};

#endif /* _C208COTI_H */
