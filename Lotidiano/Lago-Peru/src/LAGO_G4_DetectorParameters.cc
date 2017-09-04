// Detector parameters

#include "LAGO_G4_DetectorParameters.hh"
#include "LAGO_G4_DetectorMessenger.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
LAGO_G4_DetectorParameters::LAGO_G4_DetectorParameters()
{

 DetectorMessenger = new LAGO_G4_DetectorMessenger(this);
 // Default values
  fWCD_r 	= 40.*cm;
  fWCD_z 	= 40.*cm;
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

LAGO_G4_DetectorParameters::~LAGO_G4_DetectorParameters()
{
  delete DetectorMessenger;
  DetectorMessenger = 0;
}

