within MediaLookupTables.Examples;
block Interpolation_2D
  extends Modelica.Blocks.Interfaces.SI2SO;

  parameter String tablePath_u1=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/p.csv");
  parameter String tablePath_u2=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/T.csv");
  parameter String tablePath_y=Modelica.Utilities.Files.loadResource("modelica://MediaLookupTables/Resources/data/lookupTables/ParaHydrogen/pT/h.csv");

  final parameter String tablesPath=TRANSFORM.Utilities.Strings.concatenate(
      {tablePath_u1,tablePath_u2,tablePath_y},
      "|",
      3);

  replaceable function Method =
      MediaLookupTables.Examples.interpolation.bicubic_eval constrainedby
    MediaLookupTables.Examples.interpolation.PartialInterpolation annotation (
      choicesAllMatching=true);

equation

  y = Method(
    tablesPath,
    u1,
    u2);

  annotation (defaultcomponentName="lookupTable");
end Interpolation_2D;
