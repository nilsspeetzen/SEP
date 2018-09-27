#ifndef YASP_HPP_
#define YASP_HPP_

#include<set>
#include<iostream>
#include<fstream>
#include <cfloat>


  class  Yasp {
    public :
	  std::set<int> nz;
    Yasp(const int index) : nz{index} {}
       Yasp(const double& x);
       Yasp(const Yasp& x) = default;
       Yasp();
       Yasp& operator=(const Yasp& x);
       Yasp& operator=(const double& a);
  };

  Yasp  operator*(const Yasp& x1, const Yasp& x2);
  inline Yasp  operator*(const double&, const Yasp& x) {return x;}
  inline Yasp  operator*(const Yasp& x, const double& ) {return x;}

  Yasp  operator+(const Yasp& x1, const Yasp& x2);
  inline Yasp  operator+(const double& , const Yasp& x) {return x;}
  inline Yasp  operator+(const Yasp& x, const double& ) {return x;}

  Yasp  operator-(const Yasp& x1, const Yasp& x2);
  inline Yasp  operator-(const double& , const Yasp& x) {return x;}
  inline Yasp  operator-(const Yasp& x, const double&) {return x;}

  Yasp  operator/(const Yasp& x1, const Yasp& x2);
  inline Yasp  operator/(const double&, const Yasp& x) {return x;}
  inline Yasp  operator/(const Yasp& x, const double&) {return x;}

  inline Yasp operator-(const Yasp& x) { return x; }

// prints Yasp to a given file in exatly the same format as ADOL-C's methode jac_pat does. The diff operator can be used on both files.
  void  print_Yasp(std::ostream &tgtstream, Yasp *spf, int n);

  inline Yasp  sin(const Yasp& x) {return x;}
  inline Yasp  cos(const Yasp& x) {return x;}
  inline Yasp  exp(const Yasp& x) {return x;}
  inline Yasp  atan(const Yasp& x) {return x;}
  inline Yasp  sqrt(const Yasp& x) {return x;}
  inline Yasp  log(const Yasp& x) {return x;}
  inline Yasp  tanh(const Yasp& x) {return x;}
  inline Yasp  sinh(const Yasp& x) {return x;}
  inline Yasp  cosh(const Yasp& x) {return x;}
  inline Yasp  asin(const Yasp& x) {return x;}
  inline Yasp  acos(const Yasp& x) {return x;}
  Yasp  pow(const Yasp& x1, const Yasp& x2);
  inline Yasp  pow(const Yasp& x, const double& ) {return x;}
  inline Yasp  pow(const double& , const Yasp& x) {return x;}


  

#endif //YASP_HPP_

