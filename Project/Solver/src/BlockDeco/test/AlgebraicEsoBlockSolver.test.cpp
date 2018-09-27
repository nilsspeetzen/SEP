#include "BlockDeco/AlgebraicEsoBlockSolver.hpp"
#include "Eso/Models/SimpleModel.hpp"
#include "Eso/Models/MoreModel.hpp"
#include "Eso/Models/SepCes.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "Eso/Dco1Model.hpp"
#include "Eso/AlgebraicEsoView.hpp"
#include "gtest/gtest.h"
#include <iostream>

template<typename RealType>
using TangentSimpleModel = Dco1Model<SimpleModel,RealType>;



TEST(AlgebraicEsoBlockSolver, CanSolveSimpleModel)
{
  FirstOrderEso<TangentSimpleModel,double> eso;
  AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
  std::vector<double> x(decltype(algEsoView)::numVariables(),1.0);
  AlgebraicEsoBlockSolver solver;
  solver.solve(algEsoView,x);
  auto solution = SimpleModel<double>::getSolution();
  for (size_t i = 0; i < eso.numVariables(VarGroup<0>{});++i)
    EXPECT_NEAR(solution[i],x[i],1e-12);

}

TEST(AlgebraicEsoBlockSolver, CanSolveSimpleModelInSinglePrecision)
{
    FirstOrderEso<TangentSimpleModel,float> eso;
    AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
    std::vector<float> x(eso.numVariables(VarGroup<0>{}),1.0);
    AlgebraicEsoBlockSolver solver;
    solver.solve(algEsoView,x);
    auto solution = SimpleModel<double>::getSolution();
    for (size_t i = 0; i < eso.numVariables(VarGroup<0>{});++i)
      EXPECT_NEAR(solution[i],static_cast<double>(x[i]),1e-6);

}


template<typename RealType>
using TangentMoreModel = Dco1Model<MoreModel,RealType>;


TEST(AlgebraicEsoBlockSolver,CanSolverMoreModel)
{
  FirstOrderEso<TangentMoreModel,double> eso;
  AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
  std::vector<double> x(eso.numVariables(VarGroup<0>{}),1.0);
  AlgebraicEsoBlockSolver solver;
  solver.solve(algEsoView,x);
  auto solution = MoreModel<double>::getSolution();
  for (size_t i = 0; i < eso.numVariables(VarGroup<0>{});++i)
    EXPECT_NEAR(solution[i],x[i],1e-10);

}

template<typename RealType>
using TangentSepCes = Dco1Model<SepCes,RealType>;

TEST(AlgebraicEsoBlockSolver,CanSolveSepCesModel)
{
  FirstOrderEso<TangentSepCes,double> eso;
  AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
  Eigen::Matrix<double,Eigen::Dynamic,1> x(algEsoView.numVariables());
  x.setOnes();
  AlgebraicEsoBlockSolver solver;
  solver.solve(algEsoView,x);

  std::cout << "Solution of SepCes model is" << std::endl;
  std::cout << " T = " << x[3] << std::endl;
  std::cout << " x_out_1 = " << x[4] << std::endl;
  std::cout << " x_out_2 = " << x[5] << std::endl;

}

