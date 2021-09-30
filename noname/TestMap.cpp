#include <Map.hpp>
#include <Spline2D.hpp>
#include <TestAssertionMacros.hpp>

#include <stdexcept>
#include <string>

using namespace noname;

using SplineMap = Map<Spline2D<Bicubic>>;

std::string make_option(std::string const &x, std::string const &y,
                        std::string const &z) {
  char const separator = '<';
  std::string option = separator + x + separator + y + separator + z;
  return option;
};

template <typename Spline> void file_not_found() {
  std::string const valid = "dummy.csv";
  std::string const invalid = "does_not_exist.csv";
  ASSERT_NO_THROW(Map<Spline>::get_spline(make_option(valid, valid, valid)));
  ASSERT_THROW(Map<Spline>::get_spline(make_option(invalid, valid, valid)),
               std::runtime_error);
  ASSERT_THROW(Map<Spline>::get_spline(make_option(valid, invalid, valid)),
               std::runtime_error);
  ASSERT_THROW(Map<Spline>::get_spline(make_option(valid, valid, invalid)),
               std::runtime_error);
}

template <typename Spline> void invalid_number_of_options() {
  std::string const valid = "dummy.csv";
  char const separator = '<';
  ASSERT_NO_THROW(Map<Spline>::get_spline(separator + valid + separator +
                                          valid + separator + valid));
  ASSERT_THROW(Map<Spline>::get_spline(separator + valid + separator + valid +
                                       separator),
               std::runtime_error);
  ASSERT_THROW(Map<Spline>::get_spline(separator + valid + separator + valid +
                                       separator + valid + separator +
                                       "whatever"),
               std::runtime_error);
}

template <typename Spline> void control_map_growth() {
  Map<Spline>::clear();
  ASSERT(Map<Spline>::size() == 0);

  Map<Spline>::get_spline(":dummy.csv: :dummy.csv: :dummy.csv:");
  Map<Spline>::get_spline(":dummy.csv: :dummy.csv: :dummy.csv:");
  ASSERT(Map<Spline>::size() == 1);
  Map<Spline>::get_spline("|dummy.csv |dummy.csv |dummy.csv");
  Map<Spline>::get_spline("? dummy.csv? ? ? ?dummy.csv???dummy.csv");
  ASSERT(Map<Spline>::size() == 3);
  Map<Spline>::get_spline(":dummy.csv: :dummy.csv: :dummy.csv:");
  ASSERT(Map<Spline>::size() == 3);
}

int main() {

  std::cout << "file not found\n";
  file_not_found<Spline2D<Bilinear>>();
  file_not_found<Spline2D<Bicubic>>();

  std::cout << "invalid number of options\n";
  invalid_number_of_options<Spline2D<Bilinear>>();
  invalid_number_of_options<Spline2D<Bicubic>>();

  std::cout << "map size\n";
  control_map_growth<Spline2D<Bilinear>>();
  control_map_growth<Spline2D<Bicubic>>();

  std::cout << "done!\n";
}
