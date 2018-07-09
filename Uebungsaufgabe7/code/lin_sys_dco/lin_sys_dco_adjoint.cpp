#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::ga1s<DCO_BT> DCO_AM;
typedef DCO_AM::type DCO_AT;
typedef DCO_AM::tape_t DCO_AT_TAPE_T;

typedef DCO_AT DCO_T;

#include "lin_sys_dco.h"

int main(int c, char* v[]) {
  assert(c==2); int n=atoi(v[1]); 
  cout << "algorithmic adjoint: dx(n/2)/db=" << endl;

  LINEAR_SYSTEM lsys(n);
  Matrix<double,Dynamic,Dynamic> A=Matrix<double,Dynamic,Dynamic>::Random(n,n);
  Matrix<double,Dynamic,1> b=Matrix<double,Dynamic,1>::Random(n);
  for (int i=0;i<n;i++) {
    for (int j=0;j<n;j++) lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }

  DCO_AM::global_tape=DCO_AT_TAPE_T::create();

  for (int i=0;i<n;i++)
    DCO_AM::global_tape->register_variable(lsys.b(i));

  QR lsol;
  lsol.solve(lsys);

  dco::derivative(lsys.x(n/2))=1;
  DCO_AM::global_tape->interpret_adjoint();

  for (int i=0;i<n;i++)
    cout << dco::derivative(lsys.b(i)) << endl;

  DCO_AT_TAPE_T::remove(DCO_AM::global_tape);

  return 0;
}

