#ifndef CASCADEESO_H
#define CASCADEESO_H

#include "cascade.h"
#include "Eso/AlgebraicEsoView.hpp"
#include "Eso/Dco1Model.hpp"
#include "Eso/EqGroup.hpp"
#include "Eso/FirstOrderEso.hpp"
#include "BlockDeco/AlgebraicEsoBlockSolver.hpp"

template<typename RealType>
using TangentSingleFlash = Dco1Model<cascade,RealType>;

class CASCADEESO
{
public:
  CASCADEESO() {}
  void solve() {
      //FirstOrderEso<TangentSingleFlash,double> eso;
      //AlgebraicEsoView<0,0,decltype(eso)> algEsoView(eso);
      //Matrix<double,Dynamic,1> x(algEsoView.numVariables());
      //x.setOnes();
      //AlgebraicEsoBlockSolver solver;
      //solver.solve(algEsoView,x);
      //std::cout << x;
  }
};

#endif // CASCADEESO_H
