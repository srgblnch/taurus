//=============================================================================
//
// file :        Utils.h
//
// description : Include for some utility classes for the Sardana project
//				 pool device
//
// project :	Sardana pool device
//
// $Author$
//
// copyleft :     CELLS/ALBA
//				  Edifici Ciències Nord. Mòdul C-3 central.
//  			  Campus Universitari de Bellaterra. Universitat Autònoma de Barcelona
//  			  08193 Bellaterra, Barcelona
//  			  Spain
//
//=============================================================================


#ifndef UTILS_H_
#define UTILS_H_

#include <utility>
#include <functional>
#include <iostream>

#define SAFE_DELETE(x) { if((x)!=NULL) { delete (x); (x)=NULL; } } 
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] (x); (x)=NULL; } }

#define EPSILON 1E-9

#define SAFE_CATCH(A,B)\
	catch (Tango::DevFailed &e) {throw;}\
	catch (...)\
	{\
		TangoSys_OMemStream o,p;\
		o << "Controller " << A << " has thrown an unknown exception" << ends;\
		p << "Pool::" << B << ends;\
		Tango::Except::throw_exception((const char *)"Pool_UnkExceptFromCtrl",\
										o.str(), p.str());\
	}

bool doubleEqual(double, double);

/*-----------------------------------------------------------------------------
Title :	Function adaptors for pair associative containers
Owner :	Adapted from Jay Kint

	Copyright (C) 2003, Jay Kint, All Rights Reserved
-------------------------------------------------------------------------------
Description:
Make using pair associative containers (map and hash_map) a little easier with
the algorithms.
*/

//-----------------------------------------------------------------------------
//							Types and Structures
//-----------------------------------------------------------------------------

/** call_class_with_data_t */
template <typename Functor>
struct call_class_with_data_t {

    Functor f_;
    typedef typename Functor::result_type Result;
    typedef typename Functor::argument_type Arg;

    explicit call_class_with_data_t( Functor f ) : f_(f) {}
    
    template <typename A, typename B>
    Result operator()( const std::pair<A,B>& v ) const {

        Arg value = v.second;     // to enforce the equality of the data with the argument type instantiated

        return f_( value );
    }
};

/** call_func_with_data_t */
template <typename Result, typename Arg>
struct call_func_with_data_t {

    typedef Result (*Func)(Arg);
    Func f_;

    explicit call_func_with_data_t( Func f ) : f_(f) {}
    
    template <typename A, typename B>
    Result operator()( const std::pair<A,B>& v ) const {

        Arg value = v.second;     // to enforce the equality of the data with the argument type instantiated

        return f_( value );
    }
};

/** call_class_with_data_t */
template <typename Functor>
call_class_with_data_t<Functor> with_data( Functor f  )
{
    return call_class_with_data_t<Functor>( f );
}

/** call_func_with_data_t */
template <typename Result, typename Arg>
call_func_with_data_t<Result, Arg> with_data( Result (*f)(Arg) )
{
    return call_func_with_data_t<Result, Arg>( f );
}

/** call_func_with_key_t */
template <typename Result, typename Arg>
struct call_func_with_key_t {

    typedef Result (*Func)(Arg);
    Func f_;

    explicit call_func_with_key_t( Func f ) : f_(f) {}
    
    template <typename A, typename B>
    Result operator()( const std::pair<A,B>& v ) const {

        Arg value = v.first;     // to enforce the equality of the data with the argument type instantiated

        return f_( value );
    }
};

/** call_class_with_key_t */
template <typename Functor>
struct call_class_with_key_t {

    Functor f_;
    typedef typename Functor::result_type Result;
    typedef typename Functor::argument_type Arg;

    explicit call_class_with_key_t( Functor f ) : f_(f) {}
    
    template <typename A, typename B>
    Result operator()( const std::pair<A,B>& v ) const {

        Arg value = v.first;     // to enforce the equality of the data with the argument type instantiated

        return f_( value );
    }
};

/** call_func_with_key_t */
template <typename Result, typename Arg>
call_func_with_key_t<Result, Arg> with_key( Result (*f)(Arg) )
{
    return call_func_with_key_t<Result, Arg>( f );
}

/**
 * call_class_with_key_t
 */
template <typename Functor>
call_class_with_key_t<Functor> with_key( Functor f  )
{
    return call_class_with_key_t<Functor>( f );
}

/**
 * Helper method to print elements of any std object that has a const_iterator
 */
template<class T>
inline void PRINT_ELEMENTS(const T& coll, const char* optcstr="")
{
	typename T::const_iterator pos;
	std::cout << optcstr;
	for(pos = coll.begin(); pos != coll.end() ; pos++)
		std::cout << *pos << ' ';
	std::cout << std::endl;
}


#endif		// _UTILS_H
