#include <pool/ComCtrl.h>
#include <sstream>

ComController::ComController(const char *inst):Controller(inst),meth_not_impl("Pool_meth_not_implemented") 
{
	//cout << "In the ComController class ctor" << endl;
	
	comCh_Overflow = LONG_MAX;
}

ComController::~ComController ( ) 
{ 
	//cout << "In the ComController class dtor" << endl;
}

string &ComController::ReadOne(long, long /*max_read_len = -1*/)
{
	//cout << "Default ReadOne method returning 0" << endl;
	read_buff.clear();
	return read_buff;
}

string &ComController::ReadLineOne(long)
{
	//cout << "Default ReadOne method returning 0" << endl;
	read_buff.clear();
	return read_buff;
}

long ComController::WriteOne(long, string &, long /*write_len = -1*/) 
{
	//cout << "Default WriteOne method returning 0" << endl;
	return 0;
}

string &ComController::WriteReadOne(long, string &ostr, long /*write_len = -1*/, long /*max_read_len = -1*/)
{
	//cout << "Default WriteReadOne method returning 0" << endl;
	read_buff.clear();
	return read_buff;	
}

Controller::CtrlData ComController::GetPar(long,string &)
{
	//cout << "Default GetPar method returning NaN" << endl;
	Controller::CtrlData cd;
	cd.lo_data = comCh_Overflow;
	cd.data_type = Controller::LONG;
	return cd;
}
