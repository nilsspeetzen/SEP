#ifndef MODULE_H
#define MODULE_H

#include "lin_sys.h"
#include <iostream>

/**
 * @file module.h
 * @brief Basismodul
 */

/**
 * @brief Akstrakte Klasse für allgemeine Bauteile
 *
 * Diese Klasse ist eine allgemeine Klasse, welche die In- und Outputs
 * definiert und die innere Operation abstrakt lässt.
 */

//TODO allgemeine GLS als NonlinSys z.B. einfügen (abstrakt, wie z.B. in dem alten Projekt), Input/Output sind x und rechte seite?
//UPDATE _x sind alle NLS Variablen, _p ist 0, damit soll f(x) = 0 sein (f und dfdx werden in flash festgelegt)

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class Module
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;
protected:
    VTS _x;
public:
    Module() {}

    VTS& x() { return _x; }
    TS& x(int i) { return _x(i); }

    virtual TS& Lin()=0;
    virtual TS& Vin()=0;
    virtual TS& Lout()=0;
    virtual TS& Vout()=0;
    virtual VTS f()=0;
    virtual MTS dfdx()=0;
};

/**
 *@brief Newton Löser für Abgeleitete Klassen von Modul
 */

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class MODULENEWTON
{
  LINEAR_SOLVER<TS,NS>& _lsol;
  double _eps;
public:
  MODULENEWTON(LINEAR_SOLVER<TS,NS>& lsol) : _lsol(lsol), _eps(0) {}
  double& eps() { return _eps; }
  void solve(Module<TS,NP,NS,TP>& mod) {
    assert(_eps>0);
    LINEAR_SYSTEM<TS,NS> lsys(mod.x().size());
    while (mod.f().norm()>_eps) {
      lsys.A()=mod.dfdx(); lsys.b()=-mod.f();
      _lsol.solve(lsys);
      //std::cout << "A: " << std::endl << lsys.A() << std::endl << "b: " << std::endl << lsys.b() << std::endl << (mod.f().norm()>_eps) << std::endl;
      mod.x()+=lsys.x();
    }
  }
};

#endif // MODULE_H
