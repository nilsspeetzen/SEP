#include <cmath>
#include <algorithm>
#include <functional>
#include "Yasp/Yasp.hpp"

  Yasp::Yasp(const double& ) { nz.clear(); }
  Yasp::Yasp() { nz.clear(); }

  Yasp& Yasp::operator=(const Yasp& x) {
    if (this==&x) return *this;
    nz=x.nz;
    return *this;
  }

  Yasp& Yasp::operator=(const double& ) {
    nz.clear();
    return *this;
  }



  Yasp operator*(const Yasp& x1, const Yasp& x2) {
    Yasp tmp;
  std::set<int>::const_iterator i;
    for (i=x1.nz.begin();i!=x1.nz.end();i++) tmp.nz.insert(*i);
    for (i=x2.nz.begin();i!=x2.nz.end();i++) tmp.nz.insert(*i);
    return tmp;
  }


  Yasp operator+(const Yasp& x1, const Yasp& x2) {
    Yasp tmp;
    std::set<int>::const_iterator i;
    for (i=x1.nz.begin();i!=x1.nz.end();i++) tmp.nz.insert(*i);
    for (i=x2.nz.begin();i!=x2.nz.end();i++) tmp.nz.insert(*i);
    return tmp;
  }


  Yasp operator-(const Yasp& x1, const Yasp& x2) {
    Yasp tmp;
    std::set<int>::const_iterator i;
    for (i=x1.nz.begin();i!=x1.nz.end();i++) tmp.nz.insert(*i);
    for (i=x2.nz.begin();i!=x2.nz.end();i++) tmp.nz.insert(*i);
    return tmp;
  }


  Yasp operator/(const Yasp& x1, const Yasp& x2) {
    Yasp tmp;
    std::set<int>::const_iterator i;
    for (i=x1.nz.begin();i!=x1.nz.end();i++) tmp.nz.insert(*i);
    for (i=x2.nz.begin();i!=x2.nz.end();i++) tmp.nz.insert(*i);
    return tmp;
  }



  Yasp pow(const Yasp& x1, const Yasp& x2) {
    Yasp tmp;
    std::set<int>::const_iterator i;
    for (i=x1.nz.begin();i!=x1.nz.end();i++) tmp.nz.insert(*i);
    for (i=x2.nz.begin();i!=x2.nz.end();i++) tmp.nz.insert(*i);
    return tmp;
  }


  void print_Yasp(std::ostream &tgtstream, Yasp *spf, int n){
    std::set<int>::iterator pp;
    
    for (int i=0;i<n;i++) {
       for (pp=spf[i].nz.begin();pp!=spf[i].nz.end();pp++) {
          tgtstream <<  *pp << " " ;
       }
       tgtstream << "\n";
    }  
 }
