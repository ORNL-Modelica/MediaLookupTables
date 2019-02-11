within MediaLookupTables.Examples;
function noname_spline2d_eval

  input String tables
    "Path to lookup tables. Format = '|x.csv|y.csv|z.csv|";
  input Real x;
  input Real y;

  output Real z;

external"C" z = noname_spline2d_eval(tables,x,y) annotation (
    Include="#include \"noname.h\"",
    Library="noname",
    IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
    LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

end noname_spline2d_eval;
