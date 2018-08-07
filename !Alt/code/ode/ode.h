// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#ifndef _ODE_INC_
#define _ODE_INC_

#include <Eigen/LU>
#include <Eigen/QR>
using namespace Eigen;

#include "nonlin_sys.h"

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class ODE {
  typedef Matrix<TS,NS,1> VTS;
  typedef Matrix<TS,NS,NS> MTS;
  typedef Matrix<TP,NP,1> VTP;
protected:
  VTS _x; VTP _p;
public:
  ODE(int ns, int np) : _x(VTS::Zero(ns)), _p(VTP::Zero(np)) {}
  int ns() { return _x.size(); }
  int np() { return _p.size(); }
  VTS& x() { return _x; }
  TS& x(int i) { return _x(i); }
  VTP& p() { return _p; }
  TP& p(int i) { return _p(i); }
  virtual VTS g()=0;
  virtual MTS dgdx()=0;
  virtual void plot(ostream& s) {
    for (int i=0;i<ns();i++) s << static_cast<double>(i)/ns() << " " << _x(i) << endl;
  }
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class ODE_SOLVER {
protected:
  TP _T_end;
  int _nts;
public:
  ODE_SOLVER() : _T_end(1), _nts(1) {}
  TP& T_end() { return _T_end; }
  int& nts() { return _nts; }
  virtual void solve(ODE<TS,NP,NS,TP>&)=0;
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class IMPLICIT_EULER_SYSTEM : public NONLINEAR_SYSTEM<TS,NP,NS,TP>
{
  typedef Matrix<TS,NS,1> VTS;
  typedef Matrix<TS,NS,NS> MTS;
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::f;
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::dfdx;
  ODE<TS,NP,NS,TP>& _ode;
  VTS _x_prev; 
  TP _dt;
public:
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::x;
  IMPLICIT_EULER_SYSTEM(ODE<TS,NP,NS,TP>& ode, VTS& x_prev, TP dt) : NONLINEAR_SYSTEM<TS,NP,NS,TP>(ode.np(),ode.ns()), _ode(ode), _x_prev(x_prev), _dt(dt) {
    x()=_x_prev;
  }
  VTS f() { 
    _ode.x()=x();
    return _ode.x()-_x_prev-_dt*_ode.g();
  }
  MTS dfdx() { 
    _ode.x()=x();
    return Matrix<TS,NS,NS>::Identity()-_dt*_ode.dgdx();
  }
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class IMPLICIT_EULER : ODE_SOLVER<TS,NP,NS,TP> {
  NONLINEAR_SOLVER<TS,NP,NS,TP>& _nlsol;
public:
  using ODE_SOLVER<TS,NP,NS,TP>::T_end;
  using ODE_SOLVER<TS,NP,NS,TP>::nts;
  IMPLICIT_EULER(NONLINEAR_SOLVER<TS,NP,NS,TP>& nlsol) : _nlsol(nlsol) {};
  void solve(ODE<TS,NP,NS,TP>& ode) {
    for (int i=0;i<nts();i++) {
      Matrix<TS,NS,1> x_prev=ode.x();
      IMPLICIT_EULER_SYSTEM<TS,NP,NS,TP> odesys(ode,x_prev,T_end()/nts());
      _nlsol.solve(odesys);
      ode.x()=odesys.x();
    }
  }
};

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class EXPLICIT_EULER : ODE_SOLVER<TS,NP,NS,TP> {
public:
  using ODE_SOLVER<TS,NP,NS,TP>::T_end;
  using ODE_SOLVER<TS,NP,NS,TP>::nts;
  void solve(ODE<TS,NP,NS,TP>& ode) {
    for (int i=0;i<nts();i++) 
      ode.x()=ode.x()+T_end()/nts()*ode.g();
  }
};

#endif
