#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "lin_sys.h"
typedef Matrix<double,Dynamic,Dynamic> MT;
typedef Matrix<double,Dynamic,1> VT;

int main(int c, char* v[]) {
  assert(c==2); int n=atoi(v[1]); 

  LINEAR_SYSTEM lsys(n);
  lsys.A()=MT::Random(n,n); 
  lsys.b()=VT::Random(n); 
{
  cout << "LU" << endl;
  LU lsol;
  lsol.solve(lsys);
  cout << "x=" << endl << lsys.x() << endl;
}
{
  cout << "QR" << endl;
  QR lsol;
  lsol.solve(lsys);
  cout << "x=" << endl << lsys.x() << endl;
}
  return 0;
}

