#ifndef EUCLIDEANNORM_HPP
#define EUCLIDEANNORM_HPP

#include <vector>
#include <cmath>

template<typename TREAL> TREAL EuclideanNorm(const std::vector<TREAL>& x){
  using std::sqrt;
  size_t n = x.size();
  TREAL sum = 0;
  for(unsigned i = 0; i < n; i++)
    sum += x[i]*x[i];
  return sqrt(sum);
}

#endif // EUCLIDEANNORM_HPP
