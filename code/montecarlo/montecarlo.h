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
        f = range*f;
        //cout << f << endl;
        rlsys.A() = _lsys.A() + MT::Constant(_n,_n,f);
        return rlsys;
    }
    LINEAR_SYSTEM& lsys() { return _lsys; }
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
        //#pragma omp parallel for
        for(int i=0; i<num; i++){
            LINEAR_SYSTEM rlsys = _lsysg.getSystem(range);
            lu.solve(rlsys);
            //cout << "A11: " << rlsys.A()(1,1) << " X: " << rlsys.x()(1) << endl;
            sol.addSol(i,rlsys.x());
        }
        double end = omp_get_wtime();
        cout << "Zeit fürs Lösen: " << end-start << endl;
        return sol;
    }

};

#endif // MONTECARLO_H
