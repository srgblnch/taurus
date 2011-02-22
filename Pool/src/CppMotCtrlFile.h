#ifndef CPPMOTCTRLFILE_H_
#define CPPMOTCTRLFILE_H_

#include <CppCtrlFile.h>

namespace Pool_ns
{

/** 
 * The CppUndefCtrlFile class declaration 
 */
class CppUndefCtrlFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppUndefCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppUndefCtrlFile(string &,PoolClass *,Pool *);
	
	/// Destructor for CppUndefCtrlFile class
	~CppUndefCtrlFile();		
};


/** 
 * The CppMotCtrlFile class declaration 
 */
class CppMotCtrlFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppMotCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppMotCtrlFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppMotCtrlFile class from the CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppMotCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppMotCtrlFile();		
};


/** 
 * The CppPseudoMotCtrlFile class declaration 
 */
class CppPseudoMotCtrlFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppPseudoMotCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppPseudoMotCtrlFile(string &,PoolClass *,Pool *);
	
	/**
	 * "Copy" constructor for CppPseudoMotCtrlFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */	
	CppPseudoMotCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppPseudoMotCtrlFile();

protected:

	/**
	 * Obtains particular pseudo motor information of a controller class that 
	 * resides in the file represented by this class. 
	 * 
	 * @param ctrl_class_name [in] The controller class for which to retrieve 
	 *                             the information.
	 * @param info [out]           The controller information related to the
	 *                             given controller class. The particular
	 *                             information is appended to the vector.
	 */
	virtual void get_particular_info(string &, vector<string> &);

	/**
	 * Obtains particular pseudo motor information of a controller instance of
	 * a class that resides in the file represented by this class.
	 * 
	 * @param ctrl_class_name [in] The controller class for which to retrieve 
	 *                             the information.
	 * @param ctrl_inst_name [in]  The controller instance name.
	 * @param info [out]           The controller information related to the
	 *                             given controller class. The particular
	 *                             information is appended to the vector.
	 */	
	virtual void get_particular_info(string &ctrl_class_name, 
									 string &ctrl_inst_name, 
									 vector<string> &info)
	{ get_particular_info(ctrl_class_name,info); }
};


/**
 * The CppCoTiCtrlFile class declaration
 */
class CppCoTiCtrlFile : public CppCtrlFile
{
public:
	
	/**
	 * constructor for CppCoTiCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppCoTiCtrlFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppCoTiCtrlFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppCoTiCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppCoTiCtrlFile();		
};


/**
 * The CppZeroDCtrlFile class declaration
 */
class CppZeroDCtrlFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppZeroDCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppZeroDCtrlFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppZeroDCtrlFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppZeroDCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppZeroDCtrlFile();		
};


/**
 * The CppPseudoCoCtrlFile class declaration
 */
class CppPseudoCoCtrlFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppPseudoCoCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppPseudoCoCtrlFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppPseudoCoCtrlFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppPseudoCoCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppPseudoCoCtrlFile();	
	
protected:
	
	/**
	 * Obtains particular pseudo counter information of a controller class that 
	 * resides in the file represented by this class. 
	 * 
	 * @param ctrl_class_name [in] The controller class for which to retrieve 
	 *                             the information.
	 * @param info [out]           The controller information related to the
	 *                             given controller class. The particular
	 *                             information is appended to the vector.
	 */
	virtual void get_particular_info(string &, vector<string> &);

	/**
	 * Obtains particular pseudo counter information of a controller instance of
	 * a class that resides in the file represented by this class.
	 * 
	 * @param ctrl_class_name [in] The controller class for which to retrieve 
	 *                             the information.
	 * @param ctrl_inst_name [in]  The controller instance name.
	 * @param info [out]           The controller information related to the
	 *                             given controller class. The particular
	 *                             information is appended to the vector.

	 */	
	virtual void get_particular_info(string &ctrl_class_name, 
									 string &ctrl_inst_name, 
									 vector<string> &info)
	{ get_particular_info(ctrl_class_name,info); }
};


/**
 *  The CppComCtrlFile class declaration
 */
class CppComCtrlFile : public CppCtrlFile
{
public:
	
	/**
	 * constructor for CppComCtrlFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppComCtrlFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppComCtrlFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppComCtrlFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppComCtrlFile();		
};


/**
 * The CppConstraintFile class declaration
 */
class CppConstraintFile : public CppCtrlFile
{
public:

	/**
	 * constructor for CppConstraintFile class
	 * 
	 * @param f_name     The controller class file name (cpp lib or python module)
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppConstraintFile(string &,PoolClass *,Pool *);

	/**
	 * "Copy" constructor for CppConstraintFile class from the 
	 * CppUndefCtrlFile class
	 * 
	 * @param undef_ctrl The original CppUndefCtrlFile object reference 
	 * @param pool_class Pointer to the PoolClass object.
	 * @param dev        Pointer to the Pool object.
	 */
	CppConstraintFile(CppUndefCtrlFile &,PoolClass *,Pool *);
	
	/// Destructor
	~CppConstraintFile();		
};

} // End of Pool_ns namespace

#endif /*CPPMOTCTRLFILE_H_*/
