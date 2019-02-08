within MediaLookupTables.LookupTables.BaseClasses;
package SuperSimpleMedium "Medium package accessing the SuperSimple lookup table solver"

  extends MediaLookupTables.LookupTables.BaseClasses.ExternalSuperSimpleMedium(
      final libraryName="LookupTables", final substanceName=
        MediaLookupTables.LookupTables.BaseClasses.Common.CheckLookupTableOptions(
         substanceNames[1], debug=false));

  redeclare replaceable function extends isentropicEnthalpy
  algorithm
    h_is := 1.0;
  end isentropicEnthalpy;
end SuperSimpleMedium;
