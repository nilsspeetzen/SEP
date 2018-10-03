#ifndef CASCADE_H
#define CASCADE_H
#include "flash.h"
#include <vector>
#include <QtCore>

#include "dco/dco.hpp"
#include "Eso/AlgebraicEsoView.hpp"
#include "Eso/Dco1Model.hpp"
#include "Eso/EqGroup.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "BlockDeco/AlgebraicEsoBlockSolver.hpp"

/**
 * @file cascade.h
 * @brief Enthält alle Module
 */

/**
 * @brief Enthält alle Flashes
 */

//TODO erstmal einen Flash einfügen können und den dann Lösen
//später dann eine Funktion zum darstellen

template<typename realType=double>
class cascade {
    typedef Matrix<realType,Dynamic,1> VT;
    typedef Matrix<realType,Dynamic,Dynamic> MT;
private:
    int _numS;
    Matrix<realType,Dynamic,7> _a;
    std::map <int,Flash<>> _flashes;
public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    cascade(int numSubstances, Matrix<realType,Dynamic,7> a) : _numS(numSubstances), _a(a) {}
    /**
     * @brief addFlash
     */
    void addFlash(int id) {
        // _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
        Flash<> f(_numS, _a);
        f.T() = 273;
        f.F() = 100;
        f.Vin() = 0;
        f.Vout() = 50;
        f.Lin() = 0;
        f.Lout() = 50;
        f.pg() = 1000;
        for(int i=0; i<_numS; i++) {
            f.xini(i) = 0.5;
            f.yini(i) = 0.5;
            f.xi(i) = 0.5;
            f.yi(i) = 0.5;
            f.ki(i) = 0.5;
            f.pi(i) = 1000;
        }
        _flashes.insert(std::pair<int, Flash<>>(id,f));
    }
    /**
     * @brief getFlash
     * @param id
     * @return
     */
    Flash<>& getFlash(int id) {
        return _flashes[id]; //vielleicht noch besser machen
    }
    void deleteFlash(int id) {
        _flashes.erase(id);
    }
    void connectFlashes(int id1, int id2, int phase) {
        if(phase == 1) {
            _flashes[id1].LoutM() = id2;
            _flashes[id2].LinM() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Liquid";
        } else if(phase == 2) {
            _flashes[id1].VoutM() = id2;
            _flashes[id2].VinM() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Vapor";
        }
    }
    /**
     * @brief konstruiert das NLS der Kaskade
     * @return NLS der Kaskade
     */
    VT f() {
        //erstmal nur ein Flash, Noch nicht fertig, irgendwie müssen noch mehr gleichungen hin, dafür ist connections
        VT r = VT::Zero(5+6*_numS);
        r.segment(0, 2+4*_numS) = _flashes[0].f();
        VT connections = VT::Zero(3+2*_numS);
        r.segment(2+4*_numS, 3+2*_numS) = connections;
        //TODO
        /*for(int i = 0; i < flashes.size(); i++) {

        }*/
    }
    /**
     * @brief dfdx
     * @return Abl. des NLS der Kaskade
     */
    MT dfdx() {

    }

};

template<typename RealType=double>
using TangentSingleFlash = Dco1Model<cascade,RealType>;

template<typename RealType=double>
class CASCADEESO
{
public:
  CASCADEESO() {}
  void solveOneFlash() {
      FirstOrderEso<TangentSingleFlash,double> eso;
      AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
      //Eigen::Matrix<double,Eigen::Dynamic,1> x(algEsoView.numVariables());
      //x.setOnes();
      AlgebraicEsoBlockSolver solver;
      //solver.solve(algEsoView,x);
  }
};

#endif // CASCADE_H
