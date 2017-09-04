
#include "LAGO_G4_DetectorConstruction.hh"
#include "LAGO_G4_PMTSD.hh"
#include "LAGO_G4_DetectorMessenger.hh"
#include "LAGO_G4_DetectorParameters.hh"
#include "LAGO_G4_MainVolume.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


LAGO_G4_DetectorConstruction::LAGO_G4_DetectorConstruction(LAGO_G4_DetectorParameters* fDetectorPars):LAGO_G4_DetectorPars(fDetectorPars)
//LAGO_G4_DetectorConstruction::LAGO_G4_DetectorConstruction()
{
  
  fExperimentalHall_box = NULL;
  fExperimentalHall_log = NULL;
  fExperimentalHall_phys = NULL;

 // fLAGO_G4_ = fAl = fAir = fVacuum = fGlass = fWater = fTyvek = NULL;
  fAl = fAir = fVacuum = fGlass = fWater = fTyvek = NULL;
  fN = fO = fC = fH = NULL;

  //SetParameters(LAGO_G4_DetectorPars);
  //SetDefaults();
  
  //fDetectorMessenger = new LAGO_G4_DetectorMessenger(fDetectorPars);
  SetParameters();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_DetectorConstruction::~LAGO_G4_DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_DetectorConstruction::DefineMaterials(){
  //SetParameters(); 
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  G4int polyeth = 1;
  G4int nC_eth = 2*polyeth;
  G4int nH_eth = 4*polyeth;

  //***Elements
  fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
  fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
  fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
  fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);

  //***Materials
  
  //Aluminum
  fAl = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);
  //Vacuum
  fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
                          density=universe_mean_density,kStateGas,0.1*kelvin,
                          1.e-19*pascal);
  //Air
  fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
  fAir->AddElement(fN, 70*perCent);
  fAir->AddElement(fO, 30*perCent);
  //Glass
  fGlass = new G4Material("Glass", density=1.032*g/cm3,2);
  fGlass->AddElement(fC,91.533*perCent);
  fGlass->AddElement(fH,8.467*perCent);
  //water 
  fWater = new G4Material("Water", density= 1*g/cm3, 2);
  fWater->AddElement(fH, 2);
  fWater->AddElement(fO, 1);	
  //Tyvek(polyethylene)
  fTyvek = new G4Material("Tyvek", density=1000*kg/m3,2);
  fTyvek->AddElement(fH,nH_eth);
  fTyvek->AddElement(fC,nC_eth);
   
  //***Material properties tables

  G4double WCD_Energy[]    = {1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
				2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
				2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
				2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
				2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
				2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
				3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
				3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
				3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
				4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV};
  const G4int WCD_num = sizeof(WCD_Energy)/sizeof(G4double);

  // pmt glass 

  G4double RINDEX_glass[] =
      { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600, 1.600,
	1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,1.600, 1.600, 1.600 }; 
  assert(sizeof(RINDEX_glass) == sizeof(WCD_Energy));
  G4double ABSORPTION_glass[WCD_num]= 
     { 1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm, 
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm};  
  assert(sizeof(ABSORPTION_glass) == sizeof(WCD_Energy)); 
   
  G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH",WCD_Energy,ABSORPTION_glass,WCD_num);
  glass_mt->AddProperty("RINDEX",WCD_Energy,RINDEX_glass,WCD_num);
  fGlass->SetMaterialPropertiesTable(glass_mt);

  // Air an vavuum
  
  G4double vacuum_Energy[]={1.96825*eV,2.75555*eV,5.16665*eV};
  const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
  G4double vacuum_RIND[]={1.,1.,1.};
  assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
  fVacuum->SetMaterialPropertiesTable(vacuum_mt);
  fAir->SetMaterialPropertiesTable(vacuum_mt);//Give air the same rindex  


  //****Water
  
  G4double RINDEX_water[]  = {   1.3327, 1.33299, 1.33329, 1.33361,
				 1.33393, 1.33427, 1.33462, 1.33498,
				 1.33536, 1.33576, 1.33617, 1.3366,
				 1.33705, 1.33753, 1.33803, 1.33855,
				 1.33911, 1.3397, 1.34033, 1.341,
				 1.34172, 1.34248, 1.34331, 1.34419,
				 1.34515, 1.3462, 1.34733, 1.34858,
				 1.34994, 1.35145, 1.35312, 1.35498,
				 1.35707, 1.35943, 1.36211, 1.36518,
				 1.36872, 1.37287, 1.37776, 1.38362};

  assert(sizeof(RINDEX_water) == sizeof(WCD_Energy));
  // tuning water
  G4double WABST = fWAT_Abw;
  //WABST = GetWaterAbw();



  G4double ABSORPTION_water[]  = 
	{
	342.81*cm*WABST, 362.827*cm*WABST, 378.041*cm*WABST, 449.378*cm*WABST,
	739.434*cm*WABST, 1114.23*cm*WABST, 1435.56*cm*WABST, 1611.06*cm*WABST,
	1764.18*cm*WABST, 2100.95*cm*WABST, 2292.9*cm*WABST, 2431.33*cm*WABST, 
	3053.6*cm*WABST, 4838.23*cm*WABST, 6539.65*cm*WABST, 7682.63*cm*WABST, 
	9137.28*cm*WABST, 12220.9*cm*WABST, 15270.7*cm*WABST, 19051.5*cm*WABST,
	23671.3*cm*WABST, 29191.1*cm*WABST, 35567.9*cm*WABST, 42583*cm*WABST,
	49779.6*cm*WABST, 56465.3*cm*WABST, 61830*cm*WABST, 65174.6*cm*WABST,
	66143.7*cm*WABST, 64820*cm*WABST, 61635*cm*WABST, 57176.2*cm*WABST,
	52012.1*cm*WABST, 46595.7*cm*WABST, 41242.1*cm*WABST, 36146.3*cm*WABST,
	31415.4*cm*WABST, 27097.8*cm*WABST, 23205.7*cm*WABST, 19730.3*cm*WABST
	};
  
  assert(sizeof(ABSORPTION_water) == sizeof(WCD_Energy));

  G4double WGAMC = 1./fWAT_Gac;    //en mg/L
  //WGAMC = 1./GetWaterGac();

  G4double GAMIN_ABS_water[]  = 
        {
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     1.0e6*m,
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     1.0e6*m,
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     1.0e6*m,     
        1.0e6*m,		1.0e6*m,  	  1.0e6*m,	     1.0e6*m,
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     1.0e6*m,
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     1.0e6*m,     
        1.0e6*m,		1.0e6*m,	  1.0e6*m,	     7.99435*m*WGAMC,
        4.23653*m*WGAMC,	5.207785*m*WGAMC, 6.17904*m*WGAMC,   5.0122*m*WGAMC,
        3.6133*m*WGAMC,		2.2144*m*WGAMC,	  3.8949*m*WGAMC,    5.57546*m*WGAMC,     
        7.25641*m*WGAMC,	4.0105*m*WGAMC,	  0.769022*m*WGAMC,  0.893308*m*WGAMC     
        };   
  assert(sizeof(GAMIN_ABS_water) == sizeof(WCD_Energy));
 
  G4double GAMIN_EMS_water[]  =
         {0.0,		0.0,		0.0,		0.0,		
	  0.0,	        0.0,		0.0,		0.0,
          0.0,		0.0,		0.0,		0.0,		
	  0.0,	        1.931,		3.2405,		4.550,
          5.365,	6.1812,		6.996,		7.8124,		
	  8.628,        7.268,		5.917,		4.576,
          3.217,	1.858,		0.499,		0.0,		
	  0.0,	        0.0,		0.0,		0.0,
    	  0.0,		0.0,		0.0,		0.0,		
	  0.0,	        0.0,		0.0,		0.0,
         };
  assert(sizeof(GAMIN_EMS_water) == sizeof(WCD_Energy));

  fWater_mt = new G4MaterialPropertiesTable();
  fWater_mt->AddProperty("RINDEX",        WCD_Energy, RINDEX_water,      WCD_num);
  fWater_mt->AddProperty("ABSLENGTH",     WCD_Energy, ABSORPTION_water,  WCD_num);
  fWater_mt->AddProperty("WLSABSLENGTH",  WCD_Energy, GAMIN_ABS_water,   WCD_num);
  fWater_mt->AddProperty("WLSCOMPONENT",  WCD_Energy, GAMIN_EMS_water,   WCD_num);
  fWater_mt->AddConstProperty("WLSTIMECONSTANT", 1*ns);

  fWater->SetMaterialPropertiesTable(fWater_mt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LAGO_G4_DetectorConstruction::Construct(){

  if (fExperimentalHall_phys) {
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
     G4LogicalSkinSurface::CleanSurfaceTable();
     G4LogicalBorderSurface::CleanSurfaceTable();
  }

  DefineMaterials();
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LAGO_G4_DetectorConstruction::ConstructDetector()
{
  //The experimental hall walls are all 1m away from housing walls
   
  G4cout << fWCD_r << G4endl;
  G4cout << fPMT_r << G4endl;
  //SetDefaults();
  G4double expHall_x = 4*fWCD_r;
  G4double expHall_y = 4*fWCD_r;
  G4double expHall_z = 4*fWCD_z;

  //Create experimental hall
  fExperimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box,
                                             fVacuum,"expHall_log",0,0,0);
  fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                              fExperimentalHall_log,"expHall",0,false,0);

  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);

  //Place the main volume
  fMainVolume
      = new LAGO_G4_MainVolume(fExperimentalHall_log,false,1,this);
  
  //ConstructSD(); 
  return fExperimentalHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_DetectorConstruction::ConstructSDandField() {
  if (!fMainVolume) return;
  // PMT SD

  if (!fPmt_SD.Get()) {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /LAGO_G4_Det/pmtSD" << G4endl;
    LAGO_G4_PMTSD* pmt_SD = new LAGO_G4_PMTSD("/LAGO_G4_Det/pmtSD");
    fPmt_SD.Put(pmt_SD);
    pmt_SD->InitPMTs(1); //let pmtSD know # of pmts
    pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());
  }

  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events

  SetSensitiveDetector(fMainVolume->GetLogPhotoCath(), fPmt_SD.Get());

  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

