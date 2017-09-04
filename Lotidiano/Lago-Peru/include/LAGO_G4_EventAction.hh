#ifndef LAGO_G4_EventAction_h
#define LAGO_G4_EventAction_h 1

#include "LAGO_G4_EventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class LAGO_G4_RecorderBase;

class LAGO_G4_EventAction : public G4UserEventAction
{
  public:

    LAGO_G4_EventAction(LAGO_G4_RecorderBase*);
      LAGO_G4_EventAction();
    virtual ~LAGO_G4_EventAction();

  public:

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetSaveThreshold(G4int );

 //   void SetEventVerbose(G4int v){fVerbose=v;}

    void SetPMTThreshold(G4int t){fPMTThreshold=t;}

    void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

  private:

    LAGO_G4_RecorderBase* fRecorder;
    LAGO_G4_EventMessenger* fEventMessenger;

    G4int              fSaveThreshold;

//    G4int              fScintCollID;
    G4int              fPMTCollID;

//    G4int              fVerbose;

    G4int              fPMTThreshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

};

#endif
