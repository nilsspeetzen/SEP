// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include<cassert>
#include<cstdlib>
#include<iostream>
#include<cfloat>
#include<cmath>
using namespace std;

#include "diffusion.h"
typedef Matrix<double,Dynamic,1> VT;

int main(int c, char* v[]) {
  assert(c==2); int ns=atoi(v[1]); const int NP=3; 
  DIFFUSION<> nlsys(ns);
  nlsys.p(0)=.1; nlsys.p(1)=0.; nlsys.p(2)=1400.; 
  QR<> lsol;
  NEWTON<double,NP> nlsol(lsol);
  nlsol.eps()=1e-7;
  for (int i=0;i<NP;i++) {
    const double eps=sqrt(DBL_EPSILON);
    double h=(abs(nlsys.p(i))<1) ? eps : eps*abs(nlsys.p(i));
    nlsys.p(i)-=h; nlsol.solve(nlsys); VT xp=nlsys.x();
    nlsys.p(i)+=2*h; nlsol.solve(nlsys);
    cout << (nlsys.x(ns/2)-xp(ns/2))/(2*h) << endl;
    nlsys.p(i)-=h;
  }
  return 0;
}

