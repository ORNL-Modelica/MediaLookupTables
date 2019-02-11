#include <noname_SplineMap.hpp>
#include <noname_TestAssertionMacros.hpp>

#include <cassert>

using namespace noname;

void file_not_found() {
  std::string const valid = "dummy.csv";
  std::string const invalid = "does_not_exist.csv";
  char const separator = '<';

  ASSERT_NO_THROW(SplineMap::get_spline(separator + valid + separator + valid +
                                        separator + valid));
  ASSERT_THROW(SplineMap::get_spline(separator + invalid + separator + valid +
                                     separator + valid),
               std::runtime_error);
  ASSERT_THROW(SplineMap::get_spline(separator + valid + separator + invalid +
                                     separator + valid),
               std::runtime_error);
  ASSERT_THROW(SplineMap::get_spline(separator + valid + separator + valid +
                                     separator + invalid),
               std::runtime_error);
}

void invalid_number_of_options() {
  std::string const valid = "dummy.csv";
  char const separator = '<';
  ASSERT_NO_THROW(SplineMap::get_spline(separator + valid + separator + valid +
                                        separator + valid));
  ASSERT_THROW(SplineMap::get_spline(separator + valid + separator + valid),
               std::runtime_error);
  ASSERT_THROW(SplineMap::get_spline(separator + valid + separator + valid +
                                     separator + valid + separator + valid),
               std::runtime_error);
  ASSERT_THROW(SplineMap::get_spline(""), std::runtime_error);
  ASSERT_THROW(SplineMap::get_spline("?"), std::runtime_error);
}

void control_map_growth() {
  SplineMap::clear();
  assert(SplineMap::size() == 0);

  SplineMap::get_spline(":dummy.csv: :dummy.csv: :dummy.csv:");
  SplineMap::get_spline("|dummy.csv |dummy.csv |dummy.csv");
  SplineMap::get_spline("? dummy.csv? ? ? ?dummy.csv???dummy.csv  ");
  SplineMap::get_spline(" <dummy.csv <dummy.csv< dummy.csv <");
  assert(SplineMap::size() == 1);

  SplineMap::get_spline("<./dummy.csv <dummy.csv< dummy.csv <");
  assert(SplineMap::size() == 2); // FIXME
}

int main() {

  file_not_found();

  invalid_number_of_options();

  control_map_growth();
}
