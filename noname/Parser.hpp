#ifndef NONAME_PARSER
#define NONAME_PARSER

#include <istream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace noname {

static inline std::vector<double> parse_y(std::istream &is) {
  std::vector<double> y;
  for (std::string entry; std::getline(is, entry, ',');
       is.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
    y.push_back(std::stod(entry));
  }
  return y;
}

static inline std::vector<double> parse_x(std::istream &is) {
  std::vector<double> x;
  std::string line;
  std::getline(is, line);
  std::stringstream ss(line);
  for (std::string entrx; std::getline(ss, entrx, ',');) {
    x.push_back(std::stod(entrx));
  }
  return x;
}

static inline std::vector<double> parse_z(std::istream &is) {
  std::vector<double> z;
  for (std::string line; std::getline(is, line);) {
    std::stringstream ss(line);
    for (std::string entry; std::getline(ss, entry, ',');) {
      z.push_back(std::stod(entry));
    }
  }
  return z;
}

static inline std::vector<double> transpose_z(int nx, int ny,
                                              std::vector<double> const &z) {
  std::vector<double> tz(z.size());
  for (int i = 0; i < nx; ++i) {
    for (int j = 0; j < ny; ++j) {
      tz[j * nx + i] = z[i * ny + j];
    }
  }
  return tz;
}

} // namespace noname

#endif
