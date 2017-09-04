
// Geant4 Libraries
//
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include "G4SDManager.hh"


// Local Libraries
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "PMTSD.hh"


SteppingAction::SteppingAction()
  : G4UserSteppingAction()
{
  G4cout << "...SteppingAction..." << G4endl; 
  
  fScintillationCounter = 0;
  fCerenkovCounter      = 0;
  lengthMaxOk = 0;
  fEventNumber = -1;
  priEner = 0.;
  pmtSD = new PMTSD;
  phoOnPmt = 0;
  colSP = radSP = 0;
  
  detectorConstruction 
     = static_cast < const DetectorConstruction* 
     > (G4RunManager::GetRunManager()
         ->GetUserDetectorConstruction());
}

SteppingAction::~SteppingAction()
{ 
  G4cout << "msd " << priEner << G4endl;  
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // ============
  // Get Event Id
  G4int eventNumber                
    = G4RunManager::GetRunManager()
    ->GetCurrentEvent()
    ->GetEventID();

  
  // ******************************
  // Reset variables for each event
  // ******************************
  if (eventNumber != fEventNumber)
  {
    fEventNumber = eventNumber;
    fScintillationCounter = 0;
    fCerenkovCounter = 0;
    G4cout << "msd photon on PMT: " 
      << colSP
      << G4endl;
    phoOnPmt = 0;
    pmtSD->resetPMTSD();
    lengthMaxOk = 1;
    priEner = step->GetTrack()->GetKineticEnergy();
    colSP = 0.;
    radSP = 0.;
  }


  if(step->GetTrack()->GetParentID() == 0)
  {
    G4String prossName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    if(prossName == "eIoni")
    {
      colSP += step->GetTotalEnergyDeposit(); 
    //  radSP += step->GetStepLength();
//      G4cout << "colSP: " << colSP << " " << radSP << G4endl;
    }
    if(prossName == "eBrem")
    {
      radSP += step->GetTotalEnergyDeposit(); 
     // G4cout << "radSP: " << radSP << G4endl;
    } //
  
    if(step->GetTrack()->GetKineticEnergy() == 0. && lengthMaxOk)
    {
      G4cout 
        << "msd loss energy: "
        //<< (priEner - step->GetTrack()->GetKineticEnergy()) 
        //<< priEner / step->GetTrack()->GetTrackLength()
        << "col "
        << colSP 
        << " rad "
        << radSP
        << " dist "
        << step->GetTrack()->GetTrackLength()
        << G4endl;
      G4cout << "msd primary Energy: " << priEner << G4endl;
      lengthMaxOk = 0;
    }
  }

  // **************
  // Stopping Power
  // **************

  /*if(step->GetTrack()->GetParentID() == 0)
    colSP += step->GetStepLength();*/


  // ************************
  // Catching Optical Photons
  // ************************
  
  // ==========================
  // Get current PhysicalVolume
  G4VPhysicalVolume *thePostPV
    = step->GetPostStepPoint()->GetPhysicalVolume();

  G4Track* track = step->GetTrack();

  // =================
  // Get Particle Name
  G4String ParticleName 
    = track
    ->GetDynamicParticle()
    ->GetParticleDefinition()
    ->GetParticleName();


/*  if (ParticleName == "opticalphoton" 
      && thePostPV->GetName() == "pmt")
  {
    //phoOnPmt++;
    phoOnPmt += pmtSD->ProcessHits(step);
    track->SetTrackStatus(fStopAndKill);
    return;
  }*/


  const std::vector<const G4Track*>* secondaries 
    = step->GetSecondaryInCurrentStep();

  fwaterVolume
    = detectorConstruction->GetWaterVolume();

  if (secondaries->size()>0 && fwaterVolume == thePostPV)
    for(unsigned int i=0; i<secondaries->size(); ++i)
      if (secondaries->at(i)->GetParentID()>0)
        if(secondaries->at(i)->GetDynamicParticle()
            ->GetParticleDefinition() 
            == G4OpticalPhoton::OpticalPhotonDefinition())
        {
          if (secondaries->at(i)->GetCreatorProcess()
              ->GetProcessName() 
              == "Scintillation")fScintillationCounter++;
          if (secondaries->at(i)->GetCreatorProcess()
              ->GetProcessName()== "Cerenkov")
            if(secondaries->at(i)->GetKineticEnergy() > 1.6e-06
                && secondaries->at(i)
                ->GetKineticEnergy() < 3.1e-6)
              fCerenkovCounter++;
        }
}
