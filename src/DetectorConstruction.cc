// © 2026. Triad National Security, LLC. All rights reserved

#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4UnionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "ScintillatorBuilder.hh"
#include "MaterialPropertiesTable.hh"
#include "OrganicScintillatorFactory.hh"
#include "InorganicScintillatorFactory.hh"
#include "CADMesh.hh"



DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
G4UImessenger(),
UseExtendedSample(false),
UseTIM_SLICS(false)
{
  fGeomDir = new G4UIdirectory("/geometry/");
  fGeomDir->SetGuidance("Detector geometry control commands.");
  fGeomCmd = new G4UIcmdWithABool("/geometry/UseExtendedSample", this);
  fGeomCmd = new G4UIcmdWithABool("/geometry/UseTIM_SLICS", this);
  fGeomCmd->SetGuidance("Switch between default (false) and alternative (true) geometry.");
  fGeomCmd->SetParameterName("UseExtendedSample", false);
  fGeomCmd->SetParameterName("UseTIM_SLICS", false);
  fGeomCmd->AvailableForStates(G4State_PreInit); // Must be set before /run/initialize

}


DetectorConstruction::~DetectorConstruction()
{
    delete fGeomCmd;
    delete fGeomDir;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{

  G4NistManager* nistManager = G4NistManager::Instance();

  G4Element* natAl = nistManager->FindOrBuildElement("Al");
  G4Element* natGe = nistManager->FindOrBuildElement("Ge");
  G4Element* natBe = nistManager->FindOrBuildElement("Be");
  G4Element* natO = nistManager->FindOrBuildElement("O");
  G4Element* natC = nistManager->FindOrBuildElement("C");
  G4Element* natLa = nistManager->FindOrBuildElement("La");
  G4Element* natBr = nistManager->FindOrBuildElement("Br");
  G4Element* natN = nistManager->FindOrBuildElement("N");
  G4Element* natK = nistManager->FindOrBuildElement("K");
  G4Element* natCl = nistManager->FindOrBuildElement("Cl");
  G4Element* natSi = nistManager->FindOrBuildElement("Si");
  G4Element* natF = nistManager->FindOrBuildElement("F");
  G4Element* natH = nistManager->FindOrBuildElement("H");
  G4Element* natZn = nistManager->FindOrBuildElement("Zn");
  G4Element* natCu = nistManager->FindOrBuildElement("Cu");


  G4Element* elAl  = new G4Element("Aluminum",  "Al",  13.,  26.981538*g/mole);
  G4Element* elN  = new G4Element("Nitrogen","N",  7.,  14.00674*g/mole);
  G4Element* elO  = new G4Element("Oxygen",  "O",  8.,  15.9994*g/mole);
  G4Element* elGe  = new G4Element("Germanium",  "Ge",  32.,  72.64*g/mole);
  G4Element* elBr  = new G4Element("Bromine",  "Br",  35.,  79.904*g/mole);
  G4Element* elLa  = new G4Element("Lanthanum",  "La",  57.,  138.90547*g/mole);
  G4Element* elBi  = new G4Element("Bismuth",  "Bi",  83.,  208.9804*g/mole);
  G4Element* elBe  = new G4Element("Beryllium",  "Be",  4.,  9.012183*g/mole);
  G4Element* elPb  = new G4Element("Lead",  "Pb",  82.,  207.2*g/mole);
  G4Element* elSi  = new G4Element("Silicon",  "Si",  14.,  28.0855*g/mole);

  G4Material *Al = new G4Material("Al", 26.99*g/cm3, 1);
  Al->AddElement(natAl, 1);

  G4Material *Be = new G4Material("Be", 1.83*g/cm3, 1);
  Be->AddElement(natBe, 1);

  G4Material *KCl = new G4Material("KCl", 1.98*g/cm3, 2);
  KCl->AddElement(natK, 1);
  KCl->AddElement(natCl, 1);

  G4Material *vacuum = new G4Material("vacuum", universe_mean_density, 2);
  vacuum-> AddElement(elN, .7);
  vacuum-> AddElement(elO, .3);


  G4Material *Brass = new G4Material("Brass", 8.73*g/cm3, 2);
  Brass->AddElement(natZn, 0.3);
  Brass->AddElement(natCu, 0.7);


  G4Material *BunaN = new G4Material("BunaN", 1.0*g/cm3,3);
  BunaN->AddElement(natC,7);
  BunaN->AddElement(natH,9);
  BunaN->AddElement(natN,1);

  // Print materials
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{


//#### Get materials ################
G4Material* vacuum = G4Material::GetMaterial("vacuum");
G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
G4Material* Alum = G4Material::GetMaterial("Al");
G4Material* Be = G4Material::GetMaterial("Be");
G4Material* Brass = G4Material::GetMaterial("Brass");
G4Material* BunaN = G4Material::GetMaterial("BunaN");

G4Material* scntEJ200 = OrganicScintillatorFactory::GetInstance()->Get("opsc-100", true);
G4Material* scntEJ240 = OrganicScintillatorFactory::GetInstance()->Get("opsc-113", true);

G4Material* glass = G4NistManager::Instance()->FindOrBuildMaterial("G4_GLASS_PLATE");
G4Material* ssteel = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");


scntEJ200->GetMaterialPropertiesTable()->DumpTable();
scntEJ240->GetMaterialPropertiesTable()->DumpTable();


G4double photonEnergies[] = {1.771*eV, 2.0*eV, 2.5*eV, 3.0*eV, 3.875*eV};
const G4int numEntries = sizeof(photonEnergies) / sizeof(G4double);

G4double rindexAir[] = {1.0002758, 1.0002767, 1.000279, 1.000282, 1.000288};
G4double absorptionLengthAir[] = {1000*m, 1000*m, 1000*m, 1000*m, 1000*m};
G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
airMPT->AddProperty("RINDEX", photonEnergies, rindexAir, numEntries);
airMPT->AddProperty("ABSLENGTH", photonEnergies, absorptionLengthAir, numEntries);
air->SetMaterialPropertiesTable(airMPT);

G4double rindexVac[] = {1.0, 1.0, 1.0, 1.0, 1.0};
G4double absorptionLengthVac[] = {1000*m, 1000*m, 1000*m, 1000*m, 1000*m};
G4MaterialPropertiesTable* vacMPT = new G4MaterialPropertiesTable();
vacMPT->AddProperty("RINDEX", photonEnergies, rindexVac, numEntries);
vacMPT->AddProperty("ABSLENGTH", photonEnergies, absorptionLengthVac, numEntries);
vacuum->SetMaterialPropertiesTable(vacMPT);

G4double rindexGlass[] = {1.6126, 1.6175, 1.6305, 1.648, 1.696};
G4double absorptionLengthGlass[] = {2486.7*cm, 372.6*cm, 59.0*cm, 8.3*cm, 0.13*cm};
G4MaterialPropertiesTable* glassMPT = new G4MaterialPropertiesTable();
glassMPT->AddProperty("RINDEX", photonEnergies, rindexGlass, numEntries);
glassMPT->AddProperty("ABSLENGTH", photonEnergies, absorptionLengthGlass, numEntries);
glass->SetMaterialPropertiesTable(glassMPT);


G4double rindexSS[] = {2.66, 2.31, 1.85, 1.55, 1.357};
G4double absorptionLengthSS[] = {10*nm, 10*nm, 10*nm, 10*nm, 10*nm};
G4MaterialPropertiesTable* ssMPT = new G4MaterialPropertiesTable();
ssMPT->AddProperty("RINDEX", photonEnergies, rindexSS, numEntries);
ssMPT->AddProperty("ABSLENGTH", photonEnergies, absorptionLengthSS, numEntries);
ssteel->SetMaterialPropertiesTable(ssMPT);

G4double rindexBrass[] = {0.446, 0.452, 0.686, 1.405, 1.446};
G4double absorptionLengthBrass[] = {20*nm, 20*nm, 20*nm, 20*nm, 20*nm};
G4MaterialPropertiesTable* brassMPT = new G4MaterialPropertiesTable();
brassMPT->AddProperty("RINDEX", photonEnergies, rindexBrass, numEntries);
brassMPT->AddProperty("ABSLENGTH", photonEnergies, absorptionLengthBrass, numEntries);
Brass->SetMaterialPropertiesTable(brassMPT);



//#### Create world ##################
auto world_solid = new G4Box("world_solid", 50*cm, 50*cm, 150*cm);
auto world_logical = new G4LogicalVolume(world_solid, vacuum,"world_logical",0,0,0);
auto world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical,"world_physical", 0, false, 0);


// Visualization attributes
 G4VisAttributes* sblue= new G4VisAttributes(G4Colour(0,0,1.0));
 sblue->SetVisibility(true);
 G4VisAttributes* sgreen= new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
 sgreen->SetVisibility(true);
 G4VisAttributes* sred = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
 sred->SetVisibility(true);
 G4VisAttributes* sgrey = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6));
 sgrey->SetVisibility(true);
 G4VisAttributes* syellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
 syellow->SetVisibility(true);
 G4VisAttributes* smagneta = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
 smagneta->SetVisibility(true);





