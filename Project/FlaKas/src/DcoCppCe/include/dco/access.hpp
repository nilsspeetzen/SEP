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

  template <typename T> struct remove_const {
    typedef T type;
  };
  template <typename T> struct remove_const<const T> {
    typedef T type;
  };

  template <typename T> struct accessor_s {
    typedef typename remove_const<T>::type return_t;
    typedef return_t type;
    static type _dummy_;
    static inline       return_t& _value(      type &v) { return v; }
    static inline const return_t& _value(const type &v) { return v; }
    static inline       return_t& _derivative(      type &v) { return _dummy_; }
    static inline const return_t& _derivative(const type &v) { return _dummy_; }
  };

  template <typename T> typename accessor_s<T>::type accessor_s<T>::_dummy_ = 0.0; 

  template<typename REAL, typename DATA>
  struct accessor_s<const internal::basetype<REAL,DATA> > {
    typedef REAL return_t;                        
    static inline       return_t& _value(      internal::basetype<REAL,DATA> &v) { return v._value(); } 
    static inline const return_t& _value(const internal::basetype<REAL,DATA> &v) { return v._value(); } 
    static inline       return_t& _derivative(      internal::basetype<REAL,DATA> &v) { return v._data()._derivative(); } 
    static inline const return_t& _derivative(const internal::basetype<REAL,DATA> &v) { return v._data()._derivative(); } 
  };                                                                    

  template <typename T>
  inline typename accessor_s<const T>::return_t &value(T &x) {
    return accessor_s<const T>::_value(x);
  }
  
  template <typename T>
  inline const typename accessor_s<const T>::return_t &value(const T &x) {
    return accessor_s<const T>::_value(x);
  }

  template <typename T>
  inline typename accessor_s<const T>::return_t &derivative(T &x) {
    return accessor_s<const T>::_derivative(x);
  }
  
  template <typename T>
  inline const typename accessor_s<const T>::return_t &derivative(const T &x) {
    return accessor_s<const T>::_derivative(x);
  }

}

