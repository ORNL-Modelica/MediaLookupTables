within MediaLookupTables;
package Examples
  function setState_pT
      "Return thermodynamic state record from p and T"

      input Modelica.SIunits.AbsolutePressure p "pressure";
      input Modelica.SIunits.Temperature T "temperature";

      output Modelica.SIunits.SpecificEnthalpy state;

protected
    constant String file_p="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/p.csv";
    constant String file_T="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/T.csv";
    constant String file_val="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/h.csv";

    constant String files=TRANSFORM.Utilities.Strings.concatenate({file_p,file_T,file_val},"|",3);

    external"C" state = noname_spline2d_eval(
          files,
          p,
          T) annotation (
        Include="#include \"noname.h\"",
        Library="noname",
        IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
        LibraryDirectory="modelica://MediaLookupTables/Resources/Library");

  end setState_pT;

  model check_noname_spline2d_eval

    parameter String tables = Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/");
    parameter Real x = 1000;
    parameter Real y = 1e5;

    Real z = noname_spline2d_eval(combinedNames,x,y);

    String name = Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/");

    String names[3];

    String combinedNames = TRANSFORM.Utilities.Strings.concatenate(names, "|", 3);
  equation

    names[1] = name + "T.csv";
    names[2] = name + "p.csv";
    names[3] = name + "h.csv";

  //   Modelica.Utilities.Streams.print(combinedNames);

    annotation (experiment(__Dymola_NumberOfIntervals=1));
  end check_noname_spline2d_eval;
end Examples;
