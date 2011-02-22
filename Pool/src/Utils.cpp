//+=============================================================================
//
// file :         Utils.cpp
//
// description :
//
// project :      TANGO Device Server
//
// $Author: tcoutinho $
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//+=============================================================================
#include <math.h>
#include <Utils.h>

bool doubleEqual(double a, double b)
{
	return (fabs(a - b) <= EPSILON * fabs(a));
}
