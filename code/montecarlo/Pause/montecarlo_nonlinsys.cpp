#include<cassert>
#include<cstdlib>
#include<iostream>
#include<fstream>
using namespace std;

//#include "../nonlin_sys/nonlin_sys.h"
#include "montecarlo_nonlin.h"

int main(int c, char* v[]) {

    cout << endl << "-----Menue-----" << endl
                 << "1: System loesen" << endl
                 << "3: Ende" << endl;
    int n=1;
    int num=0;
    int dim=0;
    bool unlock2=false;
    cin >> n;
    while(n!=3) {
        switch (n) {
        case 1:
        {
            string input = "m.txt";
            cout << "Inputdatei: ";
            cin >> input;
            ifstream infile(input);
            infile >> dim;
            NONLINEAR_SYSTEM nlsys(dim);
            for(int i=0; i<dim;i++)
                for(int j=0; j<dim;j++)
                    infile >> nlsys.A(i,j);
            for(int i=0; i<dim;i++)
                infile >> nlsys.b(i);
            infile.close();

            //cout << "A" << endl << nlsys.A() << endl
            //     << "b" << endl << nlsys.b() << endl;
            LU nlsol;
            nlsol.solve(nlsys);
            cout << "Lösung mit Erwartungswerten:" << endl << nlsys.x() << endl;

            NONLINEAR_SYSTEM_GEN nlsysg(nlsys);
            NONLINEAR_PARRALEL_SOLVER nlsysps(nlsysg);
            double range;
            cout << "Wieviele Berechnungen sollen durchgeführt werden? ";
            cin >> num;
            cout << "Standardabweichung: ";
            cin >> range;
            DATASET data = nlsysps.psolve(num,range);
            data.writeToFile("data.txt");
            unlock2 = true;
            break;
        }
        case 2:
        {
            if(!unlock2) break;
            DATASET data("data.txt", dim, num);
            int i = 1;
            cout << "Welchen Parameter möchten Sie betrachten? ";
            cin >> i;
            data.displayRow(i-1);

        }
        default:
            break;
        }
        cout << endl << "-----Menue-----" << endl
                     << "1: System loesen" << endl
                     << "2: Ergebnisse anzeigen" << endl
                     << "3: Ende" << endl;
        cin >> n;
    }
    return 0;
}
