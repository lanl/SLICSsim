
// © 2026. Triad National Security, LLC. All rights reserved

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4OpticalPhoton.hh"
#include "G4HadronicProcess.hh"
#include "G4Isotope.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

struct UserTrackInfo : public G4VUserTrackInformation {
    G4String originVolume;
    bool crossedOnce = false;
    bool dEcrossedOnce = false;
    bool EcrossedOnce = false;
    UserTrackInfo() = default;
    UserTrackInfo(const G4String& origin) : originVolume(origin) {}
};

SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
                               EventAction* eventAction, StackingAction* stackingAction, G4String targetVolumeName)
  : fDetConstruction(detConstruction),
    fEventAction(eventAction),
    fStackingAction(stackingAction),
    fTargetVolumeName(targetVolumeName)
{}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  auto edep = step->GetTotalEnergyDeposit();
  auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();


  if ( volume == fDetConstruction->GetEPV() ) {
    fEventAction->AddE(edep);
  }

  if ( volume == fDetConstruction->GetdEPV() ) {
    fEventAction->AdddE(edep);
  }


auto currentVolume = step->GetPreStepPoint()->GetPhysicalVolume();
auto nextVolume = step->GetPostStepPoint()->GetPhysicalVolume();


  UserTrackInfo* info = dynamic_cast<UserTrackInfo*>(track->GetUserInformation());

  for (const auto* sec : *step->GetSecondaryInCurrentStep()) {

          auto touch = sec->GetTouchableHandle();
          if (!touch) continue;

          auto* vol = touch->GetVolume();
          if (!vol) continue;
          //std::cout<<"Volume "<< vol->GetName() << std::endl;

          info = new UserTrackInfo(vol->GetName());
          const_cast<G4Track*>(sec)->SetUserInformation(info);
  }




 if (info && nextVolume){

       auto dtime = step->GetPreStepPoint()->GetGlobalTime();
       G4String nextVolumeName = nextVolume->GetName();
       G4String currentVolumeVolumeName = currentVolume->GetName();

    if (nextVolumeName == "Physical_PMT" && !info->crossedOnce ) {
      //G4cout << "Photon " << track->GetTrackID() << " left "<< currentVolumeVolumeName << " and entered "<< nextVolumeName << G4endl;
      fEventAction->AddScintillationPhoton_on_PMT();

      // Sample a delay from exponential PMT response
      G4double tau = 7* CLHEP::ns;  // Optional value for PMT decay constant (e.g., for TTS or response tail)
      G4double decayDelay = -tau * std::log(G4UniformRand());
      G4double pmt_timing = dtime+decayDelay;

      fEventAction->push_optical_time(dtime);
      if(pmt_timing<35){fEventAction->Add_ShortI();} // Optional value for short integral
      if(pmt_timing<170){fEventAction->Add_LongI();} // Optional value for long integral
      info->crossedOnce = true;
    }

    if (nextVolumeName == "Physical_PMT" && info->originVolume=="Physical_dE"  && !info->dEcrossedOnce ) {
      fEventAction->Add_dE_ScintillationPhoton_on_PMT();
      fEventAction->push_optical_time_vfast(dtime);
      //G4cout << "Track: " << track->GetTrackID() <<  "   Particle: "<< track->GetDefinition()->GetParticleName()  << "Created: "<< track->GetCreatorProcess()->GetProcessName()  << " entered "<< nextVolumeName << "  time:  "<<  dtime<< G4endl;
      info->dEcrossedOnce = true;
    }

   if (nextVolumeName == "Physical_PMT" && info->originVolume=="Physical_E"  && !info->EcrossedOnce ) {
      //G4cout << "Photon " << track->GetTrackID() << " entered "<< nextVolumeName << G4endl;
      fEventAction->Add_E_ScintillationPhoton_on_PMT();
      fEventAction->push_optical_time_vslow(dtime);
      info->EcrossedOnce = true;
    }

  }

}
