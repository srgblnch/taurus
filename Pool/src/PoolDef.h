#ifndef _POOLDEF_H
#define _POOLDEF_H

#define		CTRL_MAXDEVICE_NOTDEF		-1

namespace Pool_ns
{

enum Language
{
	PYTHON = 0,
	CPP,
};

enum CtrlType
{
	UNDEF_CTRL = 0,
	PSEUDO_MOTOR_CTRL,
	MOTOR_CTRL,
	COTI_CTRL,
	ZEROD_CTRL,
	ONED_CTRL,
	TWOD_CTRL,
	PSEUDO_COUNTER_CTRL,
	COM_CTRL,
	CONSTRAINT_CTRL,
// the members below don't have a controller directly associated but they
// might be affected when a controller changes so they must be considered	
	MOTOR_GROUP_CTRL,
	MEASUREMENT_GROUP_CTRL,
	numCtrlType
};

enum ZeroDExpChannelCumType
{
	AVERAGE = 0,
	SUM,
	INTEGRAL,
	NO_COMPUTATION,
	ONE_SHOT,
	numCumType
};

enum AquisitionMode 
{
	aqNone = 0,
	aqTimer,
	aqMonitor,
	numAqMode
};

enum GrpEltType
{
	MOTOR = 0,
	GROUP,
	PSEUDO_MOTOR
};

enum MntGrpEltType
{
	ANY_CHANNEL = 0,
	CT_EXP_CHANNEL,
	ZEROD_EXP_CHANNEL,
	ONED_EXP_CHANNEL,
	TWOD_EXP_CHANNEL,
	PSEUDO_EXP_CHANNEL
};

}
#endif