//##############  dimensions of phoswich parts #################
 scint_height_dE = 1*mm;
 scint_height_E = 18.*mm;
 lg_height = 308.0*mm;

 window_radius = 6.985*cm/2.;
 window_height = 6.35*mm;

 pmt_radius = 4.0*cm/2.;
 pmt_height = 1.*mm;

 ss_radius = 2.54*cm/2.;
 ss_height = 0.025*mm;

 cathode_radius = 2.5*cm/2.;
 cathode_height = 0.0*nm;




if (UseTIM_SLICS){ // TIM-based SLICS

 scint_radius = 2.36*cm/2.;
 lg_radius = 2.6*cm/2.;

 brass_radius_in1 = 2.199*cm/2.;
 brass_radius_in2 = 2.5146*cm/2.;
 brass_radius_in3 = 3.154*cm/2.;

 brass_radius_out1 = 2.7178*cm/2.;
 brass_radius_out2 = 2.7178*cm/2.;
 brass_radius_out3 = 3.607*cm/2.;

 brass_height_1 = 1.25*mm;
 brass_height_2 = 10.4*mm;
 brass_height_3 = 10.16*mm;

 bracket_height = 1.8*mm;
 bracket_radius_in = 2.199*cm/2.0;
 bracket_radius_out = 2.5146*cm/2.0;

 SStube_radius_in = 3.1547*cm/2.;
 SStube_radius_out = 3.429*cm/2.;
 SStube_height = lg_height;
}



