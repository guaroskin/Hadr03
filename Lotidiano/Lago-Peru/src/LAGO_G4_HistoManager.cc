#include "LAGO_G4_HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


LAGO_G4_HistoManager::LAGO_G4_HistoManager(G4String* file)
{
  fileName = file;
  
  // histograms
  fTotPhot=0;
  for (G4int k=0; k<MaxPhot; k++) {
    fEnergy[k] = 0;
    fPmt[k] = 0;        
  }
  for(int i=0;i<PMTPULSESHAPESIZE;i++){
        fTime[i]= 0;
       fspecAn[i]=0;
       fspecDy[i]=0; 
    };
  
}

#include <fstream>
using namespace std;
//ofstream myfile("data");


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LAGO_G4_HistoManager::~LAGO_G4_HistoManager()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_HistoManager::digitize()
{
	return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_HistoManager::printParticle(G4int fPartId ,G4int dig4 ,G4String fUx ,G4String fUy ,G4String fUz ,G4String fX ,G4String fY ,G4String fTimeDelay ,G4String fTEnergy )
{
//dataFile.open(my_name_string, ios::out | ios::app);
    ofstream myfile(*fileName, ios::out | ios::app);	
    myfile << "# #" << " " << fPartId << " " << dig4 << " " << fUx << " " << fUy << " " << fUz << " " << fX << " " << fY << " " << fTimeDelay << " "   << fTEnergy << G4endl;
    fTotPhot=0; 
    for(int i=0;i<PMTPULSESHAPESIZE;i++){
       fspecAn[i]=0;
       fspecDy[i]=0; 
       fTime[i]= 0; 
    };
     fTotPhot=0;
    return;                  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_HistoManager::pmtPulse()
{
    
// loop on each photoelectron TimeSlot

  for (int itSlot = 0; itSlot < PMTPULSESHAPESIZE; itSlot++) {
    
    if (fTime[itSlot] != 0){
       for (int itPhot = 0; itPhot < fTime[itSlot]; itPhot++) {
         int chargebin = int (G4UniformRand() * (PHOTOELCHARGEPROBSIZE - 2));
         double scaling = PHOTOELCHARGEPROB[chargebin] + G4UniformRand() * (PHOTOELCHARGEPROB[chargebin + 1] - PHOTOELCHARGEPROB[chargebin]);
         //int tBintr = int (G4RandGauss::shoot( itSlot, PMTRANSITTIME ));
         //int tBintr = int (G4RandGauss::shoot( 1, PMTRANSITTIME ));
         for (int shBin = 0; shBin < PHOTOELPULSESHAPESIZE; shBin++) {
	    //int iBin = tBintr + PHOTOELPULSESHAPETIME[shBin] + PHOTOELPULSESTARTBIN;
            //int iBin = itSlot+PHOTOELPULSESHAPETIME[shBin] + PHOTOELPULSESTARTBIN;
            int iBin = itSlot+PHOTOELPULSESHAPESIZE-shBin;
	    if (iBin < PMTPULSESHAPESIZE) {
	       fspecAn[iBin] +=  scaling * PMTGAIN / PMTGAINFORPULSESHAPE * PHOTOELPULSESHAPE[shBin] / DYNODETOANODE;
	       fspecDy[iBin] +=  scaling * PMTGAIN / PMTGAINFORPULSESHAPE * PHOTOELPULSESHAPE[shBin];
	       }
            }  
         }      
       }
    }
}
/*
void LAGO_G4_HistoManager::printPulse()
{
    ofstream myfile(*fileName, ios::out | ios::app);	
    int k=0;
    for(int i=1;i<200;i++){
    if (fspecDy[i*25]>.01){k=i;break;};
    };
    G4cout << k << G4endl;
    
 
    if(k!=0){ 
        myfile  << "# " << k << G4endl;   
    	for(int i=k-2;i<k+10;i++){  
    	myfile  << fspecDy[i*25]    << G4endl; 
    	};
    };

  //  int j=0;
    return; 
}
*/
     
void LAGO_G4_HistoManager::printPulse()
{
    ofstream myfile(*fileName, ios::out | ios::app);	


    double y[PMTPULSESHAPESIZE + 5];
    for (int i = 0; i < PMTPULSESHAPESIZE + 5; i++) {
        y[i] = 0;
  }

  // Filter recursive equation 


  for (int i = 5; i < PMTPULSESHAPESIZE; i++) {
    y[i] =
      -FEDCOEF[0] * fspecDy[i - 5] + FEDCOEF[1] * y[i - 1] + FEDCOEF[2] * y[i - 2]
      + FEDCOEF[3] * y[i - 3] + FEDCOEF[4] * y[i - 4] + FEDCOEF[5] * y[i - 5];
  }


  int f = int (G4UniformRand() * (SAMPLINGTIME - 1));
  int pulse[25]; 
  
  for(int i=0;i<25;i++){
       pulse[i]=0;  
       };
    
  int k=0; 
  
   
  for (int i = 0; i < PMTPULSESHAPESIZE/25-1; i++) {
    if(y[f+i*25]>ADCTRIGGER){k=i;break;};
   // G4cout << 100000*y[i*25] << G4endl;
    };     

  
  if(k!=0){ 
    myfile << "# n "  << k  << G4endl;
    for(int i=0;i<12;i++){
       pulse[i]=int(y[(k+i-2)*25]) ;
         
       };
    pulse[24]=1; 
    }; 

  if(k!=0){ 
    for (int i = k+10; i < PMTPULSESHAPESIZE/25-1; i++) {
      if(y[f+i*25]>ADCTRIGGER){k=i;break;}
      else{k=0;};
      };
    };         

  if(k!=0){ 

    for(int i=0;i<12;i++){
       pulse[i+12]=int(y[(k+i-2)*25]);        
       };
    pulse[24]=2; 
    }; 

  if(pulse[24]!=0){   
    for(int i=0;i<12;i++){myfile  << pulse[i] << G4endl; }

    if(pulse[24]==2){
      myfile  << "# "  << "md"  << G4endl;
      for(int i=0;i<12;i++){myfile  << pulse[i+12] << G4endl; }
      }; 
    };   
   
    return;                  
}




/*
void LAGO_G4_HistoManager::printPulse()
{
    ofstream myfile(*fileName, ios::out | ios::app);	
    int k=0;
    for(int i=1;i<MaxPhot;i++){
    if (fTime[i]>2){k=i;break;};
    };
    if(k!=0){ 
        myfile  << "# " << k << G4endl;   
    	for(int i=k-2;i<k+10;i++){
    	myfile  << fTime[i]    << G4endl; 
    	};
    };
    int j=0;
    
    if(k!=0){
	    for(int i=k+11;i<MaxPhot;i++){
	    if (fTime[i]>2){j=i;break;};
	    };
	   
	    if(j!=0){
		myfile  << "# # md " << j << " " << j-k <<G4endl; 
	    	for(int i=j-2;i<j+10;i++){
	    	myfile  << fTime[i]    << G4endl; 
	    	};
	    };
    };
    return;                  
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAGO_G4_HistoManager::fill(G4int pmt, G4double ener, G4double time)
{
  //int tim25ns=((time/ns)/25.);
  int tim25ns=(time/ns); 
  fTime[tim25ns]++;
  fTotPhot++;
    return;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


