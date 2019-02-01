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
//#include "MyPMTSD.hh"


#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MultiFunctionalDetector.hh"

#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
//#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4VisAttributes.hh"    
#include "G4Colour.hh"             
#include "G4SystemOfUnits.hh"      

#include "G4VPhysicalVolume.hh"
#include "globals.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"

#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  :G4VUserDetectorConstruction(),
   Tank_log_vol(0), PMT_log_vol(0), Air_log_vol(0)
{
  
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ ;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // define a Material from isotopes
  //
  G4String name;
  G4String symbol;
  G4int z;  
  G4double density;
  G4int ncomponents, natoms;
  G4double fracMass; //,abundance;
   
  
  // ----------------
  // *** Elements ***
  // ----------------
  
  G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1,  1.008 * (g/mole));
  G4Element* elO  = new G4Element(name="Oxygen",  symbol="O" , z=8,  16.00 * (g/mole));
  G4Element* elC  = new G4Element(name="Carbon" , symbol="C" , z=6,  12.01 * (g/mole));
  G4Element* elNa = new G4Element(name="Sodium",  symbol="Na", z=11, 22.98 * (g/mole));
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14, 28.01 * (g/mole));
  G4Element* elCl = new G4Element(name="Chlorine",symbol="Cl", z=17, 35.45 * (g/mole));

  

  // -----------------
  // *** Molecules ***
  // -----------------

  H2O = new G4Material("fWater", density =1.000 * (g/cm3), ncomponents=2);
  H2O->AddElement(elH, natoms=2);
  H2O->AddElement(elO, natoms=1);

  G4Material* SiO2 =  new G4Material("fSand", density =1.730 * (g/cm3), ncomponents=2);
  // http://www.engineeringtoolbox.com/density-materials-d_1652.html
  SiO2->AddElement(elSi, natoms=1);
  SiO2->AddElement(elO, natoms=2);
  
  G4Material*NaCl = new G4Material("fSalt", density =2.165 * (g/cm3), ncomponents=2);
  NaCl->AddElement(elCl, natoms=1);
  NaCl->AddElement(elNa, natoms=1);


  
  // ----------------
  // *** NIST ***
  // ----------------
  
  G4NistManager* nist = G4NistManager::Instance();
  fAir = nist->FindOrBuildMaterial("G4_AIR");
  fVoid = nist->FindOrBuildMaterial("G4_Galactic");
  fParaffin = nist->FindOrBuildMaterial("G4_PARAFFIN");
  fStainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  //fWater = nist->FindOrBuildMaterial("G4_WATER");
  fGlass  = nist->FindOrBuildMaterial("G4_Pyrex_Glass");


  
  // -----------------
  // *** Materials ***
  // -----------------
  
  fSoil = new G4Material("fSoil", density = 1.6 * (g/cm3), ncomponents=3, kStateSolid);
  //http://www.agriinfo.in/?page=topic&superid=4&topicid=271
  fSoil->AddMaterial(SiO2,fracMass=50.0*perCent);
  fSoil->AddMaterial(fAir,fracMass=25.0*perCent);
  fSoil->AddMaterial(H2O,fracMass=25.0*perCent);

  
  // Salt    =  3       25      50      100   [kg]
  // density = 1.003 - 1.025 - 1.05  - 1.10  [g/cm3]
  // fracMass= 99.40 - 97.56 - 95.24 - 90.91 [%]
  
  SaltWater = new G4Material("Salt_water", density = 1.1 * (g/cm3), ncomponents=2);
  SaltWater->AddMaterial(H2O,fracMass=90.91*perCent);
  SaltWater->AddMaterial(NaCl,fracMass=9.09*perCent);	


  
  // ---------------------------------
  // *** Water Optical Propierties ***
  // ---------------------------------
  G4double photonEnergy[] =
  { 
    2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV 
  };
  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);
  

  G4double refractiveIndex1[] =
  { 
    1.3435, 1.344,  1.3445, 1.345,  1.3455,
    1.346,  1.3465, 1.347,  1.3475, 1.348,
    1.3485, 1.3492, 1.35,   1.3505, 1.351,
    1.3518, 1.3522, 1.3530, 1.3535, 1.354,
    1.3545, 1.355,  1.3555, 1.356,  1.3568,
    1.3572, 1.358,  1.3585, 1.359,  1.3595,
    1.36,   1.3608
  };
  assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

  G4double absorption[] =
  {
    3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
    15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
    45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
    52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
    30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
    17.500*m, 14.500*m 
  };
  assert(sizeof(absorption) == sizeof(photonEnergy));


  G4MaterialPropertiesTable* myMPT1 
    = new G4MaterialPropertiesTable();

  myMPT1
    ->AddProperty("RINDEX", photonEnergy, refractiveIndex1, nEntries)
    ->SetSpline(true);

  myMPT1
    ->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries)
    ->SetSpline(true);

  //*
  G4double energy_water[] = 
  {
    1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
    1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
    1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
    1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
    1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
    2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
    2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
    2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
    2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
    2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
    3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
    3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
    3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
    4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
    5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };
  const G4int numentries_water 
    = sizeof(energy_water)/sizeof(G4double);


  
  //assume 100 times larger than the rayleigh scattering for now.
  G4double mie_water[] = 
  {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };
  assert(sizeof(mie_water) == sizeof(energy_water));
  
  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3]={0.99,0.99,0.8};

  myMPT1
    ->AddProperty("MIEHG",energy_water,mie_water,numentries_water)
    ->SetSpline(true);
  myMPT1
    ->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  myMPT1
    ->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  myMPT1
    ->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);
  //*/

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  myMPT1->DumpTable();

  SaltWater->SetMaterialPropertiesTable(myMPT1);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* elCd = nist->FindOrBuildMaterial("G4_Cd");
  G4Material* elPb = nist->FindOrBuildMaterial("G4_Pb");
  G4bool const overlapChecking = true;
  
  // --------------------
  // *** world ***
  // --------------------
  
  const G4double WorldHalfSize = 200.0 * cm;       // L = 400 cm
  const G4double WorldHalfHeight = 150.0 * cm;     // H = 300 cm
  
  G4ThreeVector position;
  
  
  G4Box* const solidWorld = new G4Box("World",        // the name
				      WorldHalfSize,      // X:(-200 , 200)
				      WorldHalfSize,      // Y:(-200 , 200)
				      WorldHalfHeight);   // Z:(-150 , 150)

  
  //Puede ser creado en DetectorConstruction.hh
  G4LogicalVolume* const logicalWorld
    = new G4LogicalVolume(solidWorld,               // the solid volume
			  fAir,                     // the material
			  solidWorld->GetName());   // the name
  
  position.set(0.0, 0.0, 0.0);
  // Centrado para que el fondo quede en (Z = -150 cm) =>  Z:(-150 , 150)
  
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


  // --------------------
  //    *** Floor ***
  // --------------------
  
  const G4double floorH = 50.0 * cm;
  
  //const G4ThreeVector floorPosition(0.0, 0.0, -floorH * 0.5);
  G4Box* const solidFloor = new G4Box("Floor",         // the name
				      WorldHalfSize,   // X:(-200 , 200)
				      WorldHalfSize,   // Y:(-200 , 200)
				      floorH * 0.5);   // Z:(-150 ,-100)
  
  G4LogicalVolume* const logicalFloor
    = new G4LogicalVolume(solidFloor,               // the solid volume
			  fSoil,                // the material
			  solidFloor->GetName());   // the name
  
  // La base Floor está en el fondo del World Z =(-150 + 25 cm) = -125 cm +/- 25 => Z:(-150 , -100)
  position.set(0.0, 0.0, -WorldHalfHeight + floorH*0.5); 
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // Z:(-100 , 0)
		    logicalFloor,                     // the logical volume
		    logicalFloor->GetName(),          // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps

  Air_log_vol = logicalWorld;
  // --------------------
  // *** WCD Detector ***
  // --------------------
  
  // Create the tank
  // 
  const G4double tankWallThickness = 0.5* mm;
  const G4double TankOutRadius = 48.0 * cm;      // 96 cm de diametro
  // Tanque pequeno: Tanque Acero Inoxidable 500 Lts 
  //const G4double TankHeight = 62.0 * cm;
  //Tanque grande: Tanque Acero Inoxidable 1000 Lts 
  const G4double TankHeight = 133.0 * cm;
  const G4double Platform = 46.0 * cm;
  
  G4Tubs* const solidTank
    = new G4Tubs("Tank_Wall",                       // the name
		 0.*cm,                             // inner radius
		 TankOutRadius, // outer radius
		 TankHeight*0.5,// half height
		 0.0 * deg,                         // start angle
		 360.0 * deg);                      // end angle

  G4LogicalVolume* const logicalTank
    = new G4LogicalVolume(solidTank,                // the solid volume
			  fStainlessSteel,          // the material
			  solidTank->GetName());    // the name

  // La base SolidTank está en Z =-150 + 50 + 46 + 133/2) =  12.5 cm +/- 66.5 => Z:(-54,79)
  // La base SolidTank está en Z =-150 + 50 + 46 + 62/2)  = -23.0 cm +/- 31 => Z:(-54,8)
  G4double halfZ_det = - WorldHalfHeight + floorH + Platform + (TankHeight * 0.5);
  position.set(0.0, 0.0, halfZ_det);
  
  G4VPhysicalVolume* phys_Tank
    = new G4PVPlacement(NULL,                         // no rotation
		    position,                         // Z:(0 , 62)
		    logicalTank,                      // the logical volume
		    logicalTank->GetName(),           // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps
  

  
  // Fill tank area with water. llena el volumen dentro del tanque de acero
  G4Tubs* const solidTankH2O
    = new G4Tubs("Tank_H2O",                        // the name
		 0.*cm,                             // inner radius
		 TankOutRadius  - tankWallThickness,// outer radius
		 TankHeight*0.5 - tankWallThickness,// half height
		 0.0 * deg,                         // start angle
		 360.0 * deg);                      // end angle
  
  G4LogicalVolume* const logicalTankH2O
    = new G4LogicalVolume(solidTankH2O,  // the solid volume
			  //fAir,                     // the material
			  SaltWater,
			  solidTankH2O->GetName(),0); // the name

  position.set(0.0, 0.0, 0.0);
  
  G4VPhysicalVolume* phys_TankH2O
    = new G4PVPlacement(NULL,                             // no rotation
		    position,                         // shift to origin
		    logicalTankH2O,                   // the logical volume
		    logicalTankH2O->GetName(),        // the name
		    logicalTank,                      // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps
  
  Tank_log_vol = logicalTankH2O;

  // -------------------------- 
  // *** Optical Surfaces ***
  // --------------------------

  // *** For WCD ***
  //

  
  G4OpticalSurface* opWaterSurface 
    = new G4OpticalSurface("Tyvek");

  opWaterSurface->SetType(dielectric_LUT);
  opWaterSurface->SetFinish(groundtyvekair);
  opWaterSurface->SetModel(LUT);
  
  new G4LogicalBorderSurface("WaterSurface",
      phys_TankH2O,
      phys_Tank,
      opWaterSurface
      );


  /*NO
  G4LogicalSkinSurface* airSurface =
    new G4LogicalSkinSurface("Tyvek", logicalTankH2O, opWaterSurface);
  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
    (airSurface->GetSurface(logicalTankH2O)->GetSurfaceProperty());
  if (opticalSurface) opticalSurface->DumpInfo();
  */


  
  // --------------------
  //   *** Plomo ***
  // --------------------

  const G4double Plomo_x = 10.0 * cm;
  const G4double Plomo_y = 60.0 * cm;
  const G4double Plomo_z = 35.0 * cm;


  G4Box* const Sol_Plomo = new G4Box("Plomo",   // the name
				     Plomo_x/2.,   // X:(-200 , 200)
				     Plomo_y/2.,   // Y:(-200 , 200)
				     Plomo_z/2.);  // Z:(-150 ,-100)
  
  G4LogicalVolume* const logicalPlomo
    = new G4LogicalVolume(Sol_Plomo,               // the solid volume
			  elPb,                    // the material
			  Sol_Plomo->GetName());   // the name

  //Tiene que estar a la mitad de la altura del tanque
  // La base está en Z = -150 + 50 + 46 + 62/2)  = -23.0 cm +/- 35/2 => Z:()
  // La base está en Z = -150 + 50 + 46 + 133/2) =  12.5 cm +/- 35/2 => Z:()
  // La base está en X = -48 - 10/2) =  -53.0 cm +/- 5 => X:(-58 , -48)
  G4double Pb_posX = -TankOutRadius - Plomo_x;     //-48-10 = -58
  position.set(Pb_posX + Plomo_x/2, 0.0, halfZ_det);
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // Z:(-100 , 0)
		    logicalPlomo,                     // the logical volume
		    logicalPlomo->GetName(),          // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps

  /*
  // --------------------
  //   *** Cadmio ***
  // --------------------

  const G4double Cadmio_x = 1.35 * mm;
  const G4double Cadmio_y = 47.0 * cm;
  const G4double Cadmio_z = 19.0 * cm;
  
  G4Box* const Sol_Cadmio = new G4Box("Cadmio",   // the name
				      Cadmio_x/2.,   // X:(-200 , 200)
				      Cadmio_y/2.,   // Y:(-200 , 200)
				      Cadmio_z/2.);  // Z:(-150 ,-100)
  
  G4LogicalVolume* const logicalCadmio
    = new G4LogicalVolume(Sol_Cadmio,               // the solid volume
			  elCd,                // the material
			  Sol_Cadmio->GetName());   // the name
  
  //Tiene que estar a la mitad de la altura del tanque
  // La base está en Z = -150 + 50 + 46 + 62/2)  = -23.0 cm +/- 39/2 => Z:(-42.5,-3.5)
  // La base está en Z = -150 + 50 + 46 + 133/2) =  12.5 cm +/- 39/2 => Z:(-7,32)
  G4double Cadmio_posX = Pb_posX - Cadmio_x;     //-69.6 -0.135/2
  position.set(Cadmio_posX + Cadmio_x/2, 0.0, halfZ_det);
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // Z:(-100 , 0)
		    logicalCadmio,                     // the logical volume
		    logicalCadmio->GetName(),          // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps
  
  //G4double Cadmio_posX = Pb_posX - Cadmio_x;     //-69.6 -0.135/2
  // --------------------
  //   *** Parafina ***
  // --------------------

  const G4double Parafina_x = 11.6 * cm;
  const G4double Parafina_y = 47.0 * cm;
  const G4double Parafina_z = 39.0 * cm;
  
  G4Box* const Sol_parafina = new G4Box("Parafina",   // the name
					Parafina_x/2.,   // X:(-200 , 200)
					Parafina_y/2.,   // Y:(-200 , 200)
					Parafina_z/2.);  // Z:(-150 ,-100)
  
  G4LogicalVolume* const logicalParafina
    = new G4LogicalVolume(Sol_parafina,               // the solid volume
			  fParaffin,                // the material
			  Sol_parafina->GetName());   // the name

  //Tiene que estar a la mitad de la altura del tanque
  // La base está en Z = -150 + 50 + 46 + 62/2)  = -23.0 cm +/- 39/2 => Z:(-42.5,-3.5)
  // La base está en Z = -150 + 50 + 46 + 133/2) =  12.5 cm +/- 39/2 => Z:(-7,32)
  // La base está en X = -48 - 10 -11.6/2) =  -53.0 cm +/- 5 => X:(-58 , -48)
  G4double Para_posX = Cadmio_posX - Parafina_x;     //-58 - 11.6 = -69.6
  position.set(Para_posX + Parafina_x/2, 0.0, halfZ_det);
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // Z:(-100 , 0)
		    logicalParafina,                     // the logical volume
		    logicalParafina->GetName(),          // the name
		    logicalWorld,                     // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps

  

  
  /*
  const G4double sourceH = 20 * cm;    //
  const G4double sourceR = 5 * cm;     //
  
  G4Tubs* const solidSource
    = new G4Tubs("NeutronSource",                   // the name
		 0.0,                               // inner radius
		 sourceR,                           // outer radius
		 sourceH * 0.5,                     // half height
		 0.0 * deg,                         // start angle
		 360.0 * deg);                      // end angle
  
  G4LogicalVolume* const logicalSource
    = new G4LogicalVolume(solidSource,              // the solid volume
			  //fParaffin,              // the material
			  fAir,                     // Cuando se usa la fuente desnuda
			  solidSource->GetName());  // the name
  
  // La fuente esta a 100cm del tanque y a la misma altura que el tanque
  const G4double dist_SD = 30*cm;         // Distancia fuente - detector
  // X = -(48.0 + 100) = -148.5 cm
  position.set(-(TankOutRadius + dist_SD), 0.0, halfZ_det); 
  new G4PVPlacement(NULL,                             // no rotation
		    position,                         // shift to origin
		    logicalSource,                    // the logical volume
		    logicalSource->GetName(),         // the name
		    logicalWorld,                   // the mother volume
		    false,                            // no boolean ops
		    0,                                // copy number
		    overlapChecking);                 // check for overlaps
  */
  
  // -----------
  // *** PMT ***
  // -----------
  
  G4double pxSemiAxis = 10.16*cm;
  G4double pySemiAxis = 10.16*cm;
  G4double pzSemiAxis = 5.0*cm;
  G4double pzBottomCut = -pzSemiAxis;
  G4double pzTopCut = 0.0*cm;
  
  Pmt 
    = new G4Ellipsoid("PMTdet",
		      pxSemiAxis,
		      pySemiAxis,
		      pzSemiAxis,
		      pzBottomCut,
		      pzTopCut
		      );

  
  G4LogicalVolume* const Pmt_log
    = new G4LogicalVolume(Pmt,
			  fAir,       //glass,
			  "pmt_log"
			  );


    
  // El PMT esta dentro del agua (que es el volumen madre) 
  // halfZ = 65*0.4 = 24.8 --> Z = halfZ +/- 5 = [44.6 , 54.6] no estoy seguro de este calculo  
  position.set(0., 0., TankHeight*0.4);
  
  new G4PVPlacement(0,               //no rotation
		    position,        //at 
		    Pmt_log,         //its logical volume
		    "PMTdet",        //its name
		    logicalTankH2O,  //its mother  volume
		    false,           //no boolean operation
		    0,              //copy number
		    overlapChecking);


  PMT_log_vol = Pmt_log;
  
  G4Color
    green(0.,1.,0.),
    blue(0.,0.,1.),
    brown(0.4,0.4,0.1),
    red(1.,0.,0.),
    grey(0.,0.2,0.37);
  
  logicalFloor -> SetVisAttributes(new G4VisAttributes(brown));
  Pmt_log -> SetVisAttributes(new G4VisAttributes(red));
  logicalTankH2O -> SetVisAttributes(new G4VisAttributes(blue));
  //logicalPlomo -> SetVisAttributes(new G4VisAttributes(grey));
  return physicalWorld;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
