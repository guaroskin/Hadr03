#ifndef LAGO_G4_Detectodparameters_h
#define LAGO_G4_Detectodparameters_h 1
#include "LAGO_G4_DetectorMessenger.hh"
#include "globals.hh"

class LAGO_G4_DetectorParameters
{
  public:
    LAGO_G4_DetectorParameters();
    ~LAGO_G4_DetectorParameters();


    void SetWCDRadius(G4double dparam) {fWCD_r=dparam;} 	
    void SetWCDHeight(G4double dparam) {fWCD_z=dparam;} 	    
    void SetTyvekThickness(G4double dparam) {fTYV_t=dparam;} 
    
    void SetPMTRadius(G4double dparam) {fPMT_r=dparam;}
    void SetPMTThicknes(G4double dparam) {fPMT_t=dparam;}
    void SetPMTCathThic(G4double dparam) {fPMT_cath_t=dparam;}
    void SetPMTOffset(G4double dparam) {fPMT_offset=dparam;}

    G4double GetWcdR(){return fWCD_r;}    
    G4double GetWcdZ(){return fWCD_z;}
    G4double GetTyvekThickness(){return fTYV_t;}
    
    G4double GetPMTRadius(){return fPMT_r;} 
    G4double GetPMTThicknes(){return fPMT_t;} 
    G4double GetPMTCathThic(){return fPMT_cath_t;} 
    G4double GetPMTOffset(){return fPMT_offset;}  

  //Functiosn to modify optical properties	

    void SetTyvekRefl(G4double dparam) {fTYV_Refl=dparam;}
    void SetTyvekSigAlp(G4double dparam) {fTYV_SigAlp=dparam;}
    void SetTyvekSpecLob(G4double dparam) {fTYV_SpecLob=dparam;}
	    
    void SetWaterAbw(G4double dparam) {fWAT_Abw=dparam;}
    void SetWaterGac(G4double dparam) {fWAT_Gac=dparam;}
    
	
    G4double GetTyvekRefl(){return fTYV_Refl;}        
    G4double GetTyvekSigAlp(){return fTYV_SigAlp;}    
    G4double GetTyvekSpecLob(){return fTYV_SpecLob;}    
    
    G4double GetWaterAbw(){return fWAT_Abw;}
    G4double GetWaterGac(){return fWAT_Gac;} 

  private:

  // The messenger
  LAGO_G4_DetectorMessenger* DetectorMessenger;


  // The parameters that need to be set before LAGO_G4_DetectorConstruction
  // is created

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

 
};

#endif







