#ifndef LAGO_G4_PrimaryGeneratorAction_h
#define LAGO_G4_PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class LAGO_G4_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    LAGO_G4_PrimaryGeneratorAction();
    virtual ~LAGO_G4_PrimaryGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);

  private:

    G4ParticleGun* fParticleGun;
};

#endif
