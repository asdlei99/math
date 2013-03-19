#ifndef __STAN__MATH__MATRIX__DIMS_HPP__
#define __STAN__MATH__MATRIX__DIMS_HPP__

#include <stan/math/matrix.hpp>

namespace stan {
  namespace math {

    template <typename T>
    inline
    void
    dims(const T& x, std::vector<int>& result) {
      /* no op */
    }
    template <typename T, int R, int C>
    inline void
    dims(const Eigen::Matrix<T,R,C>& x,
         std::vector<int>& result) {
      result.push_back(x.rows());
      result.push_back(x.cols());
    }
    template <typename T>
    inline void
    dims(const std::vector<T>& x,
         std::vector<int>& result) {
      result.push_back(x.size());
      if (x.size() > 0)
        dims(x[0],result);
    }

    template <typename T>
    inline std::vector<int>
    dims(const T& x) {
      std::vector<int> result;
      dims(x,result);
      return result;
    }

  }
}
#endif
