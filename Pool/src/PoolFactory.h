#ifndef _POOLFACTORY_H_
#define _POOLFACTORY_H_

#include <pool/DeviceFactory.h>

namespace Pool_ns
{

class DefaultDeviceFactory: public IDeviceFactory
{
protected:
	PyObject *pDevProxyClass;
	
public:
	DefaultDeviceFactory();	
	virtual ~DefaultDeviceFactory();

	virtual Tango::DeviceProxy *get_new_device_proxy(string &name);
	virtual PyObject *get_new_py_device_proxy(string &name);
};

class LoggingDeviceFactory: public DefaultDeviceFactory
{
public:
	LoggingDeviceFactory();	
	virtual ~LoggingDeviceFactory();

	virtual Tango::DeviceProxy *get_new_device_proxy(string &name);
	virtual PyObject *get_new_py_device_proxy(string &name);
};

class LoggingDeviceProxy: public Tango::DeviceProxy
{
public:
	LoggingDeviceProxy(string &name, CORBA::ORB *orb=NULL): DeviceProxy(name,orb) {}
	LoggingDeviceProxy(const char *name, CORBA::ORB *orb=NULL): DeviceProxy(name,orb) {}
	
};


}
#endif /*_POOLFACTORY_H_*/