else{ // Prototype SLICS
 scint_radius = 2.54*cm/2.;
 lg_radius = 2.59*cm/2.;

 brass_radius_in1 = 2.29*cm/2.;
 brass_radius_in2 = 2.54*cm/2.;
 brass_radius_out = 3.124*cm/2.;

 brass_height_in = 5.5*mm;
 brass_height_out = 3.1*mm;

 oring_height = 1.5*mm;
 bracket_height = 1*mm;

 SStube_radius_in = 2.844*cm/2.;
 SStube_radius_out = 3.175*cm/2.;
 SStube_height = 31.0*cm + 2.5*cm;
}





//############## Phoswich DETECTOR #################
auto fSolidEJ200 = new G4Tubs("EJ200",0,scint_radius,scint_height_dE/2.0,0,twopi);
auto fLogicEJ200 = new G4LogicalVolume(fSolidEJ200,scntEJ200,"LogicEJ200",0,0,0);

auto fSolidEJ240 = new G4Tubs("EJ240",0,scint_radius,scint_height_E/2.0,0,twopi);
auto fLogicEJ240 = new G4LogicalVolume(fSolidEJ240,scntEJ240,"LogicEJ240",0,0,0);

auto fSolidGuide = new G4Tubs("LG",0,lg_radius,lg_height/2.0,0,twopi);
auto fLogicGuide = new G4LogicalVolume(fSolidGuide,glass,"LogicLG",0,0,0);

auto fSolidWindow = new G4Tubs("Window",0,window_radius,window_height/2.0,0,twopi);
auto fLogicWindow = new G4LogicalVolume(fSolidWindow,glass,"LogicWindow",0,0,0);

auto fSolidPMT = new G4Tubs("PMT",0,pmt_radius,pmt_height/2.0,0,twopi);
auto fLogicPMT = new G4LogicalVolume(fSolidPMT,glass,"LogicPMT",0,0,0);

auto fSolidSSwindow = new G4Tubs("SSwindow",0,ss_radius,ss_height/2.0,0,twopi);
auto fLogicSSwindow = new G4LogicalVolume(fSolidSSwindow,ssteel,"LogicSSwindow",0,0,0);


G4double Gap = 0.0*mm;
G4double Theta = 0.0*deg;
G4double Phi =  0.0*deg;
G4double Rho = 0.0;


G4Translate3D shiftZ(0, 0, Rho + ss_height/2. );
G4RotateY3D rotTheta(Theta);
G4RotateZ3D rotPhi(Phi);
auto transform = rotPhi*rotTheta*shiftZ;
new G4PVPlacement(transform, fLogicSSwindow,"Physical_SSwindow", world_logical, false, 0);

