#include "Eso/Models/SimpleModel.hpp"
#include "Eigen/NonLinearOptimization"
#include "gtest/gtest.h"
#include "Eso/AlgebraicEsoView.hpp"
#include "Eso/Dco1Model.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "Eso/AlgebraicEsoView.hpp"

using Eigen::VectorXd;
using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::HybridNonLinearSolver;

// Generic functor
template<typename _Scalar, int NX=Dynamic, int NY=Dynamic>
struct Functor
{
  typedef _Scalar Scalar;
  enum {
    InputsAtCompileTime = NX,
    ValuesAtCompileTime = NY
  };
  typedef Matrix<Scalar,InputsAtCompileTime,1> InputType;
  typedef Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
  typedef Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

  const int m_inputs, m_values;

  Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
  Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

  int inputs() const { return m_inputs; }
  int values() const { return m_values; }

};

struct hybrj_functor : Functor<double>
{
    hybrj_functor(void) : Functor<double>(9,9) {}

    int operator()(const VectorXd &x, VectorXd &fvec)
    {
        double temp, temp1, temp2;
        const VectorXd::Index n = x.size();
        assert(fvec.size()==n);
        for (VectorXd::Index k = 0; k < n; k++)
        {
            temp = (3. - 2.*x[k])*x[k];
            temp1 = 0.;
            if (k) temp1 = x[k-1];
            temp2 = 0.;
            if (k != n-1) temp2 = x[k+1];
            fvec[k] = temp - temp1 - 2.*temp2 + 1.;
        }
        return 0;
    }
    int df(const VectorXd &x, MatrixXd &fjac)
    {
        const VectorXd::Index n = x.size();
        assert(fjac.rows()==n);
        assert(fjac.cols()==n);
        for (VectorXd::Index k = 0; k < n; k++)
        {
            for (VectorXd::Index j = 0; j < n; j++)
                fjac(k,j) = 0.;
            fjac(k,k) = 3.- 4.*x[k];
            if (k) fjac(k,k-1) = -1.;
            if (k != n-1) fjac(k,k+1) = -2.;
        }
        return 0;
    }
};


TEST(EigenHybridNonlinearSolver, CanRunEigenTesttestHybrj1)
{

  const int n=9;
  int info;
  VectorXd x(n);

  /* the following starting values provide a rough fit. */
  x.setConstant(n, -1.);

  // do the computation
  hybrj_functor functor;
  HybridNonLinearSolver<hybrj_functor> solver(functor);
  info = solver.hybrj1(x);

  EXPECT_EQ( 1, info);
  EXPECT_EQ(11, solver.nfev);
  EXPECT_EQ( 1, solver.njev);
  EXPECT_NEAR(1.192636e-08, solver.fvec.blueNorm(),1e-9);
  VectorXd x_ref(n);
  x_ref <<
       -0.5706545,    -0.6816283,    -0.7017325,
       -0.7042129,     -0.701369,    -0.6918656,
       -0.665792,    -0.5960342,    -0.4164121;

  for(int i=0; i < n; ++i)
    EXPECT_NEAR(x_ref[i],x[i],1e-6);


}

template<class Eso>
struct EigenHybridAdapter
{
    EigenHybridAdapter(Eso& eso) : eso(eso) {}
  int operator()(const VectorXd &x, VectorXd &fvec){
    assert(fvec.size()== dimension());
    assert(x.size() == dimension());
    eso.setVariables(x);
    eso.evalAll(fvec);
    return 0;
  }

  int df(const VectorXd &x, MatrixXd &fjac) {
    assert(x.size() == dimension());
    assert(fjac.rows()==dimension());
    assert(fjac.cols()==dimension());
    eso.setVariables(x);
    eso.evalJacobianValues(fjac);
    return 0;
  }
  int dimension() const { return eso.numEquations();}
  Eso& eso;

};

template<typename RealType>
using TangentSimpleModel = Dco1Model<SimpleModel,RealType>;

TEST(EigenHybridNonlinearSolver, CanSolveSimpleProblem)
{

  FirstOrderEso<TangentSimpleModel,double> eso;
  AlgebraicEsoView<0,0,decltype (eso)> algEsoView(eso);
  EigenHybridAdapter<decltype (algEsoView)> adapter(algEsoView);
  const int n=adapter.dimension();
  int info;
  VectorXd x(n);
  x.setConstant(n, 1.);
  HybridNonLinearSolver<decltype (adapter)> solver(adapter);


  info = solver.hybrj1(x);
  EXPECT_EQ(1, info);
  auto solution = SimpleModel<double>::getSolution();
  for (int i = 0; i < n;++i)
    EXPECT_NEAR(solution[size_t(i)],x[i],1e-12);

}
