#include "Eso/Dco1Model.hpp"
#include "gtest/gtest.h"
#include "Eso/Models/VanDerPolModel.hpp"
#include "Eso/FirstOrderEso.hpp"
#include <array>

template<typename RealType>
using TangentVanDerPolModel = Dco1Model<VanDerPolModel,RealType>;

template<typename RealType>
using VanDerPolEso = FirstOrderEso<TangentVanDerPolModel,RealType>;

TEST(FirstOrderEso, CanUseIt)
{
  using RealType = double;
  VanDerPolEso<RealType> eso;
  eso.setVariables(VarGroup<0>{},std::vector<int>{0,1,2},std::array<RealType,3>{1.1,2.2,3.3});
  eso.setVariables(VarGroup<1>{},std::array<RealType,3>{0.,0.,0.});
  eso.setVariables(VarGroup<2>{},std::array<RealType,2>{1.,1.});
  auto residual = eso.eval(EqGroup<0>{},0);
  EXPECT_NEAR(2.2,residual,1e-7);
  std::array<RealType,3> residuals;
  eso.evalAll(EqGroup<0>{},residuals);
  EXPECT_NEAR(2.2,residuals[0],1e-7);
  EXPECT_NEAR(7.05,residuals[2],1e-7);
}



TEST(FirstOrderEso, CanComputeSparsityPattern)
{
  using RealType = double;
  VanDerPolEso<RealType> eso;
  Eigen::SparseMatrix<RealType> jacobian;
  eso.evalJacobianPattern(EqGroup<0>{},VarGroup<1>{},jacobian);
  int *colPtr = jacobian.outerIndexPtr();
  int *rowInd = jacobian.innerIndexPtr();
  EXPECT_EQ(int(0),colPtr[0]);
  EXPECT_EQ(int(0),rowInd[0]);
  EXPECT_EQ(int(1),colPtr[1]);
  EXPECT_EQ(int(1),rowInd[1]);
  EXPECT_EQ(int(2),colPtr[2]);
  EXPECT_EQ(int(2),rowInd[2]);
  EXPECT_EQ(int(3),colPtr[3]);

}

TEST(FirstOrderEso,CanEvaluateResiduals)
{
  using RealType = double;
  VanDerPolEso<RealType> eso;
  eso.setVariables(VarGroup<0>{},std::array<RealType,3>{1.0,1.0,1.5});
  eso.setVariables(VarGroup<1>{},std::array<RealType,3>{0.5,0.5,1.5});
  eso.setVariables(VarGroup<2>{},std::array<RealType,3>{1.0,2.0});
  std::array<RealType,3> residuals;
  eso.evalAll(EqGroup<0>{},residuals);
  EXPECT_NEAR(0.5,residuals[0],1e-7);
  EXPECT_NEAR(0.5,residuals[1],1e-7);
  EXPECT_NEAR(4.5,residuals[2],1e-7);
  residuals = std::array<RealType,3>{17,17,17};
  eso.evalBlock(EqGroup<0>{},std::array<int,2>{0,2},residuals);
  EXPECT_NEAR(0.5,residuals[0],1e-7);
  EXPECT_NEAR(4.5,residuals[1],1e-7);
  EXPECT_NEAR(17.0,residuals[2],1e-7);

}


TEST(FirstOrderEso,CanEvaluateSparseJacobianValues)
{
  using RealType = double;
  VanDerPolEso<RealType> eso;
  eso.setVariables(VarGroup<2>{},std::array<RealType,2>{1,1} );
  Eigen::SparseMatrix<RealType> jacobian;
  eso.evalJacobianPattern(EqGroup<0>{},VarGroup<1>{},jacobian);
  eso.evalJacobianValues(EqGroup<0>{},VarGroup<1>{},jacobian);
  EXPECT_NEAR(-1,jacobian.coeff(0,0),1e-7);
  EXPECT_NEAR(-1,jacobian.coeff(1,1),1e-7);
  EXPECT_NEAR(-1,jacobian.coeff(2,2),1e-7);
}


TEST(FirstOrderEso,CanEvaluateDenseJacobian)
{
  using RealType = double;
  constexpr int m = 3;
  constexpr int n = m;
  VanDerPolEso<RealType> eso;
  eso.setVariables(VarGroup<2>{},std::array<RealType,2>{1,1});
  Eigen::Matrix<RealType,Eigen::Dynamic,Eigen::Dynamic> jacobian;
  eso.evalJacobianPattern(EqGroup<0>{},VarGroup<1>{},jacobian);
  eso.evalJacobianValues(EqGroup<0>{},VarGroup<1>{},jacobian);
  for(int i=0; i < m; ++i){
    for(int j=0; j <n; ++j) {
      RealType correctValue = (i==j)? -1 : 0;
      EXPECT_NEAR(correctValue,jacobian(i,j),1e-7);
    }
  }
  jacobian.setZero();
  eso.evalBlockJacobian(EqGroup<0>{},VarGroup<1>{},std::vector<int>{0,1,2},
                        std::vector<int>{0,1,2},jacobian);

  for(int i=0; i < m; ++i){
    for(int j=0; j <n; ++j) {
      RealType correctValue = (i==j)? -1 : 0;
      EXPECT_NEAR(correctValue,jacobian(i,j),1e-7);
    }
  }

}

