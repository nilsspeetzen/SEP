#ifndef MODULE_H
#define MODULE_H

#include "LU" // habe in der FlaKas.pro den Includepfad zu Eigen reingetan
#include "nonlin_sys.h"

/**
 * @brief Akstrakte Klasse für allgemeine Bauteile
 *
 * Diese Klasse ist eine allgemeine Klasse, welche die In- und Outputs
 * definiert und die innere Operation abstrakt lässt.
 */

//TODO allgemeine GLS als NonlinSys z.B. einfügen (abstrakt, wie z.B. in dem alten Projekt), Input/Output sind x und rechte seite?

template<typename TS=double, int NP=Dynamic, int NS=Dynamic, typename TP=TS>
class Module : public NONLINEAR_SYSTEM<TS, NP, NS, TP>
{
protected:
public:
    Module();
};

#endif // MODULE_H
