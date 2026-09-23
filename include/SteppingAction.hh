// © 2026. Triad National Security, LLC. All rights reserved

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "StackingAction.hh"


class DetectorConstruction;

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(const DetectorConstruction* detConstruction,EventAction* eventAction, StackingAction* stackingAction, G4String targetVolumeName);
  ~SteppingAction() override = default;

  void UserSteppingAction(const G4Step* step) override;

private:
  const DetectorConstruction* fDetConstruction = nullptr;
  EventAction* fEventAction = nullptr;
  StackingAction* fStackingAction = nullptr;
  G4String fTargetVolumeName = nullptr;
};


#endif
