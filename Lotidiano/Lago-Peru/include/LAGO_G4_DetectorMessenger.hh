#ifndef LAGO_G4_DetectorMessenger_h
#define LAGO_G4_DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LAGO_G4_DetectorParameters;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class LAGO_G4_DetectorMessenger: public G4UImessenger
{
  public:

    LAGO_G4_DetectorMessenger(LAGO_G4_DetectorParameters*);
    virtual ~LAGO_G4_DetectorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    LAGO_G4_DetectorParameters*     fLAGO_G4_DetectorParameters;

    G4UIdirectory*               fDetectorDir;

//  Geometry of the wcd
    G4UIdirectory*               fWcdGeometryDir; 
    
    G4UIcmdWithADoubleAndUnit*   fWcd_rCmd;
    G4UIcmdWithADoubleAndUnit*   fWcd_zCmd;
    G4UIcmdWithADoubleAndUnit*   fTyv_tCmd;
    
    G4UIdirectory*               fPmtGeometryDir; 

    G4UIcmdWithADoubleAndUnit*   fPmtThicknessCmd;
    G4UIcmdWithADoubleAndUnit*   fPmtRadiusCmd;
    G4UIcmdWithADoubleAndUnit*   fPmtOffsetCmd;
    G4UIcmdWithADoubleAndUnit*   fPmtCathThicCmd; 
    
//  Tunning variables of the wcd
    
    G4UIdirectory*     		 fWcdTyvekTuningDir;
// Tyvek 

    G4UIcmdWithADouble* 	 fTyvReflCmd;  
    G4UIcmdWithADouble* 	 fTyvSigAlpCmd; 
    G4UIcmdWithADouble* 	 fTyvSpecLobCmd; 

// Water 

    G4UIdirectory*               fWcdWaterTuningDir;	
 
    G4UIcmdWithADouble* 	 fWGacCmd;
    G4UIcmdWithADouble* 	 fWAbwCmd;
    

   
};

#endif
