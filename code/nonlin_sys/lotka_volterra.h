// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "nonlin_sys.h"

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::gt1s<DCO_BT>::type DCO_TT;

template<typename TS=double, typename TP=TS, int NS=2, int NP=4>
class LOTKA_VOLTERRA : public NONLINEAR_SYSTEM<TS,NP,NS,TP>
{
  
public:
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_x;
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_p;
  LOTKA_VOLTERRA() : NONLINEAR_SYSTEM<TS,NP,NS,TP>(NP,NS) {
    static_assert(NP==4&&NS==2,"Wrong NP or NS!");
  }
  template<typename LTS=double,typename LTP=LTS>
  static Matrix<LTS,NS,1> f(Matrix<LTS,NS,1>& x, Matrix<LTP,NP,1>& p) { 
    Matrix<LTS,NS,1> r;
    r(0)=(p(0)-p(1)*x(1))*x(0);
    r(1)=-(p(2)-p(3)*x(0))*x(1);
    return r;
  }
  Matrix<TS,NS,1> f() { return f(_x,_p); }
  Matrix<TS,NS,NS> dfdx() { 
    Matrix<TS,NS,NS> drdx;
    typedef typename dco::gt1s<TS>::type DCO_TT_TS;
    Matrix<DCO_TT_TS,NS,1> x_dco,r_dco;
    for (int i=0;i<NS;i++) 
      dco::value(x_dco(i))=_x(i);
    for (int i=0;i<NS;i++) {
      dco::derivative(x_dco(i))=1;
      r_dco=f(x_dco,_p);
      for (int j=0;j<NS;j++) 
        drdx(j,i)=dco::derivative(r_dco(j)); 
      dco::derivative(x_dco(i))=0;
    }
    return drdx;
  }
};