G4Translate3D shiftZde(0, 0,  Rho+ ss_height + scint_height_dE/2. + Gap );
auto transformde = rotPhi*rotTheta*shiftZde;
fdE = new G4PVPlacement(transformde, fLogicEJ200,"Physical_dE", world_logical, false, 0);

G4Translate3D shiftZe(0, 0, Rho + ss_height + scint_height_dE + Gap + scint_height_E/2. );
auto transforme = rotPhi*rotTheta*shiftZe;
fE = new G4PVPlacement(transforme, fLogicEJ240,"Physical_E", world_logical, false, 0);

G4Translate3D shiftLG(0, 0, Rho + ss_height + scint_height_E + Gap + scint_height_dE + lg_height/2.0 );
auto transformlg = rotPhi*rotTheta*shiftLG;
new G4PVPlacement(transformlg, fLogicGuide,"Physical_LG", world_logical, false, 0);

G4Translate3D shiftWindow(0, 0, Rho + ss_height + scint_height_E+ Gap + scint_height_dE + lg_height + window_height/2.0);
auto transformWindow = rotPhi*rotTheta*shiftWindow;
fWindow = new G4PVPlacement(transformWindow, fLogicWindow,"Physical_Window", world_logical, false, 0);

G4Translate3D shiftPMT(0, 0, Rho + ss_height + scint_height_E + Gap + scint_height_dE + lg_height + window_height + cathode_height + pmt_height/2.0);
auto transformPMT = rotPhi*rotTheta*shiftPMT;
fPMT = new G4PVPlacement(transformPMT, fLogicPMT,"Physical_PMT", world_logical, false, 0);



// Conditional Detector Geometry:  Set extended sample geometry
G4double sample_thickness = 0.0*mm;
G4double sample_radius = 0.0*mm;

if (UseExtendedSample) {
  G4double sample_thickness = 1.0*mm;
  G4double sample_radius = 12.5*mm;

  G4RotationMatrix *rotSam = new G4RotationMatrix();
  rotSam->rotateY(0*deg);
  rotSam->rotateZ(0*deg);

  auto fSolidSam = new G4Tubs("Solid_body",0, sample_radius ,sample_thickness/2.0,0,twopi);
  auto fLogicSam = new G4LogicalVolume(fSolidSam,Be,"Logic_Sample",0,0,0);

  new G4PVPlacement(rotSam, G4ThreeVector(0.,0.,Rho-sample_thickness/2.), fLogicSam,"Physical_Sample", world_logical, false, 0);
  fLogicSam->SetVisAttributes(sred);

}



///############## Additional phoswich body components -- Geometry set manually ##############

