//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4HadronElasticPhysicsHP.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "NeutronHPphysics.hh"
#include "ElectromagneticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//#include "GammaNuclearPhysics.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 0;  
  SetVerboseLevel(verb);
  
  //add new units for cross sections
  //
  new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);  

  // neutronHP
  //
  RegisterPhysics( new NeutronHPphysics("neutronHP") );

  
  /*NO ES NECESARIO
  // Hadron Elastic scattering
  //
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb));

  // Hadron Inelastic physics
  //
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  */

  
  // Ion Elastic scattering
  //
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  // Ion Inelastic physics
  //
  RegisterPhysics( new G4IonPhysics(verb));
    
  // EM Physics
  //
  RegisterPhysics( new ElectromagneticPhysics("standard EM"));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(1*mm, "proton");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
