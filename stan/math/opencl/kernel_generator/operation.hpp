#ifndef STAN_MATH_OPENCL_KERNEL_GENERATOR_OPERATION_HPP
#define STAN_MATH_OPENCL_KERNEL_GENERATOR_OPERATION_HPP
#ifdef STAN_OPENCL

#include <stan/math/opencl/kernel_generator/type_str.hpp>
#include <stan/math/opencl/kernel_generator/name_generator.hpp>
#include <stan/math/opencl/matrix_cl_view.hpp>
#include <stan/math/opencl/matrix_cl.hpp>
#include <stan/math/opencl/kernel_cl.hpp>
#include <cl.hpp>
#include <string>
#include <map>

namespace stan {
namespace math {

/**
 * Parts of an OpenCL kernel, generated by an expression
 */
struct kernel_parts {
  std::string body, args;
};

/**
 * Base of all kernel generator operation. Needed for common instance counting.
 */
class operation_base {
 public:
  operation_base() : instance(instance_counter++) {}

 protected:
  static int instance_counter;
  int instance;
};

int operation_base::instance_counter = 0;

/**
 * Base for all kernel generator operations.
 * @tparam Derived derived type
 * @tparam ReturnScalar scalar type of the  result
 */
template <typename Derived, typename ReturnScalar>
class operation : public operation_base {
 public:
  static const int dynamic = -1;

  /**
   * Evaluates the expression.
   * @return Result of the expression.
   */
  matrix_cl<ReturnScalar> eval() const {
    matrix_cl<ReturnScalar> res(derived().rows(), derived().cols(),
                                derived().view());
    this->evaluate_into(res);
    return res;
  }

  /**
   * Convecting to \c matrix_cl evaluates the expression. Used when assigning to
   * a \c matrix_cl.
   */
  operator matrix_cl<ReturnScalar>() const { return derived().eval(); }

  /**
   * Evaluates the expression into given left-hand-side expression.
   * @tparam T_lhs type of the Left-hand-side expression
   * @param lhs Left-hand-side expression
   */
  template <typename T_lhs>
  inline void evaluate_into(T_lhs&& lhs) const;

 protected:
  mutable std::string var_name;
  static std::map<std::string, cl::Kernel> kernel_cache;

  /**
   * Casts the instance into its derived type.
   * @return \c this cast into derived type
   */
  inline Derived& derived() { return *static_cast<Derived*>(this); }

  /**
   * Casts the instance into its derived type.
   * @return \c this cast into derived type
   */
  inline const Derived& derived() const {
    return *static_cast<const Derived*>(this);
  }
};

template <typename Derived, typename ReturnScalar>
std::map<std::string, cl::Kernel>
    operation<Derived, ReturnScalar>::kernel_cache;

}  // namespace math
}  // namespace stan

#endif
#endif

#include <stan/math/opencl/kernel_generator/evaluate_into.hpp>
