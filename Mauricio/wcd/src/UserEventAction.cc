
// Geant4 Libraries
//
#include "G4UserEventAction.hh"
#include "G4Event.hh"


// Local Libraries
//
#include "UserEventAction.hh"


UserEventAction::UserEventAction()
{}


UserEventAction::~UserEventAction()
{}


void UserEventAction::BeginOfEventAction(const G4Event* event)
{
  G4cout << "hi msd: " << event->GetEventID() << G4endl;
  
}


void UserEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "hi, again, msd" << event->GetEventID() << G4endl;
}
