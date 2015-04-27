// -*- C++ -*-
//
// Package:    MiniAOD/BSM3G_TNT_Maker
// Class:      BSM3G_TNT_Maker
// 
//
// Authors:  Andres Florez: Universidad de los Andes, Colombia. 
// kaur amandeepkalsi: Panjab University, India. 
//

#ifndef  TREE_MAKER_H                                                                                                                                    
#define  TREE_MAKER_H


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/MuonSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/ElectronPatSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/TauSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/JetSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/PVSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/GenParticleSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/METSelector.h"
#include "NtupleMaker/BSM3G_TNT_Maker/interface/PhotonSelector.h"


#ifdef __MAKECINT__
#pragma link C++ class std::vector<std::vector<int> >+;
#pragma link C++ class std::vector<std::vector<std::string> >+;
#pragma link C++ class std::vector<std::vector<TString> >+;
#pragma link C++ class std::vector<std::vector<float> >+;
#pragma link C++ class std::vector<std::vector<double> >+;
#pragma link C++ class std::vector<std::vector<bool> >+;
#pragma extra_include "std::vector";
#endif
//
// class declaration
//

class BSM3G_TNT_Maker : public edm::EDAnalyzer {
public:
  explicit BSM3G_TNT_Maker(const edm::ParameterSet&);
  ~BSM3G_TNT_Maker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void clearvectors();
  
  // ----------member data ---------------------------
  
  TFile* file;
  TTree* tree_;
  const size_t MaxN;
  bool debug_;
  
  bool _fillmuoninfo;
  bool _fillelectronpatinfo;
  bool _filltauinfo;
  bool _filljetinfo;
  bool _fillgeninfo;
  bool _fillPVinfo;
  bool _fillMETinfo;
  bool _fillphotoninfo;
  
  MuonSelector        *muselector;
  ElectronPatSelector *elpatselector;
  TauSelector         *tauselector;
  JetSelector         *jetselector;
  GenParticleSelector *genselector;
  PVSelector          *pvselector;
  METSelector         *metselector;
  PhotonSelector      *photonselector;
};

#endif 
  
  
