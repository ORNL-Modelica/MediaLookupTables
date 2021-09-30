`Spline2D.hpp` is a thin wrapper around the interpolation objects from the GNU
Scientific Library (GSL).  It hooks directly into GSL's low-level interface
(`gsl_interp2d`) and provides the functionality of the higher-level interface
(`gsl_spline2d`) that is maintaining a copy of the data and passing it to the
evaluation functions.  `Spline2D<T>` goes one step further by also managing the
interpolation object lifetime (allocate and deallocate memory) and initializing
it for the data provided at construction.  It is templated on the interpolation
type which can be either `Bilinear` or `Bicubic`.

`Parser.hpp` provides functions to parse input from CSV files and extract `xa`
and `ya` arrays of the x and y grid points of size nx and ny as well as `za`
array of function values of size nx*ny.

`Map.hpp` declares static associative containers to store `Spline2D` objets
(with internal linkage).  Interpolation objects are mapped to a unique key that
is composed of the names of the files with the x, y, and z arrays.  The key
correspond to the character array argument of the interpolation functions in
`noname.h`.  When evaluating the interpolated value of z at point (x, y), the
interpolation object is accessed by searching the `Map` and it is inserted if
the key doesn't already exist.

# NOTE
* For examples of input files download p.csv, T.csv, and hmolar.csv from `data/ParaHydrogen/pT`
