#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "../lin_sys/lin_sys.h"

int main(int c, char* v[]) {
    cout << "kek" << endl;
    int n = 3;
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
