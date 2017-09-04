#ifndef SteppingAction_h
#define SteppingAction_h 1


// Geant4 Libraries
//
#include "G4UserSteppingAction.hh"
#include "globals.hh"


// Local Libraries
//
#include "DetectorConstruction.hh"
#include "PMTSD.hh"


/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    const DetectorConstruction *detectorConstruction;
    G4VPhysicalVolume* fwaterVolume;
    G4int fScintillationCounter;
    G4int fCerenkovCounter;
    G4int lengthMaxOk;
    G4double priEner;
    G4int fEventNumber;
    PMTSD *pmtSD;
    G4int phoOnPmt;
    G4double colSP;
    G4double radSP;
};

#endif
