#ifndef _DUMMYCOTI_H
#define _DUMMYCOTI_H

#include <pool/CoTiCtrl.h>
#include <tango.h>

extern "C"
{
	/**
	 * The create controller method for the UnixTimer controller.
	 */
	Controller *_create_UnixTimer(const char *,vector<Controller::Properties> &);
}

/**
 * This class has the purpose of providing a ghost device that could handle
 * signals for the UnixTimer controller inside a tango device server
 */
class UxTiClass: public Tango::DeviceClass
{
public:
	/// Constructor
	UxTiClass(string &na):Tango::DeviceClass(na) {}
	/// Destructor
	virtual ~UxTiClass() {}

	/// device factory
	virtual void device_factory(const Tango::DevVarStringArray *devlist_ptr) {}
	
	/// command factory
	virtual void command_factory() {}	
};


/**
 * This class has the purpose of providing a ghost device that could handle
 * signals for the UnixTimer controller inside a tango device server
 */
class UxTi: public Tango::Device_3Impl
{
public:
	/// Constructor
	UxTi(Tango::DeviceClass *cl,string &s):Tango::Device_3Impl(cl,s.c_str()) {}
	/// Destructor
	virtual ~UxTi() {}
	
	/// init device
	virtual void init_device() {}
	/// signal handler
	virtual void signal_handler(long);
};

/**
 * A Counter/Timer controller that can handle a single timer. The timing is 
 * handled by unix signals.
 */
class UnixTimer:public CoTiController
{
public:
	/// Constructor
	UnixTimer(const char *,vector<Controller::Properties> &);
	/// Destructor
	virtual ~UnixTimer();

	/// AddDevice
	virtual void AddDevice(long idx);
	/// DeleteDevice
	virtual void DeleteDevice(long idx);

	/// ReadOne
	virtual double ReadOne(long);
	/// AbortOne
	virtual void AbortOne(long idx);
	
	/// LoadOne
	virtual void LoadOne(long,double);
	/// StartOneCT
	virtual void StartOneCT(long);
	
	/// StateOne
	virtual void StateOne(long, Controller::CtrlState *);
					
protected:
	
	UxTiClass		*cl_ptr;		///< pointer to the Helper DeviceClass
	UxTi			*dev_ptr;		///< pointer to the Helper DeviceImpl
	
	long 			nb_sec;			///< number of seconds
	long 			nb_usec;		///< number of micro seconds
	
	long			nb_ms;			///< number of mili seconds
	long			stop_time_ms;	///< instance to stop (ms)
	long 			remain_ms;		///< remaining time (ms)
	struct timeval	last_read;		///< last read time
	
	int 			start_th;		///< start thread
};

#endif /* _DUMMYCOTI_H */
