#ifndef MONTECARLO_ODE_H
#define MONTECARLO_ODE_H

#include<iostream>
#include<fstream>
#include<random>
#include<chrono>
#include<omp.h>
#include"../ode/ode.h"
using namespace std;

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class ODE_MC_SOLVER
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;

protected:
    ODE<TS,NP,NS,TP> _ode;
public:
    ODE_MC_SOLVER(ODE<TS,NP,NS,TP>& ode) : _ode(ode) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_ode.n(), num);
        EXPLICIT_EULER ee;
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            ODE rode = _ode;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = range * distribution(generator);
            rode.x() = _ode.x() + VTS::Constant(NS,1,f);
            rode.p() = _ode.p() + VTP::Constant(NP,1,f);
            ee.solve(rode);
            sol.addSol(i,rode.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_ODE_H
