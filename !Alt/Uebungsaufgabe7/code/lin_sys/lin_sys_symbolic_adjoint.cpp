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

  LINEAR_SYSTEM lsys_a(n);
  VT x_a=VT::Unit(n,n/2); 
  lsys_a.A()=lsys.A().transpose();
  lsys_a.b()=x_a;
  lsol.solve(lsys_a);
  for (int i=0;i<n;i++) 
    cout << lsys_a.x(i) << endl;
  cout << "dx(n/2)/dA=" << endl;
  for (int i=0;i<n;i++) 
    for (int j=0;j<n;j++)
      cout << -lsys_a.x(i)*lsys.x(j) << endl;


  return 0;
}

