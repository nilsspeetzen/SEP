// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include "ode.h"

template<typename TS=double, int NS=Dynamic, typename TP=TS, const int NP=3>
class BLACK_SCHOLES : public ODE<TS,NP,NS,TP>
{
  using ODE<TS,NP,NS,TP>::_x;
  using ODE<TS,NP,NS,TP>::_p;
public:
  BLACK_SCHOLES(int ns) : ODE<TS,NP,NS,TP>(ns,NP) {} 
  template<typename LTS,int LNS=Dynamic,typename LTP,const int LNP=3>
  static inline Matrix<LTS,LNS,1> g(Matrix<LTS,LNS,1>& x, Matrix<LTP,LNP,1>& p, const LTP& t) {
   int ns=x.size()+1;
    Matrix<LTS,LNS,1> r=Matrix<LTS,LNS,1>::Zero(ns);
    r(0)=0.5*pow(p(2)*(0+1),2)*(u(0+1)-2*u(0)+/*=u[0-1]=*/0/**/)+0.5*p(1)*(0+1)*(u(0+1)-/*u[0-1]=*/0/**/)-p(1)*u(0);
    for (int i=1;i<ns-2;i++) 
      r(i)=0.5*pow(p(2)*(i+1),2)*(x(i+1)-2*x(i)+x(i-1))+
           0.5*p(1)*(i+1)*(u(i+1)-u(i-1))-p(1)*u(i);
    r(ns-2)=0.5*pow(p(2)*(ns-2+1),2)*(/*u(ns-2+1)=*/1-p(0)*exp(-p(1)*t)/**/-2*u(ns-2)+u(ns-2-1))+0.5*p(1)*(ns-2+1)*(/*u(nx-2+1)=*/1-p(0)*exp(-p(1)*t)/**/-u(nx-2-1))-p(1)*u(ns-2);
    return r;
  }
  Matrix<TS,NS,1> g(const TS& t) {
    return g(_x,_p,t);
  }
  Matrix<TS,NS,NS> dgdx(const TS& t) {
    int ns=_x.size();
    Matrix<TS,NS,NS> drdx=Matrix<TS,NS,NS>::Zero(ns,ns);
    typedef typename dco::gt1s<TS>::type DCO_TT_TS;
    Matrix<DCO_TT_TS,NS,1> x_dco,r_dco;
    for (int i=0;i<ns;i++)
      dco::value(x_dco(i))=_x(i);
    for (int i=0;i<ns;i++) {
      dco::derivative(x_dco(i))=1;
      r_dco=g(x_dco,_p,t);
      for (int j=0;j<ns;j++)
        drdx(j,i)=dco::derivative(r_dco(j));
      dco::derivative(x_dco(i))=0;
    }
    return drdx;
  }
};
