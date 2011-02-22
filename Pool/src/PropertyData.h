#ifndef _PROPERTYDATA_H_
#define _PROPERTYDATA_H_

#include <Python.h>
#include <tango.h>
#include <pool/Ctrl.h>

namespace Pool_ns 
{

using namespace std;

/**
 * Class to store controller property data
 */
class PropertyData
{
protected:
	Tango::CmdArgType	type;
	string				type_str;

public:
	
	string		name;
	string		descr;
	bool		has_dft_value;
	bool    	dft_overwritten;
	
	PropertyData(string &, string &, string &);
	PropertyData(const PropertyData &);
	virtual ~PropertyData();
	
	Tango::CmdArgType	get_type() { return type; }
	string				&get_type_str() { return type_str; }
	bool				is_defined_in_db() { return found_in_db; }
	void				is_in_db() { found_in_db = true; }
	void				clear_db_flag() {found_in_db = false;}
	
	void				append_to_property_vec(vector<string> &);
	bool                is_simple_type();
	void				save_default_value();
	void				restore_default_value();

	void set_value(bool b) { bool_v = b; toString(b); }
	void set_value(long l) { long_v = l; toString(l); }
	void set_value(double d) { double_v = d; toString(d); }
	void set_value(string &s) { value_str = s; }
	void set_value(const char *s) { value_str = s; }
	void set_value(vector<bool> *v) { bool_arr_v = v; toString(*v); }
	void set_value(vector<long> *v) { long_arr_v = v; toString(*v); }
	void set_value(vector<double> *v) { double_arr_v = v; toString(*v); }
	void set_value(vector<string> *v) { str_arr_v = v; toString(*v); }
	
	void get(bool &b) { b = bool_v; }
	void get(long &l) { l = long_v; }
	void get(double &d) { d = double_v; }
	void get(string &s) { s = value_str; }
	void get(vector<bool>* &b) { b = bool_arr_v; }
	void get(vector<long>* &l) { l = long_arr_v; }
	void get(vector<double>* &d) { d = double_arr_v; }
	void get(vector<string>* &s) { s = str_arr_v; }
	
	bool get_bool() { return bool_v; }
	long get_long() { return long_v; }
	double get_double() { return double_v; }
	string get_string() { return value_str; }

	vector<bool>*   get_bool_v() { return bool_arr_v; }
	vector<long>*   get_long_v() { return long_arr_v; }
	vector<double>* get_double_v() { return double_arr_v; }
	vector<string>* get_string_v() { return str_arr_v; }

protected:
	
	union 
	{
		bool			bool_v;
		long			long_v;
		double			double_v;
		// for string is not needed since value_str can hold the value
		vector<bool>	*bool_arr_v;
		vector<long>	*long_arr_v;
		vector<double>	*double_arr_v;
		vector<string>	*str_arr_v;
	};
	string				value_str;	
	
	bool 				found_in_db;
	
	void				init();
	void				to_type_str(string &);
	Tango::CmdArgType	to_CmdArgType(string &);
	PropertyData		*save_default;
	
	template<class T> void toString(T v) { toString(v, value_str); }
	
	void toString(long l, string &s) { stringstream str; str << l; s = str.str(); }
	void toString(double d, string &s) { stringstream str; str << d; s = str.str(); }
	void toString(bool b, string &s) { s = b ? "true" : "false"; }
	void toString(string &i, string &s) { s = i; }
	template <class T> void toString(vector<T> &v, string &s) 
	{
		s.clear();
		for(unsigned long ul = 0; ul < v.size(); ++ul)
		{
			string t; 
			toString(v[ul],t);
			s += t;
			if(ul < v.size()-1) s += '\n';
		}
	}
};

/**
 * Class to store python specific controller property data
 */
class PyPropertyData: public PropertyData
{
public:
	PyPropertyData(string &prop_name, string &prop_type, string &prop_descr) : PropertyData(prop_name,prop_type,prop_descr) {}
	PyPropertyData(const PyPropertyData &cpy) : PropertyData(cpy) {}
	virtual ~PyPropertyData() {};
	
	PyObject	*to_py();
	void		from_py(PyObject *);
	
protected:
		
};

/**
 * Class to store C++ specific controller property data
 */
class CppPropertyData: public PropertyData
{
public:
	CppPropertyData(string &prop_name, string &prop_type, string &prop_descr) : PropertyData(prop_name,prop_type,prop_descr) {}
	CppPropertyData(const PyPropertyData &cpy) : PropertyData(cpy) {}
	virtual ~CppPropertyData() {};
	
	Controller::Properties 	to_cpp();
	void					from_cpp(string &);
	
protected:
		
};

}
#endif /*PROPERTYDATA_H_*/
