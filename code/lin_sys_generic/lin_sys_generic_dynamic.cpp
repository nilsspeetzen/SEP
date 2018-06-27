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

#include "lin_sys.h"

int main(int c, char* v[]) {
  assert(c==2); int n=atoi(v[1]); 
  typedef Matrix<double,Dynamic,Dynamic> MT;
  typedef Matrix<double,Dynamic,1> VT;

  MT A=MT::Random(n,n); VT b=VT::Random(n); 

{
  cout << "primal: x=" << endl;
  LINEAR_SYSTEM<> lsys(n);
  lsys.A()=A; lsys.b()=b;
  QR<> lsol;
  lsol.solve(lsys);
  cout << lsys.x() << endl;
}
{
  cout << "algorithmic tangent: dx(n/2)/db=" << endl;
  LINEAR_SYSTEM<DCO_TT> lsys(n);
  for (int i=0;i<n;i++) {
    for (int j=0;j<n;j++) 
      lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }
  QR<DCO_TT> lsol;
  for (int i=0;i<n;i++) {
    dco::derivative(lsys.b(i))=1;
    lsol.solve(lsys);
    cout << dco::derivative(lsys.x(n/2)) << endl;
    dco::derivative(lsys.b(i))=0;
  }
}
{
  cout << "algorithmic adjoint: dx(n/2)/db=" << endl;
  LINEAR_SYSTEM<DCO_AT> lsys(n);
  for (int i=0;i<n;i++) {
    for (int j=0;j<n;j++) 
      lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }
  LU<DCO_AT> lsol;

  DCO_AM::global_tape=DCO_AT_TAPE_T::create();

  for (int i=0;i<n;i++)
    DCO_AM::global_tape->register_variable(lsys.b(i));

  lsol.solve(lsys);

  dco::derivative(lsys.x(n/2))=1;
  DCO_AM::global_tape->interpret_adjoint();

  for (int i=0;i<n;i++)
    cout << dco::derivative(lsys.b(i)) << endl;

  DCO_AT_TAPE_T::remove(DCO_AM::global_tape);
}
  return 0;
}

