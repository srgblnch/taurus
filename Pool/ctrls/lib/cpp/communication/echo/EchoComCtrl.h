#ifndef _DUMMYCOMCTRL_H_
#define _DUMMYCOMCTRL_H_

#include <pool/ComCtrl.h>

extern "C"
{
	Controller *_create_EchoCommunicationController(
			const char *,
			std::vector<Controller::Properties> &);
}

/**
 * Current channel state 
 */
typedef enum ChState { ChDisabled = 0, ChOpen, ChClosed };

/**
 * An 'echo like' communication controller class
 */
class EchoCommunicationController:public ComController
{
	
public:
	EchoCommunicationController(const char *,vector<Controller::Properties> &);
	virtual ~EchoCommunicationController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	/**
	 * Atempts to read data into the given buffer. 
	 * The length of the data can be checked by inspecting the buffer.
	 * 
	 * @param[in] idx communication channel id
	 * @param[in] max_read_len max length of buff to read.
	 *                         0 empties the read_buff and returns 0.
	 *                         -1 (default value) to read all available data
	 *                         from the channel.
	 */
	virtual string &ReadOne(long, long max_read_len = -1); 
	
	/**
	 * Atempts to read data into the given buffer up to a new line character. 
	 * The length of the data can be checked by inspecting the buffer.
	 * 
	 * @param[in] idx communication channel id
	 */
	virtual string &ReadLineOne(long); 
		
	/**
	 * @param idx - communication channel id
	 * @param write_buff - buffer which contains data to be written
	 * @param write_len - length of buffer to write.
	 *                    0 will not write anything
	 *                    -1 (default value) will atempt to write the entire contents of the write_buff
	 * @return the length of the data actually written
	 */
	virtual long WriteOne(long, string &, long write_len = -1); 
	
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
	virtual string &WriteReadOne(long, string &, long write_len = -1, long max_read_len = -1); 
	
	virtual void StateOne(long, Controller::CtrlState *);
	
	virtual Controller::CtrlData GetExtraAttributePar(long,string &);
	virtual void SetExtraAttributePar(long,string &,Controller::CtrlData &);
	
	virtual string SendToCtrl(string &);
					
protected:
	void bad_data_type(string &);
	
	long read_nb;
	long write_nb;

	vector<ChState> state;
	vector<string> last_write;
};

#endif /*_DUMMYCOMCHCTRL_H_*/
