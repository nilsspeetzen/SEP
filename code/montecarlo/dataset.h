#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
using namespace std;

#include <Eigen/LU>
#include <Eigen/QR>
using namespace Eigen;

typedef Matrix<double,Dynamic,Dynamic> MT;
typedef Matrix<double,Dynamic,1> VT;

class DATASET
{
protected:
    MT _x;
    int _dim;
    int _num;
public:
    DATASET() {}
    DATASET(int dim, int num) : _x(MT::Zero(dim,num)), _dim(dim), _num(num) {}
    DATASET(string name) {
        ifstream input(name);
        input >> _dim >> _num;
        _x = MT::Zero(_dim,_num);
        for(int i=0; i<_dim;i++)
            for(int j=0; j<_num;j++)
                input >> _x(i,j);
        input.close();
    }
    void writeToFile(string name) {
        ofstream output(name);
        output << _dim << " " << _num << " " << endl;
        output << _x;
        output.close();
    }
    void addSol(int i, VT sol) {
        _x.col(i) = sol;
    }
    void displayRow(int i) {
        if(0 <= i && i <= _dim-1) {
            VT line = _x.row(i);
            double avg = line.mean();
            cout << "Mittelwert:" << avg << endl;
            //cout << "Alle Werte:" << endl << line << endl;
        } else {
            cout << "Nicht so viele Dimensionen";
        }
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

#endif // MONTECARLO_H
