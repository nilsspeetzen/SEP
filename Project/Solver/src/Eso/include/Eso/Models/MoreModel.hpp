#ifndef MORE_MODEL_HPP
#define MORE_MODEL_HPP

#include "Eso/EqGroup.hpp"
#include "Eigen/Core"

template<typename RealType>
class MoreModel {
private:
	static constexpr int numVars = 15;
public:

  MoreModel() {}
  // Equation group no. 0
  static constexpr inline int numEquations(const EqGroup<0>) { return numVars;}

  inline RealType eval(EqGroup<0>, EqIndex<0>)  const{
    return 2*x[0] + x[1] + x[2] + x[3] + x[4] - 6;
  }
  inline RealType eval(EqGroup<0>, EqIndex<1>)  const{
    return  x[0] + 2*x[1] + x[2] + x[3] + x[4] - 6;
  }
  inline RealType eval(EqGroup<0>, EqIndex<2>) const{
    return x[0] + x[1] + 2*x[2] + x[3] + x[4] - 6;
  }
  inline RealType eval(EqGroup<0>, EqIndex<3>) const{
    return x[0] + x[1] + x[2] + 2*x[3] + x[4] - 6;
  }
  inline RealType eval(EqGroup<0>, EqIndex<4>) const{
    return x[0] * x[1] * x[2] * x[3] * x[4] - 1;
  }
  inline RealType eval(EqGroup<0>, EqIndex<5>) const{
      return  eval(EqGroup<0>{}, EqIndex<0>{}) + (2- x[5])*x[5]       - 2*x[6] +1 ;
  }
  inline RealType eval(EqGroup<0>, EqIndex<6>) const{
    return eval(EqGroup<0>{}, EqIndex<1>{}) + (3- x[6])*x[6] -x[5] - 2*x[7] +1 ;
  }
  inline RealType eval(EqGroup<0>, EqIndex<7>) const{
    return  eval(EqGroup<0>{}, EqIndex<2>{})  + (3- x[7])*x[7] -x[6] - 2*x[8] +1 ;
  }
  inline RealType eval(EqGroup<0>, EqIndex<8>) const{
    return eval(EqGroup<0>{}, EqIndex<3>{})  + (3- x[8])*x[8] -x[7] - 2*x[9] +1 ;
  }
  inline RealType eval(EqGroup<0>, EqIndex<9>) const{
    return eval(EqGroup<0>{}, EqIndex<4>{})  + (1- x[9])*x[9] -x[8]          +1 ;
  }
  inline RealType eval(EqGroup<0>, EqIndex<10>) const{
    using std::sin;
    return eval(EqGroup<0>{}, EqIndex<5>{}) + 5 - cosinusSum() + (1-cos(x[10])) - sin(x[10]);
  }
  inline RealType eval(EqGroup<0>, EqIndex<11>) const{
    using std::sin;
    return eval(EqGroup<0>{}, EqIndex<6>{}) + 5 - cosinusSum() + 2*(1-cos(x[11])) - sin(x[11]);
  }
  inline RealType eval(EqGroup<0>, EqIndex<12>) const{
    using std::sin;
    return eval(EqGroup<0>{}, EqIndex<7>{}) + 5 - cosinusSum() + 3*(1-cos(x[12])) - sin(x[12]);
  }
  inline RealType eval(EqGroup<0>, EqIndex<13>) const{
    using std::sin;
    return eval(EqGroup<0>{}, EqIndex<8>{}) + 5 - cosinusSum() + 4*(1-cos(x[13]))- sin(x[13]);
  }
  inline RealType eval(EqGroup<0>, EqIndex<14>) const{
    using std::sin;
    return eval(EqGroup<0>{}, EqIndex<9>{}) + 5 - cosinusSum() + 5*(1-cos(x[14])) - sin(x[14]);
  }

  static constexpr inline int numVariables(VarGroup<0>) { return numVars;}

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

  static std::array<double,numVars> getSolution()  {
    std::array<double, numVars> solution;
    for(size_t i=0; i < 10; ++i) solution[i] = 1;
    solution[10] = 0.06175491892349205;
    solution[11] = 0.06393992541842815;
    solution[12] = 0.06648669739226684;
    solution[13] = 0.06953055812954316;
    solution[14] = 0.3214890943456122;
    return solution;
  }

private:
  Eigen::Matrix<RealType, numVars,1> x;
  inline RealType cosinusSum() const
  {
    using std::cos;
    return cos(x[10])+cos(x[11])+cos(x[12])+cos(x[13])+cos(x[14]);
  }


};

#endif // MORE_MODEL_HPP
