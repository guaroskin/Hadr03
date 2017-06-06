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
#include "HistoManager.hh"

#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcess.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{ }

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
  G4Track* track = aStep->GetTrack();
  G4VProcess* process   = 
                   const_cast<G4VProcess*>(postPoint->GetProcessDefinedStep());
  G4ParticleDefinition* particle = track->GetDefinition();

  G4StepStatus stepStatus = postPoint->GetStepStatus();
  G4double stepLength = aStep->GetStepLength();
  
  G4String procName = process->GetProcessName();
  G4String partName = particle->GetParticleName();
  G4String PartType = particle->GetParticleType();

  G4ThreeVector r_prePoint = prePoint-> GetPosition();
  G4ThreeVector r_postPoint= postPoint->GetPosition();


  //Si la particula NO es un neutron ni un gamma
  if(partName != "neutron" && partName != "gamma" && partName != "e-"){
    track->SetTrackStatus(fStopAndKill);
    return;
  }
  
  G4bool transmit = (stepStatus==fWorldBoundary);
  if (transmit){
    if(partName == "neutron") { run->NSurvive(true); }
    if (partName == "gamma") { run->NSurvive(false); }
    return;
  }

  run->CountProcesses(process);
  run->SumTrack(stepLength);
  G4double stepEdep = aStep->GetTotalEnergyDeposit();
  if(stepEdep != 0) run->SumEdep(stepEdep);

  G4ThreeVector pos = (r_prePoint + r_postPoint)*0.5;
  //G4ThreeVector pos = r_prePoint + G4UniformRand()*(r_postPoint - r_prePoint);
  //G4cout << "\n\n X:\t" << G4BestUnit(pos.x(),"Length");
  

  G4double x = pos.x()/cm;// + 50;
  if(stepEdep != 0) {
    //G4cout << "\n\n Edep:\t"   << stepEdep/eV;
    //G4cout << "\n\n X:\t" << x;
    //G4cout << "\n Edep:\t"   << G4BestUnit(stepEdep, "Energy");
  }
  
  
  G4double Q = - prePoint->GetKineticEnergy();


  //initialisation of the nuclear channel identification
  //
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
  if (track->GetTrackStatus() == fAlive) {
    G4double energy = postPoint->GetKineticEnergy(); 
    Q += energy;
    //
    nuclearChannel += partName + " + ";
  }  

  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  //G4cout << "\n X:\t"   << x << G4endl;
  if (procName == "nCapture")  analysis->FillH1(1, x*cm);
  if(stepEdep != 0)   analysis->FillH1(6, x*cm, stepEdep/eV);


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
 
    if (particle == G4Gamma::Gamma())            ih = 2;
    else if (particle == G4Neutron::Neutron())   ih = 3;
    else if (particle == G4Proton::Proton())     ih = 4;
    else if (particle == G4Electron::Electron()) ih = 5;
    if (ih > 0) analysis->FillH1(ih,energy);
    Q += energy;
    //particle flag
    fParticleFlag[particle]++;
  }
  
  analysis->FillH1(7,Q);
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

