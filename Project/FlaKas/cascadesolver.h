#ifndef CASCADESOLVER_H
#define CASCADESOLVER_H

#include "cascade.h"
#include "TestEso.h"
#include "BlockDeco/AlgebraicEsoBlockSolver.hpp"

class CascadeSolver
{
public:
	CascadeSolver() {}
	void solve(cascade<double>* eso) {
		eso->initX();
		AlgebraicEsoBlockSolver solver;
		Matrix<double, Dynamic, 1> x(eso->numVariables());
		x.setOnes();
		try
		{
			solver.solve(*eso, x);
		}
		catch (const std::exception&)
		{
			qDebug() << "Konvergiert nicht";
		}
		std::stringstream sx;
		sx << x;
		qDebug() << "x:" << QString::fromStdString(sx.str());
  }
};

#endif // CASCADESOLVER_H
