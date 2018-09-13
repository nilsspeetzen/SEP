#ifndef NONLIN_SYS_H
#define NONLIN_SYS_H

// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "LU"
#include "QR"
using namespace Eigen;

#include "lin_sys.h"

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class NONLINEAR_SYSTEM
{
  typedef Matrix<TS,NS,1> VTS;
  typedef Matrix<TS,NS,NS> MTS;
  typedef Matrix<TP,NP,1> VTP;
protected:
  VTS _x; VTP _p;
public:

  NONLINEAR_SYSTEM(int np, int ns) : _x(VTS::Zero(ns)), _p(VTP::Zero(np)) {}
  int ns() { return _x.size(); }
  int np() { return _p.size(); }
  VTS& x() { return _x; }
  TS& x(int i) { return _x(i); }
  VTP& p() { return _p; }
  TP& p(int i) { return _p(i); }
  virtual VTS f()=0;
  virtual MTS dfdx()=0;
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class NONLINEAR_SOLVER
{
public:
  virtual void solve(NONLINEAR_SYSTEM<TS,NP,NS,TP>&)=0;
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class NEWTON : public NONLINEAR_SOLVER<TS,NP,NS,TP>
{
  LINEAR_SOLVER<TS,NS>& _lsol;
  double _eps;
public:
  NEWTON(LINEAR_SOLVER<TS,NS>& lsol) : _lsol(lsol), _eps(0) {}
  double& eps() { return _eps; }
  void solve(NONLINEAR_SYSTEM<TS,NP,NS,TP>& nlsys) {
    assert(_eps>0);
    LINEAR_SYSTEM<TS,NS> lsys(nlsys.ns());
    while (nlsys.f().norm()>_eps) {
      lsys.A()=nlsys.dfdx(); lsys.b()=-nlsys.f();
      _lsol.solve(lsys);
      nlsys.x()+=lsys.x();
    }
  }
};


#endif // NONLIN_SYS_H
