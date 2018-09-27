#ifndef ALGEBRAIC_ESO_SOLVER_HPP
#define ALGEBRAIC_ESO_SOLVER_HPP

#include "Eigen/NonLinearOptimization"
#include <stdexcept>
#include <type_traits>



class AlgebraicEsoSolver {
private:
  template<class AlgebraicEso>
  struct EigenHybridAdapter {
    EigenHybridAdapter(AlgebraicEso& eso) : eso(eso) {}
    template<typename EigenVectorType>
      int operator()(const EigenVectorType &x, EigenVectorType &fvec){
        eso.setVariables(x);
        eso.evalAll(fvec);
        return 0;
      }
    template<typename EigenVectorType, typename EigenMatrixType>
      int df(const EigenVectorType &x, EigenMatrixType &fjac) {
            eso.setVariables(x);
            eso.evalJacobianValues(fjac);
            return 0;
      }

      AlgebraicEso& eso;
  };

public:
  template<class AlgebraicEso, class StateType>
  void solve(AlgebraicEso& eso, StateType& x){
    checkInputs(eso,x);
    EigenHybridAdapter<AlgebraicEso> functor{eso};
    Eigen::HybridNonLinearSolver<decltype(functor),std::remove_reference_t<decltype(x[0])> > solver(functor);
    runChecked(solver,x);
  }

private:
  template<class AlgebraicEso, class StateType>
  void checkInputs(AlgebraicEso& eso, StateType& x) {
    if(eso.numVariables() != eso.numEquations())
      throw std::runtime_error("Error in AlgebraicEsoSolver::solve(): number of variables is different from number of equations!");
    if(eso.numVariables() != x.size())
      throw std::runtime_error("Error in AlgebraicEsoSolver::solve(): number of variables in ESO different from input vector!");
  }
  static inline void checkHybrj1Status (int info){
    if(info != 1)
      throw std::runtime_error("Error in HybridNonLinearSolver::hybrj1(): Algorithm did not converge!");
  }

  template<class DestVectorType, class SourceVectorType>
  static void copyVector(const SourceVectorType& src,DestVectorType& dest) {
     for(size_t i=0; i < dest.size(); ++i)
       dest[i] = src[i];
  }

  template<typename SolverType, typename StateType>
  inline static void runChecked(SolverType& solver, StateType& x)
  {
    auto xCopy = createCopy(x);
    auto info = solver.hybrj1(xCopy);
    checkHybrj1Status(info);
    copyVector(xCopy,x);
  }

  template<class StateType>
  inline static auto createCopy(const StateType& x)
  {
    using ScalarType = std::remove_const_t<std::remove_reference_t<decltype(x[0])> >;
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> xCopy(x.size());
    copyVector(x,xCopy);
    return xCopy;
  }

} ;


#endif //ALGEBRAIC_ESO_SOLVER_HPP
