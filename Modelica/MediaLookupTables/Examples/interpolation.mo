within MediaLookupTables.Examples;
package interpolation
  partial function PartialInterpolation

    extends TRANSFORM.Icons.Function;

    input String tablesPath
      "Path to interpolation tables. Format = '|x.csv|y.csv|z.csv|";
    input Real x;
    input Real y;

    output Real z;

  end PartialInterpolation;

  function bilinear_eval "Bilinear (2D) interpolation. Throws error outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bilinear_eval(
        tablesPath,
        x,
        y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bilinear_eval;

  function bicubic_eval "Bicubic (2D) interpolation. Throws error outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bicubic_eval(
        tablesPath,
        x,
        y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bicubic_eval;

  function bilinear_eval_extrap "Bilinear (2D) interpolation. Extrapolates outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bilinear_eval_extrap(
        tablesPath,
        x,
        y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bilinear_eval_extrap;

  function bicubic_eval_extrap "Bicubic (2D) interpolation. Extrapolates outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bicubic_eval_extrap(
        tablesPath,
        x,
        y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bicubic_eval_extrap;

  function bicubic_eval_deriv_x
    "Bicubic (2D) interpolation for table derivative wrt x. Throws error outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bicubic_eval_deriv_x(
      tablesPath,
      x,
      y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bicubic_eval_deriv_x;

  function bicubic_eval_deriv_y
    "Bicubic (2D) interpolation for table derivative wrt y. Throws error outside of table range."

    extends interpolation.PartialInterpolation;

  external"C" z = bicubic_eval_deriv_y(
      tablesPath,
      x,
      y) annotation (
      Include="#include \"noname.h\"",
      Library="noname",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end bicubic_eval_deriv_y;
end interpolation;
