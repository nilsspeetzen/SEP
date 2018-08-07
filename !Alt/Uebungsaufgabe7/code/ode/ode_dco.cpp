// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include<iostream>
using namespace std;

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::gt1s<DCO_BT>::type DCO_TT;
typedef dco::ga1s<DCO_BT> DCO_AM;
typedef DCO_AM::type DCO_AT;
typedef DCO_AM::tape_t DCO_AT_TAPE_T;

#include "diffusion.h"

int main() {
const int NS=50,NP=3; 
{
  cout << "algorithmic tangent: dx(n/2)/dx0=" << endl;
  DIFFUSION<DCO_TT,NS> ode(NS);
  ode.p(0)=0.1; ode.p(1)=1400; ode.p(2)=0;
  QR<DCO_TT,NS> lsol;
  NEWTON<DCO_TT,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-7;
  IMPLICIT_EULER<DCO_TT,NP,NS> odesol(nlsol);
  odesol.T_end()=1;
  odesol.nts()=1000;
  for (int i=0;i<NS;i++) {
    for (int j=0;j<NS;j++) ode.x(j)=j*log(static_cast<double>(j+1));
    dco::derivative(ode.x(i))=1;
    odesol.solve(ode);
    cout << dco::derivative(ode.x(NS/2)) << endl;
  }
}
{
  cout << "algorithmic adjoint: dx(n/2)/dx0=" << endl;
  DIFFUSION<DCO_AT,NS> ode(NS);
  ode.p(0)=0.1; ode.p(1)=1400; ode.p(2)=0;
  QR<DCO_AT,NS> lsol;
  NEWTON<DCO_AT,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-7;
  IMPLICIT_EULER<DCO_AT,NP,NS> odesol(nlsol);
  odesol.T_end()=1;
  odesol.nts()=1000;
  for (int j=0;j<NS;j++) ode.x(j)=j*log(static_cast<double>(j+1));
  DCO_AM::global_tape=DCO_AT_TAPE_T::create();
  for (int i=0;i<NS;i++)
    DCO_AM::global_tape->register_variable(ode.x(i));
  Matrix<DCO_AT,NS,1> x0=ode.x();
  odesol.solve(ode);
  dco::derivative(ode.x(NS/2))=1;
  DCO_AM::global_tape->interpret_adjoint();
  for (int i=0;i<NS;i++)
    cout << dco::derivative(x0(i)) << endl;
  DCO_AT_TAPE_T::remove(DCO_AM::global_tape);
}
  return 0;
}

