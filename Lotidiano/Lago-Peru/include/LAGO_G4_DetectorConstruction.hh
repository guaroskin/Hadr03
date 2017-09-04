
#ifndef LAGO_G4_DetectorConstruction_H
#define LAGO_G4_DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class LAGO_G4_MainVolume;
class G4Sphere;
class LAGO_G4_DetectorParameters;

#include "G4Material.hh"
#include "LAGO_G4_DetectorParameters.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "LAGO_G4_PMTSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

class LAGO_G4_DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

   //LAGO_G4_DetectorConstruction();
    LAGO_G4_DetectorConstruction(LAGO_G4_DetectorParameters* fDetectorPars);
    virtual ~LAGO_G4_DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
/*
    //Functions to modify the geometry

    void SetWCDRadius(G4double ); 	
    void SetWCDHeight(G4double ); 	    
    void SetTyvekThickness(G4double ); 
    
    void SetPMTRadius(G4double );
    void SetPMTThicknes(G4double );
    void SetPMTCathThic(G4double );
    void SetPMTOffset(G4double );
*/
    G4double GetWcdR(){return fWCD_r;}    
    G4double GetWcdZ(){return fWCD_z;}
    G4double GetTyvekThickness(){return fTYV_t;}
    
    G4double GetPMTRadius(){return fPMT_r;} 
    G4double GetPMTThicknes(){return fPMT_t;} 
    G4double GetPMTCathThic(){return fPMT_cath_t;} 
    G4double GetPMTOffset(){return fPMT_offset;}    
    
/*   	
    //Functiosn to modify optical properties	

    void SetTyvekRefl(G4double );
    void SetTyvekSigAlp(G4double );
    void SetTyvekSpecLob(G4double );
	    
    void SetWaterAbw(G4double );
    void SetWaterGac(G4double );
    
*/	
    G4double GetTyvekRefl(){return fTYV_Refl;}        
    G4double GetTyvekSigAlp(){return fTYV_SigAlp;}    
    G4double GetTyvekSpecLob(){return fTYV_SpecLob;}    
    
    G4double GetWaterAbw(){return fWAT_Abw;}
    G4double GetWaterGac(){return fWAT_Gac;} 

     void SetDefaults();
     void SetParameters();

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    LAGO_G4_DetectorMessenger* fDetectorMessenger;
    LAGO_G4_DetectorParameters* LAGO_G4_DetectorPars;
  
    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    //Materials & Elements
//    G4Material* fLAGO_G4_;
    G4Material* fAl;
    G4Material* fWater;
    G4Material* fTyvek;
      
    G4Element* fN;
    G4Element* fO;
    G4Material* fAir;
    G4Material* fVacuum;
    G4Element* fC;
    G4Element* fH;
    G4Material* fGlass;
    
    //Geometry wcd
    G4double fWCD_r;
    G4double fWCD_z;
    G4double fTYV_t;  
    
    G4double fPMT_r;
    G4double fPMT_t;
    G4double fPMT_cath_t;
    G4double fPMT_offset;
     //Optical Properties
    
    G4double fTYV_Refl;        
    G4double fTYV_SigAlp;    
    G4double fTYV_SpecLob;
  
    G4double fWAT_Abw;
    G4double fWAT_Gac; 
   
    //Geometry
    
    LAGO_G4_MainVolume* fMainVolume;

    G4MaterialPropertiesTable* fWater_mt;

    //Sensitive Detectors   
    G4Cache<LAGO_G4_PMTSD*> fPmt_SD;

};

#endif
