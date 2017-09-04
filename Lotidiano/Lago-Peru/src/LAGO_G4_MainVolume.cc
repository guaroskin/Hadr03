#include "globals.hh"

#include "LAGO_G4_MainVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_MainVolume::LAGO_G4_MainVolume(
                             G4LogicalVolume *pMotherLogical,
                             G4bool pMany,
                             G4int pCopyNo,
                             LAGO_G4_DetectorConstruction* c)
  //Pass info to the G4PVPlacement constructor
  :G4PVPlacement(0,G4ThreeVector(),
                 //Temp logical volume must be created here
                 new G4LogicalVolume(new G4Tubs("temp",0,1,1,0.,360.*deg),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
                 "tyvek",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
  CopyValues();

  G4double fTYV_r=fWCD_r+fTYV_t;
  G4double fTYV_z=fWCD_z+2*fTYV_t;
    
  //Create PMT Interior
  G4Sphere* tmpSolidInteriorWCPMT = new G4Sphere("tmpInteriorWCPMT", 0.0*m,fPMT_r,0.0*deg,360.0*deg, 0.0*deg,180.0*deg);
  G4Tubs*   tmpWater_box = new G4Tubs("water_tank_1",0.,fWCD_r,fWCD_z/2.,0.,360.*deg);
    
  G4RotationMatrix* Rot = new G4RotationMatrix;  // Rotates X and Z axes only
  Rot->rotateY(180*deg); 

  fWater_box = new G4SubtractionSolid("water_tank",tmpWater_box,tmpSolidInteriorWCPMT,Rot,G4ThreeVector(0.,0.,fWCD_z/2.+fPMT_offset));  
  fTyvek_box = new G4Tubs("tyvek_tank",0.,fTYV_r,fTYV_z/2.,0.,360.*deg);

  fTyvek_log = new G4LogicalVolume(fTyvek_box,G4Material::GetMaterial("Tyvek"),"tyvek_log",0,0,0);
  fWater_log = new G4LogicalVolume(fWater_box,G4Material::GetMaterial("Water"),"water_log",0,0,0); 

  //water inside tyvek bag
  new G4PVPlacement(0,G4ThreeVector(),fWater_log,"wcd",fTyvek_log,false,0);

  

   //****************** Build PMTs***********************************
   
  G4Sphere* tmpfPmt = new G4Sphere("tmpfPmt", fPMT_r-fPMT_t,fPMT_r,0.0*deg,360.0*deg, 0.0*deg,180.0*deg);
  G4Sphere* tmpfPhotocat = new G4Sphere("tmpfPhotocat", fPMT_r-fPMT_t,fPMT_r-fPMT_t+fPMT_cath_t,0.0*deg,360.0*deg, 0.0*deg,180.0*deg);

  fPmt = new G4IntersectionSolid("pmt_tube",tmpWater_box,tmpfPmt,Rot,G4ThreeVector(0.,0.,fWCD_z/2.+fPMT_offset));   
  fPhotocath = new G4IntersectionSolid("photocath_tube",tmpWater_box,tmpfPhotocat,Rot,G4ThreeVector(0.,0.,fWCD_z/2.+fPMT_offset));    
  
  //the "photocathode" is a metal slab at the back of the glass that
  //is only a very rough approximation of the real thing since it only
  //absorbs or detects the photons based on the efficiency set below
 
  fPmt_log = new G4LogicalVolume(fPmt,G4Material::GetMaterial("Glass"),"pmt_log");
  fPhotocath_log = new G4LogicalVolume(fPhotocath,G4Material::GetMaterial("Al"),"photocath_log");
 
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fPhotocath_log,"photocath",fPmt_log,false,0);
    
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fPmt_log,"pmt",fTyvek_log,false,pCopyNo);
  
  fPmtPositions.push_back(G4ThreeVector(0,0,fWCD_z/2.));
  
  VisAttributes();
  SurfaceProperties();

  SetLogicalVolume(fTyvek_log);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_MainVolume::CopyValues(){
  fWCD_r=fConstructor->GetWcdR();
  fWCD_z=fConstructor->GetWcdZ();

  fTYV_t=fConstructor->GetTyvekThickness();

  fPMT_r=fConstructor->GetPMTRadius();  
  fPMT_t=fConstructor->GetPMTThicknes(); 
  fPMT_cath_t=fConstructor->GetPMTCathThic();
  fPMT_offset=fConstructor->GetPMTOffset();

  fTYV_Refl=fConstructor->GetTyvekRefl();
  fTYV_SigAlp=fConstructor->GetTyvekSigAlp();
  fTYV_SpecLob=fConstructor->GetTyvekSpecLob();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_MainVolume::VisAttributes(){
  G4VisAttributes* tyvek_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  G4VisAttributes* water_va = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  fTyvek_log->SetVisAttributes(tyvek_va);
  fWater_log->SetVisAttributes(water_va);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_MainVolume::SurfaceProperties(){
  
const G4int num = 4;


G4double ephoton[num] = { 1.96825*eV, 2.63829*eV,  3.75757*eV, 5.16665*eV };

G4OpticalSurface* OpTyvekSurface =
   new G4OpticalSurface("TyvekSurface");

   OpTyvekSurface->SetType(dielectric_dielectric);
   OpTyvekSurface->SetFinish(groundbackpainted); //a guess, but seems to work   
   OpTyvekSurface->SetSigmaAlpha(fTYV_SigAlp); //cf. A. Chavarria's ~30deg 
   G4double TyRINDEX[num] ={1.5, 1.5,1.5,1.5}; // polyethylene permittivity is ~2.25
   G4double TySPECULARLOBECONSTANT[num] ={fTYV_SpecLob, fTYV_SpecLob, fTYV_SpecLob, fTYV_SpecLob};
   G4double TySPECULARSPIKECONSTANT[num] ={ 0.0, 0.0, 0.0, 0.0 };
   G4double TyBACKSCATTERCONSTANT[num] ={ 0.0, 0.0, 0.0, 0.0 };// Lambertian prob is therefore 0.25
   G4double TyREFLECTIVITY[num]={fTYV_Refl, fTYV_Refl,fTYV_Refl,fTYV_Refl};
   G4double TyEFFICIENCY[num]={0.0,0.0,0.0,0.0};

   G4MaterialPropertiesTable *TySurf_mt = new G4MaterialPropertiesTable();
   TySurf_mt->AddProperty("RINDEX", ephoton, TyRINDEX, num);
   TySurf_mt->AddProperty("SPECULARLOBECONSTANT", ephoton, TySPECULARLOBECONSTANT, num);
   TySurf_mt->AddProperty("SPECULARSPIKECONSTANT", ephoton, TySPECULARSPIKECONSTANT, num);
   TySurf_mt->AddProperty("BACKSCATTERCONSTANT", ephoton, TyBACKSCATTERCONSTANT, num);
   TySurf_mt->AddProperty("REFLECTIVITY", ephoton, TyREFLECTIVITY, num);
   TySurf_mt->AddProperty("EFFICIENCY", ephoton, TyEFFICIENCY, num);
   //use same efficiency as blacksheet, which is 0
   OpTyvekSurface->SetMaterialPropertiesTable(TySurf_mt);

   new G4LogicalSkinSurface("tyvek_surface",fTyvek_log,OpTyvekSurface);

 G4double photocath_ene[20] = {4.42801*eV, 4.13281*eV, 3.87451*eV, 3.64659*eV, 3.44401*eV,
 				3.26274*eV, 3.09960*eV, 2.95200*eV, 2.81782*eV, 2.69531*eV,
			 	2.58300*eV, 2.47968*eV, 2.38431*eV, 2.29600*eV, 2.21400*eV,
 				2.13766*eV, 2.06640*eV, 1.99975*eV, 1.93725*eV, 1.87855*eV};
    
  G4double photocath_QE[20] ={ 0.00, .0139, .0854, .169, .203,
			       .206, .211, .202,.188, .167,
			       .140, .116, .0806, .0432, .0265, 
			       .0146, .00756, .00508, .00158, 0.00};;
  
 
 // G4double photocath_EFF[num]={1.,1.,1.,1.}; //Enables 'detection' of photons
  G4double photocath_ReR[num]={1.92,1.92,1.92,1.92};
  G4double photocath_ImR[num]={1.69,1.69,1.69,1.69};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",photocath_ene,photocath_QE,20);
//  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);
////////////////////////////////////////////////////////////////////////////////////////////
  
  //**Create logical skin surfaces

  new G4LogicalSkinSurface("photocath_surf",fPhotocath_log,photocath_opsurf);

}
