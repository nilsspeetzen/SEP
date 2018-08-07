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
  cout << "finite differences: dx(n/2)/db=" << endl;

  LINEAR_SYSTEM lsys(n);
  lsys.A()=MT::Random(n,n); 
  lsys.b()=VT::Random(n); 
  QR lsol;

  for (int i=0;i<n;i++) {
    const double eps=sqrt(DBL_EPSILON);
    double h=(abs(lsys.b(i))<1) ? eps : eps*abs(lsys.b(i));
    lsys.b(i)-=h; lsol.solve(lsys); VT xp=lsys.x();
    lsys.b(i)+=2*h; lsol.solve(lsys); 
    cout << (lsys.x(n/2)-xp(n/2))/(2*h) << endl;
    lsys.b(i)-=h; 
  }

  return 0;
}

