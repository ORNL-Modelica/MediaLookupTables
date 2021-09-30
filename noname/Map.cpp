#include <Map.hpp>
#include <Parser.hpp>
#include <Spline2D.hpp>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

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
    if (s.empty()) {
      continue;
    }
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

void check_option_is_valid(std::string const &option, std::string const &name,
                           std::vector<std::string> const &valid_options) {
  if (std::find(valid_options.begin(), valid_options.end(), option) ==
      valid_options.end()) {
    std::stringstream ss;
    ss << name << " '" << option << "' not recognized";
    ss << "\n  chose from:";
    std::for_each(valid_options.begin(), valid_options.end(),
                  [&ss](std::string const &o) { ss << " '" << o << "'"; });
    throw std::runtime_error(ss.str());
  }
}

void check_valid_number_of_options(std::vector<std::string> const &options,
                                   size_t expected) {
  size_t const provided = options.size();
  if (provided != expected) {
    std::stringstream ss;
    ss << "invalid number of options provided\n";
    ss << "expected " << expected << " but got " << provided << ":";
    for (std::string const &o : options) {
      ss << " '" << o << "'";
    }
    throw std::runtime_error(ss.str());
  }
}

template <typename Spline>
Spline const &Map<Spline>::get_spline(std::string const &which) {
  // NOTE Decided to return a const reference (rather than a pointer to a
  // const-qualified Spline).  The static assertions below ensure that there
  // will be no accidental copies.
  static_assert(!std::is_copy_constructible<Spline>::value, "");
  static_assert(!std::is_copy_assignable<Spline>::value, "");
  auto it = _splines.find(which);
  if (it == _splines.end()) {
    auto const options = parse_options(which);
    check_valid_number_of_options(options, 3);

    std::vector<std::string> const filenames(options.begin(),
                                             options.begin() + 3);
    for (auto const &f : filenames) {
      check_file_exists(f);
    }
    auto extract_from = [](std::string const &filename,
                           std::vector<double> (*parse)(std::istream &)) {
      std::fstream fs(filename);
      return parse(fs);
    };
    auto x = extract_from(filenames[0], parse_x);
    auto y = extract_from(filenames[1], parse_y);
    auto z = extract_from(filenames[2], parse_z);
    std::tie(it, std::ignore) = _splines.emplace(
        which, Spline(std::move(x), std::move(y), std::move(z)));
  }
  return it->second;
}

template <typename Spline> std::map<std::string, Spline> Map<Spline>::_splines;

template class Map<Spline2D<Bicubic>>;
template class Map<Spline2D<Bilinear>>;

} // namespace noname
