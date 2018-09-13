#ifndef FLASH_H
#define FLASH_H

#include "module.h"
#include <math.h>

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

template<int NP=Dynamic, int NS=Dynamic>
class Flash : public Module<>
{
    typedef Matrix<double,NS,1> VTS;
    typedef Matrix<double,NS,NS> MTS;
    typedef Matrix<double,NP,1> VTP;
private:
    /**
     * Lin, Lout, Vin, Vout, T, pg, a
     * xini, yini, xi, yi, ki, pi
     *
     * Kaskaden - Parameter:
     * F, xfi, Vout
     */
    double Lin, Lout, Vin, Vout, T, pg;
    Matrix<double,Dynamic,7> a;
    int numS;

public:
    /**
     * @brief Flash
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     */
    Flash(int numSubstances) : NONLINEAR_SYSTEM<double, NP, NS, double>(NP, NS), numS(numSubstances) {
        /**
         * _x: Lout, Vout, T, xini..., yini..., xi..., yi..., ki..., pi...
         */
        static_assert(NP==-1&&NS==-1,"Wrong NP or NS!");
        _x = VTS::Zero(3 + 6*numSubstances);
        _p = VTP::Zero(2 + 4*numSubstances);
    }
    /**
     * @brief f ; muss gleich null sein um das NLS zu lösen
     * @return f in Abhängigkeit der Variablen
     */
    VTS f() {
        VTS r;
        r(2*numS)  = -1;
        r(1+2*numS)= -1;
        for(int i = 0; i<numS; i++) {
            r(i)           = Lin*_x(3+i) + Vin*_x(3+numS+i) - _x(0)*_x(3+2*numS+i) - _x(1)*_x(3+3*numS+i);
            r(numS + i)    = _x(3+3*numS+i) - _x(3+4*numS+i)*_x(3+2*numS+i);
            r(0+2*numS + i)+= _x(3+2*numS+i);
            r(1+2*numS + i)+= _x(3+3*numS+i);
            r(2+2*numS + i)= _x(3+4*numS+i) - _x(3+5*numS+i)/pg;
            r(2+3*numS + i)= log(_x(3+5*numS+i)) - (a(i,1) + a(i,2)/(T+a(i,3)) + a(i,4)*T + a(i,5)*log(T) + a(i,6)*pow(T,a(i,7)));
        }
    }
    /**
     * @brief dfdx ; für den Newton-Löser
     * @return dfdx in Abhängigkeit der Variablen
     */
    MTS dfdx() {
        MTS drdx;
        //TODO
        for(int i = 0; i<3+6*numS; i++) {
            for(int j = 0; j<3+6*numS; i++) {
                drdx(i,j) = 0;
            }
        }
    }
};

#endif // FLASH_H
