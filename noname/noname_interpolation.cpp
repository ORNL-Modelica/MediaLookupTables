#include <noname_Parser.hpp>
#include <noname_Spline2D.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

double foo(double x, double y) {
  // NOTE Rosenbrock function
  double const a = 1.;
  double const b = 100.;
  return (a - x) * (a - x) + b * (y - x * x) * (y - x * x);
}

int main() {
  using noname::Bicubic;
  using noname::Bilinear;
  using noname::Spline2D;

  std::vector<double> const xa = {0.0, 1.0, 2.0, 3.0};
  std::vector<double> const ya = {0.0, 1.0, 2.0, 3.0};
  size_t const nx = xa.size(); /* x grid points */
  size_t const ny = ya.size(); /* y grid points */
  std::vector<double> za(nx * ny);
  for (size_t i = 0; i < nx; ++i) {
    for (size_t j = 0; j < ny; ++j) {
      za[j + nx * i] = foo(xa[i], ya[j]);
    }
  }

  Spline2D<Bicubic> spline(xa, ya, za);

  size_t const N = 4; /* number of points to interpolate */

  /* interpolate N values in x and y and print out grid for plotting */
  for (size_t i = 0; i < N; ++i) {
    double const xi = i / (N - 1.0);

    for (size_t j = 0; j < N; ++j) {
      double const yj = j / (N - 1.0);
      double const zij = spline.eval(xi, yj);
      double const ref = foo(xi, yj);

      std::cout << xi << " " << yj << " " << zij << " " << ref << "\n";
    }
    std::cout << "\n";
  }

  std::string const s = R"(0.,1.,2. ,
3.e0,4.000  ,05.
6. ,7., 8.)";

  {
    std::stringstream ss(s);
    for (auto x : noname::parse_z(ss))
      std::cout << x << " ";
    std::cout << "\n";
  }

  {
    std::stringstream ss(s);
    for (auto x : noname::parse_y(ss))
      std::cout << x << " ";
    std::cout << "\n";
  }

  {
    std::stringstream ss(s);
    for (auto x : noname::parse_x(ss))
      std::cout << x << " ";
    std::cout << "\n";
  }

  return 0;
}
