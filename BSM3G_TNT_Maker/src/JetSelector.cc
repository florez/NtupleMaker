#include "NtupleMaker/BSM3G_TNT_Maker/interface/JetSelector.h"

JetSelector::JetSelector(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  
  SetBranches();
  jetToken_       = iConfig.getParameter<edm::InputTag>("jets");
  _vertexInputTag = iConfig.getParameter<edm::InputTag>("vertices");
  _Jet_pt_min     = iConfig.getParameter<double>("Jet_pt_min");
}

JetSelector::~JetSelector(){
  delete tree_;
}

void JetSelector::Fill(const edm::Event& iEvent){
  Clear();
  
  edm::Handle<reco::VertexCollection> vtx_h;
  iEvent.getByLabel(_vertexInputTag, vtx_h);
  
  reco::VertexCollection::const_iterator firstGoodVertex = vtx_h->end();
  // require a good vertex 
  
  edm::Handle<pat::JetCollection> jets;                                       
  iEvent.getByLabel(jetToken_, jets);                                         
  
  sumpxForMht = 0.0; 
  sumpyForMht = 0.0;                                           
  sumptForHt  = 0.0;
  
  for (const pat::Jet &j : *jets) {
    sumpxForMht = sumpxForMht - j.px(); 
    sumpyForMht = sumpyForMht - j.py();            
    sumptForHt  = sumptForHt  + j.pt();
  }     
  
  
  for (const pat::Jet &j : *jets) { 
    if (j.pt() < _Jet_pt_min) continue;
    JetPt.push_back(j.pt());         
    JetEta.push_back(j.eta());       
    JetPhi.push_back(j.phi());       
    JetEnergy.push_back(j.energy());
    JetBdisc.push_back(j.bDiscriminator("combinedSecondaryVertexBJetTags"));
    JetMass.push_back(j.mass());
    Jetnh_e.push_back(j.neutralHadronEnergy());                               
    Jetne_e.push_back(j.neutralEmEnergy());                                   
    Jethe_e.push_back(j.chargedHadronEnergy());                               
    JetcEmEnergy.push_back(j.chargedEmEnergy());                              
    JetMuonEnergy.push_back(j.muonEnergy());                                  
    JetEleEnergy.push_back(j.electronEnergy());                               
    JetPhotonEnergy.push_back(j.photonEnergy());                              
    UncorrJetPt.push_back(j.correctedJet("Uncorrected").pt());                
    
  } 
  
}

void JetSelector::SetBranches(){
  
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&JetPt,           "JetPt");
  AddBranch(&JetEta,          "JetEta");
  AddBranch(&JetPhi,          "JetPhi");
  AddBranch(&JetEnergy,       "JetEnergy");
  AddBranch(&JetBdisc,        "JetBdisc");
  AddBranch(&JetMass,         "JetMass");
  AddBranch(&Jetnh_e,         "Jetnh_e");
  AddBranch(&Jetne_e,         "Jetne_e");
  AddBranch(&Jethe_e,         "Jethe_e");
  AddBranch(&JetcEmEnergy,    "JetcEmEnergy");
  AddBranch(&JetMuonEnergy,   "JetMuonEnergy");
  AddBranch(&JetEleEnergy,    "JetEleEnergy");
  AddBranch(&JetPhotonEnergy, "JetPhotonEnergy");
  AddBranch(&UncorrJetPt,     "UncorrJetPt");
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void JetSelector::Clear(){
  
  JetPt.clear();
  JetEta.clear();
  JetPhi.clear();
  JetEnergy.clear();
  JetBdisc.clear();
  JetMass.clear();
  Jetnh_e.clear();
  Jetne_e.clear();
  Jethe_e.clear();
  JetcEmEnergy.clear();
  JetMuonEnergy.clear();
  JetEleEnergy.clear();
  JetPhotonEnergy.clear();
  UncorrJetPt.clear();
  
}
