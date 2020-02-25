#include "ElectromagneticPhysics.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ElectromagneticPhysics::ElectromagneticPhysics(const G4String& name)
: G4VPhysicsConstructor(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ElectromagneticPhysics::~ElectromagneticPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
//#include "G4NeutrinoE.hh"
//#include "G4AntiNeutrinoE.hh"

void ElectromagneticPhysics::ConstructParticle()
{
  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4Proton::ProtonDefinition();
  //G4NeutrinoE::NeutrinoEDefinition();
  //G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ProcessManager.hh"

void ElectromagneticPhysics::ConstructProcess()
{

  // Gamma Physics
  G4PhotoElectricEffect* fPhotoEffect = new G4PhotoElectricEffect();
  G4ComptonScattering* fComptonEffect = new G4ComptonScattering();
  G4GammaConversion* fPairProduction = new G4GammaConversion();
  //G4PhotoNuclearProcess* PhotoNuclear = new G4PhotoNuclearProcess();
 
    // Electron physics
  G4eMultipleScattering* fElectronMultipleScattering = new G4eMultipleScattering();
  G4eIonisation* fElectronIonisation = new G4eIonisation();
  G4eBremsstrahlung* fElectronBremsStrahlung = new G4eBremsstrahlung();
 
    //Positron physics
  G4eMultipleScattering* fPositronMultipleScattering = new G4eMultipleScattering();
  G4eIonisation* fPositronIonisation = new G4eIonisation();
  G4eBremsstrahlung* fPositronBremsStrahlung = new G4eBremsstrahlung();
  G4eplusAnnihilation* fAnnihilation = new G4eplusAnnihilation();

    //Proton physics
  G4hMultipleScattering* fProtonMultipleScattering = new G4hMultipleScattering();
  G4hIonisation* fProtonIonisation = new G4hIonisation();
  
  G4ProcessManager* pManager = 0;
  
  // Gamma Physics
  pManager = G4Gamma::Gamma()->GetProcessManager();
  pManager->AddDiscreteProcess(fPhotoEffect);
  pManager->AddDiscreteProcess(fComptonEffect);
  pManager->AddDiscreteProcess(fPairProduction);
  
  /*G4int G4ProcessManager::AddProcess(G4VProcess *aProcess, G4int ordAtRestDoIt, 
                                         G4int ordAlongStepDoIt, G4int ordPostStepDoIt)*/
  // Electron Physics
  pManager = G4Electron::Electron()->GetProcessManager();
  pManager->AddProcess(fElectronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fElectronIonisation,         -1, 2, 2);
  pManager->AddProcess(fElectronBremsStrahlung,     -1, 3, 3);  

  //Positron Physics
  pManager = G4Positron::Positron()->GetProcessManager();
  pManager->AddProcess(fPositronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fPositronIonisation,         -1, 2, 2);
  pManager->AddProcess(fPositronBremsStrahlung,     -1, 3, 3);  
  pManager->AddProcess(fAnnihilation,                0,-1, 4);  

  // Proton Physics
  pManager = G4Proton::Proton()->GetProcessManager();    
  pManager->AddProcess(fProtonMultipleScattering,     -1, 2, 2);
  pManager->AddProcess(fProtonIonisation, -1, 3, 3);
}


// EXPLICACIÓN DE LOS NÚMEROS
//  Add a process to the process List
//  return values are index to the List. Negative return value 
//  indicates that the process has not be added due to some errors
//  The first argument is a pointer to process.
//  Following arguments are ordering parameters of the process in 
//  process vectors. If value is negative, the process is
//  not added to the corresponding process vector.     
//  following methods are provided for simple processes  
//      AtRestProcess has only AtRestDoIt
//      ContinuousProcess has only AlongStepDoIt
//      DiscreteProcess has only PostStepDoIt
//  If the ording parameter is not specified, the process is
//  added at the end of List of process vectors 
//  If a process with same ordering parameter exists,
//  this new process will be added just after processes 
//   with same ordering parameter  
//  (except for processes assigned to LAST explicitly )
//  for both DoIt and GetPhysicalInteractionLength
