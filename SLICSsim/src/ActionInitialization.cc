// © 2026. Triad National Security, LLC. All rights reserved


#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "StackingAction.hh"



ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : fDetConstruction(detConstruction)
{}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}


void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);

  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);

  G4String targetVolumeName = "DetectorVolume";
  StackingAction* stackingAction = new StackingAction(eventAction, targetVolumeName);
  SetUserAction(stackingAction);
  SetUserAction(new SteppingAction(fDetConstruction,eventAction, stackingAction, targetVolumeName));

}
