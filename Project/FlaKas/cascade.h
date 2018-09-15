#ifndef CASCADE_H
#define CASCADE_H
#include "flash.h"
#include <vector>

/**
 * @file cascade.h
 * @brief Enthält alle Module
 */

/**
 * @brief Enthält alle Flashes
 */

//TODO erstmal einen Flash einfügen können und den dann Lösen
//später dann eine Funktion zum darstellen

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class cascade : public NONLINEAR_SYSTEM<TS, NP, NS, TP>
{
    typedef Matrix<TS,NS,1> VTS;
    typedef Matrix<TS,NS,NS> MTS;
    typedef Matrix<TP,NP,1> VTP;
private:
    int _numS;
    Matrix<TS,Dynamic,7> _a;
    std::vector <Flash<>> flashes;
public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    cascade(int numSubstances, Matrix<TS,Dynamic,7> a) : _numS(numSubstances), _a(a) {

    }
    /**
     * @brief addFlash
     */
    void addFlash() {
        Flash<> f(_numS, _a);
        flashes.push_back(f);
    }
    /**
     * @brief getFlash
     * @param id
     * @return
     */
    Flash<>& getFlash(unsigned int id) {
        return flashes[id]; //vielleicht noch besser machen
    }
    /**
     * @brief konstruiert das NLS der Kaskade
     * @return NLS der Kaskade
     */
    VTS f() {
        //erstmal nur ein Flash, Noch nicht fertig, irgendwie müssen noch mehr gleichungen hin, dafür ist connections
        VTS r = VTS::Zero(5+6*_numS);
        r.segment(0, 2+4*_numS) = flashes[0].f();
        VTS connections = VTS::Zero(3+2*_numS);
        r.segment(2+4*_numS, 3+2*_numS) = connections;
        //TODO
        /*for(int i = 0; i < flashes.size(); i++) {

        }*/
    }
    /**
     * @brief dfdx
     * @return Abl. des NLS der Kaskade
     */
    MTS dfdx() {

    }
};

#endif // CASCADE_H
