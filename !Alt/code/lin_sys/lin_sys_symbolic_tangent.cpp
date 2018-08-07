#include<cassert>
#include<cstdlib>
#include<iostream>
#include<cfloat>
#include<cmath>
using namespace std;

#include "lin_sys.h"
typedef Matrix<double,Dynamic,Dynamic> MT;
typedef Matrix<double,Dynamic,1> VT;

int main(int c, char* v[]) {
  assert(c==2); int n=atoi(v[1]); 
  cout << "symbolic tangent: dx(n/2)/db=" << endl;

  LINEAR_SYSTEM lsys(n);
  lsys.A()=MT::Random(n,n); lsys.b()=VT::Random(n); 
  QR lsol;
  lsol.solve(lsys);

  for (int i=0;i<n;i++) {
    LINEAR_SYSTEM lsys_t(n);
    MT A_t=MT::Zero(n,n);
    VT b_t=VT::Unit(n,i); 
    lsys_t.A()=lsys.A();
    lsys_t.b()=b_t-A_t*lsys.x();
    lsol.solve(lsys_t);
    cout << lsys_t.x(n/2) << endl;
  }

  return 0;
}