/*
void LAGO_G4_DetectorConstruction::SetTyvekThickness(G4double TYV_t){
  this->fTYV_t=TYV_t;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetWCDRadius(G4double WCD_r){
  this->fWCD_r=WCD_r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetWCDHeight(G4double WCD_z){
  this->fWCD_z=WCD_z;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

/////////////////////////////////
void LAGO_G4_DetectorConstruction::SetPMTRadius(G4double PMT_r){
  this->fPMT_r=PMT_r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetPMTThicknes(G4double PMT_t){
  this->fPMT_t=PMT_t;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetPMTCathThic(G4double PMT_cath_t){
  this->fPMT_cath_t=PMT_cath_t;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetPMTOffset(G4double PMT_offset){
  this->fPMT_offset=PMT_offset;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
// tunning WCD

void LAGO_G4_DetectorConstruction::SetTyvekRefl(G4double TYV_Refl){
  this->fTYV_Refl=TYV_Refl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetTyvekSigAlp(G4double TYV_SigAlp){
  this->fTYV_SigAlp=TYV_SigAlp;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetTyvekSpecLob(G4double TYV_SpecLob){
  this->fTYV_SpecLob=TYV_SpecLob;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//////////////////////////////////////////////////////////////////////

void LAGO_G4_DetectorConstruction::SetWaterAbw(G4double WAT_Abw){
  this->fWAT_Abw=WAT_Abw;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void LAGO_G4_DetectorConstruction::SetWaterGac(G4double WAT_Gac){
  this->fWAT_Gac=WAT_Gac;
  G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}
*/

 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_DetectorConstruction::SetDefaults() {

  //Resets to default values
  
  fMainVolume=NULL;

  fWCD_r 	= 40*cm;
  fWCD_z 	= 40*cm;
  fTYV_t        = .5*mm;   
  
  fPMT_r 	= 13.0*cm;
  fPMT_t        = 0.5*mm;
  fPMT_cath_t   = .25*mm;
  fPMT_offset   = 3.0*cm;
 
  fTYV_Refl	= 0.94 ;        
  fTYV_SigAlp	= .5;    
  fTYV_SpecLob	= .75;
  
  fWAT_Abw	= 1.0;
  fWAT_Gac	= 1.0e-6;    
}

