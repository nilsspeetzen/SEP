#ifndef MONTECARLO_ODE_H
#define MONTECARLO_ODE_H

#include<iostream>
#include<fstream>
#include<random>
#include<chrono>
#include<omp.h>
#include"../ode/ode.h"
#include"../ode/lotka_volterra.h"
using namespace std;

template<typename ODET=LOTKA_VOLTERRA<double>, typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class ODE_MC_SOLVER
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;

protected:
    ODET _ode;
public:
    ODE_MC_SOLVER(ODET ode) : _ode(ode) {}
    DATASET psolve(int num, double range, bool x, bool p) {
        DATASET sol(_ode.x().rows(), num);
        EXPLICIT_EULER<double,NP,NS> ee;
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            ODET rode = _ode;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = range * distribution(generator);
            if(x) rode.x() = _ode.x() + VTS::Constant(NS,1,f);
            if(p) rode.p() = _ode.p() + VTP::Constant(NP,1,f);
            ee.solve(rode);
            sol.addSol(i,rode.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_ODE_H
