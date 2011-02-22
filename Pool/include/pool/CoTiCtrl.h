#ifndef COTICTRL_H_
#define COTICTRL_H_

#include <pool/Ctrl.h>

/**
 * The counter/timer controller base class
 */
class CoTiController:public Controller
{
public:
	CoTiController(const char *);
	virtual ~CoTiController();
	
//
// Methods to read counter(s) value
//

	virtual void PreReadAll() {}
	virtual void PreReadOne(long) {}
	virtual void ReadAll() {}
	virtual double ReadOne(long);
	
//
// Methods to set counter(s) value
//

	virtual void PreLoadAll() {}
	virtual bool PreLoadOne(long,double) {return true;}
	virtual void LoadOne(long,double) {}
	virtual void LoadAll() {}

//
// Methods to start counting
//

	virtual void PreStartAllCT() {}
	virtual bool PreStartOneCT(long) {return true;}
	virtual void StartOneCT(long) {}
	virtual void StartAllCT() {}
	
//
// Method to abort counting
//

	virtual void AbortOne(long) {}
	
protected:
	double			mot_NaN;
	std::string		meth_not_impl;
};

		
#endif /*COTICTRL_H_*/
