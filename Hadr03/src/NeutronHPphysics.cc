#include "NeutronHPphysics.hh"

#include "G4HadronicProcess.hh"
#include "G4HadronElasticProcess.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPThermalScatteringData.hh"

#include "G4SystemOfUnits.hh"

#include "G4HadronElasticProcess.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"

#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHPphysics::NeutronHPphysics(const G4String& name)
: G4VPhysicsConstructor(name) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHPphysics::~NeutronHPphysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Neutron.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"

void NeutronHPphysics::ConstructProcess()
{

  G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4ProcessManager* pManager = neutron->GetProcessManager();
  //G4HadronElasticPhysics::ConstructProcess();
  //GetNeutronModel()->SetMinEnergy(19.5*MeV);
  
  // create process: ELASTIC
  // 
  G4HadronElasticProcess *elasticnp = new G4HadronElasticProcess;
  pManager->AddDiscreteProcess(elasticnp);
  //
  // model1a
  G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
  //G4NeutronHPElastic *elasticn = new G4NeutronHPElastic; ??
  elasticnp->RegisterMe(model1a);
  elasticnp->AddDataSet(new G4ParticleHPElasticData());
  model1a->SetMinEnergy(4*eV);   
  //
  // model1b
  G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
  elasticnp->RegisterMe(model1b);
  elasticnp->AddDataSet(new G4ParticleHPThermalScatteringData());         
  model1b->SetMaxEnergy(4.*eV);  


  /* Gamma Physics
     G4PhotoElectricEffect* fPhotoEffect = new G4PhotoElectricEffect();
     G4ComptonScattering* fComptonEffect = new G4ComptonScattering();
     G4GammaConversion* fPairProduction = new G4GammaConversion();
     G4PhotoNuclearProcess* PhotoNuclear = new G4PhotoNuclearProcess();*/
  
  // create process: INELASTIC
  //
  G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
  pManager->AddDiscreteProcess(process2);
  //
  // cross section data set
  G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
  process2->AddDataSet(dataSet2);
  //
  // models
  G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
  process2->RegisterMe(model2);  
  
  // create process: CAPTURE
  //
  G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
  pManager->AddDiscreteProcess(process3);    
  //
  // cross section data set
  G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
  process3->AddDataSet(dataSet3);                               
  //
  // models
  G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
  process3->RegisterMe(model3);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


