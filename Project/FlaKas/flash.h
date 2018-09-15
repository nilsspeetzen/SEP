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
 * Besitzt durch Module die Eigenschaften eines NLS, erweitert mit ID...
 *
 */

//TODO die GLS vom Module genauer machen, visuelles Zeug

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class Flash : public Module<>
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;
private:
    /**
     * Lin, Lout, Vin, Vout, T, pg, a
     * xini, yini, xi, yi, ki, pi
     *
     * Kaskaden - Parameter:
     * F, xfi, Vout
     */
    TS _pg, _F;
    Matrix<TS,Dynamic,1> _xf;
    Matrix<TS,Dynamic,7> _a;
    int _numS;

public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    Flash(int numSubstances, Matrix<TS,Dynamic,7> a) :
            Module<TS, NP, NS, TP>(2 + 4*numSubstances, 5 + 6*numSubstances), _numS(numSubstances), _a(a) {
        /**
         * _x: Lin, Lout, Vin, Vout, T, xini..., yini..., xi..., yi..., ki..., pi...
         * _p: Nullvektor
         */
        static_assert(NP==-1&&NS==-1,"Wrong NP or NS!");
        _x = VTS::Zero(5 + 6*numSubstances);
        _p = VTP::Zero(2 + 4*numSubstances);
        _F = 0; _pg = 1000; _xf = Matrix<TS,Dynamic,1>::Zero(numSubstances); _xf(1) = 1;
    }
    /**
     * @brief Zugriff auf pg (Druck im Flash)
     * @return _pg
     */
    double& pg() {
        return _pg;
    }
    /**
     * @brief Zuriff auf F (Flüssiger Zustrom)
     * @return _F
     */
    double& F() {
        return _F;
    }

    /**
     * @brief f ; muss gleich null sein um das NLS zu lösen (aus Aufgabenstellung)
     * @return f in Abhängigkeit der Variablen
     */
    VTS f() {
        VTS r = VTS::Zero(2+4*_numS);
        r(2*_numS)  = -1;
        r(1+2*_numS)= -1;
        for(int i = 0; i<_numS; i++) {
            r(i)           = _F*_xf(i) + _x(0)*_x(5+i) + _x(2)*_x(5+_numS+i) - _x(1)*_x(5+2*_numS+i) - _x(3)*_x(5+3*_numS+i);
            r(_numS + i)    = _x(5+3*_numS+i) - _x(5+4*_numS+i)*_x(5+2*_numS+i);
            r(0+2*_numS)   += _x(5+2*_numS+i);
            r(1+2*_numS)   += _x(5+3*_numS+i);
            r(2+2*_numS + i)= _x(5+4*_numS+i) - _x(5+5*_numS+i)/_pg;
            r(2+3*_numS + i)= log(_x(5+5*_numS+i)) - (_a(i,0) + _a(i,1)/(_x(4)+_a(i,2)) + _a(i,3)*_x(4) + _a(i,4)*log(_x(4)) + _a(i,5)*pow(_x(4),_a(i,6)));
        }
        std::cout << r;
        return r;
    }
    /**
     * @brief dfdx ; für den Newton-Löser
     * @return dfdx in Abhängigkeit der Variablen
     */
    MTS dfdx() {
        MTS drdx = MTS::Zero(2+4*_numS, 5+6*_numS);
        //TODO

        for(int j=0; j<_numS; j++) {
            drdx(0+2*_numS,5+2*_numS+j)=1;
        }

        for(int j=0; j<_numS; j++) {
            drdx(1+2*_numS,5+3*_numS+j)=1;
        }

        for(int i = 0; i<_numS; i++) {
            drdx(i,0)   = _x(5+i);
            drdx(i,1)   = -_x(5+2*_numS+i);
            drdx(i,2)   = _x(5+_numS+i);
            drdx(i,3)   = -_x(5+3*_numS+i);
            for(int j=0; j<_numS; j++) {
                drdx(i,5+j)         = _x(0);
                drdx(i,5+_numS+j)    = _x(2);
                drdx(i,5+2*_numS+j)  = -_x(1);
                drdx(i,5+3*_numS+j)  = -_x(3);
            }

            for(int j=0; j<_numS; j++) {
                drdx(_numS+i,5+2*_numS+j)  = -_x(5+4*_numS+j);
                drdx(_numS+i,5+3*_numS+j)  = 1;
                drdx(_numS+i,5+4*_numS+j)  = -_x(5+2*_numS+j);
            }

            for(int j=0; j<_numS; j++) {
                drdx(2+2*_numS+i,5+4*_numS+j)=1;
                drdx(2+2*_numS+i,5+5*_numS+j)=-1/_pg;
            }

            drdx(2+3*_numS+i,4)=-_a(i,1)/(pow(_x(4)+_a(2), 2)) - _a(3) - _a(4)/_x(4) - _a(5)*_a(6)*pow(_x(4), _a(6)-1);
            for(int j=0; j<_numS; j++) {
                drdx(2+3*_numS+i,5+5*_numS+j)=1/_x(5+5*_numS+j);
            }
        }
        return drdx;
    }
};

#endif // FLASH_H
