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
typedef Matrix<DCO_TT,Dynamic,1> VT_T;
typedef Matrix<DCO_AT,Dynamic,1> VT_A;

int main(int c, char* v[]) {
  assert(c==2); int ns=atoi(v[1]); const int NP=3; 
{
  cout << "algorithmic tangent: d(x(n/2)/dp=" << endl;
  DIFFUSION<DCO_TT> nlsys(ns);
  nlsys.p(0)=.1; nlsys.p(1)=0.; nlsys.p(2)=1400.; 
  QR<DCO_TT> lsol;
  NEWTON<DCO_TT,NP> nlsol(lsol);
  nlsol.eps()=1e-7;
  for (int i=0;i<NP;i++) {
    nlsys.x()=VT_T::Zero(ns);
    dco::derivative(nlsys.p(i))=1;
    nlsol.solve(nlsys);
    cout << dco::derivative(nlsys.x(ns/2)) << endl;
    dco::derivative(nlsys.p(i))=0;
  }
}
{
  cout << "algorithmic adjoint: d(x(n/2)/dp=" << endl;
  DIFFUSION<DCO_AT> nlsys(ns);
  nlsys.p(0)=.1; nlsys.p(1)=0.; nlsys.p(2)=1400.; 
  QR<DCO_AT> lsol;
  NEWTON<DCO_AT,NP> nlsol(lsol);
  nlsol.eps()=1e-7;
  nlsys.x()=VT_A::Zero(ns);
  DCO_AM::global_tape=DCO_AT_TAPE_T::create();
  for (int i=0;i<NP;i++) 
    DCO_AM::global_tape->register_variable(nlsys.p(i));
  nlsol.solve(nlsys);
  dco::derivative(nlsys.x(ns/2))=1;
  DCO_AM::global_tape->interpret_adjoint();
  for (int i=0;i<NP;i++) 
    cout << dco::derivative(nlsys.p(i)) << endl;
}
  return 0;
}

