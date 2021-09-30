#ifndef NONAME_SPLINE_2D
#define NONAME_SPLINE_2D

#include <gsl/gsl_errno.h>
#include <gsl/gsl_interp2d.h>

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
  ~RAII() {
    // GSL returns early if _p is null so don't bother checking
    AllocatorTraits<T>::free(_p);
    _p = nullptr;
  }
  // NOTE for some reason defining the default constructor degraded performance
  // of eval by 15%
  static_assert(!std::is_default_constructible<RAII>::value, "");
  // NOTE GSL doesn't provide any way of making a copy of the interpolation
  // object nor the evaluation acceleration
  RAII(RAII const &) = delete;
  RAII &operator=(RAII &) = delete;
  RAII(RAII &&o) {
    this->~RAII();
    _p = o._p;
    o._p = nullptr;
  }
  RAII &operator=(RAII &&o) {
    this->~RAII();
    _p = o._p;
    o._p = nullptr;
    return *this;
  }
  operator T *() { return _p; }
  operator T const *() const { return _p; }

private:
  T *_p = nullptr;
};

template <> struct AllocatorTraits<gsl_interp_accel> {
  static gsl_interp_accel *allocate() { return gsl_interp_accel_alloc(); }
  static void free(gsl_interp_accel *a) { gsl_interp_accel_free(a); }
};

template <> struct AllocatorTraits<gsl_interp2d> {
  static gsl_interp2d *allocate(gsl_interp2d_type const *T, size_t nx,
                                size_t ny) {
    return gsl_interp2d_alloc(T, nx, ny);
  }
  static void free(gsl_interp2d *s) { gsl_interp2d_free(s); }
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
  Spline2D(std::vector<double> xa, std::vector<double> ya,
           std::vector<double> za)
      : _xa{std::move(xa)}, _ya{std::move(ya)}, _za{std::move(za)},
        _spline{_check_grid_points_and_build_interpolation_object(_xa, _ya)} {
    _check_arrays_properly_sized(_xa, _ya, _za);
    gsl_interp2d_init(_spline, _xa.data(), _ya.data(), _za.data(), _xa.size(),
                      _ya.size());
  }
  double eval(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_e, x, y);
  }
  double eval_extrap(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_e_extrap, x, y);
  }
  double eval_deriv_x(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_deriv_x_e, x, y);
  }
  double eval_deriv_y(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_deriv_y_e, x, y);
  }
  double eval_deriv_xx(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_deriv_xx_e, x, y);
  }
  double eval_deriv_yy(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_deriv_yy_e, x, y);
  }
  double eval_deriv_xy(double x, double y) const {
    return _evaluate(&gsl_interp2d_eval_deriv_xy_e, x, y);
  }

private:
  static RAII<gsl_interp2d> _check_grid_points_and_build_interpolation_object(
      std::vector<double> const &xa, std::vector<double> const &ya) {
    _check_sufficient_number_of_points(xa, 'x');
    _check_sufficient_number_of_points(ya, 'y');
    _check_points_are_sorted_and_unique(xa, 'x');
    _check_points_are_sorted_and_unique(ya, 'y');
    return RAII<gsl_interp2d>{Interp2DTypeTraits<T>::value(), xa.size(),
                              ya.size()};
  }
  static void _check_arrays_properly_sized(std::vector<double> const &xa,
                                           std::vector<double> const &ya,
                                           std::vector<double> const &za) {
    // NOTE array sizes parameter validation is most important because this is
    // the only error that would not get caught by GSL.  It does not
    // necessarily out-of-bound element accesses but is always an error.
    if (za.size() != xa.size() * ya.size()) {
      std::stringstream ss;
      ss << "za the array of function values passed to the function that "
            "initializes the interpolation object has size "
         << za.size() << " != " << xa.size() << " * " << ya.size() << " where "
         << xa.size() << " and " << ya.size()
         << " are the size of the array xa and ya of the x and y grid points "
            "respectively";
      throw std::invalid_argument(ss.str());
    }
  }
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
  using EvalFunctionType = int (*)(gsl_interp2d const *spline,
                                   double const xa[], double const ya[],
                                   double const za[], double x, double y,
                                   gsl_interp_accel *xacc,
                                   gsl_interp_accel *yacc, double *z);
  double _evaluate(EvalFunctionType _gsl_eval_e, double x, double y) const {
    double z;
    int const status = _gsl_eval_e(_spline, _xa.data(), _ya.data(), _za.data(),
                                   x, y, _xacc, _yacc, &z);
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
  std::vector<double> _xa;
  std::vector<double> _ya;
  std::vector<double> _za;
  RAII<gsl_interp2d> _spline;
  mutable RAII<gsl_interp_accel> _xacc;
  mutable RAII<gsl_interp_accel> _yacc;
};
} // namespace noname

#endif
