#ifndef STAN_MATH_PRIM_SCAL_PROB_EXPONENTIAL_LCDF_HPP
#define STAN_MATH_PRIM_SCAL_PROB_EXPONENTIAL_LCDF_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/err/check_nonnegative.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <stan/math/prim/scal/err/check_positive_finite.hpp>
#include <stan/math/prim/scal/fun/size_zero.hpp>
#include <stan/math/prim/scal/fun/value_of.hpp>
#include <cmath>

namespace stan {
namespace math {

template <typename T_y, typename T_inv_scale>
inline auto exponential_lcdf(const T_y& y, const T_inv_scale& beta) {
  using T_partials = partials_return_t<T_y, T_inv_scale>;
  T_partials cdf_log(0.0);

  using std::exp;
  using std::log;

  static const char* function = "exponential_lcdf";
  check_not_nan(function, "Random variable", y);
  check_nonnegative(function, "Random variable", y);
  check_positive_finite(function, "Inverse scale parameter", beta);

  const scalar_seq_view<T_y> y_vec(y);
  const scalar_seq_view<T_inv_scale> beta_vec(beta);
  const size_t N = max_size(y, beta);
  operands_and_partials<T_y, T_inv_scale> ops_partials(y, beta);
  if (size_zero(y, beta)) {
    return ops_partials.build(cdf_log);
  }

  for (size_t n = 0; n < N; n++) {
    const T_partials beta_dbl = value_of(beta_vec[n]);
    const T_partials y_dbl = value_of(y_vec[n]);
    T_partials one_m_exp = 1.0 - exp(-beta_dbl * y_dbl);
    cdf_log += log(one_m_exp);

    T_partials rep_deriv = -exp(-beta_dbl * y_dbl) / one_m_exp;
    if (!is_constant_all<T_y>::value) {
      ops_partials.edge1_.partials_[n] -= rep_deriv * beta_dbl;
    }
    if (!is_constant_all<T_inv_scale>::value) {
      ops_partials.edge2_.partials_[n] -= rep_deriv * y_dbl;
    }
  }
  return ops_partials.build(cdf_log);
}

}  // namespace math
}  // namespace stan
#endif
