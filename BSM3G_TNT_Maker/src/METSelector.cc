#include "NtupleMaker/BSM3G_TNT_Maker/interface/METSelector.h"

METSelector::METSelector(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in METSelector constructor"<<std::endl;
  metToken_ = iConfig.getParameter<edm::InputTag>("mets");
  if(debug) std::cout<<"in pileup constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

METSelector::~METSelector(){
  delete tree_;
}

void METSelector::Fill(const edm::Event& iEvent){
  
  if(debug_)    std::cout<<"getting met info"<<std::endl;
  
  Clear();
  
  edm::Handle<pat::METCollection> mets;
  iEvent.getByLabel(metToken_, mets);
  const pat::MET &met = mets->front();
  
  MetPt = met.pt();
  MetPx = met.px();
  MetPy = met.py();
  MetPz = met.pz();
  MetPhi = met.phi();
  MetSumEt = met.sumEt();
  genMet = met.genMET()->pt();
  MetShiftedPtUp = met.shiftedPt(pat::MET::JetEnUp);
  MetShiftedPtDown  = met.shiftedPt(pat::MET::JetEnDown);
  
  if(debug_)    std::cout<<"got MET info"<<std::endl;
}

void METSelector::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  
  AddBranch(&MetPt,    "MetPt");
  AddBranch(&MetPx,    "MetPx");
  AddBranch(&MetPy,    "MetPy");
  AddBranch(&MetPz,    "MetPz");
  AddBranch(&MetPhi,  "MetPhi");
  AddBranch(&MetSumEt, "MetSumEt");
  AddBranch(&genMet,   "genMet");
  AddBranch(&MetShiftedPtUp, "MetShiftedPtUp");
  AddBranch(&MetShiftedPtDown, "MetShiftedPtDown");
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}


void METSelector::Clear(){
  MetPt = -9999999999;                                                                                                                                    
  MetPx = -9999999999;
  MetPy = -9999999999;
  MetPz = -9999999999;
  MetPhi  = -9999999999;
  MetSumEt  = -9999999999 ;
  genMet  = -9999999999;
  MetShiftedPtUp  = -9999999999;
  MetShiftedPtDown   = -9999999999;
  
}
