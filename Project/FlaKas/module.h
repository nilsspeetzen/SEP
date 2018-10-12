#ifndef MODULE_H
#define MODULE_H

#include "Eigen/LU"
using namespace Eigen;

/**
 * @file module.h
 * @brief Basismodul
 */

/**
 * @brief Akstrakte Klasse für allgemeine Bauteile
 *
 * Diese Klasse ist eine allgemeine Klasse, welche die In- und Outputs
 * definiert und die innere Operation abstrakt lässt.
 */

template<typename RealType=double>
class Module
{
    typedef Matrix<RealType,Dynamic,1> VT;
protected:
    VT _x;
public:
    Module() {}

    VT& x() { return _x; }
    RealType& x(int i) { return _x(i); }

    virtual VT f(VT v)=0;
};

#endif // MODULE_H
