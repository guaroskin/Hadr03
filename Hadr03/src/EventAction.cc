//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1EventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
  : G4UserEventAction(), fSumPhoton(0)
{

} 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* )
{
  fSumPhoton = 0;
  New_neutron = true;
  //G4cout << "NEW neutron \n" << G4endl;
  //G4cout << "ID" << evento->GetEventID() <<  G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SumPhoton()
{
  fSumPhoton++;
}

void EventAction::Pulso(G4double tiempo)
{
  pulso.push_back(tiempo);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event*)
{
  //G4cout << "pulso size " << pulso.size() << G4endl;
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  if(fSumPhoton != 0) analysis->FillH1(6,fSumPhoton);
  if(pulso.size() != 0){
    std::sort(pulso.begin(), pulso.end());
    G4double Offset = pulso[0];
    for (int i=0; i < pulso.size() ; i++){
      analysis->FillH1(8,(pulso[i]-Offset)/ns);
      //G4cout << "globalTime Ordenado " << (pulso[i]-Offset)/ns << G4endl;
    }
  }
  pulso.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
