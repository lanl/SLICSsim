// © 2026. Triad National Security, LLC. All rights reserved


#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"


class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction*);
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;

  private:
    DetectorConstruction* fDetConstruction = nullptr;
};



#endif
