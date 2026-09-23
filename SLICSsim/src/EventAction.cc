// © 2026. Triad National Security, LLC. All rights reserved

#include "EventAction.hh"
#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>
#include "StackingAction.hh"


extern G4int EventsToProcess;
G4int nevents=0;

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  // initialisation per event

  VTimes.assign(VTimes.size(),0); //Vector with the arrival times of the scintillation photons on the cathode
  VTimes.clear();

  VSlowTimes.assign(VSlowTimes.size(),0); //Vector with the arrival times of the scintillation photons from EJ240 on the cathode
  VSlowTimes.clear();

  VFastTimes.assign(VFastTimes.size(),0); //Vector with the arrival times of the scintillation photons form EJ200 on the cathode
  VFastTimes.clear();

  CEenergies.assign(CEenergies.size(),0); //Vector with the energies of conversion electrons produced
  CEenergies.clear();

  fEnergyE = 0.; //Total energy deposited in the slow scintillator
  fEnergydE = 0.; //Total energy deposited in the fast scintillator
  fNumScintillationPhotons = 0;
  fNumScintillationPhotons_on_PMT = 0;
  fNum_dE_ScintillationPhotons_on_PMT = 0;
  fNum_E_ScintillationPhotons_on_PMT = 0;
  fNum_ShortI=0;
  fNum_LongI=0;
  fEnergy_CEsource=0;
}


void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
  nevents++;

  auto analysisManager = G4AnalysisManager::Instance();

  // fill ntuple
  analysisManager->FillNtupleDColumn(0, fEnergyE);
  analysisManager->FillNtupleDColumn(1, fEnergydE);
  analysisManager->FillNtupleDColumn(2, fNumScintillationPhotons);
  analysisManager->FillNtupleDColumn(3, fNumScintillationPhotons_on_PMT);
  analysisManager->FillNtupleDColumn(4, fNum_dE_ScintillationPhotons_on_PMT);
  analysisManager->FillNtupleDColumn(5, fNum_E_ScintillationPhotons_on_PMT);
  analysisManager->FillNtupleDColumn(6, fNum_ShortI);
  analysisManager->FillNtupleDColumn(7, fNum_LongI);


  for (const auto& e : CEenergies) {
      analysisManager->FillNtupleDColumn(8, e);
      analysisManager->AddNtupleRow();
  }

  for (const auto& t : VTimes) {
      analysisManager->FillNtupleDColumn(9, t);
      analysisManager->AddNtupleRow();
  }


  for (const auto& t : VSlowTimes) {
      analysisManager->FillNtupleDColumn(10, t);
      analysisManager->AddNtupleRow();
  }

  for (const auto& t : VFastTimes) {
      analysisManager->FillNtupleDColumn(11, t);
      analysisManager->AddNtupleRow();
  }


  for (const auto& t : VTimes) {
    if((fEnergyE+fEnergydE)>0.9&&(fEnergyE+fEnergydE)<1.1){ //Optional energy range
      analysisManager->FillNtupleDColumn(12, t);
      analysisManager->AddNtupleRow();
    }


  }


  if(nevents%10000==0) {
    G4cout<<"  Processed "<<nevents<<" out of "<<  EventsToProcess  <<" events"<< G4endl;
  }


}
