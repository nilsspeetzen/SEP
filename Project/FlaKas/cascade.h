#ifndef CASCADE_H
#define CASCADE_H
#include "flash.h"

/**
 * @brief Enthält alle Flashes
 *
 */

//TODO erstmal einen Flash einfügen können und den dann Lösen
//später dann eine Funktion zum darstellen

class cascade
{
private:
    int numSubstances;
    Flash<>* f;
public:
    /**
     * @brief Konstruktor
     */
    cascade(int numSubstances) : numSubstances(numSubstances) {

    }
    /**
     * @brief addFlash
     */
    void addFlash() {

    }
    /**
     * @brief solve
     * Löst das NLS der Kaskade und trägt die Lösung in den Flashes ein
     */
    void solve() {

    }
};

#endif // CASCADE_H
