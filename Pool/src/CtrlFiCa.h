#ifndef CTRLFICA_H_
#define CTRLFICA_H_

#include <ltdl.h>
#include <ExternalFiCa.h>
#include <PoolExtraAttrTypes.h>
#include <CppCtrlFile.h>
#include <PyCtrlFile.h>

namespace Pool_ns
{

class PropertyData;

/**
 * The CtrlFiCa class declaration
 */
class CtrlFiCa : public ExternalFiCa
{
public:

	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */
	CtrlFiCa(string &, string &, string &, string &, PoolClass *, Pool *);
	
	/// Destructor
	~CtrlFiCa();
	
	/**
	 * Returns the controller type
	 * @return the type of controller
	 */
	CtrlType get_obj_type() {return obj_type;}
	
	/**
	 * Returns a reference to the list of properties of the controller
	 * @return a list of property data
	 */
	vector<PropertyData *> &get_ctrl_prop_list() {return ctrl_props;}
	
	/**
	 * Returns the list of extra attributes
	 * @return the list of extra attributes
	 */
	vector<PoolExtraAttr> &get_extra_attributes() {return extra_attributes;}
	
	/**
	 * Returns the list of feature names
	 * @return the list of feature names
	 */
	vector<string> &get_features_name() {return features_name;}
	
	/**
	 * Get and check if the controller features are in the list of pre-defined
	 * ones.
	 * 
	 * @param[in] lang            The controller language
	 * @param[in] file            The controller file
	 * @param[in] ctrl_class_name The controller class name
	 */
	void check_features(Language ,CtrlFile *,string &);
	
	/**
	 * Builds the controller features 
	 * @param[in] lang            The controller language
	 * @param[in] file            The controller file
	 * @param[in] ctrl_class_name The controller class name
	 */
	void get_features(Language ,CtrlFile *,string &);
	
	/**
	 * Determines the number of available features
	 * @return the number of available features
	 */
	long get_avail_feat_nb() {return nb_avail_feat;}
	
	/**
	 * Returns the list of features (lower cased)
	 * @return the list of features (lower cased)
	 */
	vector<string> &get_avail_lower_name() {return avail_feat_lower;}
	
	/**
	 * Returns the list of features
	 * @return the list of features
	 */	
	const char **get_avail_features() {return avail_feat;}

	/**
	 * Gets the controller file
	 * The return is a CppCtrlFile but should be more generic (TODO: check why!
	 * I am doing this)
	 * 
	 * @return a pointer to the controller file
	 */
	inline CppCtrlFile *get_ctrl_file() 
	{ 
		return static_cast<CppCtrlFile *>(my_file); 
	}
	
	/**
	 * Gets the handle for the library
	 * @return the library handle
	 */
  	inline lt_dlhandle get_lib_ptr() 
  	{
  		return static_cast<CppCtrlFile *>(my_file)->get_lib_ptr();
  	}
  	
	/**
	 * Gets the handle for the python intermidiate library
	 * @return the library handle
	 */
  	inline lt_dlhandle get_py_inter_lib_ptr() 
  	{
  		return static_cast<PyCtrlFile *>(my_file)->get_py_inter_lib_ptr();
  	}
  	
  	/**
  	 * Gets the python module object
  	 * @return the Python modeule object
  	 */
  	inline PyObject *get_py_module() 
  	{
  		return static_cast<PyCtrlFile *>(my_file)->get_py_module();
  	}

  	/**
  	 * Gets the controller info
  	 * @param[out] info vector to be filled with the controller information
  	 */
	inline void get_info(vector<string> &info) 
	{ 
		get_ctrl_file()->get_info(name,info); 
	}
	
  	/**
  	 * Gets the controller property info
  	 * @param[out] info vector to be filled with the controller property 
  	 *                  information
  	 */
	inline void get_prop_info(vector<PropertyData*> &info) 
	{ 
		get_ctrl_file()->get_prop_info(name,info); 
	}

  	/**
  	 * Gets the maximum number of devices this controller supports
  	 * @return the maximum number of devices this controller supports
  	 */
	inline virtual long get_MaxDevice() 
	{ 
		return ctrl_props.front()->get_long(); 
	}
	
protected:
	
	/** Controller type */
	CtrlType							obj_type;
	
	/** list of extra attributes */
	vector<PoolExtraAttr>				extra_attributes;
	
	/** list of feature names */
	vector<string>						features_name;
	
	/** list of controller properties */
	vector<PropertyData *>				ctrl_props;
	
	/** available features */
	const char **						avail_feat;
	
	/** number of available features */
	long 								nb_avail_feat;
	
	/** list of available features in lower case */
	vector<string>						avail_feat_lower;
	
	/**
	 * init method to initialize the object's data
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */	
	void init(string &, string &, string &, string &, PoolClass *, Pool *);
	
	/**
	 * Check if the controller features are in the list of pre-defined ones
	 * @param[in] ctrl_class_name controller class name 
	 */
	void check_valid_features(string &);
	
	/**
	 * init internal data if controller has one of the special features.
	 * 
	 * @param[in] ctrl_class_name controller class name
	 */
	virtual void init_special_features(string &) {}
};

typedef vector<CtrlFiCa *>::iterator vct_ite;

/**
 * The MotCtrlFiCa class declaration
 */
class MotCtrlFiCa : public CtrlFiCa
{
public:
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */	
	MotCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~MotCtrlFiCa() {}
	
