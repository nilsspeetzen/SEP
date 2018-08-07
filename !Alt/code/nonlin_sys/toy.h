// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "nonlin_sys.h"

template<typename TS=double, typename TP=TS, int NP=1, int NS=1>
class TOY : public NONLINEAR_SYSTEM<TS,NP,NS,TP>
{
public:
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_x;
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_p;
  TOY() : NONLINEAR_SYSTEM<TS,NP,NS,TP>(NP,NS) {
    static_assert(NP==1&&NS==1,"Wrong NP or NS!");
  }
  Matrix<TS,NS,1> f() { 
    Matrix<TS,NS,1> r;
    r(0)=_x(0)*_x(0)-_p(0); 
    return r;
  }
  Matrix<TS,NS,NS> dfdx() { 
    Matrix<TS,NS,NS> drdx;
    drdx(0,0)=2*_x(0);
    return drdx;
  } 
};

