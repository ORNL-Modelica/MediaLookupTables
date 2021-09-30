#ifndef NONAME_MAP
#define NONAME_MAP

#include <Spline2D.hpp>

#include <map>
#include <string>

namespace noname {

template <typename Spline> class Map {
public:
  static Spline const &get_spline(std::string const &which);
  static int size() { return _splines.size(); }
  static void clear() { _splines.clear(); }

private:
  static std::map<std::string, Spline> _splines;
};

} // namespace noname

#endif
