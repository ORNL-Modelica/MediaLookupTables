#include <noname_Parser.hpp>
#include <noname_SplineMap.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

namespace noname {

std::vector<std::string> parse_options(std::string const &o) {
  std::vector<std::string> vs;
  std::stringstream ss(o);
  char const separator = [&ss]() {
    std::string const valid_separators = R"(<>:"|?*)";
    char c;
    ss >> std::ws;
    ss.get(c);
    if (valid_separators.find(c) == std::string::npos) {
      ss.unget();
      throw std::runtime_error("must start with separator\nchose from " +
                               valid_separators);
    }
    return c;
  }();
  for (std::string s; std::getline(ss >> std::ws, s, separator);) {
    // allow repeating separator
    if (s == "")
      continue;
    // trim trailing whitespaces
    char const discard_any_of_these[] = " \t\n";
    auto last = s.find_last_not_of(discard_any_of_these) + 1;
    vs.push_back(s.substr(0, last));
  }
  return vs;
}

void check_file_exists(std::string const &filename) {
  std::ifstream fs(filename);
  if (!fs.is_open()) {
    std::stringstream ss;
    ss << "file '" << filename << "' not found";
    throw std::runtime_error(ss.str());
  }
}

Spline2D<Bicubic> *SplineMap::get_spline(std::string const &which) {
  auto const filenames = parse_options(which);
  if (filenames.size() != 3) {
    throw std::runtime_error("not very helpful error message");
  }
  std::string key;
  for (auto const &f : filenames) {
    check_file_exists(f);
    key += f;
  }
  // Check if the spline object already exists. If it doesn't, create the
  // object.
  if (_splines.count(key) == 0) {
    auto const x = [&filenames]() {
      std::ifstream fs(filenames[0]);
      return parse_x(fs);
    }();
    auto const y = [&filenames]() {
      std::ifstream fs(filenames[1]);
      return parse_y(fs);
    }();
    auto const z = [&filenames]() {
      std::ifstream fs(filenames[2]);
      return parse_z(fs);
    }();
    _splines[key] = std::make_unique<Spline2D<Bicubic>>(x, y, z);
  }

  return _splines[key].get();
}

std::map<std::string, std::unique_ptr<Spline2D<Bicubic>>> SplineMap::_splines;
} // namespace noname
