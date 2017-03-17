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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 98748 2016-08-09 13:42:11Z gcosmo $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
//#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryTolerance.hh"   //nueva
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"

#include "G4VisAttributes.hh"       //nueva
#include "G4Colour.hh"              //nueva
#include "G4SystemOfUnits.hh"       //nueva

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  :G4VUserDetectorConstruction()
{
  DefineMaterials();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ ;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // define a Material from isotopes
  //
  G4NistManager* nist = G4NistManager::Instance();

  fAir = nist->FindOrBuildMaterial("G4_AIR");
  fParaffin = nist->FindOrBuildMaterial("G4_Galactic");
  fStainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  fWater = nist->FindOrBuildMaterial("G4_WATER");
    
  //NE213
  G4Element* H  = new G4Element("Hydrogen" ,"H" , 1.,  1.01*g/mole);
  G4Element* C  = new G4Element("Hydrogen" ,"C" , 6., 12.00*g/mole);
  G4Material* ne213 = 
    new G4Material("NE213", 0.874*g/cm3, 2);
  ne213->AddElement(H,  9.2*perCent);
  ne213->AddElement(C,  90.8*perCent);   
  
  ///G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
#ifdef NDEBUG
  G4bool const overlapChecking = false;
#else
  G4bool const overlapChecking = true;
#endif // NDEBUG
  
  // ** world **
  const G4double worldSize = 200.0 * cm;
  const G4double worldHeight = worldSize/2;
  
  G4ThreeVector position;
  
  
  G4Box* const solidWorld = new G4Box("World",        // the name
				      worldSize,      // (-200 , 200)
				      worldSize,      // y/2 size
				      worldHeight);   // (-100 , 100)

  
  //Puede ser creado en DetectorConstruction.hh
  G4LogicalVolume* const logicalWorld
    = new G4LogicalVolume(solidWorld,               // the solid volume
			  fAir,                     // the material
			  solidWorld->GetName());   // the name
  
  // Centrado para que el fondo quede en (0,0,0)  Z:(-100 - 100 cm)
  position.set(0.0, 0.0, 0.0); 
  G4VPhysicalVolume* const physicalWorld
    = new G4PVPlacement(NULL,                       // no rotation
			position,                   // must be at origin
			logicalWorld,               // the logical volume
			logicalWorld->GetName(),    // the name
			NULL,                       // no mother volume
			false,                      // no boolean ops
			0,                          // copy number
			overlapChecking);           // check for overlaps
  logicalWorld -> SetVisAttributes(G4VisAttributes::Invisible);
  //
  // El tanqye esta suspendido en el aire
  //
  const G4double floorH = 40.0 * cm;
  
  //const G4ThreeVector floorPosition(0.0, 0.0, -floorH * 0.5);
  G4Box* const solidFloor = new G4Box("Floor",        // the name
				      worldSize,      // x/2 size
				      worldSize,      // y/2 size
				      floorH * 0.5);  // z/2 size
  G4LogicalVolume* const logicalFloor
    = new G4LogicalVolume(solidFloor,               // the solid volume
			  fAir,                // the material
			  solidFloor->GetName());   // the name
  // La base del piso está en el fondo del mundo Z:(-100 - 60 cm)
  // position.set(0.0, 0.0, -floorH * 0.5); 
  position.set(0.0, 0.0, -worldHeight + floorH*0.5); 
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // position
		    logicalFloor,                     // the logical volume
		    logicalFloor->GetName(),          // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps
  
  //
  // Create the tank
  //
  const G4double tankWallThickness = 0.2* cm;
  const G4double tankOR = 48.0 * cm;      // 96 cm de diametro
  const G4double tankH = 62.0 * cm;

  G4Tubs* const solidTank
    = new G4Tubs("Tank_Wall",                       // the name
		 0.0,                               // inner radius
		 tankOR,                            // outer radius
		 tankH * 0.5,                       // half height
		 0.0 * deg,                         // start angle
		 360.0 * deg);                      // end angle

  G4LogicalVolume* const logicalTank
    = new G4LogicalVolume(solidTank,                // the solid volume
			  fStainlessSteel,          // the material
			  solidTank->GetName());    // the name
  
  //La base empiza donde termina el piso Z: (-60 - 2 cm)
  // position.set(0.0, 0.0, tankH * 0.5);
  position.set(0.0, 0.0, - worldHeight + (tankH * 0.5 + floorH));
    new G4PVPlacement(NULL,                             // no rotation
                      position,                         // shift up
                      logicalTank,                      // the logical volume
                      logicalTank->GetName(),           // the name
                      logicalWorld,                     // the mother volume
                      false,                            // no boolean ops
                      0,                                // copy number
                      overlapChecking);                 // check for overlaps

    
    // Top 15 cms are air
    const G4double tankAirH = 8.0 * cm;
    G4Tubs* const solidTankAir
        = new G4Tubs("Tank_Air",                        // the name
                     0.0,                               // inner radius
                     tankOR - tankWallThickness,        // outer radius
                     tankAirH * 0.5,                    // half height
                     0.0 * deg,                         // start angle
                     360.0 * deg);                      // end angle
    G4LogicalVolume* const logicalTankAir
        = new G4LogicalVolume(solidTankAir,             // the solid volume
                              fAir,                     // the material
                              solidTankAir->GetName()); // the name
    
    // Top of the air is the same as the top of the tank  Z:(-6 - 2 cm)
    position.set(0.0, 0.0, (tankH - tankAirH) * 0.5);
    new G4PVPlacement(NULL,                             // no rotation
                      position,                         // shift ip
                      logicalTankAir,                   // the logical volume
                      logicalTankAir->GetName(),        // the name
                      logicalTank,                      // the mother volume
                      false,                            // no boolean ops
                      0,                                // copy number
                      overlapChecking);                 // check for overlaps



    G4Color
      green(0.0,1.0,0.0),
      blue(0.0,0.0,1.0),
      brown(0.4,0.4,0.1),
      red(1.0,0.0,0.0);

    logicalTankAir -> SetVisAttributes(new G4VisAttributes(green));
    
    // Fill remaining area with water  
    const G4double tankH2OH = (tankH - tankAirH);
    G4Tubs* const solidTankH2O
        = new G4Tubs("Tank_H2O",                        // the name
                     0.0,                               // inner radius
                     tankOR - tankWallThickness,        // outer radius
                     tankH2OH * 0.5,                    // half height
                     0.0 * deg,                         // start angle
                     360.0 * deg);                      // end angle
    G4LogicalVolume* const logicalTankH2O
        = new G4LogicalVolume(solidTankH2O,             // the solid volume
                              fWater,                // the material
                              solidTankH2O->GetName(),0); // the name
    // Shift up so that the top of the water is at the bottom of the air
    // H: 62 - 16 = 46 cm
    const G4double centerOfH2O = (tankH - tankH2OH) * 0.5 - tankAirH;
    
    position.set(0.0, 0.0, centerOfH2O);
    new G4PVPlacement(NULL,                             // no rotation
                      position,                         // shift to origin
                      logicalTankH2O,                   // the logical volume
                      logicalTankH2O->GetName(),        // the name
                      logicalTank,                      // the mother volume
                      false,                            // no boolean ops
                      0,                                // copy number
                      overlapChecking);                 // check for overlaps


     
    //
    // Neutron Source
    //
    // TODO create the AmBe material in DefineMaterials() and use it here
    //      For now steel is used, but the logical volume is used in the
    //      PrimaryGeneratorAction to know where to emit the neutrons from
    const G4double sourceH = 2 * cm;
    const G4double sourceR = 1 * cm;
    G4Tubs* const solidSource
        = new G4Tubs("NeutronSource",                   // the name
                     0.0,                               // inner radius
                     sourceR,                           // outer radius
                     sourceH * 0.5,                     // half height
                     0.0 * deg,                         // start angle
                     360.0 * deg);                      // end angle
    G4LogicalVolume* const logicalSource
        = new G4LogicalVolume(solidSource,              // the solid volume
                              fAir,          // the material
                              solidSource->GetName());  // the name
    // La fuente esta a 100cm del tanque y a 20 cm por encima de la capa de aire
    position.set(tankOR + 100*cm , 0.0, - worldHeight + (floorH + 20*cm));
    new G4PVPlacement(NULL,                             // no rotation
                      position,                         // shift to origin
                      logicalSource,                    // the logical volume
                      logicalSource->GetName(),         // the name
                      logicalWorld,                   // the mother volume
                      false,                            // no boolean ops
                      0,                                // copy number
                      overlapChecking);                 // check for overlaps
  //always return the root volume
  //
  return physicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
