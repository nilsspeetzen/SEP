#include<cassert>
#include<cstdlib>
#include<iostream>
#include<fstream>
using namespace std;

#include "../lin_sys/lin_sys.h"

int main(int c, char* v[]) {
    cout << endl << "-----Menue-----" << endl
                 << "1: System loesen" << endl
                 << "2: Ergebnisse anzeigen" << endl;
    int n=1;
    //cin >> n;
    switch (n) {
    case 1:
    {
        string input = "m.txt";
        //cout << "Inputdatei: ";
        //cin >> input;
        MT firstline;
        ifstream infile(input);
        int n, m;
        infile >> n >> m;
        LINEAR_SYSTEM lsys(n);
        lsys.A() = MT::Constant(n,m,0);

        for(int i=0; i<n;i++)
            for(int j=0; j<m;j++) {
                infile >> lsys.A(i,j);
            }

        cout << lsys.A() << endl;

        break;
    }
    default:
        break;
    }

    return 0;
}
