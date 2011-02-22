#include <ZeroDCtrl.h>
#include <stdlib.h>

#include <limits.h>

ZeroDController::ZeroDController(const char *inst):Controller(inst)
{
	//cout << "[ZeroDController] class ctor" << endl;
	
	ZeroD_NaN = strtod("NAN",NULL);	
}

ZeroDController::~ZeroDController()
{
	//cout << "[ZeroDController] class dtor" << endl;
}

double ZeroDController::ReadOne(long)
{
	//cout << "[ZeroDController] Default ReadOne method returning NaN" << endl;
	return ZeroD_NaN;
}
