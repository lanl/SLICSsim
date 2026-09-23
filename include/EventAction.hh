// © 2026. Triad National Security, LLC. All rights reserved

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"

#include <chrono>

using namespace std::chrono;


class EventAction : public G4UserEventAction
{
  public:
    EventAction() = default;
    ~EventAction() override = default;

    void  BeginOfEventAction(const G4Event* event) override;
    void  EndOfEventAction(const G4Event* event) override;

    void AddTotE(G4double de);
    void AddE(G4double de);
    void AdddE(G4double de);
    void GetEmEnergy(G4double dE);
    void AddScintillationPhoton();
    void AddScintillationPhoton_on_PMT();
    void Add_dE_ScintillationPhoton_on_PMT();
    void Add_E_ScintillationPhoton_on_PMT();
    void Add_ShortI();
    void Add_LongI();
    void push_optical_time(G4double dt);
    void push_optical_time_vslow(G4double dt);
    void push_optical_time_vfast(G4double dt);
    void Add_IC_beta();


    const std::vector<G4double>& GetPhotonTimes() const { return VTimes; }

  private:
    std::vector<G4double> VSlowTimes;          //Vector with the arrival times of scintillation photons from EJ240 on the cathode
    std::vector<G4double> VFastTimes;          //Vector with the arrival times of scintillation photons from EJ200 on the cathode
    std::vector<G4double> VTimes;              //Vector with the arrival times of any scintillation photon on the cathode
    std::vector<G4double> CEenergies;          //Vector with the energies of conversion electrons produced when a 207bi source is used


    G4double fEnergyE = 0.;     //Total energy deposited in the slow scint
    G4double fEnergydE = 0.;    //Total energy deposited in the fast scint
    G4int fNumScintillationPhotons;    //Total number of scintillation photons generated
    G4int fNumScintillationPhotons_on_PMT; //Number of scintillation photons that make it to the PMT
    G4int fNum_dE_ScintillationPhotons_on_PMT; //Number of scintillation photons from the fast scintillator that make it to the PMT
    G4int fNum_E_ScintillationPhotons_on_PMT;  //Number of scintillation photons from the slow scintillator that make it to the PMT
    G4int fNum_ShortI; //Number of scintillation photons within the short integral range
    G4int fNum_LongI;  //Number of scintillation photons within the long integral range
    G4int fNum_betas=0; //Number of conversion electrons generated in the decay of 207Bi
    G4double fEnergy_CEsource; //Energy of the conversion electrons generated in the decay of 207Bi

    high_resolution_clock::time_point start_time;
    high_resolution_clock::time_point t1;
};

inline void EventAction::AddE(G4double de) {
  fEnergyE += de;
}

inline void EventAction::AdddE(G4double de) {
  fEnergydE += de;
}


inline void EventAction::GetEmEnergy(G4double de) {
  CEenergies.push_back(de);
}

inline void EventAction::AddScintillationPhoton() {
    fNumScintillationPhotons++; // Increment scintillation photon count
}

inline void EventAction::AddScintillationPhoton_on_PMT() {
    fNumScintillationPhotons_on_PMT++; // Increment scintillation photon count
}


inline void EventAction::Add_dE_ScintillationPhoton_on_PMT() {
    fNum_dE_ScintillationPhotons_on_PMT++; // Increment scintillation photon count
}

inline void EventAction::Add_E_ScintillationPhoton_on_PMT() {
    fNum_E_ScintillationPhotons_on_PMT++; // Increment scintillation photon count
}


inline void EventAction::Add_ShortI() {
    fNum_ShortI++; // Increment scintillation photon count
}

inline void EventAction::Add_LongI() {
    fNum_LongI++; // Increment scintillation photon count
}


inline void EventAction::push_optical_time(G4double dt) {
    VTimes.push_back(dt);
}

inline void EventAction::push_optical_time_vslow(G4double dt) {
    VSlowTimes.push_back(dt);
}

inline void EventAction::push_optical_time_vfast(G4double dt) {
    VFastTimes.push_back(dt);
}


inline void EventAction::Add_IC_beta() {
    fNum_betas++;
}

#endif
