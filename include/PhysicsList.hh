// © 2026. Triad National Security, LLC. All rights reserved

#ifndef PhysicsList_hh
#define PhysicsList_hh 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
 ~PhysicsList();

public:
  virtual void ConstructParticle();
  virtual void SetCuts();
};

#endif
