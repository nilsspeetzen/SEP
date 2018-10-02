#ifndef SIMPLE_MODEL_HPP
#define SIMPLE_MODEL_HPP

#include "Eso/EqGroup.hpp"
#include "Eigen/Core"
#include <array>

template<typename RealType>
class SimpleModel {
private:
	static constexpr int numVars = 3;
public:

  SimpleModel() {}
  // Equation group no. 0
  static constexpr inline int numEquations(const EqGroup<0>) { return numVars;}

  inline RealType eval(EqGroup<0>, EqIndex<0>) const {
    return x[0] + 2 * x[1]  * x[2];
  }
  inline RealType eval(EqGroup<0>, EqIndex<1>) const {
    return x[1]*x[1] - 1.0;
  }
  inline RealType eval(EqGroup<0>, EqIndex<2>) const {
     return x[1] + x[2] + x[0];
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

  static std::array<double, numVars> getSolution()  {
    std::array<double, numVars> solution;
    solution[0] = -2;
    solution[1] =  1;
    solution[2] =  1;
    return solution;
  }

private:
  Eigen::Matrix<RealType, numVars,1> x;


};

#endif // SIMPLE_MODEL_HPP
