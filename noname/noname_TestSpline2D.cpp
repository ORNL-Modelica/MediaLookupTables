#include <noname_Spline2D.hpp>
#include <noname_TestAssertionMacros.hpp>

#include <iostream>
#include <numeric>

using namespace noname;

std::vector<double> iota_vector(size_t n, double val = 0.) {
  std::vector<double> v(n);
  std::iota(v.begin(), v.end(), val);
  return v;
}

template <typename T> void array_of_values_wrong_size() {
  size_t const nx = 4;
  size_t const ny = 6;
  std::vector<double> const x = iota_vector(nx);
  std::vector<double> const y = iota_vector(ny);
  ASSERT_NO_THROW(Spline2D<T>(x, y, std::vector<double>(nx * ny)));
  ASSERT_THROW(Spline2D<T>(x, y, std::vector<double>(nx * ny + 1)),
               std::invalid_argument);
  ASSERT_THROW(Spline2D<T>(x, y, std::vector<double>(nx * ny - 2)),
               std::invalid_argument);
}

template <typename T> void minimum_grid_points() {
  auto const min_size =
      gsl_interp2d_type_min_size(Interp2DTypeTraits<T>::value());
  std::vector<double> too_small = iota_vector(min_size - 1);
  std::vector<double> ok = iota_vector(min_size);
  ASSERT_NO_THROW(
      Spline2D<T>(ok, ok, std::vector<double>(min_size * min_size)));
  ASSERT_THROW(Spline2D<T>(ok, too_small,
                           std::vector<double>(min_size * (min_size - 1))),
               std::invalid_argument);
  ASSERT_THROW(Spline2D<T>(too_small, ok,
                           std::vector<double>(min_size * (min_size - 1))),
               std::invalid_argument);
}

template <typename T> void points_are_sorted_and_unique() {
  std::vector<double> const ok = iota_vector(4);
  std::vector<double> const not_sorted = {3, 2, 1, 4};
  std::vector<double> const has_duplicates = {1, 2, 2, 4};
  std::vector<double> z(16);
  ASSERT_NO_THROW(Spline2D<T>(ok, ok, z));
  ASSERT_THROW(Spline2D<T>(ok, not_sorted, z), std::invalid_argument);
  ASSERT_THROW(Spline2D<T>(not_sorted, ok, z), std::invalid_argument);
  ASSERT_THROW(Spline2D<T>(ok, has_duplicates, z), std::invalid_argument);
  ASSERT_THROW(Spline2D<T>(has_duplicates, ok, z), std::invalid_argument);
}

template <typename T> void out_of_domain() {
  std::vector<double> const x = iota_vector(10, 1);
  std::vector<double> const z(x.size() * x.size());
  Spline2D<T> spline(x, x, z);
  double const ok = x.front() + (x.back() - x.front()) / 2;
  double const out_of_range = x.front() + x.back();
  ASSERT_NO_THROW(spline.eval(ok, ok));
  ASSERT_NO_THROW(spline.eval_deriv_x(ok, ok));
  ASSERT_NO_THROW(spline.eval_deriv_y(ok, ok));
  ASSERT_NO_THROW(spline.eval_deriv_xx(ok, ok));
  ASSERT_NO_THROW(spline.eval_deriv_yy(ok, ok));
  ASSERT_NO_THROW(spline.eval_deriv_xy(ok, ok));
  ASSERT_THROW(spline.eval(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_x(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_y(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xx(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_yy(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xy(out_of_range, ok), std::domain_error);
  ASSERT_THROW(spline.eval(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_x(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_y(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xx(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_yy(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xy(ok, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval(out_of_range, out_of_range), std::domain_error);
  ASSERT_THROW(spline.eval_deriv_x(out_of_range, out_of_range),
               std::domain_error);
  ASSERT_THROW(spline.eval_deriv_y(out_of_range, out_of_range),
               std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xx(out_of_range, out_of_range),
               std::domain_error);
  ASSERT_THROW(spline.eval_deriv_yy(out_of_range, out_of_range),
               std::domain_error);
  ASSERT_THROW(spline.eval_deriv_xy(out_of_range, out_of_range),
               std::domain_error);

  ASSERT_NO_THROW(spline.eval(x.front(), ok));
  ASSERT_NO_THROW(spline.eval(ok, x.back()));
  ASSERT_THROW(spline.eval(x.back() + 1e-12, ok), std::domain_error);
  ASSERT_THROW(spline.eval(ok, x.front() - 1e-12), std::domain_error);
}

int main() {

  std::cout << "out of domain\n";
  out_of_domain<Bicubic>();
  out_of_domain<Bilinear>();

  std::cout << "wrong size\n";
  array_of_values_wrong_size<Bicubic>();
  array_of_values_wrong_size<Bilinear>();

  std::cout << "min size\n";
  minimum_grid_points<Bicubic>();
  minimum_grid_points<Bilinear>();

  std::cout << "sorted and unique\n";
  points_are_sorted_and_unique<Bicubic>();
  points_are_sorted_and_unique<Bilinear>();

  std::cout << "done!\n";
}
