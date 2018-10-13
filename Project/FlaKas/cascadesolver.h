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
		QString qstr = QString::fromStdString(sx.str());
		qstr.replace(QChar('\n'), QChar(' '));
		qDebug() << "x:" << qstr;
  }
};

#endif // CASCADESOLVER_H