if (UseTIM_SLICS){ // TIM-based SLICS

  auto fSolidBracket = new G4Tubs("Bracket",bracket_radius_in,bracket_radius_out,bracket_height/2.0,0,twopi);
  auto fLogicBracket = new G4LogicalVolume(fSolidBracket,ssteel,"fLogicBracket",0,0,0);
  G4Translate3D shiftPhosTip0(0, 0, -sample_thickness-bracket_height/2.);
  auto transformPhosTip0 = rotPhi*rotTheta*shiftPhosTip0;
  new G4PVPlacement( transformPhosTip0, fLogicBracket, "physical_bracket", world_logical, false, 0);

  auto fSolidBrass1 = new G4Tubs("Brass1",brass_radius_in1,brass_radius_out1,brass_height_1/2.0,0,twopi);
  auto fLogicBrass1 = new G4LogicalVolume(fSolidBrass1,Brass,"LogicBrass1",0,0,0);
  G4Translate3D shiftPhosTip1(0, 0, -Gap-sample_thickness+brass_height_1/2.);
  auto transformPhosTip1 = rotPhi*rotTheta*shiftPhosTip1;
  new G4PVPlacement( transformPhosTip1, fLogicBrass1, "physical_brass1", world_logical, false, 0);

  auto fSolidBrass2 = new G4Tubs("Brass2",brass_radius_in2,brass_radius_out2,brass_height_2/2.0,0,twopi);
  auto fLogicBrass2 = new G4LogicalVolume(fSolidBrass2,Brass,"LogicBrass2",0,0,0);
  G4Translate3D shiftPhosTip2(0, 0, -Gap-sample_thickness+brass_height_1+brass_height_2/2.0);
  auto transformPhosTip2 = rotPhi*rotTheta*shiftPhosTip2;
  new G4PVPlacement( transformPhosTip2, fLogicBrass2, "physical_brass2", world_logical, false, 0);

  auto fSolidBrass3 = new G4Tubs("Brass3",brass_radius_out2,brass_radius_out3,brass_height_3/2.0,0,twopi);
  auto fLogicBrass3 = new G4LogicalVolume(fSolidBrass3,Brass,"LogicBrass3",0,0,0);
  G4Translate3D shiftPhosTip3(0, 0, -Gap-sample_thickness+brass_height_1+brass_height_2+brass_height_3/2.0);
  auto transformPhosTip3 = rotPhi*rotTheta*shiftPhosTip3;
  new G4PVPlacement( transformPhosTip3, fLogicBrass3, "physical_brass3", world_logical, false, 0);

  auto fSolidSStube = new G4Tubs("SStube",SStube_radius_in,SStube_radius_out,SStube_height/2.0,0,twopi);
  auto fLogicSStube = new G4LogicalVolume(fSolidSStube,ssteel,"LogicSStube_body",0,0,0);
  G4Translate3D shiftPhosTip4(0, 0, brass_height_1 + brass_height_2 + brass_height_3 - Gap - sample_thickness + SStube_height/2.0 );
  auto transformPhosTip4 = rotPhi*rotTheta*shiftPhosTip4;
  new G4PVPlacement( transformPhosTip4, fLogicSStube, "physical_SStube_body", world_logical, false, 0);


  fLogicBrass3->SetVisAttributes(syellow);
  fLogicBrass2->SetVisAttributes(syellow);
  fLogicBrass1->SetVisAttributes(syellow);
  fLogicBracket->SetVisAttributes(sgrey);
}


else{ // Prototype SLICS

  auto fSolidBracket = new G4Tubs("Bracket",brass_radius_in1,brass_radius_in2,bracket_height/2.0,0,twopi);
  auto fLogicBracket = new G4LogicalVolume(fSolidBracket,BunaN,"fLogicBracket",0,0,0);
  G4Translate3D shiftPhosTip0(0, 0, -sample_thickness-bracket_height/2.);
  auto transformPhosTip0 = rotPhi*rotTheta*shiftPhosTip0;
  //new G4PVPlacement( transformPhosTip0, fLogicBracket, "physical_bracket", world_logical, false, 0);

  auto fSolidOring = new G4Tubs("Oring",brass_radius_in1,brass_radius_in2,oring_height/2.0,0,twopi);
  auto fLogicOring = new G4LogicalVolume(fSolidOring,BunaN,"fLogicOring",0,0,0);
  G4Translate3D shiftPhosTip01(0, 0, -sample_thickness-bracket_height-oring_height/2.0);
  auto transformPhosTip01 = rotPhi*rotTheta*shiftPhosTip01;
  //new G4PVPlacement( transformPhosTip01, fLogicOring, "physical_oring", world_logical, false, 0);

  auto fSolidBrass1 = new G4Tubs("Brass1",brass_radius_in1,brass_radius_in2,brass_height_in/2.0,0,twopi);
  auto fLogicBrass1 = new G4LogicalVolume(fSolidBrass1,Brass,"LogicBrass1",0,0,0);
  G4Translate3D shiftPhosTip1(0, 0, -sample_thickness-bracket_height-oring_height-brass_height_in/2.);
  auto transformPhosTip1 = rotPhi*rotTheta*shiftPhosTip1;
  //new G4PVPlacement( transformPhosTip1, fLogicBrass1, "physical_brass1", world_logical, false, 0);

  auto fSolidBrass2 = new G4Tubs("Brass2",brass_radius_in1,brass_radius_out,brass_height_out/2.0,0,twopi);
  auto fLogicBrass2 = new G4LogicalVolume(fSolidBrass2,Brass,"LogicBrass2",0,0,0);
  G4Translate3D shiftPhosTip2(0, 0, -sample_thickness-bracket_height-oring_height-brass_height_in-brass_height_out/2.);
  auto transformPhosTip2 = rotPhi*rotTheta*shiftPhosTip2;
  //new G4PVPlacement( transformPhosTip2, fLogicBrass2, "physical_brass2", world_logical, false, 0);

  auto fSolidSStube = new G4Tubs("SStube",SStube_radius_in,SStube_radius_out,SStube_height/2.0,0,twopi);
  auto fLogicSStube = new G4LogicalVolume(fSolidSStube,ssteel,"LogicSStube_body",0,0,0);
  G4Translate3D shiftPhosTip3(0, 0, SStube_height/2.0 - 0.8*cm);
  auto transformPhosTip3 = rotPhi*rotTheta*shiftPhosTip3;
  //new G4PVPlacement( transformPhosTip3, fLogicSStube, "physical_SStube_body", world_logical, false, 0);

  fLogicBrass2->SetVisAttributes(syellow);
  fLogicBrass1->SetVisAttributes(syellow);
  fLogicBracket->SetVisAttributes(sgrey);

} //End if/else statements UseTIM_SLICS






