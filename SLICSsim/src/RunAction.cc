// © 2026. Triad National Security, LLC. All rights reserved


#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


G4int EventsToProcess=0;
extern G4int nevents;


RunAction::RunAction()
{

  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  analysisManager->SetNtupleMerging(true);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("Hit", "Hit");
  analysisManager->CreateNtupleDColumn("SlowScntEdep");
  analysisManager->CreateNtupleDColumn("FastScntEdep");
  analysisManager->CreateNtupleDColumn("NumberOfPhotonsGenerated");
  analysisManager->CreateNtupleDColumn("NumberOfPhotonsTransmitted");
  analysisManager->CreateNtupleDColumn("NumberOfEJ200PhotonsTransmitted");
  analysisManager->CreateNtupleDColumn("NumberOfEJ240PhotonsTransmitted");
  analysisManager->CreateNtupleDColumn("Eshort");
  analysisManager->CreateNtupleDColumn("Elong");
  analysisManager->CreateNtupleDColumn("EmittedSecondaryElectrons");
  analysisManager->CreateNtupleDColumn("ScintillatorDecay");
  analysisManager->CreateNtupleDColumn("EJ240ScintillatorDecay");
  analysisManager->CreateNtupleDColumn("Ej200ScintillatorDecay");
  analysisManager->CreateNtupleDColumn("AveragePulseAt1MeV");

  analysisManager->FinishNtuple();
}


void RunAction::BeginOfRunAction(const G4Run* run/*run*/)
{

  EventsToProcess = run->GetNumberOfEventToBeProcessed();

  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "Output.root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  // G4String fileName = "B4.hdf5";
  // G4String fileName = "B4.xml";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4cout<<"  Processed "<<nevents<<" out of "<<  EventsToProcess  <<" events"<< G4endl;


  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
