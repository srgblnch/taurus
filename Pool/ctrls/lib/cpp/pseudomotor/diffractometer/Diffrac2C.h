#ifndef DIFFRAC2C_H_
#define DIFFRAC2C_H_

#include <vector>
#include <pool/PseudoMotCtrl.h>
#include <Diffrac.h>
#include <hkl/diffractometerfactory.h>

#define SET_REFLEC_WRONG_PARAMS		"Usage: setor <index> <tth> <th> <h> <k> <l>"

extern "C"
{
	Controller *_create_Diffrac2C(const char *,std::vector<Controller::Properties> &);
}

class Diffrac2C : public Diffrac
{
protected:
	
	hkl::twoC::vertical::Geometry &getGeometry();
	
	virtual hkl::Diffractometer* createDiffractometer();
	virtual const char *setReflection(vector<string> &);
	virtual void setReflections(vector<string> &);
	
	virtual string calcA(vector<string> &);
	virtual string calcHKL(vector<string> &);

public:
	Diffrac2C(const char *,std::vector<Controller::Properties> &);
	virtual ~Diffrac2C();
	
	virtual double CalcPhysical(long,std::vector<double> &);
	virtual double CalcPseudo(long,std::vector<double> &);
};

#endif /*DIFFRAC2C_H_*/
