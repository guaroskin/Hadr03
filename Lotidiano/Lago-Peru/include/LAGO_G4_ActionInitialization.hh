#ifndef LAGO_G4_ActionInitialization_h
#define LAGO_G4_ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class LAGO_G4_RecorderBase;
class LAGO_G4_HistoManager;
//class B4DetectorConstruction;

/// Action initialization class.
///

class LAGO_G4_ActionInitialization : public G4VUserActionInitialization
{
  public:
    LAGO_G4_ActionInitialization(LAGO_G4_RecorderBase*, LAGO_G4_HistoManager*);
    virtual ~LAGO_G4_ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

    //virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

  private:
    LAGO_G4_RecorderBase* fRecorder;
    LAGO_G4_HistoManager* fHisto;
};

#endif
