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

#include "dco/global_includes.hpp"
#include "dco/operations.hpp"
#include "dco/basetype.hpp"
#include "dco/ts_data.hpp"

namespace dco {
template <class T>
  class gt1s {
public:
  typedef T derivative_t;
  
  typedef internal::ts_data<derivative_t> _data_t;
  typedef internal::basetype<T, _data_t> type;
  
  typedef T value_t;
  typedef void tape_t;
  typedef void local_gradient_t;
  typedef void local_gradient_with_activity_t;
  typedef void external_adjoint_object_t;
  typedef external_adjoint_object_t efo_t;
  static const bool is_dco_type = true;
  static const bool is_adjoint_type = false;
  static const bool is_tangent_type = true;
};


template <class VALUE_T>
  class gvalue {
public:
  typedef VALUE_T value_t;
  typedef void derivative_t;
  typedef void tape_t;
  typedef void local_gradient_t;
  typedef void local_gradient_with_activity_t;
  typedef void external_adjoint_object_t;
  typedef external_adjoint_object_t efo_t;
  static const bool is_dco_type = false;
  static const bool is_adjoint_type = false;
  static const bool is_tangent_type = false;
};

template <typename T>
  struct mode : public gvalue<T> { };

template <typename T>
  struct mode<internal::basetype<T, typename gt1s<T>::_data_t> >  : public gt1s<T> {};

}

