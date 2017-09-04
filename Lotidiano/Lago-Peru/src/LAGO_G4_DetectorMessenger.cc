#include "LAGO_G4_DetectorMessenger.hh"
#include "LAGO_G4_DetectorParameters.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_DetectorMessenger::LAGO_G4_DetectorMessenger(LAGO_G4_DetectorParameters* detector)
 : fLAGO_G4_DetectorParameters(detector)
{
  //Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("/LAGO_G4/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");
 
  //Various commands for modifying detector geometry

  //  Geometry of the wcd   

  fWcdGeometryDir = new G4UIdirectory("/LAGO_G4/detector/WCDGeometry/");
  fWcdGeometryDir->SetGuidance("Geometry parameters of the WCD");

  fWcd_rCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/WCDGeometry/WcdRadius",this); 
  fWcd_rCmd->SetGuidance("Set the radius of the wcd.");
  fWcd_rCmd->SetParameterName("wcd_r",false); 
  fWcd_rCmd->SetDefaultUnit("cm");
  fWcd_rCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWcd_rCmd->SetToBeBroadcasted(false);

  fWcd_rCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/WCDGeometry/WcdHeight",this); 
  fWcd_rCmd->SetGuidance("Set the height of the wcd.");
  fWcd_rCmd->SetParameterName("wcd_z",false); 
  fWcd_rCmd->SetDefaultUnit("cm");
  fWcd_rCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWcd_rCmd->SetToBeBroadcasted(false);

  fTyv_tCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/WCDGeometry/TyvekThickness",this)  ;
  fTyv_tCmd->SetGuidance("Set the thickness of the tyvek.");
  fTyv_tCmd->SetParameterName("tyv_t",false);  
  fTyv_tCmd->SetDefaultUnit("cm");
  fTyv_tCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fTyv_tCmd->SetToBeBroadcasted(false);
  
  // Geometry of the PMT  

  fWcdGeometryDir = new G4UIdirectory("/LAGO_G4/detector/PMTGeometry/");
  fWcdGeometryDir->SetGuidance("Geometry parameters of the PMY"); 

  fPmtRadiusCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/PMTGeometry/PmtRadius",this);
  fPmtRadiusCmd->SetGuidance("Set the radius of the PMTs.");
  fPmtRadiusCmd->SetParameterName("pmt_r",false);
  fPmtRadiusCmd->SetDefaultUnit("cm");
  fPmtRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
  fPmtRadiusCmd->SetToBeBroadcasted(false);

  fPmtThicknessCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/PMTGeometry/PmtThickness",this);
  fPmtThicknessCmd->SetGuidance("Set the thickness of the PMTs.");
  fPmtThicknessCmd->SetParameterName("pmt_t",false);
  fPmtThicknessCmd->SetDefaultUnit("cm");
  fPmtThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fPmtThicknessCmd->SetToBeBroadcasted(false);
 
  fPmtOffsetCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/PMTGeometry/PmtOffset",this);
  fPmtOffsetCmd->SetGuidance("Set the offset of the PMTs.");
  fPmtOffsetCmd->SetParameterName("pmt_offset",false);
  fPmtOffsetCmd->SetDefaultUnit("cm");
  fPmtOffsetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fPmtOffsetCmd->SetToBeBroadcasted(false);

  fPmtCathThicCmd = new G4UIcmdWithADoubleAndUnit("/LAGO_G4/detector/PMTGeometry/PmtCathThick",this);
  fPmtCathThicCmd->SetGuidance("Set the cathode thickness of the PMTs.");
  fPmtCathThicCmd->SetParameterName("pmt_cath_thic",false);
  fPmtCathThicCmd->SetDefaultUnit("cm");
  fPmtCathThicCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fPmtCathThicCmd->SetToBeBroadcasted(false);

  //  Tunning of the wcd

  fWcdTyvekTuningDir = new G4UIdirectory("/LAGO_G4/detector/TyvekTunning/");
  fWcdTyvekTuningDir->SetGuidance("Tunning parameters of the WCD");

  fTyvReflCmd =new G4UIcmdWithADouble("/LAGO_G4/detector/TyvekTunning/TyRefl",this);
  fTyvReflCmd->SetGuidance("Set Reflectivity.");
  fTyvReflCmd->SetGuidance("From 0 to 1");  
  fTyvReflCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fTyvReflCmd->SetToBeBroadcasted(false);

  fTyvSigAlpCmd =new G4UIcmdWithADouble("/LAGO_G4/detector/TyvekTunning/TySigAlp",this);
  fTyvSigAlpCmd->SetGuidance("Set Sigma AlPha Lambertian Reflectivity.");
  fTyvSigAlpCmd->SetGuidance("From 0 to 1");  
  fTyvSigAlpCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fTyvSigAlpCmd->SetToBeBroadcasted(false);

  fTyvSpecLobCmd =new G4UIcmdWithADouble("/LAGO_G4/detector/TyvekTunning/TySpecLob",this);
  fTyvSpecLobCmd->SetGuidance("Set Specular Lobe Reflectivity.");
  fTyvSpecLobCmd->SetGuidance("From 0 to 1");  
  fTyvSpecLobCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fTyvSpecLobCmd->SetToBeBroadcasted(false);

///////////////////////////////////////////////////////////////////////////////

  fWcdWaterTuningDir = new G4UIdirectory("/LAGO_G4/detector/WaterTunning/");
  fWcdWaterTuningDir->SetGuidance("Tunning parameters of the WCD");

  fWAbwCmd = new G4UIcmdWithADouble("/LAGO_G4/detector/WaterTunning/abw",this);
  fWAbwCmd->SetGuidance("Set the water attenuation parameter");
  fWAbwCmd->SetParameterName("Abw",false);
  fWAbwCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWAbwCmd->SetToBeBroadcasted(false);

  fWGacCmd = new G4UIcmdWithADouble("/LAGO_G4/detector/WaterTunning/gac",this);
  fWGacCmd->SetGuidance("Set the water G-amine concentration");
  fWGacCmd->SetParameterName("Gac",false);
  fWGacCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fWGacCmd->SetToBeBroadcasted(false);

  //fUpdateCmd = new G4UIcommand("/LAGO_G4/detector/update",this);
  //fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  //fUpdateCmd->SetGuidance("Must be run before beamOn if detector has been changed.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_DetectorMessenger::~LAGO_G4_DetectorMessenger()
{
    

//  Geometry of the wcd
    delete   fWcdGeometryDir; 
    
    delete   fWcd_rCmd; 
    delete   fWcd_zCmd;
    delete   fTyv_tCmd;

    delete   fPmtGeometryDir;
    
    delete   fPmtThicknessCmd;
    delete   fPmtRadiusCmd;
    delete   fPmtOffsetCmd;
    delete   fPmtCathThicCmd;

//  Tunning variables of the wcd

    delete   fWcdTyvekTuningDir;

    delete   fTyvReflCmd;
    delete   fTyvSigAlpCmd; 
    delete   fTyvSpecLobCmd;   

    delete   fWcdWaterTuningDir;

    delete   fWAbwCmd;
    delete   fWGacCmd;

    delete   fDetectorDir;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  //wcd Geometry  
  if( command == fWcd_rCmd ){
    fLAGO_G4_DetectorParameters->SetWCDRadius(fWcd_rCmd->GetNewDoubleValue(newValue)); 
  }
  else if( command == fWcd_zCmd ){
    fLAGO_G4_DetectorParameters->SetWCDHeight(fWcd_zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fTyv_tCmd){
    fLAGO_G4_DetectorParameters->SetTyvekThickness(fTyv_tCmd->GetNewDoubleValue(newValue));
  }  
  /////////////////////////////////////////////////////
  else if (command == fPmtRadiusCmd){
    fLAGO_G4_DetectorParameters->SetPMTRadius(fPmtRadiusCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fPmtThicknessCmd){
    fLAGO_G4_DetectorParameters->SetPMTThicknes(fPmtThicknessCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fPmtOffsetCmd){
    fLAGO_G4_DetectorParameters->SetPMTCathThic(fPmtOffsetCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fPmtCathThicCmd){
    fLAGO_G4_DetectorParameters->SetPMTOffset(fPmtCathThicCmd->GetNewDoubleValue(newValue));
  }
 
  //WCD tunning 
////////////////////////
 else if (command == fTyvReflCmd){
    fLAGO_G4_DetectorParameters->SetTyvekRefl(fTyvReflCmd->GetNewDoubleValue(newValue));
  }
 else if (command == fTyvSigAlpCmd){
    fLAGO_G4_DetectorParameters->SetTyvekSigAlp(fTyvSigAlpCmd->GetNewDoubleValue(newValue));
  }   
  else if (command == fTyvSpecLobCmd){
    fLAGO_G4_DetectorParameters->SetTyvekSpecLob(fTyvSpecLobCmd->GetNewDoubleValue(newValue));
  }    
///////////////////////////////////////////////////////////////////////////////////
else if (command == fWAbwCmd){
    fLAGO_G4_DetectorParameters->SetWaterAbw(fWAbwCmd->GetNewDoubleValue(newValue));
  }
else if (command == fWGacCmd){
    fLAGO_G4_DetectorParameters->SetWaterGac(fWGacCmd->GetNewDoubleValue(newValue));
  }
/////////////////////////////////////////////////////



}
