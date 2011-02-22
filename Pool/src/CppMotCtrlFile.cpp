static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         CppMotCtrlFile.cpp
//
// description :  C++ source for the Pool and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Pool are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.8  2007/08/23 10:32:43  tcoutinho
// - basic pseudo counter check
// - some fixes regarding pseudo motors
//
// Revision 1.7  2007/08/17 13:07:30  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.6  2007/07/17 11:41:57  tcoutinho
// replaced comunication with communication
//
// Revision 1.5  2007/07/02 14:46:36  tcoutinho
// first stable comunication channel commit
//
// Revision 1.4  2007/06/27 08:56:28  tcoutinho
// first commit for comuncation channels
//
// Revision 1.3  2007/01/26 08:36:47  etaurel
// - We now have a first release of ZeroDController
//
// Revision 1.2  2007/01/04 11:55:03  etaurel
// - Added the CounterTimer controller
//
// Revision 1.1  2006/11/07 14:57:08  etaurel
// - Now, the pool really supports different kind of controllers (cpp and py)
//
//
// copyleft :     Alba synchrotron
//				  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
// 				  08193 Bellaterra, Barcelona
//                Spain
//
//-=============================================================================

#include <CppMotCtrlFile.h>

namespace Pool_ns
{

// ----------------------------------------------------------------------------
// CPPUndefCtrlFile

CppUndefCtrlFile::CppUndefCtrlFile(string &f_name, PoolClass *pool_class, 
                                   Pool *dev)
:CppCtrlFile(f_name,"Undefined",pool_class,dev)
{
}

CppUndefCtrlFile::~CppUndefCtrlFile()
{}

// ----------------------------------------------------------------------------
// CppMotCtrlFile

CppMotCtrlFile::CppMotCtrlFile(string &f_name, PoolClass *pool_class, Pool *dev)
:CppCtrlFile(f_name,"Motor",pool_class,dev)
{}

CppMotCtrlFile::CppMotCtrlFile(CppUndefCtrlFile &undef_ctrl,
                               PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl,"Motor",pool_class,dev)
{}

CppMotCtrlFile::~CppMotCtrlFile()
{}

// ----------------------------------------------------------------------------
// CppPseudoMotCtrlFile

CppPseudoMotCtrlFile::CppPseudoMotCtrlFile(string &f_name,PoolClass *pool_class,
                                           Pool *dev)
:CppCtrlFile(f_name, "PseudoMotor", pool_class, dev)
{ 
	include_MaxDevice = false; 
}

CppPseudoMotCtrlFile::CppPseudoMotCtrlFile(CppUndefCtrlFile &undef_ctrl,
										   PoolClass *pool_class,Pool *dev)
:CppCtrlFile(undef_ctrl, "PseudoMotor", pool_class, dev)
{ 
	include_MaxDevice = false; 
}

CppPseudoMotCtrlFile::~CppPseudoMotCtrlFile()
{}

void CppPseudoMotCtrlFile::get_particular_info(string &ctrl_class, 
		                                       vector<string> &info)
{
//
// motor role count and description of each one
//
	vector<string> motor_roles;
	get_cpp_str_array(ctrl_class,"motor_roles",motor_roles);

	stringstream motor_role_nb;
	motor_role_nb << motor_roles.size();
	info.push_back(motor_role_nb.str());

	for(unsigned long ul = 0; ul < motor_roles.size(); ul++)
		info.push_back(motor_roles[ul]);	
	
//
// pseudo motor role count and description of each one
//	
	vector<string> pseudo_motor_roles;
	try
	{
		get_cpp_str_array(ctrl_class,"pseudo_motor_roles",pseudo_motor_roles);
	}
	catch(Tango::DevFailed &e)
	{
//
// Pseudo motor roles not defined: this means the pseudo motor controller will
// have only one pseudo motor role and the name of this role will be the class 
// name
//
		pseudo_motor_roles.push_back(ctrl_class);
	}

	stringstream pseudo_motor_role_nb;
	pseudo_motor_role_nb << pseudo_motor_roles.size();
	info.push_back(pseudo_motor_role_nb.str());

	for(unsigned long ul = 0; ul < pseudo_motor_roles.size(); ul++)
		info.push_back(pseudo_motor_roles[ul]);
}

// ----------------------------------------------------------------------------
// CppCoTiCtrlFile

CppCoTiCtrlFile::CppCoTiCtrlFile(string &f_name, PoolClass *pool_class,
                                 Pool *dev)
:CppCtrlFile(f_name, "CounterTimer", pool_class, dev)
{}

CppCoTiCtrlFile::CppCoTiCtrlFile(CppUndefCtrlFile &undef_ctrl,
								 PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl, "CounterTimer", pool_class, dev)
{}

CppCoTiCtrlFile::~CppCoTiCtrlFile()
{}

// ----------------------------------------------------------------------------
// CppZeroDCtrlFile

CppZeroDCtrlFile::CppZeroDCtrlFile(string &f_name,PoolClass *pool_class,
                                   Pool *dev)
:CppCtrlFile(f_name, "ZeroDExpChannel", pool_class, dev)
{}

CppZeroDCtrlFile::CppZeroDCtrlFile(CppUndefCtrlFile &undef_ctrl,
                                   PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl, "ZeroDExpChannel", pool_class, dev)
{}

CppZeroDCtrlFile::~CppZeroDCtrlFile()
{}

// ----------------------------------------------------------------------------
// CppPseudoCoCtrlFile

CppPseudoCoCtrlFile::CppPseudoCoCtrlFile(string &f_name,PoolClass *pool_class,
                                         Pool *dev)
:CppCtrlFile(f_name,"PseudoCounter",pool_class,dev)
{ 
	include_MaxDevice = false; 
}

CppPseudoCoCtrlFile::CppPseudoCoCtrlFile(CppUndefCtrlFile &undef_ctrl,
                                         PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl,"PseudoCounter",pool_class,dev)
{ 
	include_MaxDevice = false; 
}

CppPseudoCoCtrlFile::~CppPseudoCoCtrlFile()
{}

void CppPseudoCoCtrlFile::get_particular_info(string &ctrl_class,
											  vector<string> &info)
{
//
// counter role count and description of each one
//
	vector<string> counter_roles;
	get_cpp_str_array(ctrl_class,"counter_roles",counter_roles);

	stringstream counter_role_nb;
	counter_role_nb << counter_roles.size();
	info.push_back(counter_role_nb.str());

	for(unsigned long ul = 0; ul < counter_roles.size(); ul++)
		info.push_back(counter_roles[ul]);	
}

// ----------------------------------------------------------------------------
// CppComCtrlFile

CppComCtrlFile::CppComCtrlFile(string &f_name, PoolClass *pool_class, Pool *dev)
:CppCtrlFile(f_name,"Communication",pool_class,dev)
{}

CppComCtrlFile::CppComCtrlFile(CppUndefCtrlFile &undef_ctrl,
		                       PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl,"Communication",pool_class,dev)
{}

CppComCtrlFile::~CppComCtrlFile()
{}

// ----------------------------------------------------------------------------
// CppConstraintFile

CppConstraintFile::CppConstraintFile(string &f_name, PoolClass *pool_class,
                                     Pool *dev)
:CppCtrlFile(f_name,"Constraint",pool_class,dev)
{}

CppConstraintFile::CppConstraintFile(CppUndefCtrlFile &undef_ctrl, 
                                     PoolClass *pool_class, Pool *dev)
:CppCtrlFile(undef_ctrl,"Constraint",pool_class,dev)
{}

CppConstraintFile::~CppConstraintFile()
{}

} // End of Pool_ns namespacce
