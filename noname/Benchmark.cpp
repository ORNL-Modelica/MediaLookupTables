#include <Map.hpp>
#include <Parser.hpp>
#include <Spline2D.hpp>

#include <gsl/gsl_interp2d.h>

#include <benchmark/benchmark.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace noname;

std::string const prefix = "/scratch/"; // edit this line
std::string const fx = prefix + "p.csv";
std::string const fy = prefix + "T.csv";
std::string const fz = prefix + "h.csv";
std::vector<double> extract_from(std::string const &filename,
                                 std::vector<double> (*parse)(std::istream &)) {
  std::fstream fs(filename);
  return parse(fs);
};
char const sep = '?';
std::string const k = sep + fx + sep + fy + sep + fz + sep;
auto const _x = extract_from(fx, parse_x);
auto const _y = extract_from(fy, parse_y);
auto const _z = extract_from(fz, parse_z);

static void BM_EvalSpline2D(benchmark::State &state) {
  Spline2D<Bicubic> spline(_x, _y, _z);
  // std::random_device gen(0);
  // std::uniform_real_distribution<double> dist_x(_x.front(), _x.back());
  // std::uniform_real_distribution<double> dist_y(_y.front(), _y.back());
  for (auto _ : state) {
    // spline.eval(dist_x(gen), dist_y(gen));
    spline.eval(_x.back(), _y.front());
  }
}
BENCHMARK(BM_EvalSpline2D);

static void BM_EvalRawGSL(benchmark::State &state) {
  auto *spline = gsl_interp2d_alloc(gsl_interp2d_bicubic, _x.size(), _y.size());
  gsl_interp2d_init(spline, _x.data(), _y.data(), _z.data(), _x.size(),
                    _y.size());
  auto *xacc = gsl_interp_accel_alloc();
  auto *yacc = gsl_interp_accel_alloc();
  for (auto _ : state) {
    gsl_interp2d_eval(spline, _x.data(), _y.data(), _z.data(), _x.back(),
                      _y.front(), xacc, yacc);
  }
  gsl_interp_accel_free(yacc);
  gsl_interp_accel_free(xacc);
  gsl_interp2d_free(spline);
}
BENCHMARK(BM_EvalRawGSL);

static void BM_EvalMap(benchmark::State &state) {
  Map<Spline2D<Bicubic>>::get_spline(k);
  for (auto _ : state) {
    Map<Spline2D<Bicubic>>::get_spline(k).eval(_x.back(), _y.front());
  }
}
BENCHMARK(BM_EvalMap);

static void BM_EvalMapGetSplineOnly(benchmark::State &state) {
  Map<Spline2D<Bicubic>>::get_spline(k);
  for (auto _ : state) {
    Map<Spline2D<Bicubic>>::get_spline(k);
  }
}
BENCHMARK(BM_EvalMapGetSplineOnly);

static void BM_ConstructSpline2D(benchmark::State &state) {
  for (auto _ : state) {
    auto const x = _x;
    auto const y = _y;
    auto const z = _z;
    Spline2D<Bicubic> spline(x, y, z);
  }
}
BENCHMARK(BM_ConstructSpline2D);

static void BM_ConstructSpline2DMoveArrays(benchmark::State &state) {
  for (auto _ : state) {
    auto x = _x;
    auto y = _y;
    auto z = _z;
    Spline2D<Bicubic> spline(std::move(x), std::move(y), std::move(z));
  }
}
BENCHMARK(BM_ConstructSpline2DMoveArrays);

static void BM_ConstructRawGSL(benchmark::State &state) {
  for (auto _ : state) {
    auto const x = _x;
    auto const y = _y;
    auto const z = _z;
    auto *spline = gsl_interp2d_alloc(gsl_interp2d_bicubic, x.size(), y.size());
    gsl_interp2d_init(spline, x.data(), y.data(), z.data(), x.size(), y.size());
    gsl_interp2d_free(spline);
  }
}
BENCHMARK(BM_ConstructRawGSL);

static void BM_ConstructCopyArraysOnly(benchmark::State &state) {
  for (auto _ : state) {
    auto const x = _x;
    auto const y = _y;
    auto const z = _z;
  }
}
BENCHMARK(BM_ConstructCopyArraysOnly);

BENCHMARK_MAIN();
