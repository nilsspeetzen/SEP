#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "../lin_sys/lin_sys.h"
#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
using namespace std;

class LINEAR_SYSTEM_GEN
{
protected:
    LINEAR_SYSTEM _lsys;
    int _n;
public:
    LINEAR_SYSTEM_GEN(LINEAR_SYSTEM lsys) : _lsys(lsys), _n(lsys.A().rows()) {}
    LINEAR_SYSTEM getSystem(double range) {
        LINEAR_SYSTEM rlsys = _lsys;
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        normal_distribution<double> distribution(0,range);
        double f = distribution(generator);
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
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            LINEAR_SYSTEM rlsys = _lsysg.getSystem(range);
            lu.solve(rlsys);
            sol.addSol(i,rlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_H
