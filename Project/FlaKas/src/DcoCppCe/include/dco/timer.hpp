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

#ifdef _WIN32
namespace dco {

class timer {
    __int64 PCFreq;
    __int64 start_time;

    double time;
    bool running;

public:
    static void sleep(double seconds) {
        DWORD millisecs = static_cast<DWORD>(seconds * 1000);
        Sleep(millisecs);
    };

    timer() : time(0) {
        LARGE_INTEGER li;
        if (!QueryPerformanceFrequency(&li))
            std::cout << "QueryPerformanceFrequency failed!\n";

        PCFreq = li.QuadPart;
        start();
    }

    void start() {
        running = true;
        time = 0;
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        start_time = li.QuadPart;
    }

    void restart() {
        start();
    }

    void resume() {
        running = true;
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        start_time = li.QuadPart;
    }

    void stop() {
        if(running) {
            running = false;
            LARGE_INTEGER li;
            QueryPerformanceCounter(&li);
            time += double(li.QuadPart - start_time) / PCFreq;
        }
    }

    double get() {
        return elapsed();
    }

    double elapsed() {
        if(running) {
            LARGE_INTEGER li;
            QueryPerformanceCounter(&li);
            return time + double(li.QuadPart - start_time) / PCFreq;
        } else {
            return time;
        }
    }

    //}

};

}

#else

namespace dco {

class timer {
private:
    double time;
    timespec start_time;
    bool running;

#ifndef __APPLE__
#ifndef __MACH__
    timespec diff(const timespec nstart_time, const timespec end) const {
        timespec temp;
        if ((end.tv_nsec - nstart_time.tv_nsec) < 0) {
            temp.tv_sec = end.tv_sec - nstart_time.tv_sec - 1;
            temp.tv_nsec = 1000000000 + end.tv_nsec - nstart_time.tv_nsec;
        } else {
            temp.tv_sec = end.tv_sec - nstart_time.tv_sec;
            temp.tv_nsec = end.tv_nsec - nstart_time.tv_nsec;
        }
        return temp;
    }
#endif
#endif

public:

    timer() {
        start();
    }

    static void sleep(const double seconds) {
        usleep(static_cast<useconds_t>(seconds * 1000 * 1000));
    }

    void start() {
#ifndef __APPLE__
#ifndef __MACH__
        time = 0.0;
        running = true;
        clock_gettime(0, &start_time);
#endif
#endif
    }

    void stop() {
#ifndef __APPLE__
#ifndef __MACH__
        if(running) {
            running = false;
            timespec gettime, difftime;

            clock_gettime(0, &gettime);
            difftime = diff(start_time, gettime);

            time += static_cast<double>(difftime.tv_sec) + static_cast<double>(difftime.tv_nsec) * 1e-9;
        }
#endif
#endif
    }

    void restart() {
        start();
    }

    void resume() {
        running = true;
        clock_gettime(0, &start_time);
    }

    double get() {
        return elapsed();
    }

    double elapsed() {
        if(running) {
            timespec gettime, difftime;

            clock_gettime(0, &gettime);
            difftime = diff(start_time, gettime);
            return time + static_cast<double>(difftime.tv_sec) + static_cast<double>(difftime.tv_nsec) * 1e-9;
        } else {
            return time;
        }
    }
};
}

#endif

