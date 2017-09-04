#ifndef LAGO_G4_PMTHit_h
#define LAGO_G4_PMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

class LAGO_G4_PMTHit : public G4VHit
{
  public:
 
    LAGO_G4_PMTHit();
    virtual ~LAGO_G4_PMTHit();
    LAGO_G4_PMTHit(const LAGO_G4_PMTHit &right);

    const LAGO_G4_PMTHit& operator=(const LAGO_G4_PMTHit &right);
    G4int operator==(const LAGO_G4_PMTHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b){fDrawit=b;}
    inline G4bool GetDrawit(){return fDrawit;}

    inline void IncPhotonCount(){fPhotons++;}
    inline G4int GetPhotonCount(){return fPhotons;}

    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
    inline G4VPhysicalVolume* GetPMTPhysVol(){return fPhysVol;}

    inline void SetPMTPos(G4double x,G4double y,G4double z){
      fPos=G4ThreeVector(x,y,z);
    }
 
    inline G4ThreeVector GetPMTPos(){return fPos;}

  private:

    G4int fPmtNumber;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

};

typedef G4THitsCollection<LAGO_G4_PMTHit> LAGO_G4_PMTHitsCollection;

extern G4ThreadLocal G4Allocator<LAGO_G4_PMTHit>* LAGO_G4_PMTHitAllocator;

inline void* LAGO_G4_PMTHit::operator new(size_t){
  if(!LAGO_G4_PMTHitAllocator)
      LAGO_G4_PMTHitAllocator = new G4Allocator<LAGO_G4_PMTHit>;
  return (void *) LAGO_G4_PMTHitAllocator->MallocSingle();
}

inline void LAGO_G4_PMTHit::operator delete(void *aHit){
  LAGO_G4_PMTHitAllocator->FreeSingle((LAGO_G4_PMTHit*) aHit);
}

#endif
