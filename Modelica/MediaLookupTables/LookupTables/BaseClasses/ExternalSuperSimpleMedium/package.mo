within MediaLookupTables.LookupTables.BaseClasses;
package ExternalSuperSimpleMedium "Generic external medium package as a super simple example"
  import MediaLookupTables.LookupTables.BaseClasses.Common.InputChoice;

  extends MediaLookupTables.Interfaces.Fluids.PartialSinglePhaseMedium(
      singleState=false, fluidConstants={externalFluidConstants});
  // mediumName is declared here instead of in the extends clause
  // to break a circular dependency in redeclaration that OpenModelica
  // cannot yet handle
  constant String mediumName="unusablePartialMedium" "Name of the medium";
  constant String libraryName="UnusableExternalMedium"
    "Name of the external fluid property computation library";
  constant String substanceName=substanceNames[1]
    "Only one substance can be specified";
  constant FluidConstants externalFluidConstants=FluidConstants(
      iupacName="unknown",
      casRegistryNumber="unknown",
      chemicalFormula="unknown",
      structureFormula="unknown",
      molarMass=0.1);
  constant InputChoice inputChoice=InputChoice.pT
    "Default choice of input variables for property computations";

  redeclare replaceable record ThermodynamicState
    // Fields in ASCII lexicographical order to work in Dymola
    Temperature T "temperature";
    SpecificEnthalpy h "specific enthalpy";
    AbsolutePressure p "pressure";
  end ThermodynamicState;

  redeclare replaceable model extends BaseProperties(
    p(stateSelect=if preferredMediumStates and (basePropertiesInputChoice ==
          InputChoice.ph or basePropertiesInputChoice == InputChoice.pT or
          basePropertiesInputChoice == InputChoice.ps) then StateSelect.prefer
           else StateSelect.default),
    T(stateSelect=if preferredMediumStates and (basePropertiesInputChoice ==
          InputChoice.pT or basePropertiesInputChoice == InputChoice.dT) then
          StateSelect.prefer else StateSelect.default),
    h(stateSelect=if preferredMediumStates and (basePropertiesInputChoice ==
          InputChoice.hs or basePropertiesInputChoice == InputChoice.ph) then
          StateSelect.prefer else StateSelect.default),
    d(stateSelect=if preferredMediumStates and basePropertiesInputChoice ==
          InputChoice.dT then StateSelect.prefer else StateSelect.default))
    import MediaLookupTables.LookupTables.BaseClasses.Common.InputChoice;
    parameter InputChoice basePropertiesInputChoice=inputChoice
      "Choice of input variables for property computations";
  equation
    MM = externalFluidConstants.molarMass;
    R = Modelica.Constants.R/MM;
    if (basePropertiesInputChoice == InputChoice.pT) then
      state = setState_pT(p, T);
      d = 1.0;
      h = specificEnthalpy(state);
    end if;
    // Compute the internal energy
    u = h - p/d;
  end BaseProperties;

  redeclare replaceable function setState_pT
    "Return thermodynamic state record from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input Temperature T "temperature";
    output ThermodynamicState state;
  external"C" SinglePhaseMedium_setState_pT_C_impl(
        p,
        T,
        state,
        mediumName,
        libraryName,
        substanceName) annotation (
      Include="#include \"medialookuptableslib.h\"",
      Library="MediaLookupTables",
      IncludeDirectory="modelica://MediaLookupTables/Resources/Include",
      LibraryDirectory="modelica://MediaLookupTables/Resources/Library");
  end setState_pT;

  redeclare function extends setState_pTX
  algorithm
    // The composition is an empty vector
    state := setState_pT(p, T);
  end setState_pTX;

  redeclare replaceable function specificEnthalpy_pT
    "Return specific enthalpy from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy_pT_state(
        p=p,
        T=T,
        state=setState_pT(p=p, T=T));
    annotation (Inline=true);
  end specificEnthalpy_pT;

  redeclare replaceable function extends temperature
    "Return temperature from state"
    // Standard definition
  algorithm
    T := state.T;
    /*  // If special definition in "C"
  external "C" T=  SinglePhaseMedium_temperature_C_impl(state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"medialookuptableslib.h\"", Library="MediaLookupTables", IncludeDirectory="modelica://MediaLookupTables/Resources/Include", LibraryDirectory="modelica://MediaLookupTables/Resources/Library");
*/
    annotation (Inline=true);
  end temperature;

  redeclare replaceable function extends specificEnthalpy
    "Return specific enthalpy from state"
    // Standard definition
  algorithm
    h := state.h;
    /*  // If special definition in "C"
  external "C" h=  SinglePhaseMedium_specificEnthalpy_C_impl(state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"medialookuptableslib.h\"", Library="MediaLookupTables", IncludeDirectory="modelica://MediaLookupTables/Resources/Include", LibraryDirectory="modelica://MediaLookupTables/Resources/Library");
*/
    annotation (Inline=true);
  end specificEnthalpy;

  redeclare replaceable function extends pressure
    "Return pressure from state"
    // Standard definition
  algorithm
    p := state.p;
    /*  // If special definition in "C"
  external "C" p=  SinglePhaseMedium_pressure_C_impl(state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"medialookuptableslib.h\"", Library="MediaLookupTables", IncludeDirectory="modelica://MediaLookupTables/Resources/Include", LibraryDirectory="modelica://MediaLookupTables/Resources/Library");
*/
    annotation (Inline=true);
  end pressure;
end ExternalSuperSimpleMedium;
