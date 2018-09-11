#ifndef MODULE_H
#define MODULE_H

#include "LU" // habe in der FlaKas.pro den Includepfad zu Eigen reingetan

/**
 * @brief Akstrakte Klasse für allgemeine Bauteile
 *
 * Diese Klasse ist eine allgemeine Klasse, welche die In- und Outputs
 * definiert und die innere Operation abstrakt lässt.
 */

//TODO allgemeine GLS als NonlinSys z.B. einfügen (abstrakt, wie z.B. in dem alten Projekt), Input/Output sind x und rechte seite?

class Module
{
private:

public:
    Module();
};

#endif // MODULE_H
