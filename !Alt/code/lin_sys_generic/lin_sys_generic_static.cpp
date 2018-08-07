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

int main() {
  const int N=3; 
  typedef Matrix<double,N,N> MT;
  typedef Matrix<double,N,1> VT;

  MT A=MT::Random(N,N); 
  VT b=VT::Random(N); 

{
  cout << "primal: x=" << endl;
  LINEAR_SYSTEM<double,N> lsys(N);
  lsys.A()=A;
  lsys.b()=b;
  QR<double,N> lsol;
  lsol.solve(lsys);
  cout << lsys.x() << endl;
}
{
  cout << "algorithmic tangent: dx(n/2)/db=" << endl;
  LINEAR_SYSTEM<DCO_TT,N> lsys(N);
  for (int i=0;i<N;i++) {
    for (int j=0;j<N;j++) 
      lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }
  QR<DCO_TT,N> lsol;
  for (int i=0;i<N;i++) {
    dco::derivative(lsys.b(i))=1;
    lsol.solve(lsys);
    cout << dco::derivative(lsys.x(N/2)) << endl;
    dco::derivative(lsys.b(i))=0;
  }
}
{
  cout << "algorithmic adjoint: dx(n/2)/db=" << endl;
  LINEAR_SYSTEM<DCO_AT,N> lsys(N);
  for (int i=0;i<N;i++) {
    for (int j=0;j<N;j++) 
      lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }
  LU<DCO_AT,N> lsol;

  DCO_AM::global_tape=DCO_AT_TAPE_T::create();

  for (int i=0;i<N;i++)
    DCO_AM::global_tape->register_variable(lsys.b(i));

  lsol.solve(lsys);

  dco::derivative(lsys.x(N/2))=1;
  DCO_AM::global_tape->interpret_adjoint();

  for (int i=0;i<N;i++)
    cout << dco::derivative(lsys.b(i)) << endl;

  DCO_AT_TAPE_T::remove(DCO_AM::global_tape);
}
  return 0;
}
