#include<cassert>
#include<cstdlib>
#include<iostream>
using namespace std;

#include "../lin_sys/lin_sys.h"

int main(int c, char* v[]) {
    cout << endl << "-----Menue-----" << endl
                 << "1: System loesen" << endl
                 << "2: Ergebnisse anzeigen" << endl;
    int n=0;
    cin >> n;
    switch (n) {
    case 1:
    {
        string input = "kek.txt";
        cout << "Bitte geben sie den Namen ihrer Inputdatei ein: ";
        cin >> input;
        cout << input << endl;

        /*LINEAR_SYSTEM lsys(n);
        lsys.A()=MT::Random(n,n);
        lsys.b()=VT::Random(n);
        cout << "LU" << endl;
        LU lsol;
        lsol.solve(lsys);
        cout << "x=" << endl << lsys.x() << endl;*/
        break;
    }
    default:
        cout << endl << "-----Fehler-----" << endl;
        break;
    }

    return 0;
}
