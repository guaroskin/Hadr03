#ifndef LAGO_G4_Trajectory_h
#define LAGO_G4_Trajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class LAGO_G4_Trajectory : public G4Trajectory
{
  public:

    LAGO_G4_Trajectory();
    LAGO_G4_Trajectory(const G4Track* aTrack);
    LAGO_G4_Trajectory(LAGO_G4_Trajectory &);
    virtual ~LAGO_G4_Trajectory();
 
    virtual void DrawTrajectory() const;
 
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetDrawTrajectory(G4bool b){fDrawit=b;}
//    void WLS(){fWls=true;}
    void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

  private:

//    G4bool fWls;
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<LAGO_G4_Trajectory>* LAGO_G4_TrajectoryAllocator;

inline void* LAGO_G4_Trajectory::operator new(size_t)
{
  if(!LAGO_G4_TrajectoryAllocator)
      LAGO_G4_TrajectoryAllocator = new G4Allocator<LAGO_G4_Trajectory>;
  return (void*)LAGO_G4_TrajectoryAllocator->MallocSingle();
}

inline void LAGO_G4_Trajectory::operator delete(void* aTrajectory)
{
  LAGO_G4_TrajectoryAllocator->FreeSingle((LAGO_G4_Trajectory*)aTrajectory);
}

#endif
