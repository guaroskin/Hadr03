//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 98748 2016-08-09 13:42:11Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
//#include "B1EventAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"

#include "HistoManager.hh"

#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4HadronicProcess.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
  : G4UserSteppingAction(), gammas(0), gammasec(0), Neutrones_vol(0), Camino(0),
    E_dep(0), fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

  Run* run 
    = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  G4StepPoint* startPoint = aStep->GetPreStepPoint();
  G4StepPoint* endPoint = aStep->GetPostStepPoint();

  
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }
  
  //Get volume of the current step
  //Hereafter we call current volume the volume where the step has just gone through.
  //Geometrical informations are available from preStepPoint.
  G4LogicalVolume* volume  = startPoint->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;
  

  // ******************************* //
  // ******************************* //
  // ******************************* //
  // ******************************* //

  
  //Algunas definiciones
  //
  G4VProcess* process   = 
    const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
  G4String procName = process->GetProcessName();
  //G4bool transmit = (stepStatus==fWorldBoundary);
  G4Track* track = aStep->GetTrack();
  G4double stepLength = aStep->GetStepLength();
  G4ParticleDefinition* particle = track->GetDefinition();
  G4String partName = particle->GetParticleName();
  G4String PartType = particle->GetParticleType();
  G4String PartSubType = particle->GetParticleSubType();

  
  /*To check that the particle has just entered in the current volume
  (i.e. it is at the first step in the volume; the preStepPoint is at the boundary):*/
  if (startPoint->GetStepStatus() == fGeomBoundary){

    //aseguro que son neutrones que ingresan al tanque Y no fueron creados en su proximidad
    if(partName == "neutron" && track->GetTrackLength()/m > 1.0 ){
      run->NInside(true);
      Camino = 0.;
    }
    
    if(partName == "gamma" ){
      run->NInside(false);
    }  
  }

    


  
  
  //(creo que debe ir despues del endPoint->GetStepStatus() == fGeomBoundary para que no cuente el step en el aire, aunque se pierde un poco de informacion de los que salen
  if (partName == "neutron"){
    Camino += stepLength;
    if(track->GetTrackStatus () == fStopAndKill){
      run->SumTrack(Camino);
      Camino = 0.;
    }
  }
  
  //Si la particula es un nucleo pero no un alpha es eliminada
  if(PartType == "nucleus" && partName != "alpha"){    
    track->SetTrackStatus(fStopAndKill);
    return;
  }

  // Se suma el proceso
  // 
  run->CountProcesses(process);


  //energy-momentum balance initialisation
  //CREO QUE ES INUTIL, REVISAR DESPUES
  const G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4double Q             = - prePoint->GetKineticEnergy();
  G4ThreeVector Pbalance = - prePoint->GetMomentum();
  
  //initialisation of the nuclear channel identification
  // NO ENTIENDO ESTAS LINEAS
  G4String nuclearChannel = partName;
  G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
  const G4Isotope* target = NULL;
  if (hproc) target = hproc->GetTargetIsotope();
  G4String targetName = "XXXX";  
  if (target) targetName = target->GetName();
  nuclearChannel += " + " + targetName + " --> ";
  if (targetName == "XXXX") run->SetTargetXXX(true);
    
  //scattered primary particle (if any)
  // REVISAR DESPUES
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  G4int ih = 1;
  if (track->GetTrackStatus() == fAlive) {
    G4double energy = endPoint->GetKineticEnergy();      
    analysis->FillH1(ih,energy);
    //
    G4ThreeVector momentum = endPoint->GetMomentum();
    Q        += energy;
    Pbalance += momentum;
    //
    nuclearChannel += partName + " + ";
  }  
  
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
    if (particle == G4Gamma::Gamma()){
      gammasec++;
      ih = 2;
    }
    else if (particle == G4Neutron::Neutron())   ih = 3;
    else if (particle == G4Proton::Proton())     ih = 4;
    else if (particle == G4Deuteron::Deuteron()) ih = 5;
    else if (particle == G4Alpha::Alpha())       ih = 6;       
    else if (type == "nucleus")                  ih = 7;
    else if (type == "baryon")                   ih = 8;
    else if (particle == G4Electron::Electron()) ih = 9;
    if (ih > 0) analysis->FillH1(ih,energy);
    //atomic mass
    if (type == "nucleus") {
      G4int A = particle->GetAtomicMass();
      analysis->FillH1(12, A);
    }
    //energy-momentum balance
    G4ThreeVector momentum = (*secondary)[lp]->GetMomentum();
    Q        += energy;
    Pbalance += momentum;
    //particle flag
    fParticleFlag[particle]++;
  }
  
  //energy-momentum balance
  G4double Pbal = Pbalance.mag();
  run->Balance(Pbal);
  ih = 10;
  analysis->FillH1(ih,Q);
  ih = 11;
  analysis->FillH1(ih,Pbal);  
  
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
 
  ///G4cout << "\n nuclear channel: " << nuclearChannel << G4endl;
  run->CountNuclearChannel(nuclearChannel, Q);
    
  fParticleFlag.clear();

  if(partName == "gamma"){
    if( procName != "compt"){
      gammas ++;
      //G4cout << "gammas Existen" << "= " << gammas << G4endl;
      //G4cout << "gammas Secunda" << "= " << gammasec << G4endl;
    }
    //ELIMINA LOS GAMMAS FANTASMAS
    /*if(gammas > gammasec){
      track->SetTrackStatus(fStopAndKill);
      //G4cout << "elID" << "= " << track->GetParentID() << G4endl;
      return;
      }*/
  }

  /*To check that the particle is leaving the current volume (i.e. it is at the last step in the volume; the postStepPoint is at the boundary):*/
  if (endPoint->GetStepStatus() == fGeomBoundary){
    if(partName == "neutron") {
      run->NSurvive(true);
    }
    if (partName == "gamma" ) {
      run->NSurvive(false); }
    return;
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*G4cout << G4endl;    
    G4cout << std::setw( 5) << "#Step#"     << " "
	   << std::setw( 6) << "X"          << "    "
	   << std::setw( 6) << "Y"          << "    "  
	   << std::setw( 6) << "Z"          << "    "
	   << std::setw( 9) << "KineE"      << " "
	   << std::setw( 9) << "dEStep"     << " "  
	   << std::setw(10) << "StepLeng"     
	   << std::setw(10) << "TrakLeng" 
	   << std::setw(10) << "Volume"    << "  "
	   << std::setw(10) << "ID"   << G4endl;                  
    
    x
    G4cout << std::setw( 5) << track->GetCurrentStepNumber() << " "
	   << std::setw(6) << G4BestUnit(track->GetPosition().x(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetPosition().y(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetPosition().z(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetKineticEnergy(),"Energy")
	   << std::setw(6) << G4BestUnit(aStep->GetTotalEnergyDeposit(),"Energy")
	   << std::setw(6) << G4BestUnit(aStep->GetStepLength(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetTrackLength(),"Length")
	   << std::setw(10) << track->GetVolume()->GetName()
      	   << std::setw(6) << track->GetParentID() << G4endl;



    

    G4cout << "PROCESO" << procName << G4endl; 
    G4cout << std::setw( 5) << "#Step#"     << " "
	   << std::setw( 6) << "X"          << "    "
	   << std::setw( 6) << "Y"          << "    "  
	   << std::setw( 6) << "Z"          << "    "
	   << std::setw( 9) << "TotalE"     << " "
	   << std::setw( 9) << "KineE"      << " "
	   << std::setw( 9) << "Camino"     << " "    //dEStep
	   << std::setw(10) << "StepLeng"   << "  "  
	   << std::setw(10) << "TrakLeng"   << "  "
	   << std::setw(10) << "Volume"     << "  "
           << std::setw(10) << "TrackID"    << "  "
	   << std::setw(10) << "ParentID"   << "  "
	   << std::setw(10) << "Status"   << G4endl;                  
    
    
    G4cout << std::setw( 5) << track->GetCurrentStepNumber() << " "
	   << std::setw(6) << G4BestUnit(track->GetPosition().x(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetPosition().y(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetPosition().z(),"Length")
	   << std::setw(6) << G4BestUnit(track->GetTotalEnergy(),"Energy")
	   << std::setw(6) << G4BestUnit(track->GetKineticEnergy(),"Energy")
      //<< std::setw(6) << G4BestUnit(aStep->GetTotalEnergyDeposit(),"Energy")
	   << std::setw(6) << G4BestUnit(Camino,"Length")
	   << std::setw(6) << G4BestUnit(stepLength,"Length")
	   << std::setw(6) << G4BestUnit(track->GetTrackLength(),"Length")
	   << std::setw(10)<< track->GetVolume()->GetName()
	   << std::setw(6) << track->GetTrackID() 
	   << std::setw(6) << track->GetParentID()
	   << std::setw(6) << track->GetTrackStatus ()  << G4endl;
*/
