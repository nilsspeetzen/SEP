#ifndef DCO1_MODEL_HPP
#define DCO1_MODEL_HPP

#include "EqGroup.hpp"
#include "dco.hpp"
#include "Core"
#include <type_traits>



template<template<typename> class ModelTemplate, typename RealType>
class Dco1Model
{
public:
  template<int I>
  static constexpr inline int numEquations(EqGroup<I> eqGroup) {
    return TangentModel::numEquations(eqGroup);
  }
  template<int K>
  static constexpr inline int numVariables(VarGroup<K> varGroup) {
    return TangentModel::numVariables(varGroup);
  }


  template<int I>
  inline RealType eval(EqGroup<I> , int equationIndex) const {
    return Eval<I,0>{*this}.eval(equationIndex);
  }

  template<int I>
  inline RealType evalTangent(EqGroup<I> , int equationIndex) const {
    return Eval<I,0>{*this}.evalTangent(equationIndex);
  }


  template<int K>
  inline void setVariable(VarGroup<K> varGroup, const int variableIndex, const RealType& value){
    tangentModel.setVariable(varGroup,variableIndex,value);
  }

  template<int K>
  inline RealType getVariable(VarGroup<K> varGroup, const int variableIndex) const {
    return dco::value(tangentModel.getVariable(varGroup,variableIndex));
  }

  template<int K>
  inline void setTangent(VarGroup<K> varGroup, const int varIndex, const RealType& value){
    TangentType variable = tangentModel.getVariable(varGroup,varIndex);
    dco::derivative(variable) = value;
    tangentModel.setVariable(varGroup,varIndex,variable);
  }

  template<int K>
  inline RealType getTangent(VarGroup<K> varGroup, const int varIndex) const {
    return dco::derivative(tangentModel.getVariable(varGroup,varIndex));
  }


  template<int I, typename  V>
  void  evalAll(EqGroup<I> , V& residuals)
  {
    Eval<I,0>{*this}.evalAll(residuals);
  }



private:
  using TangentType = dco::gt1s_t<RealType>;
  using TangentModel = ModelTemplate<TangentType>;
  struct Empty {
    Empty(const Dco1Model<ModelTemplate,RealType>&) {}
    template<typename V>
    void evalAll(V& ) const { }
    RealType eval(int) const  { return RealType{};}
    RealType evalTangent(int) const  { return RealType{};}
  };

  template<int I, int J>
  struct Eval {
  public:
    Eval(const Dco1Model<ModelTemplate,RealType>& tangentModel) : tangentModel(tangentModel) {}
    inline RealType eval(int equationNumber){
      if(equationNumber == J) {
        return tangentModel.eval(EqGroup<I>{},EqIndex<J>{});
      }
      else
      {
        std::conditional_t<J+1 < numEquations(EqGroup<I>{}),Eval<I,J+1>,Empty>
            nextEval{tangentModel};
        return nextEval.eval(equationNumber);
      }
    }
    inline RealType evalTangent(int equationNumber){
      if(equationNumber == J) {
        return tangentModel.evalTangent(EqGroup<I>{},EqIndex<J>{});
      }
      else
      {
        std::conditional_t<J+1 < numEquations(EqGroup<I>{}),Eval<I,J+1>,Empty>
            nextEval{tangentModel};
        return nextEval.evalTangent(equationNumber);
      }
    }

    template<typename V>
    inline void evalAll(V& residuals) const {
      residuals[J] = tangentModel.eval(EqGroup<I>{},EqIndex<J>{});
      std::conditional_t<J+1 < numEquations(EqGroup<I>{}),Eval<I,J+1>,Empty>
          nextEval{tangentModel};
      nextEval.evalAll(residuals);
    }
  private:
    const Dco1Model<ModelTemplate,RealType>& tangentModel;
  };

  template<int I, int J>
  inline RealType evalTangent(EqGroup<I> eqGroup, EqIndex<J> eqIndex) const {
    return dco::derivative(tangentModel.eval(eqGroup,eqIndex));
  }

  template<int I, int J>
  inline RealType eval(EqGroup<I> eqGroup, EqIndex<J> eqIndex) const {
    return dco::value(tangentModel.eval(eqGroup,eqIndex));
  }

  TangentModel tangentModel;
};



#endif // DCO1_MODEL_HPP
