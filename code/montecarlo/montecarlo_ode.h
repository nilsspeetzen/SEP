#ifndef MONTECARLO_ODE_H
#define MONTECARLO_ODE_H

#include<iostream>
#include<fstream>
#include<random>
#include<chrono>
#include<omp.h>
#include</home/oem/Desktop/PRAKSA_KOD/SEP-master/code/ode/ode.h>
using namespace std;

class ODE_GEN {
    protected:
        ODE _ode;
        int _n1;
        int _n2;
    public:
        ODE_GEN(ODE ode) : _ode(ode), _n1(size(ode.x())), _n2(size(ode.p()));
        ODE getSystem(double range) {
            ODE ode_new = _ode;
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine generator(seed);
            normal_distribution<double> distribution(0,range);
            double f = distribution(generator);
            f  = range*f;
            //cout << f << endl;
            ode_new.x() = _ode.x() + VTS::Constant(_n1,_n1,f);
            ode_new.p() = _ode.p() + VTP::Constant(_n2,_n2,f);
            return ode_new;
    }
    ODE& ode() { return _ode; }
    int& n1()  { return _n1;  }
    int& n2()  { return _n2;  }
};
    

    
class DATASET_ODE {
protected:
    VTS _x;
    int _dim;
    int _num;
public:
    DATASET_ODE() {}
    DATASET_ODE(int dim, int num) : _x(VTS::Zero(dim,num)), _dim(dim), _num(num) {}
    DATASET_ODE(string name, int dim, int num) : _x(VTS::Zero(dim,num)), _dim(dim), _num(num) {
        ifstream input(name);
        for(int i=0; i<dim;i++)
            for(int j=0; j<num;j++)
                input >> _x(i,j);
        input.close();
    }
    void writeToFile(string name) {
        ofstream output(name);
        output << _x;
        output.close();
    }
    void addSol(int i, VTP sol) {
        _x.col(i) = sol;
    }
    void displayRow(int i) {
        VTP line = _x.row(i);
        double avg = line.mean();
        cout << "Mittelwert:" << avg << endl;
        //cout << "Alle Werte:" << endl << line << endl;
    }
    void displayRow(int i, VT x) {
        VTP line = _x.row(i);
        VTP diff = line-VTP::Constant(line.size(), x(i));
        double maxDiff = diff.cwiseAbs().maxCoeff();
        cout << "Auswertung für Parameter " << i+1 << ":" << endl;
        cout << "Exakt:   " << x(i) << endl;
        cout << "Maxdiff: " << maxDiff << endl;
        //cout << "Alle Werte:" << endl << line << endl;
    }
};

class ODE_PARRALEL_SOLVER
{
protected:
    ODE_GEN _odegen;
public:
    ODE_PARRALEL_SOLVER(ODE_GEN odegen) : _odegen(ode) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_ode.n(), num);
        LU lu;
        double start = omp_get_wtime();
        //#pragma omp parallel for
        for(int i=0; i<num; i++){
            ODE ode= _ode.getSystem(range);
            lu.solve(ode);
            sol.addSol(i,ode.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_H

    
    
