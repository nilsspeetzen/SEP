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

#include<iostream>
#include<vector>
using namespace std;

#include "driver.hpp"

int main() {
  const int m=2, n=4; cout.precision(15);
  vector<double> xv(n), xt(n), yv(m), yt(m);
  for (int i=0;i<n;i++) { xv[i]=1; xt[i]=1; }
  driver(xv,xt,yv,yt);
  for (int i=0;i<m;i++) 
    cout << "y[" << i << "]=" << yv[i] << endl;
  for (int i=0;i<m;i++) 
    cout << "y^{(1)}[" << i << "]=" << yt[i] << endl; 
  return 0;
}
