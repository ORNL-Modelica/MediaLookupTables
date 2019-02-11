#include <noname.h>
#include <noname_SplineMap.hpp>

using namespace noname;

double noname_spline2d_eval(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval(x, y);
}
double noname_spline2d_eval_deriv_x(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval_deriv_x(x, y);
}
double noname_spline2d_eval_deriv_y(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval_deriv_y(x, y);
}
double noname_spline2d_eval_deriv_xx(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval_deriv_xx(x, y);
}
double noname_spline2d_eval_deriv_yy(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval_deriv_yy(x, y);
}
double noname_spline2d_eval_deriv_xy(char const *o, double x, double y) {
  auto spline = SplineMap::get_spline(o);
  return spline->eval_deriv_xy(x, y);
}

// NOTE can get rid of these with C++17
char constexpr Interp2DTypeTraits<Bicubic>::name[];
char constexpr Interp2DTypeTraits<Bilinear>::name[];
