#ifndef SEP_CES_HPP
#define SEP_CES_HPP

#include "Eso/EqGroup.hpp"
#include "Eigen/Core"

template<typename RealType>
class SepCes {
public:

  SepCes() {}
  // Equation group no. 0
  static constexpr inline int numEquations(const EqGroup<0>) { return 16;}

  inline RealType eval(EqGroup<0>, EqIndex<0>) const {
      return   F * x_f_1 + L_in() * x_in_1() - L_out() * x_1() - V_out * y_1();
  }
  inline RealType eval(EqGroup<0>, EqIndex<1>) const {
      return  F * x_f_2 + L_in() * x_in_2() - L_out() * x_2() - V_out * y_2();
  }
  inline RealType eval(EqGroup<0>, EqIndex<2>) const {
    return y_1() - k_1() * x_1();
  }
  inline RealType eval(EqGroup<0>, EqIndex<3>) const {
    return y_2() - k_2() * x_2();
  }
  inline RealType eval(EqGroup<0>, EqIndex<4>) const {
    return x_1() + x_2() - 1;
  }
  inline RealType eval(EqGroup<0>, EqIndex<5>) const {
    return y_1() + y_2() - 1;
  }
  inline RealType eval(EqGroup<0>, EqIndex<6>) const {
      return k_1() - p_1()/p_g;
  }
  inline RealType eval(EqGroup<0>, EqIndex<7>) const {
      return k_2() - p_2()/p_g;
  }
  inline RealType eval(EqGroup<0>, EqIndex<8>) const {
     return  log(p_1()) - (a(1,1) + a(1,2) /(T() + a(1,3))
                           + a(1,4)*T() + a(1,5) * log(T()) + a(1,6) * pow(T(),a(1,7)));
  }
  inline RealType eval(EqGroup<0>, EqIndex<9>) const {
      return  log(p_2()) - (a(2,1) + a(2,2) /(T() + a(2,3))
                         + a(2,4)*T() + a(2,5) * log(T()) + a(2,6) * pow(T(),a(2,7)));
  }

  inline RealType eval(EqGroup<0>, EqIndex<10>) const {
    return x_in_1()-0.5;
  }
  inline RealType eval(EqGroup<0>, EqIndex<11>) const {
    return x_in_2()-0.5;
  }
  inline RealType eval(EqGroup<0>, EqIndex<12>) const {
      return  y_in_1()-0.5;
  }
  inline RealType eval(EqGroup<0>, EqIndex<13>) const {
      return  y_in_2()-0.5;
  }
  inline RealType eval(EqGroup<0>, EqIndex<14>) const {
    return L_in();
  }
  inline RealType eval(EqGroup<0>, EqIndex<15>) const {
    return V_in();
  }


  static constexpr inline int numVariables(VarGroup<0>) { return 16;}

  template<int K>
  inline void setVariable(VarGroup<K> , const int index, const RealType& value){
    static_assert (K==0,"only one variable group present" );
    x[index] = value;
  }

  template<int K>
  inline RealType getVariable(VarGroup<K> , const int index) const {
    static_assert (K==0,"only one variable group present" );
    return x[index];
  }



  inline RealType L_in() const {return  x[0];}
  inline RealType L_out() const {return  x[1];}
  inline RealType V_in()  const {return  x[2];}
  inline RealType T()  const {return  x[3];}
  inline RealType x_1()  const {return  x[4];}
  inline RealType x_2()  const {return  x[5];}
  inline RealType y_1()  const {return  x[6];}
  inline RealType y_2()  const {return  x[7];}
  inline RealType k_1()  const {return  x[8];}
  inline RealType k_2()  const {return  x[9];}
  inline RealType p_1()  const {return  x[10];}
  inline RealType p_2()  const {return  x[11];}
  inline RealType x_in_1()  const {return  x[12];}
  inline RealType x_in_2()  const {return  x[13];}
  inline RealType y_in_1()  const {return  x[14];}
  inline RealType y_in_2()  const {return  x[15];}
  inline double a(const int i,const int j)  const {return  aData[7*(i-1)+j-1];}
  double V_out = 50.;
  double x_f_1 = 0.5;
  double x_f_2 = 0.5;
  double p_g = 1.013e5;
  double F = 100;
private:
  static constexpr int lenStates = numVariables(VarGroup<0>{});
  Eigen::Matrix<RealType,lenStates,1> x;
  std::array<double,14> aData{73.649,  -7258.2, 0., 0., -7.3037, 4.1653e-6,  2.,
                                79.276, -10105,   0., 0., -7.521,  7.3408e-19, 6.};


};

#endif // SEP_CES_HPP
