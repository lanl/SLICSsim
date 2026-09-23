// © 2026. Triad National Security, LLC. All rights reserved

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithABool.hh"
class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;



class DetectorConstruction : public G4VUserDetectorConstruction, public G4UImessenger
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();
  public:
    G4VPhysicalVolume* Construct() override;
    virtual void SetNewValue(G4UIcommand* command, G4String newValue) override;

    const G4VPhysicalVolume* GetEPV() const;
    const G4VPhysicalVolume* GetdEPV() const;
    const G4VPhysicalVolume* GetPMTPV() const;
    const G4VPhysicalVolume* GetWindowPV() const;

  private:

    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    G4bool UseExtendedSample;
    G4bool UseTIM_SLICS;
    G4UIdirectory*    fGeomDir;
    G4UIcmdWithABool* fGeomCmd;

    G4VPhysicalVolume* fE = nullptr;
    G4VPhysicalVolume* fdE = nullptr;
    G4VPhysicalVolume* fPMT = nullptr;
    G4VPhysicalVolume* fWindow = nullptr;
    G4VPhysicalVolume* fCathode = nullptr; //Not used
    G4VPhysicalVolume* fBrass = nullptr;
    G4VPhysicalVolume* fPhosBody = nullptr;

    G4bool fCheckOverlaps = true; // option to activate checking of volumes overlaps

    //Brass and orings size
    G4double brass_radius_in1 ;
    G4double brass_radius_in2 ;
    G4double brass_radius_in3 ;
    G4double brass_radius_out1 ;
    G4double brass_radius_out2 ;
    G4double brass_radius_out3 ;
    G4double brass_radius_out;
    G4double brass_height_1 ;
    G4double brass_height_2;
    G4double brass_height_3 ;
    G4double brass_height_in;
    G4double brass_height_out;
    G4double bracket_height ;
    G4double bracket_radius_in ;
    G4double bracket_radius_out ;

    //SS body size
    G4double SStube_radius_in ;
    G4double SStube_radius_out ;
    G4double SStube_height ;
    G4double window_radius ;
    G4double window_height ;
    G4double pmt_radius ;
    G4double pmt_height ;
    G4double ss_radius ;
    G4double ss_height;
    G4double cathode_radius ;
    G4double cathode_height ;
    G4double scint_radius;
    G4double scint_height_dE;
    G4double scint_height_E ;
    G4double lg_radius ;
    G4double lg_height ;
    G4double oring_height;
};


inline const G4VPhysicalVolume* DetectorConstruction::GetEPV() const  {
  return fE;
}

inline const G4VPhysicalVolume* DetectorConstruction::GetdEPV() const  {
  return fdE;
}

inline const G4VPhysicalVolume* DetectorConstruction::GetPMTPV() const  {
  return fPMT;
}

inline const G4VPhysicalVolume* DetectorConstruction::GetWindowPV() const  {
  return fWindow;
}

#endif