	/** 
	 * Determines if the controller has backlash feature.
	 * @return <code>true</code> if it has backlash feature or
	 *         <code>false</code> otherwise
	 */
	bool ctrl_has_backlash() {return ctrl_backlash;}
	
	/** 
	 * Determines if the controller has rounding feature.
	 * @return <code>true</code> if it has rounding feature or
	 *         <code>false</code> otherwise
	 */
	bool ctrl_want_rounding() {return ctrl_rounding;}
	
protected:
	
	/**
	 * init internal data if controller has one of the special features.
	 * Special features are:
	 * - Backlash
	 * - Rounding
	 * 
	 * @param[in] ctrl_class_name controller class name
	 */	
	virtual void init_special_features(string &);
	
	bool								ctrl_backlash; ///< backlash feature
	bool								ctrl_rounding; ///< rounding feature	
};

typedef vector<MotCtrlFiCa *>::iterator motfica_ite;

/**
 * The PseudoMotCtrlFiCa class declaration
 */
class PseudoMotCtrlFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */		
	PseudoMotCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~PseudoMotCtrlFiCa() {}
	
	/**
	 * Gets the number of pseudo motor roles
	 * @return the number of pseudo motor roles
	 */
	inline long get_pseudo_motor_role_nb()	
	{ 
		return (long)pseudo_motor_roles.size();
	}

	/**
	 * Gets the number of motor roles
	 * @return the number of motor roles
	 */
	inline long get_motor_role_nb() 
	{ 
		return (long)motor_roles.size(); 
	}
	
	/**
	 * Gets the pseudo motor role names
	 * @return the pseudo motor role names
	 */
	inline vector<string> &get_pseudo_motor_roles()	
	{ 
		return pseudo_motor_roles; 
	}

	/**
	 * Gets the motor role names
	 * @return the motor role names
	 */
	inline vector<string> &get_motor_roles()
	{
		return motor_roles; 
	}
	
  	/**
  	 * Gets the maximum number of devices this controller supports
  	 * @return the maximum number of devices this controller supports
  	 */
	inline virtual long get_MaxDevice() 
	{ 
		return get_pseudo_motor_role_nb(); 
	}
	
protected:
	
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */		
	virtual void init_special_features(string &);
	
	/** Pseudo motor role names */
	vector<string>						pseudo_motor_roles;
	
	/** Motor role names */
	vector<string>						motor_roles;
};

typedef vector<PseudoMotCtrlFiCa *>::iterator pseudomotfica_ite;

/**
 * The CoTiCtrlFiCa class declaration
 */
class CoTiCtrlFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */			
	CoTiCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~CoTiCtrlFiCa() {}
	
protected:
	
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */		
	virtual void init_special_features(string &);
};

//--------------------------------------------------------------
//
//			The ZeroDCtrlFiCa class declaration
//
//--------------------------------------------------------------

class ZeroDCtrlFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */			
	ZeroDCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~ZeroDCtrlFiCa() {}
	
protected:
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */	
	virtual void init_special_features(string &ctrl_class_name);
};

/** 
 * The PseudoCoTiCtrlFiCa class declaration
 */
class PseudoCoCtrlFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */			
	PseudoCoCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~PseudoCoCtrlFiCa() {}

	/**
	 * Gets the number of counter roles
	 * @return the number of counter roles
	 */
	inline long get_counter_role_nb()
	{ 
		return (long)counter_roles.size();
	}
	
	/**
	 * Gets the counter role names
	 * @return the counter role names
	 */
	inline vector<string> &get_counter_roles()			
	{
		return counter_roles;
	}
	
  	/**
  	 * Gets the maximum number of devices this controller supports
  	 * @return the maximum number of devices this controller supports
  	 */	
	inline virtual long get_MaxDevice() 
	{ 
		return get_counter_role_nb(); 
	}
	
protected:
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */	
	virtual void init_special_features(string &);
	
	/** counter role names */
	vector<string>						counter_roles;
};

/**
 * The ComCtrlFiCa class declaration
 */
class ComCtrlFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */		
	ComCtrlFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~ComCtrlFiCa() {}
	
protected:
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */	
	virtual void init_special_features(string &);
};


/**
 * The ConstraintFiCa class declaration
 */
class ConstraintFiCa : public CtrlFiCa
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param type            the controller file_class name (ctrl_class_name/inst name 
	 *                        in lowercase letter)
	 * @param f_name          the controller class file name (cpp lib or python 
	 *                        module)
	 * @param ctrl_class_name the controller class name (case dependant)
	 * @param ob_type         the type of controller.
	 * @param pool_class      pointer to the PoolClass object
	 * @param dev             pointer to the Pool device
	 */		
	ConstraintFiCa(string &,string &,string &,string &,PoolClass *,Pool *);
	
	/// Destructor
	~ConstraintFiCa() {}
	
  	/**
  	 * Gets the maximum number of devices this controller supports
  	 * @return the maximum number of devices this controller supports
  	 */		
	virtual long get_MaxDevice()	{ return 0; }
	
	/** 
	 * Gets the number of roles
	 * @return the number of roles
	 */
	long get_role_nb()				{ return (long)roles.size(); }
	
	/** 
	 * Gets the role names
	 * @return the role names
	 */
	vector<string> &get_roles()		{ return roles; }
	
protected:
	/**
	 * init internal data if controller has one of the special features.
	 * @param[in] ctrl_class_name controller class name
	 */	
	virtual void init_special_features(string &);
	
	/** role names */
	vector<string>						roles;
};


} // End of Pool_ns namespace

#endif /*CTRLFICA_H_*/
