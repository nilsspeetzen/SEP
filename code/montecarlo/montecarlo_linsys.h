#ifndef MONTECARLO_LINSYS_H
#define MONTECARLO_LINSYS_H

#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include "../lin_sys_generic/lin_sys.h"
#include "dataset.h"
using namespace std;

class LINEAR_MC_SOLVER
{
protected:
    LINEAR_SYSTEM<> _lsys;
    int _n;
public:
    LINEAR_MC_SOLVER(LINEAR_SYSTEM<> lsys) : _lsys(lsys), _n(lsys.A().rows()) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_n, num);
        LU lsol;
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            LINEAR_SYSTEM rlsys = _lsys;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = range*distribution(generator);
            rlsys.A() = _lsys.A() + MT::Constant(_n,_n,f);
            lsol.solve(rlsys);
            sol.addSol(i,rlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_LINSYS_H
