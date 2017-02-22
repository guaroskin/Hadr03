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
/// \file hadronic/Hadr03/src/GammaNuclearPhysics.cc
/// \brief Implementation of the GammaNuclearPhysics class
//
// $Id: GammaNuclearPhysics.cc 66587 2012-12-21 11:06:44Z ihrivnac $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GammaNuclearPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes

#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaNuclearPhysics::GammaNuclearPhysics(const G4String& name)
:  G4VPhysicsConstructor(name)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaNuclearPhysics::~GammaNuclearPhysics()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaNuclearPhysics::ConstructProcess()
{
   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
   //
   G4PhotoElectricEffect * thePhotoElectricEffect = new G4PhotoElectricEffect();
   G4ComptonScattering * theComptonScattering = new G4ComptonScattering();
   G4GammaConversion* theGammaConversion = new G4GammaConversion();
   G4PhotoNuclearProcess* PhotoNuclear = new G4PhotoNuclearProcess();
   //
   G4CascadeInterface* bertini = new G4CascadeInterface();
   bertini->SetMaxEnergy(10*GeV);
   PhotoNuclear->RegisterMe(bertini);
   //
   pManager->AddDiscreteProcess(PhotoNuclear);
   pManager->AddDiscreteProcess(thePhotoElectricEffect);
   pManager->AddDiscreteProcess(theComptonScattering);
   pManager->AddDiscreteProcess(theGammaConversion);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/* una forma para obtener todos los efectos electromagneticos es en el ejemplo B3
   esto deberia ir en la phyisics list en vez de 
   ** RegisterPhysics( new GammaNuclearPhysics("gamma"));**
   lo cual (supongo) elimina la necesidad de este archivo y da todos los efectos EM de una.
   ¿dudas? mira el ejemplo B3b ;)

   #include "G4EmStandardPhysics.hh"
   
   // link http://www.apc.univ-paris7.fr/~franco/g4doxy/html/classG4EmStandardPhysics.html
   // aqui esta todos los efectos EM. por ahora vamos solo con los de gammas.
   B3PhysicsList::B3PhysicsList() 
   : G4VModularPhysicsList(){
   SetVerboseLevel(1);
   
   // EM physics
   RegisterPhysics(new G4EmStandardPhysics());
   }*/
