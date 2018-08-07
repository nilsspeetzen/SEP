// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018

#include<iostream>
using namespace std;

#include "toy.h"
#include "lotka_volterra.h"
#include "diffusion.h"

int main() {
{
  cout << "Toy" << endl;
  const int NS=1,NP=1; 
  TOY<double> nlsys;
  nlsys.x(0)=1; nlsys.p(0)=2;
  QR<double,NS> lsol;
  NEWTON<double,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-7;
  nlsol.solve(nlsys);
  cout << nlsys.x() << endl;
}
{
  cout << "Lotka-Volterra" << endl;
  const int NS=2,NP=4; 
  LOTKA_VOLTERRA<double> nlsys;
  nlsys.x(0)=1000; nlsys.x(1)=20;
  nlsys.p(0)=0.015; nlsys.p(1)=0.0001; nlsys.p(2)=0.03; nlsys.p(3)=0.0001;
  QR<double,NS> lsol;
  NEWTON<double,NP,NS> nlsol(lsol);
  nlsol.eps()=1e-7;
  nlsol.solve(nlsys);
  cout << nlsys.x() << endl;
}
{
  int ns;
  const int NP=3; 
  cout << "Diffusion" << endl;
  cout << "ns="; cin >> ns;
  DIFFUSION<> nlsys(ns);
  nlsys.p(0)=0.1; nlsys.p(1)=0.; nlsys.p(2)=1400.; 
  QR<> lsol;
  NEWTON<double,NP> nlsol(lsol);
  nlsol.eps()=1e-7;
  nlsol.solve(nlsys);
  cout << nlsys.x() << endl;
}
  return 0;
}