///############## Phoswich body Using CAD models EXAMPLE #################
/*
auto SS_body = CADMesh::TessellatedMesh::FromOBJ("Parts/PhosBody.obj");
SS_body->SetScale(25.399);
SS_body->SetOffset(-32.8*cm, 10.6*mm, -10*mm); //Offset 1.245 cm
auto SS_body_solid = SS_body->GetSolid();

G4Translate3D shiftPhos(0.9125*cm, 0.0*cm, 1.0*cm);
G4RotateY3D rotThetaP(90*deg);
G4RotateZ3D rotPhiP(0.*deg);
auto transformPhos = rotPhiP*rotThetaP*shiftPhos;

auto SS_body_logical = new G4LogicalVolume( SS_body_solid, ssteel, "logical_phos", 0, 0, 0);
fPhosBody = new G4PVPlacement( transformPhos, SS_body_logical, "physical_phos", world_logical, false, 0);

auto Brass_body = CADMesh::TessellatedMesh::FromOBJ("Parts/BrasTip.obj");
Brass_body->SetScale(24.0);
Brass_body->SetOffset(-309.85*mm+Be_thickness, 10.*mm, -10*mm);
auto Brass_body_solid = Brass_body->GetSolid();

G4Translate3D shiftPhosTip(1.*cm, 0.0*cm, 1.0*cm);
auto transformPhosTip = rotPhiP*rotThetaP*shiftPhosTip;
auto Brass_body_logical = new G4LogicalVolume( Brass_body_solid, Brass, "logical_brass", 0, 0, 0);
fBrass = new G4PVPlacement( transformPhos, Brass_body_logical, "physical_brass", world_logical, false, 0);

auto ring1_body = CADMesh::TessellatedMesh::FromOBJ("Parts/Bracket.obj");
ring1_body->SetScale(25.35);
ring1_body->SetOffset(-328.1*mm+Be_thickness, 10.55*mm, -10*mm);
auto ring1_body_solid = ring1_body->GetSolid();

G4Translate3D shiftPhosring1(1.*cm, 0.0*cm, 1.0*cm);
auto transformPhosring1 = rotPhiP*rotThetaP*shiftPhosring1;
auto Phosring1_logical = new G4LogicalVolume( ring1_body_solid, BunaN, "logical_ring1", 0, 0, 0);
//new G4PVPlacement( transformPhosring1, Phosring1_logical, "physical_Phosring1", world_logical, false, 0);

auto ring2_body = CADMesh::TessellatedMesh::FromOBJ("Parts/Oring.obj");
ring2_body->SetScale(25);
ring2_body->SetOffset(-323.67*mm+sample_thickness, 10.5*mm, -10*mm);
auto ring2_body_solid = ring2_body->GetSolid();

G4Translate3D shiftPhosring2(1.*cm, 0.0*cm, 1.0*cm);
auto transformPhosring2 = rotPhiP*rotThetaP*shiftPhosring2;
auto Phosring2_logical = new G4LogicalVolume( ring2_body_solid, BunaN, "logical_ring2", 0, 0, 0);
//new G4PVPlacement( transformPhosring2, Phosring2_logical, "physical_Phosring2", world_logical, false, 0);

*/


 world_logical->SetVisAttributes(false);
 fLogicEJ200->SetVisAttributes(sblue);
 fLogicGuide->SetVisAttributes(sgreen);
 fLogicWindow->SetVisAttributes(smagneta);


  return world_physical;
}

void DetectorConstruction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fGeomCmd) {
        UseExtendedSample = fGeomCmd->GetNewBoolValue(newValue);
        UseTIM_SLICS = fGeomCmd->GetNewBoolValue(newValue);
    }
}
