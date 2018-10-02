#include "SimpleModel.hpp"
#include "MoreModel.hpp"
#include "BlockDeco/EuclideanNorm.hpp"
#include <iostream>
#include <cmath>
#include "BlockDeco/getjsp.hpp"
#include "BlockDeco/getbtf.hpp"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "MetaProgEso/AlgebraicEso.hpp"
#include "BlockDeco/BlockDecoTypes.hpp"
#include "BlockDeco/BlockDecoUtil.hpp"
#include "BlockDeco/BlockDecoSolve.hpp"
#include "gtest/gtest.h"





// Residual function of dimension 3 that can be partioned into 2 blocks
template<typename TREAL>
void EvalSimpleProblem(TREAL *G, TREAL* x)
{
  AlgebraicEso<SimpleModel,TREAL> eso;
  eso.setStates(x);
  eso.evalAllResiduals(G);
}

//MoreFunction with n=5 und m = 3, i.e., dimension = 15
template<typename TREAL>
void EvalMoreProblem(TREAL *F, TREAL* x)
{

  AlgebraicEso<MoreModel,TREAL> eso;
  eso.setStates(x);
  eso.evalAllResiduals(F);

}


TEST(BlockDeco, CanSolveMoreProblem)
{
  const unsigned dim = 15;
  std::vector<double> x(dim,1.0); // initial guess


  // set functions for input
  CommFuncJac comm;
  comm.dim = dim;
  comm.doubleEvalF = &EvalMoreProblem<double>;
  comm.jspEvalF = &EvalMoreProblem<jsp>;
  comm.dcoEval = &EvalMoreProblem<dco::gt1s_t<double>>;

  //bool success = SolveAsCompleteSystem(x.data(),&comm);
  bool success = SolveAsBlockSystem(x.data(),&comm);

  EXPECT_TRUE(success);
  auto solution = MoreModel<double>::getSolution();
  for (size_t i = 0; i < MoreModel<double>::NUM_STATES;++i)
    EXPECT_NEAR(solution[i],x[i],1e-12);


}


TEST(BlockDeco, CanSolveSimpleProblemAsBlockSystem)
{
  const unsigned dim = 3;
  std::vector<double> x(dim,1.0); // initial guess


  // set functions for input
  CommFuncJac comm;
  comm.dim = dim;
  comm.doubleEvalF = &EvalSimpleProblem<double>;
  comm.jspEvalF = &EvalSimpleProblem<jsp>;
  comm.dcoEval = &EvalSimpleProblem<dco::gt1s_t<double>>;

  bool success = SolveAsBlockSystem(x.data(),&comm);

  EXPECT_TRUE(success);
  auto solution = SimpleModel<double>::getSolution();
  for (size_t i = 0; i < SimpleModel<double>::NUM_STATES;++i)
    EXPECT_NEAR(solution[i],x[i],1e-12);

}

TEST(BlockDeco, CanSolveSimpleProblemAsFullSystem)
{
  const unsigned dim = 3;
  std::vector<double> x(dim,1.0); // initial guess


  // set functions for input
  CommFuncJac comm;
  comm.dim = dim;
  comm.doubleEvalF = &EvalSimpleProblem<double>;
  comm.jspEvalF = &EvalSimpleProblem<jsp>;
  comm.dcoEval = &EvalSimpleProblem<dco::gt1s_t<double>>;

  bool success = SolveAsCompleteSystem(x.data(),&comm);


  EXPECT_TRUE(success);
  auto solution = SimpleModel<double>::getSolution();
  for (size_t i = 0; i < SimpleModel<double>::NUM_STATES;++i)
    EXPECT_NEAR(solution[i],x[i],1e-12);

}



