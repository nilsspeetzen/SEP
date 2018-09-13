#ifndef FLASH_H
#define FLASH_H

#include "module.h"

/**
 * @brief Flashmodul
 *
 * Ein spezifisches Bauteil. Es handelt sich um einen Flash, einen
 * einstufigen Entspannungsverdampfer.
 *
 * Enthält ein Nonlin_sys
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

public:
    Flash(){
        /**
         * _x enthält die folgenden Variablen:
         * Lin, Lout, Vin, Vout, T, xini, yini, xi, yi, ki, pi
         *
         * Parameter:
         * F, xfi, Vout, pg, aij
         */
        _x = 2;
    }
    VTS f();
    MTS dfdx();
};

#endif // FLASH_H
