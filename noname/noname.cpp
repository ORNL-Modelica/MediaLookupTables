#include <noname.h>

#include <Map.hpp>
#include <Spline2D.hpp>

using namespace noname;

double bilinear_eval(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval(x, y);
}
double bilinear_eval_deriv_x(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_deriv_x(x, y);
}
double bilinear_eval_deriv_y(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_deriv_y(x, y);
}
double bilinear_eval_deriv_xx(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_deriv_xx(x, y);
}
double bilinear_eval_deriv_yy(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_deriv_yy(x, y);
}
double bilinear_eval_deriv_xy(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_deriv_xy(x, y);
}
double bilinear_eval_extrap(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bilinear>>::get_spline(o);
  return spline.eval_extrap(x, y);
}

double bicubic_eval(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval(x, y);
}
double bicubic_eval_deriv_x(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_deriv_x(x, y);
}
double bicubic_eval_deriv_y(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_deriv_y(x, y);
}
double bicubic_eval_deriv_xx(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_deriv_xx(x, y);
}
double bicubic_eval_deriv_yy(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_deriv_yy(x, y);
}
double bicubic_eval_deriv_xy(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_deriv_xy(x, y);
}
double bicubic_eval_extrap(char const *o, double x, double y) {
  auto const &spline = Map<Spline2D<Bicubic>>::get_spline(o);
  return spline.eval_extrap(x, y);
}

// NOTE can get rid of these with C++17
char constexpr Interp2DTypeTraits<Bicubic>::name[];
char constexpr Interp2DTypeTraits<Bilinear>::name[];
