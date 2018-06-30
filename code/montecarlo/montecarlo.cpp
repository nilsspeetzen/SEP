#include<cassert>
#include<cstdlib>
#include<iostream>
#include<fstream>
using namespace std;

//#include "../lin_sys/lin_sys.h"
#include "montecarlo.h"

int main(int c, char* v[]) {
    cout << endl << "-----Menue-----" << endl
                 << "1: System loesen" << endl
                 << "2: Ergebnisse anzeigen" << endl;
    int n=1;
    cin >> n;
    while(n!=3) {
    switch (n) {
    case 1:
    {
        string input = "m.txt";
        cout << "Inputdatei: ";
        cin >> input;
        MT firstline;
        ifstream infile(input);
        int n;
        infile >> n;
        LINEAR_SYSTEM lsys(n);

        for(int i=0; i<n;i++)
            for(int j=0; j<n;j++)
                infile >> lsys.A(i,j);

        for(int i=0; i<n;i++)
            infile >> lsys.b(i);
        infile.close();
        //cout << "A" << endl << lsys.A() << endl
        //     << "b" << endl << lsys.b() << endl;
        LU lsol;
        lsol.solve(lsys);
        cout << "Lösung mit Erwartungswerten:" << endl << lsys.x() << endl;

        LINEAR_SYSTEM_GEN lsysg(lsys);
        LINEAR_PARRALEL_SOLVER lsysps(lsysg);
        DATASET data = lsysps.psolve(10,0.1);
        data.writeToFile("data.txt");

        break;
    }
    case 2:
    {
        if(data == nullptr) break;
        int i = 1;
        cout << "Welchen Parameter möchten Sie betrachten? ";
        cin >> i;
        data.displayRow(i-1, lsys.x());

    }
    default:
        break;
    }
    cin >> n;
    }
    return 0;
}
