#ifndef NONAME_SPLINE_2D
#define NONAME_SPLINE_2D

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline2d.h>

#include <algorithm>
#include <exception>
#include <sstream>
#include <type_traits>
#include <vector>

namespace noname {
template <typename T> struct AllocatorTraits {
  template <typename... Args> static T *allocate(Args &&... args);
  static void free(T *);
};

template <typename T> class RAII {
public:
  template <typename Arg, typename = std::enable_if_t<
                              !std::is_same<std::decay_t<Arg>, RAII>::value>>
  RAII(Arg &&arg) : _p{AllocatorTraits<T>::allocate(std::forward<Arg>(arg))} {}
  template <typename... Args,
            typename = std::enable_if_t<(sizeof...(Args) != 1)>>
  RAII(Args &&... args)
      : _p{AllocatorTraits<T>::allocate(std::forward<Args>(args)...)} {}
  ~RAII() { AllocatorTraits<T>::free(_p); }
  operator T *() { return _p; }
  operator T const *() const { return _p; }

private:
  T *_p;
};

template <> struct AllocatorTraits<gsl_interp_accel> {
  static gsl_interp_accel *allocate() { return gsl_interp_accel_alloc(); }
  static void free(gsl_interp_accel *a) { gsl_interp_accel_free(a); }
};

template <> struct AllocatorTraits<gsl_spline2d> {
  static gsl_spline2d *allocate(gsl_interp2d_type const *T, size_t nx,
                                size_t ny) {
    return gsl_spline2d_alloc(T, nx, ny);
  }
  static void free(gsl_spline2d *s) { gsl_spline2d_free(s); }
};

template <typename T> struct Interp2DTypeTraits {};
struct Bilinear {};
struct Bicubic {};
// NOTE gsl_interp2d_bilinear and gsl_interp2d_bicubic are not usable in
// constant expression :(
template <> struct Interp2DTypeTraits<Bilinear> {
  static gsl_interp2d_type const *value() { return gsl_interp2d_bilinear; }
  static char constexpr name[] = "bilinear";
};
template <> struct Interp2DTypeTraits<Bicubic> {
  static gsl_interp2d_type const *value() { return gsl_interp2d_bicubic; }
  static char constexpr name[] = "bicubic";
};

template <typename T> class Spline2D {
public:
  Spline2D(std::vector<double> const &x, std::vector<double> const &y,
           std::vector<double> const &z)
      : _spline{[&x, &y]() {
          _check_sufficient_number_of_points(x, 'x');
          _check_sufficient_number_of_points(y, 'y');
          _check_points_are_sorted_and_unique(x, 'x');
          _check_points_are_sorted_and_unique(y, 'y');
          return RAII<gsl_spline2d>{Interp2DTypeTraits<T>::value(), x.size(),
                                    y.size()};
        }()} {
    // NOTE array sizes parameter validation below is important because this is
    // the only error that would not get caught by GSL (not necessarily
    // out-of-bound element accesses but always an error)
    if (z.size() != x.size() * y.size()) {
      std::stringstream ss;
      ss << "za the array of function values passed to the function that "
            "initializes the interpolation object has size "
         << z.size() << " != " << x.size() << " * " << y.size() << " where "
         << x.size() << " and " << y.size()
         << " are the size of the array xa and ya of the x and y grid points "
            "respectively";
      throw std::invalid_argument(ss.str());
    }
    gsl_spline2d_init(_spline, x.data(), y.data(), z.data(), x.size(),
                      y.size());
  }
  double eval(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_e, x, y);
  }
  double eval_deriv_x(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_deriv_x_e, x, y);
  }
  double eval_deriv_y(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_deriv_y_e, x, y);
  }
  double eval_deriv_xx(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_deriv_xx_e, x, y);
  }
  double eval_deriv_yy(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_deriv_yy_e, x, y);
  }
  double eval_deriv_xy(double x, double y) const {
    return _evaluate(&gsl_spline2d_eval_deriv_xy_e, x, y);
  }

private:
  static void _check_sufficient_number_of_points(std::vector<double> const &v,
                                                 char d) {
    auto const min_size =
        gsl_interp2d_type_min_size(Interp2DTypeTraits<T>::value());
    auto const name = Interp2DTypeTraits<T>::name;
    if (v.size() < min_size) {
      std::stringstream ss;
      ss << d << "a the array of the " << d << " grid points has size "
         << v.size()
         << " which is an insufficient number of points for "
            "interpolation type '"
         << name << "' that requires a minimum of " << min_size
         << " in either direction";
      throw std::invalid_argument(ss.str());
    }
  }
  static void _check_points_are_sorted_and_unique(std::vector<double> const &v,
                                                  char d) {
    if (std::adjacent_find(v.begin(), v.end(), [](double l, double r) {
          return l >= r;
        }) != v.end()) {
      std::stringstream ss;
      ss << d << "a the array of the " << d
         << " grid points must be in strictly ascending order";
      throw std::invalid_argument(ss.str());
    }
  }
  using EvalFunctionType = int (*)(gsl_spline2d const *spline, double x,
                                   double y, gsl_interp_accel *xacc,
                                   gsl_interp_accel *yacc, double *z);
  double _evaluate(EvalFunctionType _gsl_eval_e, double x, double y) const {
    double z;
    int const status = _gsl_eval_e(_spline, x, y, _xacc, _yacc, &z);
    if (status == GSL_EDOM) {
      std::stringstream ss;
      ss << x << " is outside the range of xa or " << y
         << " is outside the range ya";
      throw std::domain_error(ss.str());
    } else if (status != GSL_SUCCESS) {
      std::stringstream ss;
      ss << "unexpected non-zero error code " << status;
      throw std::runtime_error(ss.str());
    }
    return z;
  }
  RAII<gsl_spline2d> _spline;
  mutable RAII<gsl_interp_accel> _xacc;
  mutable RAII<gsl_interp_accel> _yacc;
};
} // namespace noname

#endif
