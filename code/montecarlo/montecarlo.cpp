#include<cassert>
#include<cstdlib>
#include<iostream>
#include<fstream>
using namespace std;

#include "dataset.h"
#include "montecarlo_linsys.h"
#include "montecarlo_nonlin.h"
#include "montecarlo_ode.h"

int main(int c, char* v[]) {

    cout << endl << "-----Menue-----" << endl
                 << "1: System loesen" << endl
                 << "3: Ende" << endl;
    int n=1;
    int dim=0;
    int num=0;
    bool unlock2=false;
    cin >> n;
    while(n==1 || n==2) {
        switch (n) {
        case 1:
        {
            cout << endl << "---------------" << endl
                         << "1:LIN" << endl
                         << "2:NONLIN" << endl
                         << "3:ODE" << endl;
            int systype = 1;
            cin >> systype;

            switch (systype) {
            case 1:
            {
                string input = "m.txt";
                cout << "Inputdatei: ";
                cin >> input;
                ifstream infile(input);
                infile >> dim;
                LINEAR_SYSTEM lsys(dim);
                for(int i=0; i<dim;i++)
                    for(int j=0; j<dim;j++)
                        infile >> lsys.A(i,j);
                for(int i=0; i<dim;i++)
                    infile >> lsys.b(i);
                infile.close();

                LU lsol;
                lsol.solve(lsys);
                cout << "Lösung mit Erwartungswerten:" << endl << lsys.x() << endl;

                LINEAR_MC_SOLVER lmcsol(lsys);
                double range;
                cout << "Wieviele Berechnungen sollen durchgeführt werden? ";
                cin >> num;
                cout << "Standardabweichung: ";
                cin >> range;
                bool A,b;
                cout << "A verändern? ";
                cin >> A;
                cout << "b verändern? ";
                cin >> b;
                DATASET data = lmcsol.psolve(num,range,A,b);
                data.writeToFile("data.txt");
                unlock2 = true;
                break;
            }
            case 2:
            {
                cout << "Toy" << endl;
                const int NS=1,NP=1;
                TOY<double> nlsys;

                cout << "X (z.B.1): ";
                cin >> nlsys.x(0);
                cout << "P (z.B.2): ";
                cin >> nlsys.p(0);

                double range;
                cout << "Wieviele Berechnungen sollen durchgeführt werden? ";
                cin >> num;
                cout << "Standardabweichung: ";
                cin >> range;
                bool x,p;
                cout << "x verändern? ";
                cin >> x;
                cout << "p verändern? ";
                cin >> p;

                NONLINEAR_MC_SOLVER<TOY<double>, double, NS, NP> nlmcsol(nlsys);
                DATASET data = nlmcsol.psolve(num, range, x, p);
                data.writeToFile("data.txt");
                unlock2 = true;
                break;
            }
            case 3:
            {
                cout << "LV" << endl;
                const int NS=2,NP=4;
                LOTKA_VOLTERRA<double> ode;

                cout << "Beute (z.B.1000): ";
                cin >> ode.x(0);
                cout << "Räuber (z.B.20): ";
                cin >> ode.x(1);

                ode.p(0)=0.015; ode.p(1)=0.0001; ode.p(2)=0.03; ode.p(3)=0.0001;

                double range, T_end, nts;
                cout << "Wieviele Berechnungen sollen durchgeführt werden? ";
                cin >> num;
                cout << "Standardabweichung: ";
                cin >> range;
                cout << "Endzeit: ";
                cin >> T_end;
                cout << "Zeitschritte: ";
                cin >> nts;
                bool x,p;
                cout << "x verändern? ";
                cin >> x;
                cout << "p verändern? ";
                cin >> p;

                ODE_MC_SOLVER<LOTKA_VOLTERRA<double>, double, NP, NS> odesol(ode);
                DATASET data = odesol.psolve(num, range);
                data.writeToFile("data.txt");
                unlock2 = true;
                break;
            }
            default:
                break;
            }

            break;
        }
        case 2:
        {
            if(!unlock2) break;
            DATASET data("data.txt");
            //int i = 1;
            //cout << "Welchen Parameter möchten Sie betrachten? ";
            //cin >> i;
            data.displayRows();
            break;

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
