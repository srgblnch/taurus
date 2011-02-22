#ifndef _COMCONTROLLER_H
#define _COMCONTROLLER_H

#include <pool/Ctrl.h>

/**
  * class ComController
  */

class ComController : public Controller
{
public:
	std::string read_buff;

	ComController(const char *);
	virtual ~ComController();

	/**
	 * @param idx - communication channel id
	 */
	virtual void OpenOne(long) {}
	
	/**
	 * @param idx - communication channel id
	 */
	virtual void CloseOne(long) {}

	/**
	 * Atempts to read data into the given buffer. 
	 * The length of the data can be checked by inspecting the buffer.
	 * 
	 * @param idx - communication channel id
	 * @param max_read_len - max length of buff to read.
	 *                        0 empties the read_buff and returns 0.
	 *                       -1 to read all available data from the channel.
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
	
//
// Methods to Get/Set motor parameters
//

	virtual Controller::CtrlData GetPar(long,std::string &);
	virtual void SetPar(long,std::string &,Controller::CtrlData &) {}

protected:
	std::string		meth_not_impl;
	long			comCh_Overflow;
};

#endif // COMCHCONTROLLER_H
