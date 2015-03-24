#include "NtupleMaker/BSM3G_TNT_Maker/interface/ElectronPatSelector.h"

ElectronPatSelector::ElectronPatSelector(std::string name, TTree* tree, bool debug, const pset& iConfig, edm::ConsumesCollector && ic): 
  baseTree(name,tree,debug),
  electronVetoIdMapToken_(ic.consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("electronVetoIdMap"))),
  electronLooseIdMapToken_(ic.consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("electronLooseIdMap"))),
  electronMediumIdMapToken_(ic.consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("electronMediumIdMap"))),
  electronTightIdMapToken_(ic.consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("electronTightIdMap")))
{
  
  SetBranches();
  _vertexInputTag          = iConfig.getParameter<edm::InputTag>("vertices");
  _patElectronToken      = iConfig.getParameter<edm::InputTag>("patElectrons");
  _patElectron_pt_min    = iConfig.getParameter<double>("patElectron_pt_min");
  _patElectron_eta_max   = iConfig.getParameter<double>("patElectron_eta_max");
}

ElectronPatSelector::~ElectronPatSelector(){
  delete tree_;
}

void ElectronPatSelector::Fill(const edm::Event& iEvent){
  Clear();
  
  int elcoun = 0;
  
  edm::Handle<reco::VertexCollection> vtx;
  iEvent.getByLabel(_vertexInputTag, vtx);
  if (vtx->empty()) return;
  
  edm::Handle<edm::View<pat::Electron> > electron_pat;
  iEvent.getByLabel(_patElectronToken, electron_pat);
  
  edm::Handle<edm::ValueMap<bool> > veto_id_decisions;
  edm::Handle<edm::ValueMap<bool> > loose_id_decisions;
  edm::Handle<edm::ValueMap<bool> > medium_id_decisions;
  edm::Handle<edm::ValueMap<bool> > tight_id_decisions;
  
  iEvent.getByToken(electronVetoIdMapToken_,veto_id_decisions);
  iEvent.getByToken(electronLooseIdMapToken_,loose_id_decisions);
  iEvent.getByToken(electronMediumIdMapToken_,medium_id_decisions);
  iEvent.getByToken(electronTightIdMapToken_,tight_id_decisions);  
  
  reco::VertexCollection::const_iterator firstGoodVertex = vtx->end();
  for(edm::View<pat::Electron>::const_iterator el = electron_pat->begin(); el != electron_pat->end(); el++){
    if (el->pt() < _patElectron_pt_min) continue;
    if (fabs(el->eta()) > _patElectron_eta_max) continue;  
    
    patElectronPt.push_back(el->pt());
    patElectronEta.push_back(el->eta());
    patElectronPhi.push_back(el->phi());
    patElectronCharge.push_back(el->charge());
    patElectronEnergy.push_back(el->energy());
    
    // Look up the ID decision for this electron in
    // the ValueMap object and store it. We need a Ptr object as the key.
    const Ptr<pat::Electron> elPtr(electron_pat, el - electron_pat->begin() );
    bool isPassVeto   = (*veto_id_decisions)[ elPtr ];
    bool isPassLoose  = (*loose_id_decisions)[ elPtr ];
    bool isPassMedium = (*medium_id_decisions)[ elPtr ];
    bool isPassTight  = (*tight_id_decisions)[ elPtr ];
    
    passVetoId_.push_back( isPassVeto );
    passLooseId_.push_back( isPassLoose );
    passMediumId_.push_back( isPassMedium );
    passTightId_.push_back( isPassTight );
    
    patElectron_d0.push_back( (-1) * el->gsfTrack()->dxy(firstGoodVertex->position() ) );
    patElectron_dz.push_back( el->gsfTrack()->dz( firstGoodVertex->position() ) );
    expectedMissingInnerHits.push_back(el->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS) );
    passConversionVeto_.push_back( el->passConversionVeto() );
    
    // Isolation
    reco::GsfElectron::PflowIsolationVariables pfIso = el->pfIsolationVariables();
    // Compute isolation with delta beta correction for PU
    isoChargedHadrons_.push_back( pfIso.sumChargedHadronPt );
    isoNeutralHadrons_.push_back( pfIso.sumNeutralHadronEt );
    isoPhotons_.push_back( pfIso.sumPhotonEt );
    
    elcoun++;
  }
  
}

void ElectronPatSelector::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  
  AddBranch(&patElectronPt            ,"patElectronPt");
  AddBranch(&patElectronEta           ,"patElectronEta");
  AddBranch(&patElectronPhi           ,"patElectronPhi");
  AddBranch(&patElectronEnergy        ,"patElectronEnergy");
  AddBranch(&patElectronCharge        ,"patElectronCharge");
  AddBranch(&passVetoId_              ,"patElectron_isPassVeto");          
  AddBranch(&passLooseId_             ,"patElectron_isPassLoose");
  AddBranch(&passMediumId_            ,"patElectron_isPassMedium");
  AddBranch(&passTightId_             ,"patElectron_isPassTight");
  AddBranch(&patElectron_d0           ,"patElectron_d0");
  AddBranch(&patElectron_dz           ,"patElectron_dz");
  AddBranch(&expectedMissingInnerHits ,"patElectron_expectedMissingInnerHits");
  AddBranch(&passConversionVeto_      ,"patElectron_passConversionVeto"); 
  AddBranch(&isoChargedHadrons_       ,"patElectron_isoChargedHadrons");
  AddBranch(&isoNeutralHadrons_       ,"patElectron_isoNeutralHadrons");
  AddBranch(&isoPhotons_              ,"patElectron_isoPhotons");
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void ElectronPatSelector::Clear(){
  
  patElectronPt.clear();
  patElectronEta.clear();
  patElectronPhi.clear();
  patElectronEnergy.clear();
  patElectronCharge.clear(); 
  passVetoId_.clear();
  passLooseId_.clear();
  passMediumId_.clear();
  passTightId_.clear();  
  patElectron_d0.clear();
  patElectron_dz.clear();
  expectedMissingInnerHits.clear();
  passConversionVeto_.clear();
  isoChargedHadrons_.clear();
  isoNeutralHadrons_.clear();
  isoPhotons_.clear();
}
