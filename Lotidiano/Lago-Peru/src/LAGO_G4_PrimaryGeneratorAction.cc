#include "LAGO_G4_PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_PrimaryGeneratorAction::LAGO_G4_PrimaryGeneratorAction(){
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
                                     ///FindParticle(particleName="gamma"));
					
  //Default energy,position,momentum
  //fParticleGun->SetParticleEnergy(511.*keV);
  fParticleGun->SetParticleEnergy(10000.*keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0 , 0.0, 0.0*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_PrimaryGeneratorAction::~LAGO_G4_PrimaryGeneratorAction(){
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
