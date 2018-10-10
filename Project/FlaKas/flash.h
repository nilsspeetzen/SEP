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

template<typename RealType=double>
class Flash : public Module<RealType>
{
    typedef Matrix<RealType,Dynamic,1> VT;
    typedef Matrix<RealType,Dynamic,Dynamic> MT;

    using Module<RealType>::_x;
    using Module<RealType>::Lin;
    using Module<RealType>::Lout;
    using Module<RealType>::Vin;
    using Module<RealType>::Vout;

private:
    /**
     * _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
     */
    RealType _pg, _F;
    Matrix<RealType,Dynamic,1> _xf;
    const int _numS;
    Matrix<RealType,Dynamic,7> _a;

    //connection ids
    int _LinM, _VinM, _LoutM, _VoutM;

public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    Flash(int numSubstances, Matrix<RealType,Dynamic,7> a) :
            Module<RealType>(), _numS(numSubstances), _a(a) {
        _x = VT::Zero(5 + 6*numSubstances);
        _pg = 1000;
        _xf = Matrix<RealType,Dynamic,1>::Zero(numSubstances);
        _xf(0) = 0.5; _xf(1) = 0.5;
        _LinM = -1; _LoutM = -1; _VinM = -1; _VoutM = -1;
    }
	Flash() : Module<RealType>(), _numS(2) {} //Für die map (Standardkonstruktor)

    constexpr int numVariables() { return 5 + 6*_numS; }
    constexpr int numEquations() { return 5 + 6*_numS; }

    RealType& pg() { return _pg; }
    RealType& F() { return _F; }

    int& LinM() { return _LinM; }
    int& VinM() { return _VinM; }
    int& LoutM() { return _LoutM; }
    int& VoutM() { return _VoutM; }

    inline RealType Lin()  const { return _x(0); }
    inline RealType Lout() const { return _x(1); }
    inline RealType Vin()  const { return _x(2); }
    inline RealType Vout() const { return _x(3); }
    inline RealType T()    const { return _x(4); }
    inline RealType xini(int i) const { return _x(5+i); }
    inline RealType yini(int i) const { return _x(5+i+_numS); }
    inline RealType xi(int i) const { return _x(5+i+2*_numS); }
    inline RealType yi(int i) const { return _x(5+i+3*_numS); }
    inline RealType ki(int i) const { return _x(5+i+4*_numS); }
    inline RealType pi(int i) const { return _x(5+i+5*_numS); }

    /**
     * @brief f ; muss gleich null sein um das NLS zu lösen (aus Aufgabenstellung)
     * @return f in Abhängigkeit der Variablen
     */
    VT f() {
        VT r = VT::Zero(_x.size());
        //GLS
        for(int i = 0; i<_numS; i++) {
            r(i)            = _F*_xf(i) + Lin()*xini(i) + Vin()*yini(i) - Lout()*xi(i) - Vout()*yi(i); //1
            r(_numS + i)    = yi(i) - ki(i)*xi(i); //2
            r(0+2*_numS)   += xi(i); //3
            r(1+2*_numS)   += yi(i); //4
            r(2+2*_numS + i)= ki(i) - pi(i)/_pg; //5
            r(2+3*_numS + i)= log(pi(i)) - (_a(i,0) + _a(i,1)/(T()+_a(i,2)) + _a(i,3)*T() + _a(i,4)*log(T()) + _a(i,5)*pow(T(),_a(i,6))); //6
        }
        r(0+2*_numS) -= 1; //3
        r(1+2*_numS) -= 1; //4

        //Input und Output
        r(2+4*_numS) = Vout() - 50;
        r(3+4*_numS) = Lin() - 100;
        r(4+4*_numS) = Vin() - 0;
        for(int i = 0; i<_numS; i++) {
            r(4+4*_numS+i) = xini(i) - 0.5;
            r(4+5*_numS+i) = yini(i) - 0.5;
        }
        return r;
    }
};

#endif // FLASH_H
