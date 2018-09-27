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

namespace dco {
  namespace internal {

    template<class REAL>
    struct ts_data {
      typedef REAL derivative_t;
      REAL tlm;

      ts_data() : tlm(0) {}
      ts_data(const REAL& d) : tlm(d) {}

      ts_data &operator = (const ts_data &b) {
        tlm = b.tlm;
        return *this;
      }

      inline const REAL &_derivative() const {
        return tlm;
      }

      inline REAL &_derivative() {
        return tlm;
      }

      inline void clear() {
        _derivative() = 0;
      }

    };
  }
}
