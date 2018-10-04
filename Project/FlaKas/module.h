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

template<typename RealType=double>
class Module
{
    typedef Matrix<RealType,Dynamic,1> VTS;
    typedef Matrix<RealType,Dynamic,Dynamic> MTS;
    typedef Matrix<RealType,Dynamic,1> VTP;
protected:
    VTS _x;
public:
    Module() {}

    VTS& x() { return _x; }
    RealType& x(int i) { return _x(i); }

    virtual RealType Lin() const=0;
    virtual RealType Vin() const=0;
    virtual RealType Lout() const=0;
    virtual RealType Vout() const=0;
    virtual VTS f()=0;
    //virtual MTS dfdx()=0;
};

/**
 *@brief Newton Löser für Abgeleitete Klassen von Modul
 */
/*
template<typename RealType=double, int NP=Dynamic, int NS=Dynamic, typename TP=RealType>
class MODULENEWTON
{
  LINEAR_SOLVER<RealType,NS>& _lsol;
  double _eps;
public:
  MODULENEWTON(LINEAR_SOLVER<RealType,NS>& lsol) : _lsol(lsol), _eps(0) {}
  double& eps() { return _eps; }
  void solve(Module<RealType>& mod) {
    assert(_eps>0);
    LINEAR_SYSTEM<RealType,NS> lsys(mod.x().size());
    while (mod.f().norm()>_eps) {
      lsys.A()=mod.dfdx(); lsys.b()=-mod.f();
      _lsol.solve(lsys);
      //std::cout << "A: " << std::endl << lsys.A() << std::endl << "b: " << std::endl << lsys.b() << std::endl << (mod.f().norm()>_eps) << std::endl;
      mod.x()+=lsys.x();
    }
  }
};
*/
#endif // MODULE_H
