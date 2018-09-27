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

#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <complex>
#include <string>
#include <stack>
#include <exception>
#include <stdexcept>
#include <string>
#include <bitset>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sstream>

#include <cassert>
#include <limits>
#include <iomanip>

#if !defined(_WIN32) & !defined(__APPLE__)
#include <sys/mman.h>
#include <sys/time.h>
#endif
#if !defined(_WIN32)
#include <unistd.h>
#endif

// for stce::timer
#if !defined(DCO_DOXYGEN) & defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#endif

#include <fcntl.h>
#include <typeinfo>


