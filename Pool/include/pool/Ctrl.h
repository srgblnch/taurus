#ifndef _CTRL_H
#define _CTRL_H

#include <iostream>
#include <string>
#include <vector>

#include <tango.h>

#define		DEFAULT_DEFINITION_STR		"Pool_meth_not_implemented"

/**
 * The Controller base class
 */
class Controller
{
public:
	
	/** 
	 * Constructor
	 * 
	 * @param inst [in] the controller instance name.
	 */
	Controller(const char *inst): inst_name(inst) {}
	
	/// Destructor
	virtual ~Controller() {}
	
	/**
	 *	@name Management
	 *	Controller add/remove devices related methods
	 */
	//@{
	
	/**
	 * @brief adds (activates) a device in the controller given by the index.
	 * 
	 * @param idx the device index to be added (starts with 1).
	 */
	virtual void AddDevice(long idx) = 0;

	/**
	 * @brief removes a device in the controller given by the index.
	 * 
	 * @param idx the device index to be removed (starts with 1).
	 */
	virtual void DeleteDevice(long idx) = 0;
	//@}
	
	/**
	 * @brief Sends the given string to the controller. Default implementation
	 *        throws an exception. Must be overwritten by a non abstract 
	 *        controller class
	 * 
	 * @param the_str the string to be sent.
	 * 
	 * @return a string with the controller response.
	 */
	virtual std::string SendToCtrl(std::string &the_str)
	{
		std::string def_string("Compiler quiet");

		TangoSys_OMemStream o;
		o << "Method SendToCtrl() not re-defined in controller " << inst_name << ends;

		Tango::Except::throw_exception((const char *)"Pool_CantCreatePyController",o.str(),
		        						(const char *)"Ctrl::send_to_ctrl()");
		        						
		return def_string;
	}
	
	/**
	 *  @brief Gets the controller instance name.
	 *  
	 *  @return A reference to the string that contains the controller instance
	 *          name
	 */ 
	std::string &get_name() {return inst_name;}

	/**
	 *	@name State
	 *	Controller state related methods.
	 */
	//@{
	
	/**
	 * Controller state structure
	 */
	typedef struct CtrlState
	{
		long			state;       ///< the state
		std::string		status;      ///< the status
	};

	/**
	 * @brief PreStateAll. Default implementation does nothing.
	 */
	virtual void PreStateAll() {}

	/**
	 * @brief PreStateOne. Default implementation does nothing.
	 * 
	 * @param idx [in] device index (starts with 1).
	 */
	virtual void PreStateOne(long) {}
	
	/**
	 * @brief StateAll. Default implementation does nothing.
	 */	
	virtual void StateAll() {}
	
	/**
	 * @brief StateOne. Default implementation does nothing.
	 * 
	 * @param idx         [in] device index (starts with 1).
	 * @param ctrl_state [out] pointer to the state object that will contain the
	 *                         controller state.  
	 */
	virtual void StateOne(long, CtrlState *) {}
	//@}
	

	/**
	 *	@name Extra Attributes
	 *	Extra attributes related methods.
	 */
	//@{
	
	/**
	 * Extra attribute Data type. Self explanatory.
	 */
	enum CtrlDataType
	{
		BOOLEAN = 0,
		LONG,
		DOUBLE,
		STRING,
	};
	
	/**
	 * Extra attribute data value structure
	 */
	typedef struct CtrlData
	{
		double			db_data;         ///< double value
		long			lo_data;         ///< long value          
		std::string		str_data;        ///< string value
		bool			bo_data;         ///< boolean value
		CtrlDataType	data_type;       ///< Data type
	};

	/** 
	 * @brief Sets the given extra attribute parameter with the given value on
	 *        the given device index.
	 * 
	 * @param idx       [in] device index (starts at 1)
	 * @param attr_name [in] extra attribute name
	 * @param ctrl_data [in] new value reference object
	 */
	virtual void SetExtraAttributePar(long,std::string &,Controller::CtrlData &) {}
	
	/** 
	 * @brief Get the given extra attribute parameter value for the given device
	 *        index.
	 * 
	 * @param idx       [in] device index (starts at 1)
	 * @param attr_name [in] extra attribute name
	 * 
	 * @return a CtrlData object containning the extra attribute value
	 */	
	virtual Controller::CtrlData GetExtraAttributePar(long,std::string &)
	{
		Controller::CtrlData cd;
		cd.str_data = DEFAULT_DEFINITION_STR;
		cd.data_type = Controller::STRING;
		return cd;	
	}

	/// Property information
	typedef struct PropInfo
	{
		const char *name;             ///< name
		const char *desc;             ///< description
		const char *type;             ///< type
		const char *default_value;    ///< default value
	};
	
	/// Extra attribute information
	typedef struct ExtraAttrInfo
	{
		const char *name;             ///< name
		const char *type;             ///< type
		const char *rw_type;          ///< read-write or read-only
	};
	
	/// Property data
	typedef struct PropData
	{
		std::vector<bool>			bool_prop;    ///< boolean data
		std::vector<long>			long_prop;    ///< long data
		std::vector<double>			double_prop;  ///< double data
		std::vector<std::string>	string_prop;  ///< string data
	};
	
	/// Property information
	typedef struct Properties
	{
		std::string	name;             ///< name
		PropData	value;            ///< value (default one)
	};
	
	
protected:
	std::string		inst_name;        ///< controller instance name
};

typedef Controller *(*Ctrl_creator_ptr)(const char *,vector<Controller::Properties> &);

#endif /* _CTRL_H */
