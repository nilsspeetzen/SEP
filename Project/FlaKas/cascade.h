#ifndef CASCADE_H
#define CASCADE_H
#include "flash.h"
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
template<typename RealType=double>
class cascade {
    typedef Matrix<RealType,Dynamic,1> VT;
    typedef Matrix<RealType,Dynamic,Dynamic> MT;
private:
    int _numS;
    VT _x;
    Matrix<RealType,Dynamic,7> _a;
    std::map <int,Flash<RealType>> _flashes;
public:
    /**
     * @brief Konstruktor
     * @param numSubstances Anzahl der verschiedenen Substanzen im Gemisch
     * @param a Antoine-Parameter für die Substanzen (numSubstances*7 Matrix)
     */
    cascade(int numSubstances, Matrix<RealType,Dynamic,7> a) : _numS(numSubstances), _a(a) {}
    cascade() {}

    //FÜR ESO ----------------------------------------------

    static constexpr int numVariables(VarGroup<0>&) { return 5 + 6; }
    static constexpr int numEquations(EqGroup<0>&) { return 5 + 6; }

    template<int K>
    inline void setVariable(VarGroup<K> , const int index, const RealType& value){
        static_assert (K==0,"only one variable group present" );
        _x[index] = value;
    }

    template<int K>
    inline RealType getVariable(VarGroup<K> , const int index) const {
        static_assert (K==0,"only one variable group present" );
        return _x[index];
    }

    template<int K>
    inline RealType eval(EqGroup<0>, EqIndex<K>) const {
        //HIER MÜSSEN DIE ENTSPRECHENDEN GL DER FLASHES UND DIE VERBINDUNGSGL HIN
        return _flashes.at(K).T();
    }

    //KASKADENMODIFIKATION ---------------------------------
    /**
     * @brief Erstellt einne neuen Flash
     * @param id ID
     */
    void addFlash(int id) {
        Flash<RealType> f(_numS, _a);
        _flashes.insert(std::pair<int, Flash<RealType>>(id,f));
    }

    /**
     * @brief Zugriff auf einen gespeicherten Flash
     * @param id ID
     * @return Flash mit angegebener id
     */
    Flash<>& getFlash(int id) {
        return _flashes.at(id); //vielleicht noch besser machen
    }

    /**
     * @brief Löscht einen Flash
     * @param id ID
     */
    void deleteFlash(int id) {
        _flashes.erase(id);
    }

    /**
     * @brief Speichert Verbindung in den Flashes
     * @param id1 OutputID
     * @param id2 InputID
     * @param phase 1:Flüssig 2:Gasförmig
     */
    void connectFlashes(int id1, int id2, int phase) {
        if(phase == 1) {
            _flashes[id1].LoutM() = id2;
            _flashes[id2].LinM() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Liquid";
        } else if(phase == 2) {
            _flashes[id1].VoutM() = id2;
            _flashes[id2].VinM() = id1;
            qDebug() << "Cascade connected: " << id1 << id2 << "Vapor";
        }
    }
};

#endif // CASCADE_H
