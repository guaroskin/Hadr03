#ifndef LAGO_G4_SteppingAction_H
#define LAGO_G4_SteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class LAGO_G4_RecorderBase;
class LAGO_G4_EventAction;
class LAGO_G4_TrackingAction;
class LAGO_G4_SteppingMessenger;
class LAGO_G4_HistoManager;

class LAGO_G4_SteppingAction : public G4UserSteppingAction
{
  public:

    LAGO_G4_SteppingAction(LAGO_G4_RecorderBase*, LAGO_G4_HistoManager*);
    LAGO_G4_SteppingAction();
    virtual ~LAGO_G4_SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    LAGO_G4_RecorderBase* fRecorder;
    G4bool fOneStepPrimaries;
    LAGO_G4_SteppingMessenger* fSteppingMessenger;
    LAGO_G4_HistoManager* fHistoMan;

    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