void LAGO_G4_DetectorConstruction::SetParameters() {
  //fMainVolume=NULL;
  fWCD_r 	= LAGO_G4_DetectorPars->GetWcdR();
  fWCD_z 	= LAGO_G4_DetectorPars->GetWcdZ();
  fTYV_t        = LAGO_G4_DetectorPars->GetPMTThicknes();   
  
  fPMT_r 	= LAGO_G4_DetectorPars->GetPMTRadius();
  fPMT_t        = LAGO_G4_DetectorPars->GetPMTThicknes();
  fPMT_cath_t   = LAGO_G4_DetectorPars->GetPMTCathThic();
  fPMT_offset   = LAGO_G4_DetectorPars->GetPMTOffset();
 
  fTYV_Refl	= LAGO_G4_DetectorPars->GetTyvekRefl();        
  fTYV_SigAlp	= LAGO_G4_DetectorPars->GetTyvekSigAlp();    
  fTYV_SpecLob	= LAGO_G4_DetectorPars->GetTyvekSpecLob();
  
  fWAT_Abw	= LAGO_G4_DetectorPars->GetWaterAbw();
  fWAT_Gac	= LAGO_G4_DetectorPars->GetWaterGac();

  
  //G4RunManager::GetRunManager()->ReinitializeGeometry();
}



















