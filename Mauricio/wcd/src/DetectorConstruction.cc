
// Geant4 Libraries
//
#include "G4Material.hh"
#include "G4Element.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


// Local Libraries
//
//#include "PMTSD.hh"
#include "DetectorConstruction.hh"
#include "PMTSD.hh"

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  G4cout << "...DetectorConstruction..." << G4endl;
  
  fExpHall_x = fExpHall_y = fExpHall_z = 120.*cm;
  fTank_x    = fTank_y    = fTank_z    =  60.0*cm;
  
  O = H = NULL;
  water = glass = alum = NULL;
//    expHall_mat = glass = alum = NULL;

  expHall_box = NULL;
  wcd_tank = wcd_thickness = NULL;
  Pmt = NULL;

  wcd_tank_log = Pmt_log = NULL;  
}


DetectorConstruction::~DetectorConstruction()
{}


// ****************************
// Doing Elements and Materials
// ****************************

void DetectorConstruction::DefineMaterials()
{

  // ----------------
  // *** Elements ***
  // ----------------
  G4double a, z, density;

  //N = new G4Element("Nitrogen", "N", z = 7 , a = 14.01*g/mole);
  O = new G4Element("Oxygen"  , "O", z = 8 , a = 16.00*g/mole);
  H = new G4Element("Hydrogen", "H", z=1 , a = 1.01*g/mole);


  // -----------------
  // *** Materials ***
  // -----------------
  G4NistManager* nist = G4NistManager::Instance();
  

  // *** Experimental Hall ***
  //  
  expHall_mat
    = nist->FindOrBuildMaterial("G4_AIR");

  // *** Water ***
  // 
  water 
    = new G4Material("Water", density= 1.0*g/cm3, 2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  // *** Glass ***
  //
  glass 
    = nist->FindOrBuildMaterial("G4_Pyrex_Glass");

  // *** Aluminum ***
  //
  alum
    = nist->FindOrBuildMaterial("G4_Al");


  // -------------------------
  // *** Water Propierties ***
  // -------------------------
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
    ->AddProperty("RINDEX", 
        photonEnergy, refractiveIndex1, nEntries)
    ->SetSpline(true);

  myMPT1
    ->AddProperty("ABSLENGTH", photonEnergy, 
        absorption, nEntries)
    ->SetSpline(true);


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

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  myMPT1->DumpTable();

  water->SetMaterialPropertiesTable(myMPT1);
}
  

// *************************
// Doing Mechanical Detector
// ************************* 

G4VPhysicalVolume* DetectorConstruction::Construct()
{                        

  // ----------------------
  // *** Make Materials ***
  // ----------------------
  DefineMaterials();

  // -------------------------
  // *** Experimental Hall ***
  // -------------------------
  G4bool checkOverlaps = true;

  expHall_box 
    = new G4Box("expHall_box",
        fExpHall_x,
        fExpHall_y,
        fExpHall_z
        );

  expHall_log
    = new G4LogicalVolume(expHall_box,
        expHall_mat,
        "expHall_log",
        0,
        0,
        0
        );

  G4VPhysicalVolume* expHall_phys
    = new G4PVPlacement(0,
        G4ThreeVector(),
        expHall_log,
        "expHall",
        0,
        false,
        0,
        checkOverlaps
        );


  // --------------------
  // *** WCD Detector ***
  // --------------------

  // WCD Tank
  //
  G4double thick = 10.*mm;

  wcd_thickness
    = new G4Tubs("wcdThickness",
        0.*cm,
        51.5*cm + thick,
        29.5*cm + thick/2,
        0.*deg,
        360.*deg
        
/*    = new G4Box("wcdThickness",
      fTank_x + thick,
        fTank_y + thick,
        fTank_z + thick*/
        );

  wcd_thickness_log
    = new G4LogicalVolume(wcd_thickness,
        expHall_mat,
        "wcdThickness",
        0,
        0,
        0
        );
  
  G4VPhysicalVolume* wcd_thickness_phys
    = new G4PVPlacement(0,
        G4ThreeVector(),
        wcd_thickness_log,
        "wcdThickness",
        expHall_log,
        false,
        0
        );

  wcd_tank 
    = new G4Tubs("Tank",
        0.*cm,
        51.5*cm,
        29.5*cm, 
        0.*deg,
        360.*deg

    /*= new G4Box("Tank",
        fTank_x,
        fTank_y,
        fTank_z*/
        );

  wcd_tank_log
    = new G4LogicalVolume(wcd_tank,
        water,
        "Tank",
        0,
        0,
        0
        );

  G4VPhysicalVolume* wcd_tank_phys
    = new G4PVPlacement(0,
        G4ThreeVector(),
        wcd_tank_log,
        "Tank",
        wcd_thickness_log, //expHall_log,
        false,
        0
        );  

  fwaterVolume = wcd_tank_phys;

  
  // -------------------------- 
  // *** Optical Surfaces ***
  // --------------------------

  // *** For WCD ***
  //
  G4OpticalSurface* opWaterSurface 
    = new G4OpticalSurface("WaterSurface");

  opWaterSurface->SetType(dielectric_LUT);
  opWaterSurface->SetFinish(groundtyvekair);
  opWaterSurface->SetModel(LUT);

  new G4LogicalBorderSurface("WaterSurface",
      wcd_tank_phys,
      wcd_thickness_phys, //expHall_phys, //wcd_thickness_phys,
      opWaterSurface
      );
   
  // ----------------------------------------
  // Generate & Add Material Properties Table 
  // attached to the optical surfaces
  // ----------------------------------------
/*  const G4int num = 2;
  G4double ephoton[num] = {2.034*eV, 4.136*eV};

  // *** OpticalWaterSurface ***
  //
  G4double refractiveIndex[num] = {1.35, 1.40};
  G4double specularLobe[num]    = {0.3, 0.3};
  G4double specularSpike[num]   = {0.2, 0.2};
  G4double backScatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* myST1 
    = new G4MaterialPropertiesTable();

  myST1
    ->AddProperty("RINDEX", ephoton, refractiveIndex, num);
  myST1
    ->AddProperty("SPECULARLOBECONSTANT", ephoton, 
        specularLobe, num);
  myST1
    ->AddProperty("SPECULARSPIKECONSTANT", ephoton, 
        specularSpike, num);
  myST1
    ->AddProperty("BACKSCATTERCONSTANT", ephoton, 
        backScatter, num);

  G4cout << "Water Surface G4MaterialPropertiesTable" << G4endl;
  myST1->DumpTable();

  opWaterSurface->SetMaterialPropertiesTable(myST1);
*/

  // -----------
  // *** PMT ***
  // -----------
/*  G4double innerRadius_pmt = 0.*cm;
  G4double outerRadius_pmt = 10.*cm;
  G4double height_pmt = 2.5*cm;//0.0635/2.*cm; 
  G4double startAngle_pmt = 0.*deg;
  G4double spanningAngle_pmt = 360.*deg;*/

  G4double pxSemiAxis = 10.1*cm;
  G4double pySemiAxis = 10.1*cm;
  G4double pzSemiAxis = 6.5*cm;
  G4double pzBottomCut = -pzSemiAxis;
  G4double pzTopCut = 0.*cm;


  Pmt 
    = new G4Ellipsoid("pmt_tube",
    pxSemiAxis,
    pySemiAxis,
    pzSemiAxis,
    pzBottomCut,
    pzTopCut
    );

/*  Pmt
    = new G4Tubs("pmt_tube",
        innerRadius_pmt,
        outerRadius_pmt,
        height_pmt,
        startAngle_pmt,
        spanningAngle_pmt
        );*/

  Pmt_log
    = new G4LogicalVolume(Pmt,
        expHall_mat,//glass,
        "pmt_log"
        );

  new G4PVPlacement(0,
      G4ThreeVector(0., 0., 29.5*cm),//295.*cm),//60.*cm),
      Pmt_log,
      "pmt",
      wcd_tank_log,
      false,
      0
      );  
     
  return expHall_phys;
}
