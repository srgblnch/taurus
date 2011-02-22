#ifndef _SERIALCOMCTRL_H_
#define _SERIALCOMCTRL_H_

#include <pool/ComCtrl.h>
#include <tango.h>

extern "C"
{
	Controller *_create_PyRemoteTangoSerialController(const char *,vector<Controller::Properties> &);
}
/**
 * A C++ communication controller class that talks to a Serial Line python 
 * device server 
 */
class RemotePyTangoSerialController:public ComController
{
protected:

	struct SerialLineData 
	{
		Tango::DeviceProxy	*proxy;
		bool				device_available;
		std::string			tango_device;
		/*
		Tango::DevShort		baud_rate;
		Tango::DevShort		data_bits;
		string				flow_control;
		Tango::DevShort		input_buffer;
		string				parity;
		string				port;
		Tango::DevShort		stop_bits;
		string				terminator;
		Tango::DevShort		time_out;
		*/
	};
	
	long max_device;
	
	std::map<long,SerialLineData*> sl_data; 
	
public:

	RemotePyTangoSerialController(const char *,std::vector<Controller::Properties> &);
	virtual ~RemotePyTangoSerialController();

	/**
	 * @param idx - communication channel id
	 */
	virtual void OpenOne(long);
	
	/**
	 * @param idx - communication channel id
	 */
	virtual void CloseOne(long);
	

	virtual void AddDevice(long);
	virtual void DeleteDevice(long);

	/**
	 * Atempts to read data into the given buffer. 
	 * The length of the data can be checked by inspecting the buffer.
	 * 
	 * @param idx - communication channel id
	 * @param max_read_len - max length of buff to read.
	 *                        0 empties the read_buff and returns 0.
	 *                       -1 (default value) to read all available data from the channel.
	 */
	virtual std::string &ReadOne(long, long max_read_len = -1); 
	
	/**
	 * Atempts to read data into the given buffer up to a new line character. 
	 * The length of the data can be checked by inspecting the buffer.
	 * 
	 * @param idx - communication channel id
	 */
	virtual std::string &ReadLineOne(long); 
		
	/**
	 * @param idx - communication channel id
	 * @param write_buff - buffer which contains data to be written
	 * @param write_len - length of buffer to write.
	 *                    0 will not write anything
	 *                    -1 (default value) will atempt to write the entire contents of the write_buff
	 * @return the length of the data actually written
	 */
	virtual long WriteOne(long, std::string &, long write_len = -1); 
	
	/**
	 * @param idx - communication channel id
	 * @param write_buff - buffer which contains data to be written
	 * @param write_len - length of buffer to write.
	 *                    0 will not write anything
	 *                    -1 will atempt to write the entire contents of the write_buff
	 * @param max_read_len - max length of buff to read.
	 *                        0 empties the read_buff and returns 0.
	 *                       -1 to read all available data from the channel.
	 * @return the length of the data actually written
	 */
	virtual std::string &WriteReadOne(long, std::string &, long write_len = -1, long max_read_len = -1); 
	
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	
	virtual std::string SendToCtrl(std::string &);
					
protected:
	void bad_data_type(std::string &);

};
#endif /*_SERIALCOMCTRL_H_*/
