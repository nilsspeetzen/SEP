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
template<typename RealType, typename ConstType=RealType>
class cascade {
    typedef Matrix<RealType,Dynamic,1> VT;
    typedef Matrix<RealType,Dynamic,Dynamic> MT;
protected:
    int _numS;
    Matrix<ConstType,Dynamic,7> _a;
    std::map <int,Flash<RealType, ConstType>> _flashes;
public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    cascade(int numSubstances, Matrix<ConstType,Dynamic,7> a) : _numS(numSubstances), _a(a) {}
	cascade() {}

	Matrix<ConstType, Dynamic, 7>& a() { return _a; }
	std::map <int, Flash<RealType, ConstType>>& flashes() { return _flashes; }

    //FÜR ESO ----------------------------------------------

	RealType& x(int i) { 
		Matrix<int, 2, 1> pos = getIdAndRestForEquation(i);
		int id = pos(0);
		int varid = pos(1);
		return _flashes.at(id).x(i);
	}

	int numVariables() {
		int numV = 0;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			numV += i->second.numVariables();
		}
		return numV;
	}

	int numEquations() { 
		int numE = 0;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			numE += i->second.numEquations();
		}
		return numE; 
	}

	template<class X, class V>
	inline void setVariable(X variableIndex, const V& variableValue)
	{
		Matrix<int, 2, 1> pos = getIdAndRestForEquation(variableIndex);
		int id = pos(0);
		int varid = pos(1);
		return _flashes.at(id).setVariable(varid, variableValue);
	}

	template<class X, class V>
	inline void setVariables(X indices, const V& values) {
		int irow = 0;
		for (auto i = cbegin(indices); i != cend(indices); ++i, ++irow) {
			qDebug() << "Setze: " << *i << values(irow);
			setVariable(*i, values(irow));
		}
	}

	//Eval
	Matrix<int, 2, 1> getIdAndRestForEquation(int eq) {
		Matrix<int, 2, 1> id;
		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			if (eq < i->second.numEquations()) {
				id(0) = i->first;
				id(1) = eq;
				break;
			}
			eq -= i->second.numEquations();
		}
		return id;
	}
	
	RealType eval(int eq) {
		Matrix<int, 2, 1> pos = getIdAndRestForEquation(eq);
		//qDebug() << "Flash: " << pos(0) << "for Equation:" << eq << pos(1);
		int id = pos(0);
		int eqid = pos(1);
		return _flashes.at(id).f(eqid);
	}

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

	//Derivative
	template<class X, class Y, typename Jacobian>
	inline void evalBlockJacobian(const X& eqIndices, const Y& varIndices, Jacobian& jac) {
		using std::cbegin;
		using std::cend;
		int jcol = 0;
		for (auto j = cbegin(varIndices); j != cend(varIndices); ++j, ++jcol) {
			int irow = 0;
			for (auto i = cbegin(eqIndices); i != cend(eqIndices); ++i, ++irow) {
				jac(irow, jcol) = evalDerivative(*i, *j);
			}
		}
	}

	RealType evalDerivative(int i, int j) {
		typedef gt1s_t<RealType> DCOTYPE;
		cascade<DCOTYPE, ConstType> tangentCascade(_numS, _a);
		tangentCascade.copyFlashSetup(flashes());
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
		cascade<Yasp, RealType> patternCascade(_numS, _a);

		for (auto i = cbegin(_flashes); i != cend(_flashes); i++) {
			patternCascade.addFlash(i->first);
		}

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
        Flash<RealType, ConstType> f(_numS, _a);
        _flashes.insert(std::pair<int, Flash<RealType, ConstType>>(id, f));
    }

    Flash<RealType, ConstType>& getFlash(int id) {
        return _flashes.at(id); //vielleicht noch besser machen
    }

    void deleteFlash(int id) {
		if (_flashes.at(id).LinID() != -1) _flashes.at(id).LinM()->LoutID() = -1;
		if (_flashes.at(id).VinID() != -1) _flashes.at(id).VinM()->VoutID() = -1;
		if (_flashes.at(id).LoutID() != -1) _flashes.at(id).LoutM()->LinID() = -1;
		if (_flashes.at(id).VoutID() != -1) _flashes.at(id).VoutM()->VinID() = -1;
        _flashes.erase(id);
    }

    void connectFlashes(int id1, int id2, int phase) {
		if(id1 == -1 || id2 == -1) return;
        if(phase == 1) {
			if (_flashes.at(id1).LoutID() != -1) _flashes.at(id1).LoutM()->LinID() = -1;
			if (_flashes.at(id2).LinID() != -1) _flashes.at(id2).LinM()->LoutID() = -1;
			_flashes.at(id1).LoutM() = &_flashes.at(id2);
			_flashes.at(id1).LoutID() = id2;
            _flashes.at(id2).LinM() = &_flashes.at(id1);
			_flashes.at(id2).LinID() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Liquid";
        } else if(phase == 2) {
			if (_flashes.at(id1).VoutID() != -1) _flashes.at(id1).VoutM()->VinID() = -1;
			if (_flashes.at(id2).VinID() != -1) _flashes.at(id2).VinM()->VoutID() = -1;
			_flashes.at(id1).VoutM() = &_flashes.at(id2);
			_flashes.at(id1).VoutID() = id2;
			_flashes.at(id2).VinM() = &_flashes.at(id1);
			_flashes.at(id2).VinID() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Vapor";
        }
    }

	template<typename R, typename C>
	void copyFlashSetup(std::map <int, Flash<R, C>> f) {
		_flashes.clear();
		for (auto i = cbegin(f); i != cend(f); i++) {
			addFlash(i->first);
		}
		for (auto i = cbegin(f); i != cend(f); i++) {
			int id = i->first;
			Flash<R,C> flash = i->second;
			connectFlashes(id, flash.LoutID(), 1);
			connectFlashes(flash.LinID(), id, 1);
			connectFlashes(id, flash.VoutID(), 2);
			connectFlashes(flash.VinID(), id, 2);
		}
	}
};

#endif // CASCADE_H
