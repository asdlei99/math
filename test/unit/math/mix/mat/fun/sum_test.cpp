#include <test/unit/math/test_ad.hpp>
#include <limits>

template <typename T>
void expect_sum(const T& m) {
  auto f = [](const auto& x) { return stan::math::sum(x); };
  Eigen::VectorXd v = stan::test::to_vector(m);
  Eigen::RowVectorXd rv = stan::test::to_row_vector(m);
  auto sv = stan::test::to_std_vector(m);
  stan::test::expect_ad(f, m);
  stan::test::expect_ad(f, v);
  stan::test::expect_ad(f, rv);
  stan::test::expect_ad(f, sv);
}

TEST(MathMixMatFun, sum) {
  Eigen::MatrixXd v00(0, 0);
  expect_sum(v00);

  Eigen::MatrixXd v11(1, 1);
  v11 << 1;
  expect_sum(v11);

  Eigen::MatrixXd v12(1, 2);
  v12 << 1, 2;
  expect_sum(v12);

  Eigen::MatrixXd v31(3, 1);
  v31 << 1, 2, 3;
  expect_sum(v31);

  Eigen::MatrixXd v22(2, 2);
  v22 << 1, 2, 3, 4;
  expect_sum(v22);

  Eigen::MatrixXd v32(3, 2);
  v32 << 1, 2, 3, 4, 5, 6;
  expect_sum(v32);
}
