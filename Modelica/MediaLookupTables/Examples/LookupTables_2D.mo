within MediaLookupTables.Examples;
block LookupTables_2D
  extends Modelica.Blocks.Interfaces.SI2SO;

  parameter String tablePath_u1=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/T.csv");
  parameter String tablePath_u2=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/p.csv");
  parameter String tablePath_y=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/h.csv");

  parameter String interpolationMethod = "bicubic";
  parameter String edgeMethod = "error";

  final parameter String tables=TRANSFORM.Utilities.Strings.concatenate(
      {tablePath_u1,tablePath_u2,tablePath_y,interpolationMethod,edgeMethod},
      "|",
      3);

equation
  y = noname_spline2d_eval(
    tables,
    u1,
    u2);

  annotation (defaultcomponentName="lookupTable");
end LookupTables_2D;
