#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "../lin_sys/lin_sys.h"
#include <fstream>
using namespace std;

class LINEAR_SYSTEM_GEN
{
protected:
    LINEAR_SYSTEM _lsys;
    int _n;
public:
    LINEAR_SYSTEM_GEN(LINEAR_SYSTEM lsys) : _lsys(lsys), _n(lsys.A().rows()) {}
    LINEAR_SYSTEM getSystem(double range) {
        LINEAR_SYSTEM rlsys(_n);
        srand (time(NULL));
        double f = (double)rand() / RAND_MAX;
        rlsys.A() = _lsys.A() + MT::Constant(_n,_n,2*range*f - range);
        return rlsys;
    }
    LINEAR_SYSTEM& lsys() { return _lsys; }
    int& n() { return _n; }
};

class DATASET
{
protected:
    MT _x;
public:
    DATASET(int dim, int num) : _x(MT::Zero(dim,num)) {}
    void writeToFile(string name) {
        ofstream output(name);
        output << _x;
        output.close();
    }
    void addSol(int i, VT sol) {
        _x.col(i) = sol;
    }
};

class LINEAR_PARRALEL_SOLVER
{
protected:
    LINEAR_SYSTEM_GEN _lsysg;
public:
    LINEAR_PARRALEL_SOLVER(LINEAR_SYSTEM_GEN lsysg) : _lsysg(lsysg) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_lsysg.n(), num);
        LU lu;
        for(int i=0; i<num; i++){
            LINEAR_SYSTEM rlsys = _lsysg.getSystem(range);
            lu.solve(rlsys);
            sol.addSol(i,rlsys.x());
        }
        return sol;
    }

};

#endif // MONTECARLO_H
