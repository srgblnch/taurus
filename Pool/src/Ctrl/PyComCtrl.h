#ifndef _PYCOMCHCONTROLLER_H
#define _PYCOMCHCONTROLLER_H

#include <Python.h>
#include <PyCtrl.h>
#include <pool/ComCtrl.h>
#include <tango.h>

extern "C"
{
	Controller *_create_PyCommunicationController(const char *,const char *,PyObject *,PyObject *);
}


/**
 * The Python communication controller base class
 */
class PyComController : public PyController, public ComController
{
public:
	PyComController(const char *,const char *,PyObject *,PyObject *);
	~PyComController();

	virtual void AddDevice(long idx);
	virtual void DeleteDevice(long idx);

	/**
	 * @param idx - communication channel id
	 */
	virtual void OpenOne(long);
	
	/**
	 * @param idx - communication channel id
	 */
	virtual void CloseOne(long);

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

	virtual void PreStateAll();
	virtual void PreStateOne(long);
	virtual void StateAll();
	virtual void StateOne(long, Controller::CtrlState *);

	virtual std::string SendToCtrl(std::string &);
	
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &);
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &);
				
protected:
	void clear_method_flag();
	void check_existing_methods(PyObject *);

	PyObject 	*mod;
	std::string	py_class_name;

	bool		read_one;
	bool		read_line_one;
	bool		write_one;
	bool		write_read_one;
	bool		open_one;
	bool		close_one;
};

typedef Controller *(*PyCtrl_creator_ptr)(const char *,const char *,PyObject *,PyObject *);

#endif // _PYCOMCHCONTROLLER_H
