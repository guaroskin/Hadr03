#ifndef LAGO_G4_GeneralPhysics_h
#define LAGO_G4_GeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"



class LAGO_G4_GeneralPhysics : public G4VPhysicsConstructor
{
  public:

    LAGO_G4_GeneralPhysics(const G4String& name = "general");
    virtual ~LAGO_G4_GeneralPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

};

#endif
