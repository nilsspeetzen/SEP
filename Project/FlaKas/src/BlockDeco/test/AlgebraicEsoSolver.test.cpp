#include "BlockDeco/AlgebraicEsoSolver.hpp"
#include "Eso/Models/SimpleModel.hpp"
#include "Eso/Models/MoreModel.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "Eso/Dco1Model.hpp"
#include "Eso/AlgebraicEsoView.hpp"
#include "gtest/gtest.h"


template<typename RealType>
using TangentSimpleModel = Dco1Model<SimpleModel,RealType>;



TEST(AlgebraicEsoSolver, CanSolveSimpleModel)
{
  FirstOrderEso<TangentSimpleModel,double> eso;
  AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
  std::vector<double> x(decltype(algEsoView)::numVariables(),1.0);
  AlgebraicEsoSolver solver;
  solver.solve(algEsoView,x);
  auto solution = SimpleModel<double>::getSolution();
  for (size_t i = 0; i < algEsoView.numVariables();++i)
    EXPECT_NEAR(solution[i],x[i],1e-12);

}





TEST(AlgebraicEsoSolver, CanSolveSimpleModelInSinglePrecision)
{
  FirstOrderEso<TangentSimpleModel,float> eso;
  AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
  std::vector<float> x(algEsoView.numVariables(),1.0);;
  AlgebraicEsoSolver solver;
  solver.solve(algEsoView,x);
  auto solution = SimpleModel<float>::getSolution();
  for (size_t i = 0; i < algEsoView.numVariables();++i)
    EXPECT_NEAR(solution[i],static_cast<double>(x[i]),1e-6);

}



TEST(AlgebraicEsoSolver, ThrowsExceptionOnDimensionMismatch)
{

    FirstOrderEso<TangentSimpleModel,double> eso;
    AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
    std::vector<double> x(algEsoView.numVariables()+1);
    AlgebraicEsoSolver solver;
    EXPECT_THROW(solver.solve(algEsoView,x),std::runtime_error);

}
