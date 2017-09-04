#ifndef LAGO_G4_SteppingMessenger_h
#define LAGO_G4_SteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LAGO_G4_SteppingAction;
class G4UIcmdWithABool;

class LAGO_G4_SteppingMessenger: public G4UImessenger
{
  public:
    LAGO_G4_SteppingMessenger(LAGO_G4_SteppingAction*);
    virtual ~LAGO_G4_SteppingMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    LAGO_G4_SteppingAction*        fStepping;
    G4UIcmdWithABool*  fOneStepPrimariesCmd;
 
};

#endif
