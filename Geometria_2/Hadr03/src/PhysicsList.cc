//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
//#include "G4HadronPhysicsQGSP_BIC_HP.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "NeutronHPphysics.hh"
#include "ElectromagneticPhysics.hh"
#include "OpticalPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

/*
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4PionDecayMakeSpin.hh"
#include "G4DecayWithSpin.hh"

#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
*/
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
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  
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

  // Optic Physics   
  RegisterPhysics(fOpticalPhysics = new OpticalPhysics("Optical"));
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{

  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.
  
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

  /*
  G4VModularPhysicsList::ConstructParticle();
  
  G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
  MuonPlusDecayTable -> Insert(new
			       G4MuonDecayChannelWithSpin("mu+",0.986));
  MuonPlusDecayTable -> Insert(new
			       G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
  G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);
  
  G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
  MuonMinusDecayTable -> Insert(new
				G4MuonDecayChannelWithSpin("mu-",0.986));
  MuonMinusDecayTable -> Insert(new
				G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
  G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);
  */  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(1*mm, "proton");
}


void PhysicsList::SetNbOfPhotonsCerenkov(G4int maxNumber)
{
   fOpticalPhysics->SetNbOfPhotonsCerenkov(maxNumber);
}

void PhysicsList::SetVerbose(G4int verbose)
{
   fOpticalPhysics->GetCerenkovProcess()->SetVerboseLevel(verbose);
   //fOpticalPhysics->GetScintillationProcess()->SetVerboseLevel(verbose);
   fOpticalPhysics->GetAbsorptionProcess()->SetVerboseLevel(verbose);
   //fOpticalPhysics->GetRayleighScatteringProcess()->SetVerboseLevel(verbose);
   //fOpticalPhysics->GetMieHGScatteringProcess()->SetVerboseLevel(verbose);
   fOpticalPhysics->GetBoundaryProcess()->SetVerboseLevel(verbose);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
