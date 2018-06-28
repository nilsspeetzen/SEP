#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "../lin_sys/lin_sys.h"

int main(int c, char* v[]) {
    cout << endl << "-----Menue-----" << endl
                 << "1: linsys" << endl;
    int n=0;
    cin >> n;
    switch (n) {
    case 1:
    {
        LINEAR_SYSTEM lsys(n);
        lsys.A()=MT::Random(n,n);
        lsys.b()=VT::Random(n);
        cout << "LU" << endl;
        LU lsol;
        lsol.solve(lsys);
        cout << "x=" << endl << lsys.x() << endl;
        break;
    }
    default:
        cout << endl << "-----Fehler-----" << endl;
        break;
    }

    return 0;
}
