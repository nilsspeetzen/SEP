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

#include <vector> 
using namespace std;

#include "dco.hpp"
namespace dco = dco_cpp_ce;

using namespace dco;

typedef double DCO_BASE_TYPE;
typedef gt1s<DCO_BASE_TYPE> DCO_MODE;
typedef DCO_MODE::type DCO_TYPE;

#include "f.hpp"

void driver(
    const vector<double>& xv, const vector<double>& xt, 
    vector<double>& yv, vector<double>& yt        
) {
  const int n=xv.size(), m=yv.size();
  vector<DCO_TYPE> x(n), y(m); 
  for (int i=0;i<n;i++) { value(x[i])=xv[i]; derivative(x[i])=xt[i]; }
  f(x,y); 
  for (int i=0;i<m;i++) { yv[i]=value(y[i]); yt[i]=derivative(y[i]); } 
}
