cmake_minimum_required(VERSION 3.12)

project(noname LANGUAGES CXX)

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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(noname STATIC noname.cpp noname_SplineMap.cpp)
target_include_directories(noname PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(noname PUBLIC ${install_dir}/include)
target_link_libraries(noname PRIVATE gsl)
set_target_properties(noname PROPERTIES PUBLIC_HEADER "noname.h")
install (TARGETS noname
  PUBLIC_HEADER DESTINATION include
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
 )

add_executable(interp2d noname_interpolation.cpp)
target_link_libraries(interp2d noname)

add_executable(Spline2D noname_TestSpline2D.cpp)
target_link_libraries(Spline2D noname)
enable_testing()
add_test(NAME testSpline2D COMMAND Spline2D)
# FIXME
file(COPY ${CMAKE_SOURCE_DIR}/dummy.csv DESTINATION ${CMAKE_BINARY_DIR})

add_executable(SplineMap noname_TestSplineMap.cpp)
target_link_libraries(SplineMap noname)
add_test(NAME testSplineMap COMMAND SplineMap)
