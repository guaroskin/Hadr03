#ifndef LAGO_G4_TrackingAction_h
#define LAGO_G4_TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class LAGO_G4_RecorderBase;

class LAGO_G4_TrackingAction : public G4UserTrackingAction {

  public:

    LAGO_G4_TrackingAction(LAGO_G4_RecorderBase*);
    LAGO_G4_TrackingAction();
    virtual ~LAGO_G4_TrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:

    LAGO_G4_RecorderBase* fRecorder;

};

#endif
