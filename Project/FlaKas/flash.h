#ifndef FLASH_H
#define FLASH_H

#include "module.h"
#include <math.h>

/**
 * @file flash.h
 * @brief Flash als Erweiterung des Moduls
 */

/**
 * @brief Flashmodul
 *
 * Ein spezifisches Bauteil. Es handelt sich um einen Flash, einen
 * einstufigen Entspannungsverdampfer.
 *
 * Besitzt durch Module die Eigenschaften eines NLS
 *
 */

//TODO die GLS vom Module genauer machen, visuelles Zeug

template<typename RealType=double, typename ConstType=RealType>
class Flash : public Module<RealType>
{
    typedef Matrix<RealType,Dynamic,1> VT;
    typedef Matrix<RealType,Dynamic,Dynamic> MT;

    using Module<RealType>::_x;

private:
    /**
     * _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
     */
    RealType _pg, _F;
    Matrix<ConstType,Dynamic,1> _xf;
    const int _numS;
    Matrix<ConstType,Dynamic,7> _a;

    //connection flashes
	Flash* _LinM;
	Flash* _VinM;
	Flash* _LoutM;
	Flash* _VoutM;
	int _LinID, _VinID, _LoutID, _VoutID;

public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    Flash(int numSubstances, Matrix<ConstType,Dynamic,7> a) : Module<RealType>(), _numS(numSubstances), _a(a) {
		initX();
		_xf = Matrix<ConstType, Dynamic, 1>::Zero(numSubstances);
		_F = 100;
		_xf.setConstant(0.5);
		_pg = 1.013e5;
        _LinID = _LoutID = _VinID = _VoutID = -1;
    }
	Flash() : Module<RealType>(), _numS(0) {} //Für die map (Standardkonstruktor)

	int numVariables() const { return 5 + 6 * _numS; }
	int numEquations() const { return 5 + 6 * _numS; }

	int numS() const { return _numS; }
	ConstType& pg() { return _pg; }
	ConstType& F() { return _F; }

	inline int& LinID() { return _LinID; }
	inline int& VinID() { return _VinID; }
	inline int& LoutID() { return _LoutID; }
	inline int& VoutID() { return _VoutID; }
	inline Flash*& LinM() { return _LinM; }
	inline Flash*& VinM() { return _VinM; }
	inline Flash*& LoutM() { return _LoutM; }
	inline Flash*& VoutM() { return _VoutM; }
	
    inline RealType& Lin() { return _x(0); }
    inline RealType& Lout() { return _x(1); }
    inline RealType& Vin() { return _x(2); }
    inline RealType& Vout() { return _x(3); }
    inline RealType& T() { return _x(4); }
    inline RealType& xini(int i) { return _x(5+i); }
    inline RealType& yini(int i) { return _x(5+i+_numS); }
    inline RealType& xi(int i) { return _x(5+i+2*_numS); }
    inline RealType& yi(int i) { return _x(5+i+3*_numS); }
    inline RealType& ki(int i) { return _x(5+i+4*_numS); }
    inline RealType& pi(int i) { return _x(5+i+5*_numS); }

	inline void setVariable(int i, const RealType& value) {
		_x(i) = value;
	}

	inline void initX() {
		_x = VT::Zero(5 + 6 * _numS);
		Lin() = 0;
		Lout() = 0;
		Vin() = 0;
		Vout() = 0;
		T() = 273;
		for (int i = 0; i < _numS; i++) {
			xini(i) = 0;
			yini(i) = 0;
			xi(i) = 0;
			yi(i) = 0;
			ki(i) = 1;
			pi(i) = 1000;
		}
	}

    /**
     * @brief f ; muss gleich null sein um das NLS zu lösen (aus Aufgabenstellung)
     * @return f in Abhängigkeit der Variablen
     */
    RealType f(int eq) {
        VT r = VT::Zero(_x.size());
        //GLS

	/**
	 * _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
	 */
        for(int i = 0; i<_numS; i++) {
            r(i)            = _F*_xf(i) + Lin()*xini(i) + Vin()*yini(i) - Lout()*xi(i) - Vout()*yi(i); //1
            r(_numS + i)    = yi(i) - ki(i)*xi(i); //2
            r(0+2*_numS)    = r(0+2*_numS) + xi(i); //3
            r(1+2*_numS)    = r(1+2*_numS) + yi(i); //4
            r(2+2*_numS + i)= ki(i) - pi(i)/_pg; //5
            r(2+3*_numS + i)= log(pi(i)) - (_a(i,0) + _a(i,1)/(T()+_a(i,2)) + _a(i,3)*T() + _a(i,4)*log(T()) + _a(i,5)*pow(T(),_a(i,6))); //6
        }
        r(0+2*_numS) = r(0+2*_numS) - 1; //3
        r(1+2*_numS) = r(1+2*_numS) - 1; //4

        //Input und Output
		r(2 + 4 * _numS) = Vout() - 50;
		r(3 + 4 * _numS) = Lin();
		r(4 + 4 * _numS) = Vin();
		//if(_VoutID != -1)
		//	r(2+4*_numS) = Vout() - _VoutM->Vin();	// <- alt
		if(_LinID != -1)
			r(3+4*_numS) = Lin() - _LinM->Lout();
		if(_VinID != -1)
			r(4+4*_numS) = Vin() - _VinM->Vout();	// <- z.B. hier besteht nur eine Abhängigkeit von Vin() von diesem Flash

        for(int i = 0; i<_numS; i++) {
            r(5+4*_numS+i) = xini(i);
            r(5+5*_numS+i) = yini(i);

			if (_LinID != -1)
				r(5+4*_numS+i) = xini(i) - _LinM->xi(i);
			if (_VinID != -1)
				r(5+5*_numS+i) = yini(i) - _VinM->yi(i);
        }
        return r(eq);
    }
};

#endif // FLASH_H
