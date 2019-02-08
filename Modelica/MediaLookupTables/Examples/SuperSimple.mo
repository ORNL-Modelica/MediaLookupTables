within MediaLookupTables.Examples;
model SuperSimple

  extends TRANSFORM.Icons.Example;

  replaceable package Medium =
      MediaLookupTables.LookupTables.SuperSimple;

  Medium.BaseProperties medium(T(start=T.offset));

  Modelica.Blocks.Sources.Constant p(k=1e5)
    annotation (Placement(transformation(extent={{-60,20},{-40,40}})));
  Modelica.Blocks.Sources.Ramp T(
    height=5000,
    duration=1,
    offset=50)
    annotation (Placement(transformation(extent={{-60,-20},{-40,0}})));
equation
  medium.p = p.y;
  medium.h = T.y;

  annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
        coordinateSystem(preserveAspectRatio=false)));
end SuperSimple;
