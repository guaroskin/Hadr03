//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

#include "G4LossTableManager.hh"

#include "OpticalPhysics.hh"
/*#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"*/

OpticalPhysics::OpticalPhysics(const G4String& name)
: G4VPhysicsConstructor(name)
{
//fWLSProcess                = NULL;
//fScintProcess              = NULL;
  fCerenkovProcess           = NULL;
  fBoundaryProcess           = NULL;
  fAbsorptionProcess         = NULL;
  fRayleighScattering        = NULL;
  fMieHGScatteringProcess    = NULL;

}

OpticalPhysics::~OpticalPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4OpticalPhoton.hh"

void OpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//#include "G4Threading.hh"
#include "G4ProcessManager.hh"

void OpticalPhysics::ConstructProcess()
{

/*fScintillationProcess = new G4Scintillation("Scintillation");
  fScintillationProcess->SetScintillationYieldFactor(1.);
  fScintillationProcess->SetTrackSecondariesFirst(true);*/

  G4int fMaxNumPhotonStep = 300;   // 300 en WLSOpticalPhy

  fCerenkovProcess = new G4Cerenkov("Cerenkov");
  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  fCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  fCerenkovProcess->SetTrackSecondariesFirst(true);



  fAbsorptionProcess = new G4OpAbsorption();
  fRayleighScattering     = new G4OpRayleigh();
  fMieHGScatteringProcess = new G4OpMieHG();
  fBoundaryProcess = new G4OpBoundaryProcess();

  G4ProcessManager* pManager =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  
  if (!pManager) {
    std::ostringstream o;
    o << "Optical Photon without a Process Manager";
    G4Exception("OpticalPhysics::ConstructProcess()","",
		FatalException,o.str().c_str());
  }

  pManager->AddDiscreteProcess(fAbsorptionProcess);

  //pManager->AddDiscreteProcess(fRayleighScattering);
  //pManager->AddDiscreteProcess(fMieHGScatteringProcess); tambien asi en el original WLSOptical...cc

  pManager->AddDiscreteProcess(fBoundaryProcess);

  

  
  /* Use Birks Correction in the Scintillation process
  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  fScintProcess->AddSaturation(emSaturation);
*/

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){

    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    pManager = particle->GetProcessManager();

    if (!pManager) {
       std::ostringstream o;
       o << "Particle " << particleName << "without a Process Manager";
       G4Exception("OpticalPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
    }

    if (fCerenkovProcess->IsApplicable(*particle)) {
      pManager->AddProcess(fCerenkovProcess);
      pManager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    /*if (fScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fScintillationProcess);
      pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
      }*/
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalPhysics::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
  fCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}
