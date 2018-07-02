#ifndef MONTECARLO_NONLIN_H
#define MONTECARLO_NONLIN_H

#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include <iostream>
#include "../nonlin_sys/nonlin_sys.h"
#include "../nonlin_sys/toy.h"

using namespace std;

template<typename NLS=TOY<double>, typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class NONLINEAR_MC_SOLVER
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;

protected:
    NLS _nlsys;
    int _n;
public:
    NONLINEAR_MC_SOLVER(NLS nlsys) : _nlsys(nlsys), _n(nlsys.x().rows()) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_n, num);
        LU<TS,NS> lsol;
        NEWTON<TS,NS,NP> nlsol(lsol);
        nlsol.eps() = 1e-7;
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            NLS nrlsys = _nlsys;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = range*distribution(generator);
            nrlsys.x() = _nlsys.x() + MT::Constant(_n,1,f);
            nlsol.solve(nrlsys);
            sol.addSol(i,nrlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        cout << "Anzahl Threads:  " << omp_get_max_threads() << endl;
        return sol;
    }

};

#endif // MONTECARLO_NONLIN_H
