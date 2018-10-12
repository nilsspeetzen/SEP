#ifndef FIRST_ORDER_ESO_HPP
#define FIRST_ORDER_ESO_HPP

#include "EqGroup.hpp"
#include "Yasp/Yasp.hpp"
#include "Eigen/Core"
#include "Eigen/SparseCore"

template<template<typename> class TangentModelType, typename RealType>
class FirstOrderEso
{

public:
  FirstOrderEso() {}
  template<int K>
  inline void setVariable(VarGroup<K> varGroup, int variableIndex, const RealType& variableValue)
  {
    tangentModel.setVariable(varGroup,variableIndex,variableValue);
  }

  template<int I>
  static constexpr inline int numEquations(EqGroup<I> eqGroup) {
    return TangentModelType<RealType>::numEquations(eqGroup);
  }

  template<int K>
  static constexpr inline int numVariables(VarGroup<K> varGroup) {
    return TangentModelType<RealType>::numVariables(varGroup);
  }

  template<int K, class V>
  inline void setVariables(VarGroup<K> varGroup, const V& values){
    constexpr int numVars = TangentModelType<RealType>::numVariables(VarGroup<K>{});
    for(int i=0; i <numVars;++i)
      setVariable(varGroup,i,values[i]);
  }

  template<int K, class Indices, class Values>
  inline void setVariables(VarGroup<K> varGroup, const Indices& indices, const Values& values)
  {
      using std::cbegin;
      using std::cend;
      int index = 0;
      for(auto p = begin(indices); p != cend(indices); ++p,++index)
      {
        setVariable(varGroup,*p,values[index]);
      }
  }

  template<int I>
  inline RealType eval(EqGroup<I> eqGroup,int equationIndex)
  {
    return tangentModel.eval(eqGroup,equationIndex);
  }

  template<int I, class EquationIndices, class Residuals>
  inline void evalBlock(EqGroup<I> eqGroup,const EquationIndices& indices, Residuals& residuals)
  {
      int index=0;
      for(auto i : indices){
          residuals[index++] = tangentModel.eval(eqGroup,i);
      }
  }

  template<int I, typename  V>
  inline void  evalAll(EqGroup<I> eqGroup, V& residuals)
  {
    tangentModel.evalAll(eqGroup,residuals);
  }

  template<int I, int K>
  inline RealType evalDerivative(EqGroup<I> eqGroup, int equationIndex, VarGroup<K> varGroup, int variableIndex){
    tangentModel.setTangent(varGroup,variableIndex,1.0);
    RealType result = tangentModel.evalTangent(eqGroup,equationIndex);
    tangentModel.setTangent(varGroup,variableIndex,0.0);
    return result;
  }

  template<int I, int K, typename _Scalar, int _Rows, int _Cols, int _Options,
           int _MaxRows, int _MaxCols>
  inline void evalJacobianPattern(EqGroup<I>, VarGroup<K> ,
                                  Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                  _MaxRows, _MaxCols >& jacobian)
  {
    constexpr int numEqns = numEquations(EqGroup<I>{}); // number of equations
    constexpr int numVars = numVariables(VarGroup<K>{}); // number of variables
    jacobian.resize(numEqns,numVars);
  }

  template<int I, int K, typename _Scalar, int _Rows, int _Cols, int _Options,
           int _MaxRows, int _MaxCols>
  inline void evalJacobianValues(EqGroup<I> eqGroup, VarGroup<K> varGroup,
                                  Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                  _MaxRows, _MaxCols >& jacobian)
  {
    constexpr int numEqns = numEquations(EqGroup<I>{}); // number of equations
    constexpr int numVars = numVariables(VarGroup<K>{}); // number of variables
    for(int eqIndex=0; eqIndex < numEqns;++eqIndex) {
      for(int varIndex=0; varIndex < numVars; ++varIndex){
        jacobian(eqIndex,varIndex) =  evalDerivative(eqGroup,eqIndex,varGroup,varIndex);
      }
    }
  }


  template<int I, int K,
           class EquationIndices,
           class VariableIndices,
           typename Jacobian>
  void evalBlockJacobian(EqGroup<I> eqGroup, VarGroup<K> varGroup, const EquationIndices& equationIndices,
                         const VariableIndices& stateIndices,
                         Jacobian& jacobian)
  {
    using std::cbegin;
    using std::cend;
    int jcol =0;
    for(auto j = cbegin(stateIndices); j != cend(stateIndices); ++j,++jcol) {
      int irow = 0;
      for(auto i = cbegin(equationIndices); i != cend(equationIndices);++i,++irow)
        jacobian(irow,jcol) = evalDerivative(eqGroup,*i,varGroup,*j);
    }
  }


  template<int I, int K, typename _Scalar>
  inline void evalJacobianValues(EqGroup<I> eqGroup, VarGroup<K> varGroup,
                                  Eigen::SparseMatrix<_Scalar>& jacobian)
  {
    const int numVars = jacobian.cols(); // number of variables

    _Scalar *jacobianValues = jacobian.valuePtr();
    auto *rowIndices  = jacobian.innerIndexPtr();
    auto *colPointers = jacobian.outerIndexPtr();

    for(int varIndex=0; varIndex < numVars; ++varIndex){
      for(int i=colPointers[varIndex]; i < colPointers[varIndex+1];++i ) {
        auto eqIndex = rowIndices[i];
        jacobianValues[i] = evalDerivative(eqGroup,eqIndex,varGroup,varIndex);
      }
    }
  }


  template<int I, int K, typename _Scalar>
  inline void evalJacobianPattern(EqGroup<I> eqGroup, VarGroup<K> varGroup,
                                  Eigen::SparseMatrix<_Scalar>& jacobian)
  {

    constexpr int numEqns = numEquations(EqGroup<I>{}); // number of equations
    constexpr int numVars = numVariables(VarGroup<K>{}); // number of variables

    for(int i=0; i < numVars;++i){
      sparsityPatternModel.setVariable(varGroup,i,Yasp{i});
    }


    Eigen::Matrix<Yasp,numEqns,1> residuals;
    evalAllPattern(eqGroup,residuals);

    size_t nnz=0;
    for(int i=0; i < numEqns; ++i){
      nnz += residuals[i].nz.size();
    }

    using TripletType = Eigen::Triplet<_Scalar>;
    std::vector<TripletType> triplets;
    triplets.reserve(nnz);

    // second iteration to fill entris
    for(int irow=0; irow < numEqns; irow++){
      for(auto icol : residuals[irow].nz) {
        triplets.emplace_back(TripletType{irow,icol,0.0});
      }
    }

    // reset jacobian patterns to avoid conflicts with other components
    for(int i=0; i < numVars;++i){
      sparsityPatternModel.setVariable(varGroup,i,Yasp{});
    }

    jacobian.resize(numEqns,numVars);
    jacobian.setFromTriplets(triplets.begin(),triplets.end());
    jacobian.makeCompressed();

  }


private:
  template<int I, typename  V>
  inline void  evalAllPattern(EqGroup<I> eqGroup, V& residuals)
  {
    sparsityPatternModel.evalAll(eqGroup, residuals);
  }

  TangentModelType<RealType> tangentModel;
  TangentModelType<Yasp>  sparsityPatternModel;
};

#endif // FIRST_ORDER_ESO_HPP
