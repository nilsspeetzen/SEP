// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "ode.h"

template<typename TS=double, typename TP=TS, const int NP=4, const int NS=2>
class LOTKA_VOLTERRA : public ODE<TS,NP,NS,TP>
{
  using ODE<TS,NP,NS,TP>::_x;
  using ODE<TS,NP,NS,TP>::_p;
public:
  LOTKA_VOLTERRA() : ODE<TS,NP,NS,TP>(NS,NP) {} 
  Matrix<TS,NS,1> g() { 
    Matrix<TS,NS,1> r; 
    r(0)=(_p(0)-_p(1)*_x(1))*_x(0);
    r(1)=-(_p(2)-_p(3)*_x(0))*_x(1);
    return r;
  }
  Matrix<TS,NS,NS> dgdx() { 
    Matrix<TS,NS,NS> drdx;
    drdx(0,0)=_p(0)-_p(1)*_x(1); 
    drdx(0,1)=-_p(1)*_x(0);
    drdx(1,0)=_p(3)*_x(1); 
    drdx(1,1)=-(_p(2)-_p(3)*_x(0));
    return drdx;
  }
};
