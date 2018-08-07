// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::gt1s<DCO_BT>::type DCO_TT;
typedef dco::ga1s<DCO_BT> DCO_AM;
typedef DCO_AM::type DCO_AT;
typedef DCO_AM::tape_t DCO_AT_TAPE_T;

#include "lotka_volterra.h"
#include "diffusion.h"

int main() {
{
  const int NS=100,NP=3; 
  DIFFUSION<double,NS> ode(NS);
  for (int i=0;i<NS;i++) ode.x(i)=(i+1)*log(static_cast<double>(i+2));
  ode.p(0)=1e-3; ode.p(1)=14; ode.p(2)=0;
  EXPLICIT_EULER<double,NP,NS> odesol;
  odesol.T_end()=1;
  odesol.nts()=10000;
  odesol.solve(ode);
  cout << ode.x() << endl;
  ofstream s("sol1.out");
  ode.plot(s);
}
{
  const int NS=100,NP=3; 
  DIFFUSION<double,NS> ode(NS);
  for (int i=0;i<NS;i++) ode.x(i)=(i+1)*log(static_cast<double>(i+2));
  ode.p(0)=1e-3; ode.p(1)=14; ode.p(2)=0;
  QR<double,NS> lsol;
  NEWTON<double,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-7;
  IMPLICIT_EULER<double,NP,NS> odesol(nlsol);
  odesol.T_end()=1;
  odesol.nts()=200;
  odesol.solve(ode);
  cout << ode.x() << endl;
  ofstream s("sol2.out");
  ode.plot(s);
}
{
  const int NS=2,NP=4; 
  LOTKA_VOLTERRA<double> ode;
  ode.x(0)=1000; ode.x(1)=20;
  ode.p(0)=0.015; ode.p(1)=0.0001; ode.p(2)=0.03; ode.p(3)=0.0001;
  EXPLICIT_EULER<double,NP,NS> odesol;
  odesol.T_end()=1000;
  odesol.nts()=100000000;
  odesol.solve(ode);
  cout << ode.x() << endl;
}
{
  const int NS=2,NP=4; 
  LOTKA_VOLTERRA<double> ode;
  ode.x(0)=1000; ode.x(1)=20;
  ode.p(0)=0.015; ode.p(1)=0.0001; ode.p(2)=0.03; ode.p(3)=0.0001;
  QR<double,NS> lsol;
  NEWTON<double,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-12;
  IMPLICIT_EULER<double,NP,NS> odesol(nlsol);
  odesol.T_end()=1000;
  odesol.nts()=1000000;
  odesol.solve(ode);
  cout << ode.x() << endl;
}
  return 0;
}

