// A Case Study in Simulation Software Engineering with C++
// Uwe Naumann, STCE, RWTH Aachen, 2018
#ifndef LIN_SYS_GENERIC_H
#define LIN_SYS_GENERIC_H

#include "LU"
#include "QR"
using namespace Eigen;

/**
 * @file lin_sys.h
 * @brief Lineare Systeme
 */

/**
 * @brief Lineare Systeme
 * Basierend auf lin_sys.h von Uwe Naumann
 */

template<typename TA=double, int N=Dynamic, typename Tb=TA, typename Tx=Tb>
class LINEAR_SYSTEM
{
  typedef Matrix<TA,N,N> MTA;
  typedef Matrix<Tb,N,1> VTb;
  typedef Matrix<Tx,N,1> VTx;
protected:
  VTx _x; VTb _b; MTA _A;
public:
  LINEAR_SYSTEM(const int& n)
    : _x(VTx::Zero(n)),_b(VTb::Zero(n)),_A(MTA::Zero(n,n)) {}
  VTx& x() { return _x; }
  Tx& x(int i) { return _x(i); }
  VTb& b() { return _b; }
  Tb& b(int i) { return _b(i); }
  MTA& A() { return _A; }
  TA& A(int i, int j) { return _A(i,j); }
};

template<typename TA=double, int N=Dynamic, typename Tb=TA, typename Tx=Tb>
class LINEAR_SOLVER
{
public:
  virtual void solve(LINEAR_SYSTEM<TA,N,Tb,Tx>&)=0;
};

template<typename TA=double, int N=Dynamic, typename Tb=TA, typename Tx=Tb>
class LU : public LINEAR_SOLVER<TA,N,Tb,Tx>
{
  typedef Matrix<TA,N,N> MTA;
public:
  void solve(LINEAR_SYSTEM<TA,N,Tb,Tx>& s) {
    PartialPivLU<MTA> LU(s.A());
    s.x()=LU.solve(s.b());
  }
};

template<typename TA=double, int N=Dynamic, typename Tb=TA, typename Tx=Tb>
class QR : public LINEAR_SOLVER<TA,N,Tb,Tx>
{
  typedef Matrix<TA,N,N> MTA;
public:
  void solve(LINEAR_SYSTEM<TA,N,Tb,Tx>& s) {
    HouseholderQR<MTA> QR(s.A());
    s.x()=QR.solve(s.b());
  }
};

#endif
