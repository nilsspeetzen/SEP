#ifndef ALGEBRAIC_ESO_BLOCK_SOLVER_HPP
#define ALGEBRAIC_ESO_BLOCK_SOLVER_HPP


#include "BlockDeco/Block.hpp"
#include "Eigen/SparseCore"
#include "BlockDecoBTF/btf.h"
#include "Eigen/NonLinearOptimization"
#include <vector>
#include <numeric>

using BDB = BlockDecoBlock::Block;

class AlgebraicEsoBlockSolver
{
public:

  template<class AlgebraicEso>
  struct EigenHybridBlockAdapter {
    EigenHybridBlockAdapter(AlgebraicEso& eso) : eso(eso) {}
    template<typename EigenVectorType>
      int operator()(const EigenVectorType &x, EigenVectorType &fvec){       
          eso.setVariables(block.VariableIndices,x);
          eso.evalBlock(block.EquationIndices,fvec);
        return 0;
      }
    template<typename EigenVectorType, typename EigenMatrixType>
      int df(const EigenVectorType &x, EigenMatrixType &fjac) {
          eso.setVariables(block.VariableIndices,x);
          eso.evalBlockJacobian(block.EquationIndices,block.VariableIndices,fjac);
          return 0;
      }

      BDB block;
      AlgebraicEso& eso;

  public:
      void setBlock(const BDB &value) {block = value;}
  };

  template<class AlgebraicEso, class StateType>
  void solve(AlgebraicEso& eso, StateType& x)
  {
    std::vector<int> allIndices(eso.numVariables());
    std::iota(std::begin(allIndices),std::end(allIndices),0);// Fill with 0, 1, 2, ...
    //eso.setVariables(allIndices,x);
    computeBlocks(eso,x);
    EigenHybridBlockAdapter<AlgebraicEso> functor{eso};
    using RealType = std::remove_reference_t<decltype(x[0])>;
    Eigen::HybridNonLinearSolver<decltype(functor), RealType> solver(functor);
    for(size_t iBlock = 0; iBlock < blocks.size(); ++iBlock)
    {
      functor.setBlock(blocks[iBlock]);
      const auto currentDim = static_cast<int>(blocks[iBlock].size());
      Eigen::Matrix<RealType,Eigen::Dynamic,1> xs(currentDim);
      using std::cbegin;
      int index = 0;
      for(auto p = cbegin(blocks[iBlock].VariableIndices); index < currentDim;++p,++index)
      {
        xs[index] = x[*p];
      }

      auto info = solver.hybrj1(xs);
      checkHybrj1Status(info);

      index = 0;
      for(auto p = cbegin(blocks[iBlock].VariableIndices); index < currentDim;++p,++index)
      {
        x[*p] = xs[index];
      }
      eso.setVariables(blocks[iBlock].VariableIndices,xs);
    }


  }


  template<class AlgebraicEso, class StateType>
  void computeBlocks(AlgebraicEso& eso, StateType& x){
    using RealType = std::remove_reference_t<decltype(x[0])>;
    Eigen::SparseMatrix<RealType> jacobian;
    eso.evalJacobianPattern(jacobian);
    double maxwork = -1;
    double work=0;
    unsigned dim = eso.numEquations();
    std::vector<int> P(dim,0);
    std::vector<int> Q(dim,0);
    std::vector<int> R(dim+1,0);
    int nmatch = 0;
    std::vector<int> Work(5*dim,0);
    int numBlocks = btf_order(dim,jacobian.outerIndexPtr(),jacobian.innerIndexPtr(),
                              maxwork,&work,P.data(),Q.data(),
                              R.data(),&nmatch,Work.data());

    blocks.resize(numBlocks);
    maxDimBlock = 0;
    for(int i=0; i<numBlocks; i++) {
      int dimBlock = R[i+1]-R[i];
      if (dimBlock > maxDimBlock)
        maxDimBlock = dimBlock;
      int index=R[i];
      for(int j=0; j<dimBlock; j++) {
        blocks[numBlocks-1-i].EquationIndices.insert(P[index+j]);
        blocks[numBlocks-1-i].VariableIndices.insert(Q[index+j]);
      }
    }
  }

  static inline void checkHybrj1Status (int info){
    if(info != 1)
      throw std::runtime_error("Error in HybridNonLinearSolver::hybrj1(): Algorithm did not converge!");
  }

  std::vector<BDB> blocks;
  int maxDimBlock;
};

#endif // ALGEBRAIC_ESO_BLOCK_SOLVER_HPP
