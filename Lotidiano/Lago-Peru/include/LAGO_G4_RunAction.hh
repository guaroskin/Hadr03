#include "G4UserRunAction.hh"

#ifndef LAGO_G4_RunAction_h
#define LAGO_G4_RunAction_h 1

class LAGO_G4_RecorderBase;

class LAGO_G4_RunAction : public G4UserRunAction
{
  public:

    LAGO_G4_RunAction(LAGO_G4_RecorderBase*);
    LAGO_G4_RunAction();
    virtual ~LAGO_G4_RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:

    LAGO_G4_RecorderBase* fRecorder;
};

#endif
