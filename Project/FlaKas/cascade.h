#ifndef CASCADE_H
#define CASCADE_H
#include "flash.h"
#include "Eigen/SparseCore"
#include "Eigen/Core"
#include "Yasp/Yasp.hpp"
#include <math.h>
#include <vector>
#include <QtCore>

#include "dco/dco.hpp"
#include "Eso/Dco1Model.hpp"

/**
 * @file cascade.h
 * @brief Enthält alle Module
 */

/**
 * @brief Enthält alle Flashes
 */
template<typename RealType>
class cascade {
    typedef Matrix<RealType,Dynamic,1> VT;
    typedef Matrix<RealType,Dynamic,Dynamic> MT;
protected:
    int _numS;
    Matrix<RealType,Dynamic,7> _a;
    std::map <int,Flash<RealType>> _flashes;
    VT _x;
public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    cascade(int numSubstances, Matrix<RealType,Dynamic,7> a) : _numS(numSubstances), _a(a) {}
	cascade() {}

	VT& x() { return _x; }
	RealType& x(int i) { return _x(i); }
	int& numS() { return _numS; }
	Matrix<RealType, Dynamic, 7>& a() { return _a; }
	std::map <int, Flash<RealType>>& flashes() { return _flashes; }

    //FÜR ESO ----------------------------------------------

	void initX() {
		_x = VT::Zero(5);//VT::Zero(numVariables());
	}

	int numVariables() {
		int numV = 0;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			numV += i->second.numVariables();
		}
		return 5;//numV;
	}

	int numEquations() { 
		int numE = 0;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			numE += i->second.numEquations();
		}
		return 5;//numE; 
	}

	inline void setVariable(int variableIndex, const RealType& variableValue)
	{
		_x(variableIndex) = variableValue;
	}

	template<class X, class V>
	inline void setVariables(X indices, const V& values) {
		int irow = 0;
		for (auto i = cbegin(indices); i != cend(indices); ++i, ++irow)
			setVariable(*i, values(irow));
	}

	//Eval
	template<class V>
	inline void evalAll(V& residuals) {
		for (int i = 0; i < numEquations(); i++)
			residuals[i] = eval(i);
	}

	template<class X, class V>
	inline void evalBlock(X indices, V& residuals) {
		int irow = 0;
		for (auto i = cbegin(indices); i != cend(indices); ++i, ++irow)
			residuals[irow] = eval(*i);
	}

	RealType eval(int i) {
		return x(i) - i;
	}

	//Derivative
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

	RealType evalDerivative(int i, int j) {
		cascade<gt1s_t<RealType>> tangentCascade;
		tangentCascade.initX();
		for (int k = 0; k < numVariables(); k++) {
			value(tangentCascade.x(k)) = x(k);
			derivative(tangentCascade.x(k)) = 0;
		}
		derivative(tangentCascade.x(j)) = 1;
		return derivative(tangentCascade.eval(i));
	}

	//Jacobianpattern
	template<typename _Scalar>
	inline void evalJacobianPattern(Eigen::SparseMatrix<_Scalar>& jacobian)
	{
		cascade<Yasp> patternCascade;
		patternCascade.numS() = _numS;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			Flash<Yasp> f;
			patternCascade.flashes().insert(std::pair<int, Flash<Yasp>>(i->first,f));
		}
		patternCascade.initX();

		int numEqns = numEquations();
		int numVars = numVariables();


		for (int i = 0; i < numVars; ++i) {
			patternCascade.setVariable(i, Yasp{ i });
		}


		Eigen::Matrix<Yasp, -1, 1> residuals;
		residuals.resize(numEqns, 1);
		patternCascade.evalAll(residuals);

		size_t nnz = 0;
		for (int i = 0; i < numEqns; ++i) {
			nnz += residuals[i].nz.size();
		}

		using TripletType = Eigen::Triplet<_Scalar>;
		std::vector<TripletType> triplets;
		triplets.reserve(nnz);

		// second iteration to fill entries
		for (int irow = 0; irow < numEqns; irow++) {
			for (auto icol : residuals[irow].nz) {
				triplets.emplace_back(TripletType{ irow,icol,0.0 });
			}
		}

		jacobian.resize(numEqns, numVars);
		jacobian.setFromTriplets(triplets.begin(), triplets.end());
		jacobian.makeCompressed();

	}
	
    //KASKADENMODIFIKATION ---------------------------------

    void addFlash(int id) {
        Flash<RealType> f(_numS, _a);
        _flashes.insert(std::pair<int, Flash<RealType>>(id,f));
    }

    Flash<>& getFlash(int id) {
        return _flashes.at(id); //vielleicht noch besser machen
    }

    void deleteFlash(int id) {
		if (_flashes.at(id).LinM() != nullptr) _flashes.at(id).LinM()->LoutM() = nullptr;
		if (_flashes.at(id).VinM() != nullptr) _flashes.at(id).VinM()->VoutM() = nullptr;
		if (_flashes.at(id).LoutM() != nullptr) _flashes.at(id).LoutM()->LinM() = nullptr;
		if (_flashes.at(id).VoutM() != nullptr) _flashes.at(id).VoutM()->VinM() = nullptr;
        _flashes.erase(id);
    }

    void connectFlashes(int id1, int id2, int phase) {
        if(phase == 1) {
			if (_flashes.at(id1).LoutM() != nullptr) _flashes.at(id1).LoutM()->LinM() = nullptr;
			if (_flashes.at(id2).LinM() != nullptr) _flashes.at(id2).LinM()->LoutM() = nullptr;
            _flashes.at(id1).LoutM() = &_flashes.at(id2);
            _flashes.at(id2).LinM() = &_flashes.at(id1);
            qDebug() << "Cascade connected: " << id1 << id2 << "Liquid";
        } else if(phase == 2) {
			if (_flashes.at(id1).VoutM() != nullptr) _flashes.at(id1).VoutM()->VinM() = nullptr;
			if (_flashes.at(id2).VinM() != nullptr) _flashes.at(id2).VinM()->VoutM() = nullptr;
            _flashes.at(id1).VoutM() = &_flashes.at(id2);
            _flashes.at(id2).VinM() = &_flashes.at(id1);
            qDebug() << "Cascade connected: " << id1 << id2 << "Vapor";
        }
    }
};

#endif // CASCADE_H
