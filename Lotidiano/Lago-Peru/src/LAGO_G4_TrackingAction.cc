#include "LAGO_G4_Trajectory.hh"
#include "LAGO_G4_TrackingAction.hh"
#include "LAGO_G4_UserTrackInformation.hh"
#include "LAGO_G4_DetectorConstruction.hh"
#include "LAGO_G4_RecorderBase.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_TrackingAction::LAGO_G4_TrackingAction(LAGO_G4_RecorderBase* r)
  : fRecorder(r) {}
//LAGO_G4_TrackingAction::LAGO_G4_TrackingAction(){}  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new LAGO_G4_Trajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new LAGO_G4_UserTrackInformation);

  /*  const G4VProcess* creator = aTrack->GetCreatorProcess();
  if(creator)
    G4cout<<creator->GetProcessName()<<G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_TrackingAction::PostUserTrackingAction(const G4Track* aTrack){
  LAGO_G4_Trajectory* trajectory=(LAGO_G4_Trajectory*)fpTrackingManager->GimmeTrajectory();
  LAGO_G4_UserTrackInformation*
    trackInformation=(LAGO_G4_UserTrackInformation*)aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the sphere and a pmt
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

  //const G4VProcess* creator=aTrack->GetCreatorProcess(); 
  if(trackInformation->GetTrackStatus()&hitPMT){
        trajectory->SetDrawTrajectory(true);}

    //const G4VProcess* creator=aTrack->GetCreatorProcess();
/*    if(creator && creator->GetProcessName()=="OpWLS"){
      trajectory->WLS();
      trajectory->SetDrawTrajectory(true);
    }

    if(LAGO_G4_DetectorConstruction::GetSphereOn()){
      if((trackInformation->GetTrackStatus()&hitPMT)&&
         (trackInformation->GetTrackStatus()&hitSphere)){
        trajectory->SetDrawTrajectory(true);
      }
    }
    else{
      if(trackInformation->GetTrackStatus()&hitPMT)
        trajectory->SetDrawTrajectory(true);
    }
*/
//trajectory->SetDrawTrajectory(true);
  }
  else //draw all other trajectories
    {trajectory->SetDrawTrajectory(true);}

  //if(trackInformation->GetForceDrawTrajectory())
  //  {trajectory->SetDrawTrajectory(true);}

  if(fRecorder)fRecorder->RecordTrack(aTrack);
}
