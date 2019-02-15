within MediaLookupTables.LookupTables.BaseClasses;
package SuperSimpleMedium_new
  "Medium package accessing the SuperSimple lookup table solver"
  extends MediaLookupTables.LookupTables.BaseClasses.ExternalSuperSimpleMedium_new;

  redeclare replaceable function extends isentropicEnthalpy
  algorithm
    h_is := 1.0;
  end isentropicEnthalpy;
end SuperSimpleMedium_new;
