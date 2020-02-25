// $Id: MyHit.cc 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file MyHit.cc
/// \brief Implementation of the MyHit class

#include "MyHit.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyHit::MyHit()
 :  waveLength(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyHit::~MyHit() {}

G4int MyHit::QuantumE(G4double phoEner)
{

  this->GetWavelength(phoEner);
  
  if(waveLength >= 250. && waveLength < 300.)
  {
    if(G4UniformRand() <= 0.01)
      return 1;
    else 
      return 0;
  }
  else if(waveLength >= 300. && waveLength < 350.)
  {
    if(G4UniformRand() <= 0.03) 
      return 1;
    else
      return 0;
  }
  else if(waveLength >= 350. && waveLength < 400.)
  {
    if(G4UniformRand() <= 0.2) 
      return 1;
    else 
      return 0;
  }
  else if(waveLength >= 400. && waveLength < 450.)
  {
    if(G4UniformRand() <= 0.25)
      return 1;
    else
      return 0;
  }
  else if(waveLength >= 450. && waveLength < 500.)
  {
    if(G4UniformRand() <= 0.2) 
      return 1;
    else 
      return 0;
  }
  else if(waveLength >= 500. && waveLength < 550.)
  {
    if(G4UniformRand() <= 0.14) 
      return 1;
    else
      return 0;
  }
  else if(waveLength >= 550. && waveLength < 600.)
  {
    if(G4UniformRand() <= 0.07)
      return 1;
    else 
      return
        0;
  }
  else if(waveLength >= 600. && waveLength < 650.)
  {
    if(G4UniformRand() <= 0.03) 
      return 1;
    else 
      return 0;
  }
  else if(waveLength >= 650. && waveLength < 700.)
  {
    if(G4UniformRand() <= 0.06)
      return 1;
    else 
      return 0;
  }
  else
   return 0;
}


void MyHit::GetWavelength(G4double ener)
{
  waveLength = (1240. / ener)*nm;
}

