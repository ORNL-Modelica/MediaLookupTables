#ifndef NONAME_PARSER
#define NONAME_PARSER

#include <istream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace noname {

static inline std::vector<double> parse_x(std::istream &is) {
  std::vector<double> x;
  for (std::string entry; std::getline(is, entry, ',');
       is.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
    x.push_back(std::stod(entry));
  }
  return x;
}

static inline std::vector<double> parse_y(std::istream &is) {
  std::vector<double> y;
  std::string line;
  std::getline(is, line);
  std::stringstream ss(line);
  for (std::string entry; std::getline(ss, entry, ',');) {
    y.push_back(std::stod(entry));
  }
  return y;
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

} // namespace noname

#endif
