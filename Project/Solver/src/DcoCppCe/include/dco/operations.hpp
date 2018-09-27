//
// dco/c++/communityedition - Algorithmic Differentiation by Operator Overloading in C++
// Copyright (C) 2014-2016 K. Leppkes, J. Lotz, U. Naumann <info@stce.rwth-aachen.de>
//
// This file is part of dco/c++/communityedition.
//
// dco/c++/communityedition is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dco/c++/communityedition is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dco/c++/communityedition.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include "dco/access.hpp"

namespace dco {
  namespace operations {
    // (void)s for supress compiler warning if PVAL1 is independent of _value, arg1, or arg2
#define BIN_OPERATOR_STRUCT(OPNAME, OPERATORX, PVAL1, PVAL2)	\
    template<typename REAL> struct dco_##OPNAME {	\
      template<typename T1, typename T2> static inline		\
      const REAL						\
      eval (const T1 &arg1, const T2 &arg2) {			\
	return value(arg1) OPERATORX value(arg2);		\
      }								\
								\
      template<typename T1, typename T2> static inline		\
      const REAL						\
      calc_partial1(const REAL &_value ,			\
		    const T1 &arg1,				\
		    const T2 &arg2) {				\
	(void) _value; (void)arg1; (void)arg2; return PVAL1;	\
      }								\
								\
      template<class T1,class T2>static inline			\
      const REAL						\
      calc_partial2(const REAL &_value,				\
		    const T1 &arg1,				\
		    const T2 &arg2) {				\
	(void) _value; (void)arg1; (void)arg2; return PVAL2;	\
      }								\
    };
    
    BIN_OPERATOR_STRUCT(add, +, 1.0, 1.0)
    BIN_OPERATOR_STRUCT(sub, -, 1.0, -1.0)
    BIN_OPERATOR_STRUCT(mul, *, value(arg2), value(arg1))
    BIN_OPERATOR_STRUCT(div, / , 1.0 / value(arg2), -_value / value(arg2))

#undef BIN_OPERATOR_STRUCT
    
#define UNARY_FUNCTION_STRUCT(FUU, DFUU, PREPROCESS, DEFINE)	\
    using DEFINE::FUU;						\
    template<typename REAL> struct dco_##FUU{		\
      template<typename T> static inline			\
      const REAL eval(const T &arg) {				\
	return FUU(value(arg));					\
      }								\
								\
      template<typename T> static inline			\
      const REAL calc_partial(const REAL &_value ,		\
			      const T &x) {			\
	(void)_value; return (DFUU);				\
      }								\
    };

    UNARY_FUNCTION_STRUCT(sin, cos(value(x)), , std)
    UNARY_FUNCTION_STRUCT(cos, -sin(value(x)), , std)
    UNARY_FUNCTION_STRUCT(tan, (1.0 + (tan(value(x))*tan(value(x)))), , std)
    UNARY_FUNCTION_STRUCT(cosh, sinh(value(x)), , std)
    UNARY_FUNCTION_STRUCT(sinh, cosh(value(x)), , std)
    UNARY_FUNCTION_STRUCT(asin, 1 / sqrt(1.0 - value(x)*value(x)), , std)
    UNARY_FUNCTION_STRUCT(acos, -1 / sqrt(1.0 - value(x)*value(x)), , std)

    UNARY_FUNCTION_STRUCT(exp, exp(value(x)), , std)
    UNARY_FUNCTION_STRUCT(atan, 1.0 / (1.0 + value(x)*value(x)), , std)

    // UNARY_FUNCTION_STRUCT(tanh, 1.0/cosh(value(x))/cosh(value(x)), ,std)
    UNARY_FUNCTION_STRUCT(tanh, 1.0 - tanh(value(x))*tanh(value(x)), , std)

    UNARY_FUNCTION_STRUCT(sqrt, 1.0 / (2.0 * sqrt(value(x))), , std)
    UNARY_FUNCTION_STRUCT(log, 1.0 / value(x), , std)

    // functions not available (or working properly) under Windows
