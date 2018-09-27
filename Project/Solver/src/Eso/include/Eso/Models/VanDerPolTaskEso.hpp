#ifndef VAN_DER_POL_TASK_ESO_HPP
#define VAN_DER_POL_TASK_ESO_HPP

#include "Eso/Models/VanDerPolTask.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "Eso/Dco1Model.hpp"
#include <array>

template<typename RealType>
class VanDerPolTaskEso {
public:
  using States = VarGroup<0>;
  using DiffStates = VarGroup<1>;
  using Controls = VarGroup<2>;
  using Parameters = VarGroup<3>;
  using DaeEquations = EqGroup<0>;
  using ObjectiveFunc = EqGroup<4>;
  using ControlFunc = EqGroup<5>;
  using InitialValueFunc = EqGroup<1>;
  using InitialTimeFunc = EqGroup<2>;
  using FinalTimeFunc = EqGroup<3>;

  static constexpr States states = States{};
  static constexpr DiffStates diffStates = DiffStates{};
  static constexpr Controls controls = Controls{};
  static constexpr Parameters parameters = Parameters{};
  static constexpr DaeEquations daeEquations = DaeEquations{};
  static constexpr ObjectiveFunc objectiveFunc = ObjectiveFunc{};
  static constexpr ControlFunc controlFunc = ControlFunc{};
  static constexpr InitialValueFunc initialValueFunc = InitialValueFunc{};
  static constexpr InitialTimeFunc initialTimeFunc = InitialTimeFunc{};
  static constexpr FinalTimeFunc finalTimeFunc = FinalTimeFunc{};


  VanDerPolTaskEso() { eso.setVariables(controls,std::array<RealType,2>{1,1}); eso.setVariables(diffStates,std::array<RealType,3> {0,0,0});}

  static constexpr inline int numStates() { return EsoType::numVariables(States()); }
  static constexpr inline int numControls() {return EsoType::numVariables(Controls{}); }
  static constexpr inline int numParameters() { return EsoType::numVariables(Parameters{}); }
  static constexpr inline int numObjectives() {return EsoType::numEquations(ObjectiveFunc{}); }

  template<int I, int K,typename T, typename U>
  inline void evalAdjoints (EqGroup<I> eqGroup, VarGroup<K> varGroup, const T& adjointY, U& adjointX){
    Eigen::SparseMatrix<RealType> jac;
    evalJacobianPattern(eqGroup,varGroup,jac);
    evalJacobianValues(eqGroup,varGroup,jac);
    constexpr int numEqs = EsoType::numEquations(EqGroup<I>{});
    constexpr int numVars = EsoType::numVariables(VarGroup<K>{});
    Eigen::Matrix<RealType,1      ,numEqs> tmpAdjointY;
    Eigen::Matrix<RealType,numVars,1     > tmpAdjointX;
    for(int i = 0; i < numEqs; ++i) tmpAdjointY[i] = adjointY[i];
    tmpAdjointX = tmpAdjointY * jac;
    for(int i = 0; i < numVars; ++i) adjointX[i] = tmpAdjointX[i];

  }

  template<int I, typename V>
  inline void evalAll(EqGroup<I> eqGroup, V& residuals){
    eso.evalAll(eqGroup,residuals);
  }

  template<int I, int K, typename _Scalar>
  inline void evalJacobianValues(EqGroup<I> eqGroup, VarGroup<K> varGroup,Eigen::SparseMatrix<_Scalar>& jacobian)
  {
    eso.evalJacobianValues(eqGroup,varGroup,jacobian);
  }

  template<int I, int K, typename _Scalar>
  inline void evalJacobianPattern(EqGroup<I> eqGroup, VarGroup<K> varGroup,Eigen::SparseMatrix<_Scalar>& jacobian)
  {
    eso.evalJacobianPattern(eqGroup,varGroup,jacobian);
  }

  template<int I, int K, typename _Scalar, int _Rows, int _Cols, int _Options,
           int _MaxRows, int _MaxCols>
  inline void evalJacobianValues(EqGroup<I> eqGroup, VarGroup<K> varGroup, Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                  _MaxRows, _MaxCols >& jacobian)
  {
     eso.evalJacobianValues(eqGroup,varGroup,jacobian);
  }

  template<int I, int K, typename _Scalar, int _Rows, int _Cols, int _Options,
           int _MaxRows, int _MaxCols>
  inline void evalJacobianPattern(EqGroup<I> eqGroup, VarGroup<K> varGroup, Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                  _MaxRows, _MaxCols >& jacobian)
  {
    eso.evalJacobianPattern(eqGroup,varGroup,jacobian);
  }

  template<int K, class V>
  inline void setVariables(VarGroup<K> varGroup, const V& values)
  {
    eso.setVariables(varGroup,values);
  }

private:
  template<typename T>
  using TangentModel = Dco1Model<VanDerPolTask,T>;

  using EsoType = FirstOrderEso<TangentModel,RealType>;
  EsoType eso;
};

#endif // VAN_DER_POL_TASK_ESO_HPP
