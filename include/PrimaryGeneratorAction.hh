
// © 2026. Triad National Security, LLC. All rights reserved

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
#include "G4PrimaryVertex.hh"
#include "G4GeneralParticleSource.hh"

class G4ParticleGun;
class G4GeneralParticleSource;

class G4Event;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event* event) override;


private:
  G4ParticleGun* fParticleGun = nullptr;
  G4GeneralParticleSource* fParticleSource = nullptr;


  G4int counter;
  G4PrimaryVertex* g4vtx;

  G4String junk;

};


#endif
