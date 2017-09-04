#ifndef LAGO_G4_StackingAction_H
#define LAGO_G4_StackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class LAGO_G4_StackingAction : public G4UserStackingAction
{
  public:

    LAGO_G4_StackingAction();
    virtual ~LAGO_G4_StackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
