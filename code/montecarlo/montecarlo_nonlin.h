#ifndef MONTECARLO_NONLIN_H
#define MONTECARLO_NONLIN_H

#include "../nonlin_sys/nonlin_sys.h"
#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include <iostream>
using namespace std;

class NONLINEAR_SYSTEM_GEN
{
protected:
    NONLINEAR_SYSTEM _nlsys;
    int _n;
public:
    NONLINEAR_SYSTEM_GEN(NONLINEAR_SYSTEM nlsys) : _nlsys(nlsys), _n(nlsys.A().rows()) {}
    NONLINEAR_SYSTEM getSystem(double range) {
        NONLINEAR_SYSTEM nrlsys = _nlsys;
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        normal_distribution<double> distribution(0,range);
        double f = distribution(generator);
        f = range*f;
        //cout << f << endl;
        nrlsys.A() = _nlsys.A() + MT::Constant(_n,_n,f);
        return nrlsys;
    }
    NONLINEAR_SYSTEM& nlsys() { return _nlsys; }
    int& n() { return _n; }
};

class DATASET
{
protected:
    MT _x;
    int _dim;
    int _num;
public:
    DATASET() {}
    DATASET(int dim, int num) : _x(MT::Zero(dim,num)), _dim(dim), _num(num) {}
    DATASET(string name, int dim, int num) : _x(MT::Zero(dim,num)), _dim(dim), _num(num) {
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
    void addSol(int i, VT sol) {
        _x.col(i) = sol;
    }
    void displayRow(int i) {
        VT line = _x.row(i);
        double avg = line.mean();
        cout << "Mittelwert:" << avg << endl;
        //cout << "Alle Werte:" << endl << line << endl;
    }
    void displayRow(int i, VT x) {
        VT line = _x.row(i);
        VT diff = line-VT::Constant(line.size(), x(i));
        double maxDiff = diff.cwiseAbs().maxCoeff();
        cout << "Auswertung für Parameter " << i+1 << ":" << endl;
        cout << "Exakt:   " << x(i) << endl;
        cout << "Maxdiff: " << maxDiff << endl;
        //cout << "Alle Werte:" << endl << line << endl;
    }
};

class NONLINEAR_PARRALEL_SOLVER
{
protected:
    NONLINEAR_SYSTEM_GEN _nlsysg;
public:
    NONLINEAR_PARRALEL_SOLVER(NONLINEAR_SYSTEM_GEN nlsysg) : _nlsysg(nlsysg) {}
    DATASET psolve(int num, double range) {
        DATASET sol(_nlsysg.n(), num);
        LU lu;
        double start = omp_get_wtime();
        //#pragma omp parallel for
        for(int i=0; i<num; i++){
            NONLINEAR_SYSTEM nrlsys = _nlsysg.getSystem(range);
            lu.solve(nrlsys);
            //cout << "A11: " << rlsys.A()(1,1) << " X: " << rlsys.x()(1) << endl;
            sol.addSol(i,nrlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_NONLIN_H
