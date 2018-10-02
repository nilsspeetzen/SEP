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

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class Flash : public Module<>
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;
private:

    TS _pg, _F;
    Matrix<TS,Dynamic,1> _xf;
    int _numS;
    Matrix<TS,Dynamic,7> _a;

    //connections
    Module<TS,NP,NS,TP>* _LinM;
    Module<TS,NP,NS,TP>* _VinM;
    Module<TS,NP,NS,TP>* _LoutM;
    Module<TS,NP,NS,TP>* _VoutM;

public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    Flash(int numSubstances, Matrix<TS,Dynamic,7> a) :
            Module<TS, NP, NS, TP>(), _numS(numSubstances), _a(a) {
        /**
         * _x: Lin, Lout, Vin, Vout, T, xini, yini, xi..., yi..., ki..., pi...
         */
        static_assert(NP==-1&&NS==-1,"Wrong NP or NS!");
        _x = VTS::Zero(5 + 6*numSubstances);
        _pg = 1000;
        _xf = Matrix<TS,Dynamic,1>::Zero(numSubstances);
        _xf(0) = 0.5;
        _xf(1) = 0.5;
    }
    /**
     * @brief Zugriff auf pg (Druck im Flash)
     * @return _pg
     */
    double& pg() { return _pg; }
    /**
     * @brief Zuriff auf F (Flüssiger Zustrom)
     * @return _F
     */
    double& F() { return _F; }

    Module<TS,NP,NS,TP>* LinM() { return _LinM; }
    Module<TS,NP,NS,TP>* VinM() { return _VinM; }
    Module<TS,NP,NS,TP>* LoutM() { return _LoutM; }
    Module<TS,NP,NS,TP>* VoutM() { return _VoutM; }

    TS& Lin() { return _x(0); }
    TS& Lout() { return _x(1); }
    TS& Vin() { return _x(2); }
    TS& Vout() { return _x(3); }
    TS& T() { return _x(4); }
    TS& xini(int i) { return _x(5+i); }
    TS& yini(int i) { return _x(5+i+_numS); }
    TS& xi(int i) { return _x(5+i+2*_numS); }
    TS& yi(int i) { return _x(5+i+3*_numS); }
    TS& ki(int i) { return _x(5+i+4*_numS); }
    TS& pi(int i) { return _x(5+i+5*_numS); }

    /**
     * @brief f ; muss gleich null sein um das NLS zu lösen (aus Aufgabenstellung)
     * @return f in Abhängigkeit der Variablen
     */
    VTS f() {
        VTS r = VTS::Zero(_x.size());
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
    MTS dfdx() {
        MTS drdx = MTS::Zero(_x.size(), _x.size());
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
