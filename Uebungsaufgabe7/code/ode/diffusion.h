// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "ode.h"

template<typename TS=double, int NS=Dynamic, typename TP=TS, const int NP=3>
class DIFFUSION : public ODE<TS,NP,NS,TP>
{
  using ODE<TS,NP,NS,TP>::_x;
  using ODE<TS,NP,NS,TP>::_p;
public:
  DIFFUSION(int ns) : ODE<TS,NP,NS,TP>(ns,NP) {} 
  template<typename LTS,int LNS=Dynamic,typename LTP,const int LNP=3>
  static inline Matrix<LTS,LNS,1> g(Matrix<LTS,LNS,1>& x, Matrix<LTP,LNP,1>& p) {
   int ns=x.size();
    Matrix<LTS,LNS,1> r=Matrix<LTS,LNS,1>::Zero(ns);
    for (int i=0;i<ns;i++) {
      if (i==0) 
        r(i)=p(0)*x(i)*ns*ns*(p(1)-2*x(i)+x(i+1));
      else if (i==ns-1)
        r(i)=p(0)*x(i)*ns*ns*(x(i-1)-2*x(i)+p(2));
      else
        r(i)=p(0)*x(i)*ns*ns*(x(i-1)-2*x(i)+x(i+1));
    }
    return r;
  }
  Matrix<TS,NS,1> g() {
    return g(_x,_p);
  }
  Matrix<TS,NS,NS> dgdx() {
    int ns=_x.size();
    Matrix<TS,NS,NS> drdx=Matrix<TS,NS,NS>::Zero(ns,ns);
    typedef typename dco::gt1s<TS>::type DCO_TT_TS;
    Matrix<DCO_TT_TS,NS,1> x_dco,r_dco;
    for (int i=0;i<ns;i++)
      dco::value(x_dco(i))=_x(i);
    for (int i=0;i<ns;i++) {
      dco::derivative(x_dco(i))=1;
      r_dco=g(x_dco,_p);
      for (int j=0;j<ns;j++)
        drdx(j,i)=dco::derivative(r_dco(j));
      dco::derivative(x_dco(i))=0;
    }
    return drdx;
  }
};
