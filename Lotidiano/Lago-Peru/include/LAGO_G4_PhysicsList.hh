#ifndef LAGO_G4_PhysicsList_h
#define LAGO_G4_PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class LAGO_G4_PhysicsList: public G4VModularPhysicsList
{
  public:

    LAGO_G4_PhysicsList();
    virtual ~LAGO_G4_PhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};

#endif
