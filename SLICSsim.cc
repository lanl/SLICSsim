// © 2026. Triad National Security, LLC. All rights reserved


#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"


int main(int argc,char** argv)
{


  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = new G4UIExecutive(argc, argv);

  // Construct the default run manager
  //
  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default,4);

  // Set mandatory initialization classes
  //
  auto detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);



  //Initialize Physics list
  auto physicsList = new PhysicsList();
  runManager->SetUserInitialization(physicsList);

  auto actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);



  // Initialize visualization
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  //auto visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (argv[1]){
  G4String macro = argv[1];


  if ( macro.size() ) {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }

}
  else  {
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
    //if (ui->IsGUI()) {
    //  UImanager->ApplyCommand("/control/execute macros/gui.mac");
    //}
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
