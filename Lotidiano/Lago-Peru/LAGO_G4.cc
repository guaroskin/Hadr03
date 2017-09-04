#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4String.hh"

#include "LAGO_G4_PhysicsList.hh"
#include "LAGO_G4_DetectorConstruction.hh"
#include "LAGO_G4_ActionInitialization.hh"
#include "LAGO_G4_RecorderBase.hh"
#include "LAGO_G4_DetectorParameters.hh"
#include "LAGO_G4_DetectorMessenger.hh"
#include "LAGO_G4_HistoManager.hh" 

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
   G4String filename; 
  
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  LAGO_G4_HistoManager*  histo = new LAGO_G4_HistoManager(&filename);

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  LAGO_G4_DetectorParameters* tuningpars = new LAGO_G4_DetectorParameters();

  //file_exists("tuning_parameters.mac");
  UImanager->ApplyCommand("/control/execute tuning_parameters.mac");


  runManager->SetUserInitialization(new LAGO_G4_DetectorConstruction(tuningpars));
  runManager->SetUserInitialization(new LAGO_G4_PhysicsList());

  LAGO_G4_RecorderBase* recorder = NULL; //No recording is done in this example

  runManager->SetUserInitialization(new LAGO_G4_ActionInitialization(recorder,histo));




 
  // get the pointer to the UI manager and set verbosities


  

  if(argc==1)
{
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  runManager->Initialize();

#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
//    if (ui->IsGUI())
//       UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
  delete visManager;
#endif

  }
/*  else{
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    UImanager->ApplyCommand(command+filename);
  }
*/

//  if(recorder)delete recorder;



else if(argc==3){

runManager->Initialize();
  G4UImanager* UI = G4UImanager::GetUIpointer();
   UI->ApplyCommand("/run/verbose 0");
   UI->ApplyCommand("/tracking/verbose 0");
       //UImanager->ApplyCommand("/control/execute config.mac");
      //else if(argc==2){
    //ifstream inc(argv[2],ios::in);
    filename = argv[2];

    ifstream inc(argv[1],ios::in);
    //G4String command = "/control/execute ";
    //G4String filename = argv[1];
    //UImanager->ApplyCommand(command+filename);
    if ( !inc ){
    	G4cout<<"Error al abrir archivo de datos"<<G4endl;
        }
    else {  

	 G4String particleName;
         G4String fUx,fUy,fUz,fX,fY,fZ,fTimeDelay,fTEnergy;
         G4int dig4,fPartId;

         while (inc>>fPartId>>dig4>>fUx>>fUy>>fUz>>fX>>fY>>fTimeDelay>>fTEnergy)
        {
           
          if ( fPartId == 1)
            {
              particleName = "gamma";
            }
          else if ( fPartId == 2 )
            {                       //e+
              particleName = "e+";
            }
          else if ( fPartId == 3 )
            {                       //e-
              particleName = "e-";
            }
          else if ( fPartId == 5 )
            {                       //mu+
              particleName = "mu+";
            }
          else if ( fPartId == 6 )
            {                       //mu-
              particleName = "mu-";
            }
          else//do not simulate other particles
            continue;
          fZ="150.0";
          
	  G4cout << "##" << " " << fPartId << " " << dig4 << " " << fUx << " " << fUy << " " << fUz << " " << fX << " " << fY << " " << fTimeDelay << " " << fTEnergy << G4endl;

          UImanager->ApplyCommand("/gun/particle " + particleName);
          UImanager->ApplyCommand("/gun/energy " + fTEnergy +" GeV");
          UImanager->ApplyCommand("/gun/time " + fTimeDelay +" ns");
          UImanager->ApplyCommand("/gun/position " + fUx + " " + fUy + " " + fUz );  
          UImanager->ApplyCommand("/gun/direction " + fX + " " + fY + " " + fZ + " cm");   
          UImanager->ApplyCommand("/run/beamOn");   
         //     getchar();

           histo->pmtPulse();
           histo->printPulse();
           histo->printParticle(fPartId , dig4 , fUx , fUy , fUz , fX , fY , fTimeDelay , fTEnergy );
         }  
      }
}











  // job termination
  delete runManager;
  return 0;
}
