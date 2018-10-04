#ifndef FLASH_H
#define FLASH_H

#include "module.h"
#include <math.h>
#include <iostream>

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
    RealType _pg, _F;
    Matrix<RealType,Dynamic,1> _xf;
    int _numS;
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
        /**
         * _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
         */
        _x = VT::Zero(5 + 6*numSubstances);
        _pg = 1000;
        _xf = Matrix<RealType,Dynamic,1>::Zero(numSubstances);
        _xf(0) = 0.5; _xf(1) = 0.5;
        _LinM = -1; _LoutM = -1; _VinM = -1; _VoutM = -1;
    }
    Flash() : Module<RealType>() {} //Für die map (Standardkonstruktor)
    /**
     * @brief Zugriff auf pg (Druck im Flash)
     * @return _pg
     */
    RealType& pg() { return _pg; }
    /**
     * @brief Zuriff auf F (Flüssiger Zustrom)
     * @return _F
     */
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
    /**
     * @brief dfdx ; für den Newton-Löser
     * @return dfdx in Abhängigkeit der Variablen
     */
    MT dfdx() {
        MT drdx = MT::Zero(_x.size(), _x.size());
        //TODO
        // _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...

        //3
        for(int j=0; j<_numS; j++) {
            drdx(0+2*_numS,5+2*_numS+j)=1;
        }

        //4
        for(int j=0; j<_numS; j++) {
            drdx(1+2*_numS,5+3*_numS+j)=1;
        }

        //1256
        for(int i = 0; i<_numS; i++) {
            // _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
            //1
            drdx(i,0)   = xini(i);
            drdx(i,1)   = -xi(i);
            drdx(i,2)   = yini(i);
            drdx(i,3)   = -yi(i);
            drdx(i,5+i)          = Lin();
            drdx(i,5+_numS+i)    = Vin();
            drdx(i,5+2*_numS+i)  = -Lout();
            drdx(i,5+3*_numS+i)  = -Vout();

            //2
            drdx(_numS+i,5+2*_numS+i)  = -ki(i);
            drdx(_numS+i,5+3*_numS+i)  = 1;
            drdx(_numS+i,5+4*_numS+i)  = -xi(i);

            //5
            drdx(2+2*_numS+i,5+4*_numS+i)=1;
            drdx(2+2*_numS+i,5+5*_numS+i)=-1/_pg;

            //6
            drdx(2+3*_numS+i,4)=-_a(i,1)/(pow(T()+_a(i,2), 2)) - _a(i,3) - _a(i,4)/T() - _a(i,5)*_a(i,6)*pow(T(), _a(i,6)-1);
            drdx(2+3*_numS+i,5+5*_numS+i)=1/pi(i);
        }

        //Input und Output
        drdx(2+4*_numS, 3) = 1;
        drdx(3+4*_numS, 0) = 1;
        drdx(4+4*_numS, 2) = 1;
        for(int i = 0; i<_numS; i++) {
            drdx(5+4*_numS+i, 5+i) = 1;
            drdx(5+5*_numS+i, 5+i+_numS) = 1;
        }
        return drdx;
    }
};

#endif // FLASH_H
