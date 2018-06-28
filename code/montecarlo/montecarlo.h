#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "../lin_sys/lin_sys.h"

class LINEAR_SYSTEM_GEN
{
protected:
    LINEAR_SYSTEM _lsys;
    int _n;
public:
    LINEAR_SYSTEM_GEN(LINEAR_SYSTEM lsys) : _lsys(lsys), _n(lsys.A().rows()) {}
    LINEAR_SYSTEM getSystem(int range) {
        LINEAR_SYSTEM rlsys(_n);
        rlsys.A() = rlsys.A() + MT::Constant(_n,_n,1);// * (rand() % range);
        return rlsys;
    }

    LINEAR_SYSTEM& lsys() { return _lsys; }
};

#endif // MONTECARLO_H
