within MediaLookupTables.Examples;
model LookupTables_2D_Test
  Interpolation_2D lookupTables_2D(redeclare function Method =
        interpolation.bilinear_eval)
    annotation (Placement(transformation(extent={{-10,-10},{10,10}})));
  Modelica.Blocks.Sources.Constant const(k=102341.14021054539)
    annotation (Placement(transformation(extent={{-60,10},{-40,30}})));
  Modelica.Blocks.Sources.Ramp ramp(
    height=5400,
    duration=1,
    offset=100)
    annotation (Placement(transformation(extent={{-60,-30},{-40,-10}})));
equation
  connect(const.y, lookupTables_2D.u1) annotation (Line(points={{-39,20},{-26,
          20},{-26,6},{-12,6}}, color={0,0,127}));
  connect(ramp.y, lookupTables_2D.u2) annotation (Line(points={{-39,-20},{-26,
          -20},{-26,-6},{-12,-6}}, color={0,0,127}));
  annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
        coordinateSystem(preserveAspectRatio=false)));
end LookupTables_2D_Test;
