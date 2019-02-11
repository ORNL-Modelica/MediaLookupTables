#ifndef NONAME_SPLINEMAP
#define NOMAME_SPLINEMAP

#include <noname_Spline2D.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace noname {

class SplineMap {
public:
  static Spline2D<Bicubic> *get_spline(std::string const &which);
  static int size() { return _splines.size(); }
  static void clear() { _splines.clear(); }

private:
  static std::map<std::string, std::unique_ptr<Spline2D<Bicubic>>> _splines;
};

} // namespace noname

#endif
