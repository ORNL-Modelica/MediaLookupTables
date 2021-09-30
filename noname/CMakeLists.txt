cmake_minimum_required(VERSION 3.10)

project(noname LANGUAGES CXX)

option(GSL_EXTERNAL_PROJECT "Whether to download and build the GNU Scientific Library (ON) or to search for it (OFF)" ON)

if(GSL_EXTERNAL_PROJECT)

include(ExternalProject)

ExternalProject_Add(project_gsl
  URL http://mirror.keystealth.org/gnu/gsl/gsl-2.5.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gsl
  PATCH_COMMAND patch -p1 --verbose --directory ${CMAKE_CURRENT_BINARY_DIR}/gsl/src/project_gsl <  ${CMAKE_SOURCE_DIR}/wrong_return_gsl_interp2d_eval_e.patch 
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gsl/src/project_gsl/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/gsl
  BUILD_COMMAND make
  INSTALL_COMMAND make install
)

ExternalProject_Get_Property(project_gsl install_dir)

add_library(gsl STATIC IMPORTED)

set_property(TARGET gsl PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libgsl.a)

add_dependencies(gsl project_gsl)

else()

# NOTE FindGSL "official" CMake module explicitly select shared libraries so this might need adjustment
find_package(GSL 2.5 REQUIRED)

endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(noname STATIC noname.cpp Map.cpp)
target_include_directories(noname PUBLIC ${CMAKE_SOURCE_DIR})
if(GSL_EXTERNAL_PROJECT)
target_include_directories(noname PUBLIC ${install_dir}/include)
target_link_libraries(noname PRIVATE gsl)
else()
# NOTE did not seem to work with PRIVATE (would not find GSL headers)
target_link_libraries(noname PUBLIC GSL::gsl)
endif()
set_target_properties(noname PROPERTIES PUBLIC_HEADER "noname.h")
install (TARGETS noname
  PUBLIC_HEADER DESTINATION include
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
 )

add_executable(Spline2D TestSpline2D.cpp)
target_link_libraries(Spline2D noname)
enable_testing()
add_test(NAME TestSpline2D COMMAND Spline2D)
# FIXME
file(COPY ${CMAKE_SOURCE_DIR}/dummy.csv DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Map TestMap.cpp)
target_link_libraries(Map noname)
add_test(NAME TestMap COMMAND Map)

add_executable(Example Example.cpp)
target_link_libraries(Example noname)
add_test(NAME Example COMMAND example)

find_package(benchmark)
if(benchmark_FOUND)
  add_executable(Benchmark Benchmark.cpp)
  target_link_libraries(Benchmark noname benchmark::benchmark)
  add_test(NAME Benchmark COMMAND benchmark)
endif()
