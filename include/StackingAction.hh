// © 2026. Triad National Security, LLC. All rights reserved


#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "EventAction.hh"
#include <set>

class EventAction; 

class StackingAction : public G4UserStackingAction {
public:
    StackingAction(EventAction* eventAction, G4String targetVolumeName);
    virtual ~StackingAction() {}

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    bool IsScintillationPhoton(G4int trackID);
    bool Is_dE_ScintillationPhoton(G4int trackID);
    bool Is_E_ScintillationPhoton(G4int trackID);


private:
    EventAction* fEventAction;
    G4String fTargetVolumeName;
    std::set<G4int> fScintillationPhotonTracks;
    std::set<G4int> f_dE_ScintillationPhotonTracks;
    std::set<G4int> f_E_ScintillationPhotonTracks;

};

#endif
