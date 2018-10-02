#ifndef VAN_DER_POL_TASK_HPP
#define VAN_DER_POL_TASK_HPP

#include "Eso/Models/VanDerPolModel.hpp"
#include "Eigen/Core"

template<typename RealType>
class VanDerPolTask : public VanDerPolModel<RealType>
{
public:



  // Equation group no. 1 (intial values)
  static constexpr inline int numEquations(EqGroup<1>) { return 3;}
  inline RealType eval(EqGroup<1>, EqIndex<0>) const {
    return p[2];
  }
  inline RealType eval(EqGroup<1>, EqIndex<1>) const {
    return p[3];
  }
  inline RealType eval(EqGroup<1>, EqIndex<2>) const {
    return p[4];
  }


  //Equation group no. 2 (initial time)
  static constexpr inline int numEquations(EqGroup<2>) { return 1;}
  inline RealType eval(EqGroup<2>, EqIndex<0>) const {
    return p[0];
  }

  //Equation group no. 3 (final time)
  static constexpr inline int numEquations(EqGroup<3>) { return 1;}
  inline RealType eval(EqGroup<3>, EqIndex<0>) const {
    return p[1];
  }

  //Equation group no. 4 (objective)
  static constexpr inline int numEquations(EqGroup<4>) { return 3;}

  inline RealType eval(EqGroup<4>, EqIndex<0>) const {
    return getVariable(VarGroup<0>{},0);
  }
  inline RealType eval(EqGroup<4>, EqIndex<1>) const {
    return getVariable(VarGroup<0>{},1);
  }
  inline RealType eval(EqGroup<4>, EqIndex<2>) const {
    return getVariable(VarGroup<0>{},2);
  }

  //Equation group no. 5 (controls)
  static constexpr inline int numEquations(EqGroup<5>) {return  2;}
  inline RealType eval(EqGroup<5>, EqIndex<0>) const {
    return p[5];
  }
  inline RealType eval(EqGroup<5>, EqIndex<1>) const {
    return p[6];
  }


  // parameters
  static constexpr inline int numVariables(VarGroup<3>) { return 7;}
  inline void setVariable(VarGroup<3>,const int varIndex, const RealType& value){
    p[varIndex] = value;
  }

  inline RealType getVariable(VarGroup<3>,const int varIndex)  const {
    return p[varIndex];
  }

  using VanDerPolModel<RealType>::numVariables;
  using VanDerPolModel<RealType>::numEquations;
  using VanDerPolModel<RealType>::setVariable;
  using VanDerPolModel<RealType>::getVariable;
  using VanDerPolModel<RealType>::eval;

private:

  Eigen::Matrix<RealType,7,1> p;

};




#endif // VAN_DER_POL_TASK_HPP
