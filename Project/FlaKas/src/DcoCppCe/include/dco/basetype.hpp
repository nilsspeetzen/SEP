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

#include "dco/basetype_decl.hpp"

namespace dco {
  namespace internal {

    template <typename T>
    struct passive_value_type_of {
      typedef T type;
    };

    template<class REAL, class DATA> struct basetype {
    private:
      REAL _value_;
      DATA _data_;

    public:
      typedef REAL value_t;
      typedef DATA data_t;
      typedef typename passive_value_type_of<REAL>::type passive_value_t;
      
      inline const REAL& _value() const { return _value_; }
      inline       REAL& _value()       { return _value_; }

      inline const DATA& _data() const { return _data_; }
      inline       DATA& _data()       { return _data_; }

      inline void _clear() { _data_.clear(); }

      inline basetype() : _value_(static_cast<REAL>(0.0)) {}

      template<class REAL_TMP, class DATA_TMP>
      inline basetype(const basetype<REAL_TMP, DATA_TMP> &val)  : _value_(val)  {}

      template<class D>
      inline basetype(const REAL &val, const D& d)  : _value_(val), _data_(d)  { }

      inline basetype(const passive_value_t &val)  : _value_(val)  {}

      inline basetype& operator=(const basetype &x) {
        this->_value_ = x._value_;
        this->_data_ = x._data_;
        return *this;
      }

      inline basetype& operator=(const passive_value_t& rhs)  {
        this->_value_ = rhs;
        this->_data_.clear();
        return *this;
      }

      template<class REAL_TMP, class DATA_TMP>
      inline basetype& operator=(const basetype<REAL_TMP, DATA_TMP>& rhs)  {
        this->_value_ = rhs;
        this->_data_.clear();
        return *this;
      }

      template<class DATA_TMP> inline
      basetype& operator+=(const basetype<REAL,DATA_TMP> &x){
	*this = *this + x; return *this;
      }
      inline basetype& operator+=(const REAL &x) {
	this->_value_ += x; return *this;
      }

      template<class DATA_TMP> inline
      basetype& operator-=(const basetype<REAL,DATA_TMP> &x){
	*this = *this - x; return *this;
      }
      inline basetype& operator-=(const REAL &x) {
	this->_value_ -= x; return *this;
      }

      template<class DATA_TMP> inline
      basetype& operator*=(const basetype<REAL,DATA_TMP> &x){
	*this = *this * x; return *this;
      }
      inline basetype& operator*=(const REAL &x) {
	*this = *this * x; return *this;
      }

      template<class DATA_TMP> inline
      basetype& operator/=(const basetype<REAL,DATA_TMP> &x){
	*this = *this / x; return *this;
      }
      inline basetype& operator/=(const REAL &x) {
	*this = *this / x; return *this;
      }

      inline basetype &operator++() { ++this->_value_; return *this; }
      inline basetype &operator--() { --this->_value_; return *this; }
      inline basetype operator++(int) {
	basetype ret(*this);
        ++this->_value_; return ret;
      }
      inline basetype operator--(int) {
        basetype ret(*this);
        --this->_value_; return ret;
      }
    };

