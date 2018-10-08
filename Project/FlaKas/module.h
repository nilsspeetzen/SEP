#ifndef MODULE_H
#define MODULE_H

#include "lin_sys.h"

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
    typedef Matrix<RealType,Dynamic,1> VTS;
    typedef Matrix<RealType,Dynamic,Dynamic> MTS;
    typedef Matrix<RealType,Dynamic,1> VTP;
protected:
    VTS _x;
public:
    Module() {}

    VTS& x() { return _x; }
    RealType& x(int i) { return _x(i); }

    virtual RealType Lin() const=0;
    virtual RealType Vin() const=0;
    virtual RealType Lout() const=0;
    virtual RealType Vout() const=0;
    virtual VTS f()=0;
};

#endif // MODULE_H
