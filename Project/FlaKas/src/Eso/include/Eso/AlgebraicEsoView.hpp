#ifndef ALGEBRAIC_ESO_VIEW_HPP
#define ALGEBRAIC_ESO_VIEW_HPP

#include "EqGroup.hpp"
#include "Dco1Model.hpp"
#include "FirstOrderEso.hpp"

template <int I, int J, class Eso>
class AlgebraicEsoView {
public:
    AlgebraicEsoView(Eso& eso) : eso(eso) {}

    static constexpr inline int numEquations() {
        return Eso::numEquations(EqGroup<I>{});
    }

    static constexpr inline int numVariables() {
        return Eso::numVariables(VarGroup<J>{});
    }

    template< class V>
    inline void setVariables(const V& values){
        eso.setVariables(VarGroup<J>{},values);
    }

    template<typename RealType>
    inline void setVariable(const int index, const RealType& value)
    {
        eso.setVariable(VarGroup<J>{},index,value);
    }

    template<class Indices, class Values>
    inline void setVariables(const Indices& indices, const Values& values)
    {
        eso.setVariables(VarGroup<J>{},indices,values);
    }

    template<typename RealType>
    inline RealType eval(int equationIndex)
    {
      return eso.eval(EqGroup<I>{},equationIndex);
    }

    template<class EquationIndices, class Residuals>
    inline void evalBlock(const EquationIndices& indices, Residuals& residuals)
    {
        eso.evalBlock(EqGroup<I>{},indices,residuals);
    }

    template<typename  V>
    inline void  evalAll(V& residuals)
    {
        eso.evalAll(EqGroup<I>{},residuals);
    }


    template<class EquationIndices,
             class VariableIndices,
             typename Jacobian>
    void evalBlockJacobian(const EquationIndices& equationIndices,
                           const VariableIndices& stateIndices,
                           Jacobian& jacobian)
    {
      eso.evalBlockJacobian(EqGroup<I>{},VarGroup<J>{},equationIndices,stateIndices,jacobian);
    }


    template<typename _Scalar>
    inline void evalJacobianValues(Eigen::SparseMatrix<_Scalar>& jacobian)
    {
      eso.evalJacobianValues(EqGroup<I>{},VarGroup<J>{},jacobian);
    }


    template<typename _Scalar>
    inline void evalJacobianPattern(Eigen::SparseMatrix<_Scalar>& jacobian)
    {
       eso.evalJacobianPattern(EqGroup<I>{},VarGroup<J>{},jacobian);
    }

    template<typename _Scalar, int _Rows, int _Cols, int _Options,
             int _MaxRows, int _MaxCols>
    inline void evalJacobianPattern(Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                    _MaxRows, _MaxCols >& jacobian)
    {
        eso.evalJacobianPattern(EqGroup<I>{},VarGroup<J>{},jacobian);
    }

    template<typename _Scalar, int _Rows, int _Cols, int _Options,
             int _MaxRows, int _MaxCols>
    inline void evalJacobianValues( Eigen::Matrix< _Scalar, _Rows, _Cols, _Options,
                                    _MaxRows, _MaxCols >& jacobian)
    {
        eso.evalJacobianValues(EqGroup<I>{},VarGroup<J>{},jacobian);
    }

private:
  Eso& eso;
};


#endif // ALGEBRAIC_ESO_VIEW_HPP
