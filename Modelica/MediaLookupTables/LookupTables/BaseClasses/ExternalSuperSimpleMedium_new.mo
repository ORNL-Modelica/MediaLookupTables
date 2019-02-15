within MediaLookupTables.LookupTables.BaseClasses;
package ExternalSuperSimpleMedium_new
  "Generic external medium package as a super simple example"
  import MediaLookupTables.LookupTables.BaseClasses.Common.InputChoice;

  extends MediaLookupTables.Interfaces.Fluids.PartialSinglePhaseMedium(
      singleState=false, fluidConstants={externalFluidConstants});
  // mediumName is declared here instead of in the extends clause
  // to break a circular dependency in redeclaration that OpenModelica
  // cannot yet handle
  //   constant String mediumName="unusablePartialMedium" "Name of the medium";

  constant FluidConstants externalFluidConstants=FluidConstants(
      iupacName="unknown",
      casRegistryNumber="unknown",
      chemicalFormula="unknown",
      structureFormula="unknown",
      molarMass=0.1);
  constant InputChoice inputChoice=InputChoice.pT
    "Default choice of input variables for property computations";


  replaceable function Method =
      MediaLookupTables.Examples.interpolation.bicubic_eval constrainedby
    MediaLookupTables.Examples.interpolation.PartialInterpolation "Interpolation method selection";
  constant String tablePath="modelica://MediaLookupTables/Resources/data/lookupTables/"
       + mediumName + (if inputChoice == InputChoice.pT then "/pT/" else "/error/");
  constant String tablePath_p=Modelica.Utilities.Files.loadResource(tablePath + "p.csv");
  constant String tablePath_T=Modelica.Utilities.Files.loadResource(tablePath + "T.csv");
  constant String tablePath_h=Modelica.Utilities.Files.loadResource(tablePath + "h.csv");

  constant String tablesPath_pT_h=TRANSFORM.Utilities.Strings.concatenate(
      {tablePath_p,tablePath_T,tablePath_h},
      "|",
      3);

  redeclare record extends ThermodynamicState
    Temperature T "Temperature";
    SpecificEnthalpy h "Specific enthalpy";
    AbsolutePressure p "Pressure";
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
  algorithm
    state := ThermodynamicState(
        p=p,
        T=T,
        h=Method(
          tablesPath_pT_h,
          p,
          T));
  end setState_pT;

  redeclare function extends setState_pTX
  algorithm
    state := setState_pT(p, T);
  end setState_pTX;

  redeclare replaceable function specificEnthalpy_pT
    "Return specific enthalpy from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := Method(
        tablesPath_pT_h,
        p,
        T);
    annotation (Inline=true);
  end specificEnthalpy_pT;

  redeclare replaceable function extends temperature
    "Return temperature from state"
  algorithm
    T := state.T;
    annotation (Inline=true);
  end temperature;

  redeclare replaceable function extends specificEnthalpy
    "Return specific enthalpy from state"
  algorithm
    h := state.h;
    annotation (Inline=true);
  end specificEnthalpy;

  redeclare replaceable function extends pressure
    "Return pressure from state"
  algorithm
    p := state.p;
    annotation (Inline=true);
  end pressure;
end ExternalSuperSimpleMedium_new;
