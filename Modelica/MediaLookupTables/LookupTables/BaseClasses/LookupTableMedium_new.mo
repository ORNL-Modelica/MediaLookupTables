within MediaLookupTables.LookupTables.BaseClasses;
package LookupTableMedium_new
  "Medium package accessing the lookup table solver"
  extends MediaLookupTables.LookupTables.BaseClasses.ExternalSinglePhaseMedium_new;

  redeclare replaceable function extends isentropicEnthalpy
  protected
    SpecificEntropy s_ideal;
    ThermodynamicState state_ideal;
  algorithm
    s_ideal := specificEntropy(refState);
    state_ideal := setState_psX(p_downstream, s_ideal);
    h_is := specificEnthalpy(state_ideal);
  end isentropicEnthalpy;
end LookupTableMedium_new;
