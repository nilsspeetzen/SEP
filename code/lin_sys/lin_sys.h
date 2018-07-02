#ifndef LIN_SYS_H
#define LIN_SYS_H

#include <Eigen/LU>
#include <Eigen/QR>
using namespace Eigen;
typedef Matrix<double,Dynamic,Dynamic> MT;
typedef Matrix<double,Dynamic,1> VT;

class LINEAR_SYSTEM 
{
protected:
  VT _x, _b; MT _A;
public:
  LINEAR_SYSTEM(const int& n) 
    : _x(VT::Zero(n)),_b(VT::Zero(n)),_A(MT::Zero(n,n)) {}
  VT& x() { return _x; }
  double& x(int i) { return _x(i); }
  VT& b() { return _b; }
  double& b(int i) { return _b(i); }
  MT& A() { return _A; }
  double& A(int i, int j) { return _A(i,j); }
};

class LINEAR_SOLVER 
{ 
  virtual void solve(LINEAR_SYSTEM& s)=0;
};

class LU : public LINEAR_SOLVER
{
public:
  void solve(LINEAR_SYSTEM& s) {
    PartialPivLU<MT> LU(s.A()); 
    s.x()=LU.solve(s.b());
  }
};

class QR : public LINEAR_SOLVER
{
public:
  void solve(LINEAR_SYSTEM& s) {
    HouseholderQR<MT> QR(s.A()); 
    s.x()=QR.solve(s.b());
  }
};

#endif
