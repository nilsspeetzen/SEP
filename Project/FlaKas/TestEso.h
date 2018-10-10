#pragma once

#include "BlockDeco/Block.hpp"
#include "Eigen/SparseCore"
#include <vector>
#include <math.h>

template<typename RealType>
class TestEso
{
public:
	Eigen::Matrix<RealType, 2, 1> x;

	// 0 = x_1 ^2 + x_2 ^2 - 1
	// 0 = x_1 ^2 - 1

	int numVariables() {
		return 2;
	}
	int numEquations() {
		return 2;
	}

	inline void setVariable(int variableIndex, const RealType& variableValue)
	{
		x(variableIndex) = variableValue;
	}

	RealType eval(int i) {
		if (i == 0) return pow(x(0),2) + pow(x(1),2) - 1;
		if (i == 0) return x(0) - 1;
		return 0;
	}

	RealType evalDerivative(int i, int j) {
		if (i == 0 && j == 0) return 2 * x(0);
		if (i == 0 && j == 1) return 2 * x(1);
		if (i == 1 && j == 0) return 1;
		if (i == 1 && j == 1) return 0;
		return 0;
	}

	template<class X, class V>
	inline void setVariables(X indices, const V& values) {
		int irow = 0;
		for (auto i = cbegin(indices); i != cend(indices); ++i, ++irow)
			setVariable(*i, values(irow));
	}

	template<class X, class V>
	inline void evalBlock(X indices, V& residuals) {
		int irow = 0;
		for (auto i = cbegin(indices); i != cend(indices); ++i, ++irow)
			residuals[irow] = eval(*i);
	}

	template<class X, class Y, typename Jacobian>
	inline void evalBlockJacobian(const X& eqIndices, const Y& varIndices, Jacobian& jac) {
		using std::cbegin;
		using std::cend;
		int jcol = 0;
		for (auto j = cbegin(varIndices); j != cend(varIndices); ++j, ++jcol) {
			int irow = 0;
			for (auto i = cbegin(eqIndices); i != cend(eqIndices); ++i, ++irow)
				jac(irow, jcol) = evalDerivative(*i, *j);
		}
	}

	template<typename _Scalar>
	inline void evalJacobianPattern(Eigen::SparseMatrix<_Scalar>& jacobian)
	{

		int numEqns = numEquations(); // number of equations
		int numVars = numVariables(); // number of variables

		/*
		for (int i = 0; i < numVars; ++i) {
			sparsityPatternModel.setVariable(varGroup, i, Yasp{ i });
		}


		Eigen::Matrix<Yasp, numEqns, 1> residuals;
		evalAllPattern(eqGroup, residuals);

		size_t nnz = 0;
		for (int i = 0; i < numEqns; ++i) {
			nnz += residuals[i].nz.size();
		}

		using TripletType = Eigen::Triplet<_Scalar>;
		std::vector<TripletType> triplets;
		triplets.reserve(nnz);

		// second iteration to fill entris
		for (int irow = 0; irow < numEqns; irow++) {
			for (auto icol : residuals[irow].nz) {
				triplets.emplace_back(TripletType{ irow,icol,0.0 });
			}
		}

		// reset jacobian patterns to avoid conflicts with other components
		for (int i = 0; i < numVars; ++i) {
			sparsityPatternModel.setVariable(varGroup, i, Yasp{});
		}*/
		using TripletType = Eigen::Triplet<_Scalar>;
		std::vector<TripletType> triplets;
		triplets.reserve(3);

		triplets.emplace_back(0, 0, 0.0);
		triplets.emplace_back(0, 1, 0.0);
		triplets.emplace_back(1, 0, 0.0);

		jacobian.resize(numEqns, numVars);
		jacobian.setFromTriplets(triplets.begin(), triplets.end());
		jacobian.makeCompressed();

	}

	
};
