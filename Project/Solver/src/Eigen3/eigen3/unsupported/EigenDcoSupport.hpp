#ifndef EIGENDCOSUPPORT_HPP
#define EIGENDCOSUPPORT_HPP

#include "Eigen/Core"
#include "dco/dco.hpp"

namespace dco {

template<typename T>
inline const  dco::gt1s_t<T>& conj(const  dco::gt1s_t<T>& x)  { return x; }

template<typename T>
inline const dco::gt1s_t<T>& real(const dco::gt1s_t<T>& x)  { return x; }

template<typename T>
inline dco::gt1s_t<T> imag(const dco::gt1s_t<T>&)    { return 0.; }

template<typename T>
inline dco::gt1s_t<T> abs(const dco::gt1s_t<T>&  x)  { return fabs(x); }

template<typename T>
inline dco::gt1s_t<T> abs2(const dco::gt1s_t<T>& x)  { return x*x; }
}

namespace Eigen {

template<typename T> struct NumTraits<dco::gt1s_t<T>>
    : NumTraits<T>
{
  typedef dco::gt1s_t<T> Real;
  typedef dco::gt1s_t<T> NonInteger;
  typedef dco::gt1s_t<T> Nested;
  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned = 1,
    RequireInitialization = 1,
    ReadCost = 1,
    AddCost = 1,
    MulCost = 1
  };
};

template<typename T, typename BinaryOp>
struct ScalarBinaryOpTraits<T,dco::gt1s_t<T>,BinaryOp>
{
  typedef dco::gt1s_t<T> ReturnType;
};

template<typename T, typename BinaryOp>
struct ScalarBinaryOpTraits<dco::gt1s_t<T>,T,BinaryOp>
{
  typedef dco::gt1s_t<T> ReturnType;
};

}


#endif // EIGENDCOSUPPORT_HPP
