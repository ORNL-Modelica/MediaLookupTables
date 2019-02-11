within MediaLookupTables.Examples;
model test

    parameter Modelica.SIunits.AbsolutePressure p = 1e5 "pressure";
    parameter Modelica.SIunits.Temperature T = 1000 "temperature";

    Modelica.SIunits.SpecificEnthalpy state;

  constant String file_p="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/p.csv";
  constant String file_T="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/T.csv";
  constant String file_val="modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/h.csv";

  constant String files=TRANSFORM.Utilities.Strings.concatenate({file_p,file_T,file_val},"|",3);

equation

   state = setState_pT(p, T);
//   Modelica.Utilities.Streams.print(files);

  annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
        coordinateSystem(preserveAspectRatio=false)),
    experiment(__Dymola_NumberOfIntervals=1));
end test;
