#include "LAGO_G4_ActionInitialization.hh"

#include "LAGO_G4_PrimaryGeneratorAction.hh"
#include "LAGO_G4_HistoManager.hh" 
#include "LAGO_G4_RunAction.hh"
#include "LAGO_G4_EventAction.hh"
#include "LAGO_G4_TrackingAction.hh"
#include "LAGO_G4_SteppingAction.hh"
#include "LAGO_G4_StackingAction.hh"
//#include "LAGO_G4_SteppingVerbose.hh"

#include "LAGO_G4_RecorderBase.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_ActionInitialization::LAGO_G4_ActionInitialization(LAGO_G4_RecorderBase* recorder, LAGO_G4_HistoManager* histo) 
 : G4VUserActionInitialization(), fRecorder(recorder), fHisto(histo)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_ActionInitialization::~LAGO_G4_ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_ActionInitialization::BuildForMaster() const
{
  SetUserAction(new LAGO_G4_RunAction(fRecorder));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_ActionInitialization::Build() const
{
  SetUserAction(new LAGO_G4_PrimaryGeneratorAction());

  SetUserAction(new LAGO_G4_StackingAction());

  SetUserAction(new LAGO_G4_RunAction(fRecorder));
  SetUserAction(new LAGO_G4_EventAction(fRecorder));
  SetUserAction(new LAGO_G4_TrackingAction(fRecorder));
  SetUserAction(new LAGO_G4_SteppingAction(fRecorder,fHisto));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
G4VSteppingVerbose* LAGO_G4_ActionInitialization::InitializeSteppingVerbose() const
{
  return new LAGO_G4_SteppingVerbose();
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
