//=============================================================================
//
// file :        PoolExtraAttrTypes.h
//
// description : Include for the Pool device extra attributes data types.
//
// project :	Sardana device pool
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.1  2007/08/17 13:10:09  tcoutinho
// - pseudo motor restructure
// - pool base dev class restructure
// - initial commit for pseudo counters
//
// Revision 1.2  2007/01/30 15:57:42  etaurel
// - Add a missing data member init
// - Add code to manage case with different controller of the same Tango class
// with extra attribute of the same name but with different data type
//
// Revision 1.1  2007/01/16 14:22:25  etaurel
// - Initial revision
//
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//==============================================================================

#ifndef _POOLEXTRAATTRTYPES_H
#define _POOLEXTRAATTRTYPES_H


namespace Pool_ns
{

//==================================================================================
//
//  Pool extra attribute 
//	---------------------
//
//
// A pool device extra attribute is defined by three informations which are:
// 	1 - The extra attribute name
//	2 - The extra attribute data type
//	3 - The extra attribute write type
//
// Four data types are supported. They are:
// 	- BOOLEAN
//	- LONG
//	- DOUBLE
//	- STRING
//
// Two write types are supported for each of the data type. They are:
//	- READ
//	- READ_WRITE
//
//==================================================================================


enum ExtraAttrDataType
{
	BOOLEAN = 0,
	LONG,
	DOUBLE,
	STRING,
};

enum ExtraAttrDataWrite
{
	READ = 0,
	READ_WRITE
};

typedef struct PoolExtraAttr
{
	string				ExtraAttr_name;
	ExtraAttrDataType	ExtraAttr_data_type;
	ExtraAttrDataWrite	ExtraAttr_write_type;
};

}

#endif /* _POOLEXTRAAATTRTYPES_H */
