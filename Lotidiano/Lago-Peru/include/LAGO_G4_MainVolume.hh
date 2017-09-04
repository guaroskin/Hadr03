#ifndef LAGO_G4_MainVolume_H
#define LAGO_G4_MainVolume_H 1

#include "G4PVPlacement.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "LAGO_G4_DetectorConstruction.hh"

class LAGO_G4_MainVolume : public G4PVPlacement
{
  public:

    LAGO_G4_MainVolume(G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 LAGO_G4_DetectorConstruction* c);

    G4LogicalVolume* GetLogPhotoCath() {return fPhotocath_log;}

   std::vector<G4ThreeVector> GetPmtPositions() {return fPmtPositions;}

  private:

    void VisAttributes();
    void SurfaceProperties();

    /*void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot,
                   G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin,
                   G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k);
*/
    void CopyValues();

    LAGO_G4_DetectorConstruction* fConstructor;

    G4double fWCD_r;
    G4double fWCD_z;
    G4double fTYV_t;
    
    G4double fPMT_r;  
    G4double fPMT_t;
    G4double fPMT_cath_t;
    G4double fPMT_offset;  

    G4double fTYV_Refl;        
    G4double fTYV_SigAlp;    
    G4double fTYV_SpecLob;
  

    //Basic Volumes
    //
    G4Tubs* fTyvek_box;
   
    G4SubtractionSolid* fWater_box;
    G4IntersectionSolid* fPmt;
    G4IntersectionSolid* fPhotocath;
      

    // Logical volumes
    //
    G4LogicalVolume* fWater_log;
    G4LogicalVolume* fTyvek_log;
    G4LogicalVolume* fPmt_log;
    G4LogicalVolume* fPhotocath_log;


    // Sensitive Detectors positions
    std::vector<G4ThreeVector> fPmtPositions;

};

#endif
