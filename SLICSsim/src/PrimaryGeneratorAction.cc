
// © 2026. Triad National Security, LLC. All rights reserved

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"



PrimaryGeneratorAction::PrimaryGeneratorAction()
{

  G4int nofParticles = 1;
  fParticleSource = new G4GeneralParticleSource();
  counter=0;

}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fParticleSource;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  fParticleSource->GeneratePrimaryVertex(anEvent);

}