#ifndef _MSC_VER
    UNARY_FUNCTION_STRUCT(erf, 2.0 / sqrt(3.14159265358979323846264338327950288) * exp(-value(x) * value(x)), , )
    UNARY_FUNCTION_STRUCT(erfc, -2.0 / sqrt(3.14159265358979323846264338327950288) * exp(-value(x) * value(x)), , )
    UNARY_FUNCTION_STRUCT(asinh, 1. / sqrt(1. + (value(x)*value(x))), , )
    UNARY_FUNCTION_STRUCT(acosh, 1. / sqrt((value(x)*value(x)) - 1.), , )
    UNARY_FUNCTION_STRUCT(expm1, exp(value(x)), , )
    UNARY_FUNCTION_STRUCT(atanh, 1. / (1. - (value(x)*value(x))), , )
    UNARY_FUNCTION_STRUCT(log1p, 1.0 / (value(x) + 1), , )
    UNARY_FUNCTION_STRUCT(log10, 1.0 / (value(x)*log(10)), , )
#endif
#undef UNARYFUNCTION

    template<typename REAL> struct dco_minus {
    template<typename T> static inline
    const REAL eval(const T &arg1) {
    return -value(arg1);
  }
    template<typename T> static inline
    const REAL calc_partial(const REAL &_value , const T &arg1 ) {
    (void)_value; (void) arg1; return -1.0;
  }
  };
    
    template<typename REAL> struct dco_plus {
    template<typename T> static inline
    const REAL eval(const T &arg1) {
    return value(arg1);
  }
    template<typename T> static inline
    const REAL calc_partial(const REAL &_value , const T &arg1 ) {
    (void)_value; (void)arg1; return 1.0;
  }
  };
    
    using ::fabs;
    template<typename REAL> struct dco_fabs {
    template<typename T> static inline
    const REAL eval(const T &arg1) {
    return fabs(value(arg1));
  }
    template<typename T> static inline
    const REAL calc_partial(const REAL &_value , const T &arg1 ) {
    (void) _value;
    if (value(arg1) < 0) return -1.0;
    else                 return  1.0;
  }
  };
    
#define BIN_FUNCTION_STRUCT(OPNAME, FUNCTION, PVAL1, PVAL2)	\
    template<typename REAL> struct dco_##OPNAME {	\
    template<typename T1, typename T2> static inline		\
    const REAL eval(const T1 &arg1, const T2 &arg2) {		\
    (void) arg1; (void) arg2;					\
    return FUNCTION(value(arg1), value(arg2));			\
  }								\
    template<typename T1,typename T2> static inline		\
    const REAL calc_partial1(const REAL _value ,		\
      const T1 &arg1, const T2 &arg2) {				\
    (void) _value; (void) arg1; (void) arg2;			\
    return PVAL1;						\
  }								\
    template<typename T1,typename T2> static inline		\
    const REAL calc_partial2(const REAL _value ,		\
      const T1 &arg1, const T2 &arg2) {				\
    (void) _value; (void) arg1; (void) arg2;			\
    return PVAL2;						\
    }								\
    };
    
    
    BIN_FUNCTION_STRUCT(atan2, atan2,
      value(arg2) / (value(arg2) * value(arg2) + value(arg1) * value(arg1)),
      -value(arg1) / (value(arg2) * value(arg2) + value(arg1) * value(arg1)))

    BIN_FUNCTION_STRUCT(hypot, hypot,
      value(arg1) / _value,
      value(arg2) / _value)

      using std::max;
      using std::min;
    BIN_FUNCTION_STRUCT(max, max,
			value(arg1)>value(arg2)?1.0:0.0,
			value(arg1)>value(arg2)?0.0:1.0)
    
    BIN_FUNCTION_STRUCT(min, min,
			value(arg1)<value(arg2)?1.0:0.0,
			value(arg1)<value(arg2)?0.0:1.0)

#undef BIN_FUNCTION_STRUCT

    template<typename REAL>
    struct dco_pow {
    template<typename T1, typename T2> static inline
    const REAL eval(const T1 &arg1, const T2 &arg2) {
    return pow(value(arg1), value(arg2));
  }
    template<typename T1, typename T2> static inline
    const REAL calc_partial1(const REAL _value , const T1 &arg1, const T2 &arg2) {
    (void)_value;
    return value(arg2) * pow(value(arg1), value(arg2) - 1.0);
  }
    template<typename T1, typename T2> static inline
    const REAL calc_partial2(const REAL _value , const T1 &arg1, const T2 &arg2) {
    (void) _value;
    if (arg1 <= 0)
      return 0;
    else
      return log(value(arg1)) * pow(value(arg1), value(arg2));
  }
  };
  }
  }

