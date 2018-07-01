#include<cassert>
#include<cstdlib>
#include<iostream>
#include<fstream>
using namespace std;

#include ".../lin_sys/lin_sys.h"
#include "montecarlo_ode.h"

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
            ODE _ode(dim);
            for(int i=0; i<dim;i++)
                for(int j=0; j<dim;j++)
                    infile >> _ode.A(i,j);
            for(int i=0; i<dim;i++)
                infile >> _ode.b(i);
            infile.close();

            //cout << "A" << endl << _ode.A() << endl
            //     << "b" << endl << _ode.b() << endl;
            LU odesol;
            odesol.solve(_ode);
            cout << "Lösung mit Erwartungswerten:" << endl << _ode.x() << endl;

            ODE_GEN _odeg(_ode);
            ODE_PARRALEL_SOLVER _odeps(_odeg);
            double range;
            cout << "Wieviele Berechnungen sollen durchgeführt werden? ";
            cin >> num;
            cout << "Standardabweichung: ";
            cin >> range;
            DATASET_ODE data = _odeps.psolve(num,range);
            data.writeToFile("data.txt");
            unlock2 = true;
            break;
        }
        case 2:
        {
            if(!unlock2) break;
            DATASET_ODE data("data.txt", dim, num);
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
