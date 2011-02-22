#include <PoolFactory.h>
#include <ExternalFiCa.h>

namespace Pool_ns
{
	
DefaultDeviceFactory::DefaultDeviceFactory(): pDevProxyClass(NULL)
{
	PyObject *pModDict = PyImport_GetModuleDict();
	
	PyObject *pModName = PyString_FromString("PyTango");
	
	PyObject *pModule;
	bool new_module = false;
	if(PyDict_Contains(pModDict,pModName) == 1)
	{
		pModule = PyImport_AddModule("PyTango");
	}
	else
	{
		pModule = PyImport_ImportModule("PyTango");
		new_module = pModule != NULL;	
	}
	
	Py_DECREF(pModName);
	
	if(pModule != NULL)
	{
		// pDict is borrowed
		PyObject *pDict = PyModule_GetDict(pModule);
		
		pDevProxyClass = PyDict_GetItemString(pDict, "DeviceProxy");

	    if (pDevProxyClass == NULL || !PyCallable_Check(pDevProxyClass))
	    {
	    	Py_DECREF(pDevProxyClass);
	    	pDevProxyClass = NULL;
	    }
	}
	else
	{
		PyErr_Clear();
		TangoSys_OMemStream o;
		o << "Essencial python module 'PyTango' could not be imported.";
		o << "\nMake sure you have PyTango installed on your system."<< ends;
		Tango::Except::throw_exception((const char *)"Pool_CantImportPythonModule",o.str(),
			        	(const char *)"DefaultPoolFactory::DefaultPoolFactory()");
	}

}

DefaultDeviceFactory::~DefaultDeviceFactory()
{
	Py_XDECREF(pDevProxyClass);
	pDevProxyClass = NULL;
}

Tango::DeviceProxy *DefaultDeviceFactory::get_new_device_proxy(string &name)
{
	Tango::DeviceProxy *proxy = new Tango::DeviceProxy(name);
	proxy->set_transparency_reconnection(true);
	return proxy;
}

PyObject *DefaultDeviceFactory::get_new_py_device_proxy(string &name)
{
    // Create an instance of the class
	AutoPythonLock apl = AutoPythonLock();
	PyObject *param = Py_BuildValue("(s)",name.c_str());
	PyObject *pInstance = PyObject_CallObject(pDevProxyClass, param);
    Py_DECREF(param);
	return pInstance;
}

LoggingDeviceFactory::LoggingDeviceFactory(): DefaultDeviceFactory()
{
}

LoggingDeviceFactory::~LoggingDeviceFactory()
{
	Py_XDECREF(pDevProxyClass);
	pDevProxyClass = NULL;
}

Tango::DeviceProxy *LoggingDeviceFactory::get_new_device_proxy(string &name)
{
	Tango::DeviceProxy *proxy = new LoggingDeviceProxy(name);
	proxy->set_transparency_reconnection(true);
	return proxy;
}

PyObject *LoggingDeviceFactory::get_new_py_device_proxy(string &name)
{
	return DefaultDeviceFactory::get_new_py_device_proxy(name);
}

}
