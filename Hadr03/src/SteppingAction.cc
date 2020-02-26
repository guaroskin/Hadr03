//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 98748 2016-08-09 13:42:11Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "Run.hh"
#include "G4Event.hh"
#include "EventAction.hh"
#include "MyHit.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcess.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


//supuesta linea para quitar el warning del supuesto overlap
#include "G4TransportationManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* evt)
: G4UserSteppingAction(),
  Tank_log_vol(0), PMT_log_vol(0), Air_log_vol(0), eventAct(evt), Trigger(0.0)
{
  //supuesta linea para quitar el warning del supuesto overlap
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetPushVerbosity(0);
  //pmtSD = new PMTSD;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
 Run* run 
   = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4StepPoint* postPoint = aStep->GetPostStepPoint();
  //G4StepStatus stepStatus = postPoint->GetStepStatus();
  G4Track* track = aStep->GetTrack();
  G4int TrackID = track->GetTrackID();
  //G4int CurrentStep = track->GetCurrentStepNumber();
  
  G4VProcess* process   = 
                   const_cast<G4VProcess*>(postPoint->GetProcessDefinedStep());
  G4ParticleDefinition* particle = track->GetDefinition();

  //G4StepStatus stepStatus = postPoint->GetStepStatus();
  G4double stepLength = aStep->GetStepLength();
  
  G4String procName = process->GetProcessName();
  G4String partName = particle->GetParticleName();
  G4String PartType = particle->GetParticleType();

  G4ThreeVector r_prePoint = prePoint-> GetPosition();
  G4ThreeVector r_postPoint= postPoint->GetPosition();

  //G4double globalTime = track->GetGlobalTime();  //time since the beginning of the event

  
  /*if (partName == "neutron"){
    G4cout << "Track " << TrackID << "   Step " << CurrentStep << G4endl;; 
    G4cout << globalTime/ns << "ns" << G4endl;
    }*/
      
  if (!Tank_log_vol) { 
    const DetectorConstruction* detectorConstruction
      = static_cast <const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
     Tank_log_vol = detectorConstruction->Get_Tank_log_vol();   
  }
  
  if (!PMT_log_vol) { 
    const DetectorConstruction* detectorConstruction
      = static_cast <const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    PMT_log_vol = detectorConstruction->Get_PMT_log_vol();   
  }

  if (!Air_log_vol) { 
    const DetectorConstruction* detectorConstruction
      = static_cast <const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    Air_log_vol = detectorConstruction->Get_Air_log_vol();   
  }

  //Get volume of the current step
  G4LogicalVolume* Log_volume  = prePoint->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  // Verificar que la particula está en el tanque de agua o en el PMT
  if (Log_volume != Tank_log_vol && Log_volume != PMT_log_vol) return;


  
  //Si la particula es un nucleo
  if(PartType == "nucleus" && partName != "alpha"){
    track->SetTrackStatus(fStopAndKill);
    return;
  }

  G4String Pos_volume_name  = postPoint->GetTouchableHandle()
    ->GetVolume()->GetName();
  
  G4LogicalVolume* Pos_volume  = postPoint->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();


  //Contar el numero de gammas y neutrones que escapan del tanque
  if(partName == "neutron" || partName == "gamma"){
    if (Pos_volume_name == "Tank_Wall" || Pos_volume_name == "World" ){
      G4ThreeVector posT = track->GetPosition();
      G4ThreeVector dirT = track->GetMomentumDirection();
      G4double y = posT.y()/cm;
      //G4cout << "\n X:\t" << G4BestUnit(posT.x(),"Length");
      if(partName == "neutron") {
	if(y <= 0 && dirT.x() < 0.0) { run->NSurvive(false);}
	else run->NSurvive(true);
      }
      if (partName == "gamma") { run->GSurvive(); }
      //track->SetTrackStatus(fStopAndKill);
      return;
    }
  }
  run->CountProcesses(process);

  if(partName == "neutron" && TrackID == 1) {   run->SumTrack(stepLength); }

  G4ThreeVector pos = (r_prePoint + r_postPoint)*0.5;
  //G4ThreeVector pos = r_prePoint + G4UniformRand()*(r_postPoint - r_prePoint);
  //G4cout << "\n\n X:\t" << G4BestUnit(pos.x(),"Length");
  G4double x = pos.x()/cm;//
  
  G4double Q = - prePoint->GetKineticEnergy();


  //initialisation of the nuclear channel identification
  // SE GUARDA EL NOMBRE DE LA INTERACCION
  G4String nuclearChannel = partName;
  G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
  const G4Isotope* target = NULL;
  if (hproc) target = hproc->GetTargetIsotope();
  G4String targetName = "XXXX";  
  if (target) targetName = target->GetName();
  nuclearChannel += " + " + targetName + " --> ";
  if (targetName == "XXXX") run->SetTargetXXX(true);
  
  //scattered primary particle (if any)
  //
  G4int ih = 0;
  G4double Post_energy = postPoint->GetKineticEnergy(); 
  if (track->GetTrackStatus() == fAlive) {
    Q += Post_energy;
    nuclearChannel += partName + " + ";
  }


  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  G4double stepEdep = aStep->GetTotalEnergyDeposit();
  if (procName == "nCapture")  analysis->FillH1(1, x*cm);
  if(stepEdep > 0. && partName == "neutron"){
    analysis->FillH1(2, x*cm, stepEdep/eV);
    run->SumEdep(stepEdep);
  }

  //G4cout << "\n\n Edep:\t"   << stepEdep/eV;
  //G4cout << "\n\n X:\t" << x;
  //G4cout << "\n Edep:\t"   << G4BestUnit(stepEdep, "Energy");
  
  //secondaries
  //
  const std::vector<const G4Track*>* secondary 
                                    = aStep->GetSecondaryInCurrentStep();  
  for (size_t lp=0; lp<(*secondary).size(); lp++) {
    particle = (*secondary)[lp]->GetDefinition(); 
    G4String name   = particle->GetParticleName();
    G4String type   = particle->GetParticleType();      
    G4double energy = (*secondary)[lp]->GetKineticEnergy();
    run->ParticleCount(name,energy);
    //energy spectrum
    ih = 0;
    if (particle == G4Gamma::Gamma())            ih = 3;
    else if (particle == G4Neutron::Neutron())   ih = 4;
    else if (particle == G4Electron::Electron()) ih = 5;
    //else if (particle == G4Positron::Positron()) ih = 6;
    if (ih > 0) analysis->FillH1(ih,energy);
    Q += energy;
    //particle flag
    fParticleFlag[particle]++;
  }

  if ( partName == "opticalphoton" && Pos_volume == PMT_log_vol ){
    MyHit* PhotonHit = new MyHit();
    if(PhotonHit->QuantumE(Post_energy)){
      G4double localTime = track->GetLocalTime(); //Tiempo desde que nace la particula
      analysis->FillH1(7,localTime/ns);
      if(eventAct->New_neutron){
	Trigger = track->GetGlobalTime();      //tiempo desde que llega el primer foton al PMT
	eventAct->New_neutron = false;
      }
      eventAct->SumPhoton();
      G4double globalTime = track->GetGlobalTime();
      eventAct->Pulso(globalTime);
      //G4double PMT_time = globalTime - Trigger;
      //G4cout << "LocalTime  " << localTime/ns << G4endl;
    }
    track->SetTrackStatus(fStopAndKill);
  }
  
  
  //analysis->FillH1(7,Q);
  // nuclear channel
  const G4int kMax = 16;  
  const G4String conver[] = {"0","","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ",
                             "10 ","11 ","12 ","13 ","14 ","15 ","16 "};
  std::map<G4ParticleDefinition*,G4int>::iterator ip;               
  for (ip = fParticleFlag.begin(); ip != fParticleFlag.end(); ip++) {
    particle = ip->first;
    G4String name = particle->GetParticleName();      
    G4int nb = ip->second;
    if (nb > kMax) nb = kMax;   
    G4String Nb = conver[nb];    
    if (particle == G4Gamma::Gamma()) {
     run->CountGamma(nb);
     Nb = "N ";
    } 
    if (ip != fParticleFlag.begin()) nuclearChannel += " + ";
    nuclearChannel += Nb + name;
  }
  
  run->CountNuclearChannel(nuclearChannel, Q);

  fParticleFlag.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
