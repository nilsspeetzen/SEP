#ifndef VAN_DER_POL_MODEL_HPP
#define VAN_DER_POL_MODEL_HPP

#include "Eso/EqGroup.hpp"
#include "Eigen/Core"

template<typename RealType>
class VanDerPolModel {
public:

  VanDerPolModel() {}
  // Equation group no. 0
  static constexpr inline int numEquations(const EqGroup<0>) { return 3;}

  inline RealType eval(EqGroup<0>, EqIndex<0>) const {
    return x(1) - der_x(0);
  }
  inline RealType eval(EqGroup<0>, EqIndex<1>) const {
    return ((1.0-x(0)*x(0))*x(1)-x(0)+u(1))/u(0) - der_x(1);
  }
  inline RealType eval(EqGroup<0>, EqIndex<2>) const {
    return x(0)*x(0) + x(1)*x(1) + u(1)*u(1) - der_x(2);
  }

  static constexpr inline int numVariables(VarGroup<0>) { return 3;}
  static constexpr inline int numVariables(VarGroup<1>) { return 3;}
  static constexpr inline int numVariables(VarGroup<2>) { return 2;}

  template<int K>
  inline void setVariable(VarGroup<K> varGroup, const int index, const RealType& value){
    data[index+variableIndexOffset(varGroup)] = value;
  }

  template<int K>
  inline RealType getVariable(VarGroup<K> varGroup, const int index) const {
    return data[index+variableIndexOffset(varGroup)];
  }

  inline RealType x(const int i) const { return getVariable(VarGroup<0>{},i); }
  inline RealType der_x(const int i) const { return getVariable(VarGroup<1>{},i); }
  inline RealType u(const int i) const { return getVariable(VarGroup<2>{},i); }


private:


  static constexpr inline int variableIndexOffset(VarGroup<0>){
    return 0;
  }

  template<int K>
  static constexpr inline int variableIndexOffset(VarGroup<K>){
    return variableIndexOffset(VarGroup<K-1>{}) + numVariables(VarGroup<K-1>{});
  }

  static constexpr int lenData = numVariables(VarGroup<0>{})
      + numVariables(VarGroup<1>{})
      + numVariables(VarGroup<2>{}) ;
  Eigen::Matrix<RealType,lenData,1> data;


};









#endif //VAN_DER_POL_MODEL_HPP
