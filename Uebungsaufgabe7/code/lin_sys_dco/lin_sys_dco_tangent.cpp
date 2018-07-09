#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "dco.hpp"
typedef double DCO_BT;
typedef dco::gt1s<DCO_BT>::type DCO_TT;
typedef DCO_TT DCO_T;

#include "lin_sys_dco.h"

int main(int c, char* v[]) {
  assert(c==2); int n=atoi(v[1]); 
  cout << "algorithmic tangent: dx(n/2)/db=" << endl;

  LINEAR_SYSTEM lsys(n);
  Matrix<double,Dynamic,Dynamic> A=Matrix<double,Dynamic,Dynamic>::Random(n,n); 
  Matrix<double,Dynamic,1> b=Matrix<double,Dynamic,1>::Random(n); 
  for (int i=0;i<n;i++) {
    for (int j=0;j<n;j++) lsys.A(i,j)=A(i,j);
    lsys.b(i)=b(i);
  }
  QR lsol;

  for (int i=0;i<n;i++) {
    dco::derivative(lsys.b(i))=1;
    lsol.solve(lsys);
    cout << dco::derivative(lsys.x(n/2)) << endl;
    dco::derivative(lsys.b(i))=0;
  }

  return 0;
}