    template<typename REAL, typename DATA>
    struct passive_value_type_of<basetype<REAL, DATA> > {
      typedef typename passive_value_type_of<REAL>::type type;
    };
    
#define UNARY_FCT(FCT, NAME)						\
    template <typename REAL, typename DATA>				\
    basetype<REAL, DATA> FCT(const basetype<REAL, DATA>& x) {		\
      REAL v(dco::operations::dco_##NAME<REAL>::eval(x)); \
      return basetype<REAL, DATA>(v, derivative(x)*dco::operations::dco_##NAME<REAL>::calc_partial(v, x)); \
    }
    
    UNARY_FCT(operator -, minus)
    UNARY_FCT(operator +, plus)
    UNARY_FCT(sin, sin)
    UNARY_FCT(cos, cos)
    UNARY_FCT(tan, tan)
    UNARY_FCT(cosh, cosh)
    UNARY_FCT(sinh, sinh)
    UNARY_FCT(asin, asin)
    UNARY_FCT(acos, acos)
    UNARY_FCT(exp, exp)
    UNARY_FCT(atan, atan)
    UNARY_FCT(tanh, tanh)
    UNARY_FCT(sqrt, sqrt)
    UNARY_FCT(log, log)
#ifndef _MSC_VER
    UNARY_FCT(erf, erf)
    UNARY_FCT(erfc, erfc)
    UNARY_FCT(expm1, expm1)
    UNARY_FCT(asinh, asinh)
    UNARY_FCT(acosh, acosh)
    UNARY_FCT(atanh, atanh)
    UNARY_FCT(log1p, log1p)
    UNARY_FCT(log10, log10)
#endif
    UNARY_FCT(fabs, fabs)
    UNARY_FCT(abs, fabs)
#undef UNARY_FCT


      template <typename T> struct identity {
        typedef T type;
      };
      
#define BIN_FCT(OP, NAME)						\
    template <typename REAL, typename DATA>				\
    basetype<REAL, DATA> OP(const basetype<REAL, DATA> &x1, const basetype<REAL, DATA> &x2)  { \
      REAL v(dco::operations::dco_##NAME<REAL>::eval(x1, x2)); \
      return basetype<REAL, DATA>					\
	(v,								\
	 derivative(x1)*dco::operations::dco_##NAME<REAL>::calc_partial1(v, x1, x2) + \
	 derivative(x2)*dco::operations::dco_##NAME<REAL>::calc_partial2(v, x1, x2) \
	 );								\
    }									\
    template <typename REAL, typename DATA>				\
    basetype<REAL, DATA> OP(const basetype<REAL, DATA> &x1, const typename identity<REAL>::type &x2)  { \
      REAL v(dco::operations::dco_##NAME<REAL>::eval(x1, x2)); \
      return basetype<REAL, DATA>					\
	(v,								\
	 derivative(x1)*dco::operations::dco_##NAME<REAL>::calc_partial1(v, x1, x2) \
	 );								\
    }									\
    template <typename REAL, typename DATA>				\
    basetype<REAL, DATA> OP(const typename identity<REAL>::type &x1, const basetype<REAL, DATA> &x2)  { \
      REAL v(dco::operations::dco_##NAME<REAL>::eval(x1, x2)); \
      return basetype<REAL, DATA>					\
	(v,								\
	 derivative(x2)*dco::operations::dco_##NAME<REAL>::calc_partial2(v, x1, x2)); \
    }									
    
    BIN_FCT(operator+, add)
    BIN_FCT(operator-, sub)
    BIN_FCT(operator*, mul)
    BIN_FCT(operator/, div)
    BIN_FCT(atan2, atan2)
    BIN_FCT(pow, pow)
    BIN_FCT(hypot, hypot)
    BIN_FCT(max, max)
    BIN_FCT(min, min)
#undef BIN_FCT

#define COMPARE(OP)							\
    template <typename REAL, typename DATA>				\
    static inline bool operator OP(const basetype<REAL, DATA>& x1,	\
				   const basetype<REAL, DATA>& x2) {	\
      return value(x1) OP value(x2);					\
    }									\
    template <typename REAL, typename DATA>				\
    static inline bool operator OP(const basetype<REAL, DATA>& x1,	\
				   const typename identity<REAL>::type& x2) { \
      return value(x1) OP value(x2);					\
    }									\
    template <typename REAL, typename DATA>				\
    static inline bool operator OP(const typename identity<REAL>::type& x1, \
				   const basetype<REAL, DATA>& x2) {	\
      return value(x1) OP value(x2);					\
    }									\
    
    COMPARE(==)
    COMPARE(!=)
    COMPARE(<)
    COMPARE(<=)
    COMPARE(>)
    COMPARE(>=)
#undef COMPARE

    template <typename REAL, typename DATA>
    inline std::istream &operator >> (std::istream &in,
				      const basetype<REAL, DATA>  &x) {
      in >> value(x);
      return in;
    }

    template <typename REAL, typename DATA>
    inline std::ostream& operator << (std::ostream& out,		
				      const basetype<REAL, DATA>  &x) {	
      out << value(x);							
      return out;							
    }
    

#ifdef _MSC_VER
    using std::ceil;
    template <typename REAL, typename DATA>
    inline REAL ceil(const basetype<REAL, DATA>& x) {     
      return ceil(value(x));				
    }
#else
    using std::ceil;				
    using std::floor;				
    using std::isfinite;			
    using std::isnan;				
    using std::isinf;
    
    template <typename REAL, typename DATA>
    static inline bool isnan(const basetype<REAL, DATA>& x) {      
      return isnan(value(x));				
    }                                                   
    template <typename REAL, typename DATA>
    static inline bool isinf(const basetype<REAL, DATA>& x) {      
      return isinf(value(x));				
    }                                                   
    template <typename REAL, typename DATA>
    static inline double ceil(const basetype<REAL, DATA>& x) {     
      return ceil(value(x));				
    }                                                   
    template <typename REAL, typename DATA>
    static inline double floor(const basetype<REAL, DATA>& x) {	
      return floor(value(x));				
    }                                                   
    template <typename REAL, typename DATA>
    static inline bool isfinite(const basetype<REAL, DATA>& x) {   
      return isfinite(value(x));			
    }
#endif
  }
}

