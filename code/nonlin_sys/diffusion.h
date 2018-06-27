// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "nonlin_sys.h"

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::gt1s<DCO_BT>::type DCO_TT;

template<typename TS=double, int NS=Dynamic, typename TP=TS, const int NP=3>
class DIFFUSION : public NONLINEAR_SYSTEM<TS,NP,NS,TP>
{
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_x;
  using NONLINEAR_SYSTEM<TS,NP,NS,TP>::_p;
public:
  DIFFUSION(int ns) : NONLINEAR_SYSTEM<TS,NP,NS,TP>(NP,ns) {
    static_assert(NP==3,"Wrong NP!");
  }
  template<typename LTS=double,int LNS=Dynamic,typename LTP=LTS>
  static inline Matrix<LTS,LNS,1> f(Matrix<LTS,LNS,1>& x, Matrix<LTP,NP,1>& p) {
    int ns=x.size(); 
    Matrix<LTS,LNS,1> r=Matrix<LTS,LNS,1>::Zero(ns);
    for (int i=0;i<ns;i++) {
      if (i==0)
        r(i)=p(0)*ns*ns*(p(1)-2*x(i)+x(i+1));
      else if (i==ns-1)
        r(i)=p(0)*ns*ns*(x(i-1)-2*x(i)+p(2));
      else 
        r(i)=p(0)*ns*ns*(x(i-1)-2*x(i)+x(i+1));
    }
    return r;
  }
  Matrix<TS,NS,1> f() { return f(_x,_p); }
  Matrix<TS,NS,NS> dfdx() {
    int ns=_x.size();
    Matrix<TS,NS,NS> drdx=Matrix<TS,NS,NS>::Zero(ns,ns);
    Matrix<typename dco::gt1s<TS>::type,NS,1> 
      x_dco=Matrix<typename dco::gt1s<TS>::type,NS,1>::Zero(ns),
      r_dco=Matrix<typename dco::gt1s<TS>::type,NS,1>::Zero(ns);
    for (int i=0;i<ns;i++) dco::value(x_dco(i))=_x(i);
    for (int i=0;i<ns;i++) {
      dco::derivative(x_dco(i))=1;
      r_dco=f(x_dco,_p);
      for (int j=0;j<ns;j++) drdx(j,i)=dco::derivative(r_dco(j));
      dco::derivative(x_dco(i))=0;
    }
    return drdx;
  }
};
