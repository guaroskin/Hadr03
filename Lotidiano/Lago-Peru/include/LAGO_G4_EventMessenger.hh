#ifndef LAGO_G4_EventMessenger_h
#define LAGO_G4_EventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LAGO_G4_EventAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class LAGO_G4_EventMessenger: public G4UImessenger
{
  public:

    LAGO_G4_EventMessenger(LAGO_G4_EventAction*);
    virtual ~LAGO_G4_EventMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    LAGO_G4_EventAction*        fLAGO_G4_Event;
    G4UIcmdWithAnInteger*  fSaveThresholdCmd;
    G4UIcmdWithAnInteger*  fVerboseCmd;
    G4UIcmdWithAnInteger*  fPmtThresholdCmd;
    G4UIcmdWithABool*      fForceDrawPhotonsCmd;
    G4UIcmdWithABool*      fForceDrawNoPhotonsCmd;
};

#endif
