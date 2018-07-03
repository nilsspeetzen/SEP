#ifndef MONTECARLO_LINSYS_H
#define MONTECARLO_LINSYS_H

#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include "../lin_sys_generic/lin_sys.h"
#include "dataset.h"
using namespace std;

template<typename TA=double, int N=Dynamic, typename Tb=TA, typename Tx=Tb>
class LINEAR_MC_SOLVER
{
    typedef Matrix<TA,N,N> MTA;
    typedef Matrix<Tb,N,1> VTb;
    typedef Matrix<Tx,N,1> VTx;

protected:
    LINEAR_SYSTEM<TA,N,Tb,Tx> _lsys;
    int _n;
public:
    LINEAR_MC_SOLVER(LINEAR_SYSTEM<TA,N,Tb,Tx> lsys) : _lsys(lsys), _n(lsys.A().rows()) {}
    DATASET psolve(int num, double range, bool A, bool b) {
        DATASET sol(_n, num);
        LU lsol;
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(int i=0; i<num; i++){
            LINEAR_SYSTEM<TA,N,Tb,Tx> rlsys = _lsys;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = range*distribution(generator);
            if(A) rlsys.A() = _lsys.A() + MT::Constant(_n,_n,f);
            if(b) rlsys.b() = _lsys.b() + MT::Constant(_n,1,f);
            lsol.solve(rlsys);
            sol.addSol(i,rlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_LINSYS_H
