within MediaLookupTables.LookupTables.BaseClasses.ExternalSuperSimpleMedium;
function specificEnthalpy_pT_state
  "returns specific enthalpy for given p and T"
  extends Modelica.Icons.Function;
  input AbsolutePressure p "Pressure";
  input Temperature T "Temperature";
  input ThermodynamicState state;
  output SpecificEnthalpy h "specific enthalpy";
algorithm
  h := specificEnthalpy(state);
  annotation (
    Inline=false,
    LateInline=true);
end specificEnthalpy_pT_state;
