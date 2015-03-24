#include "NtupleMaker/BSM3G_TNT_Maker/interface/MuonSelector.h"

MuonSelector::MuonSelector(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  
  SetBranches();
  _muonToken               = iConfig.getParameter<edm::InputTag>("muons");
  _vertexInputTag          = iConfig.getParameter<edm::InputTag>("vertices");
  _Muon_pt_min             = iConfig.getParameter<double>("Muon_pt_min");
  _Muon_eta_max            = iConfig.getParameter<double>("Muon_eta_max");
  _Muon_vtx_ndof_min       = iConfig.getParameter<int>("Muon_vtx_ndof_min");
  _Muon_vtx_rho_max        = iConfig.getParameter<int>("Muon_vtx_rho_max");
  _Muon_vtx_position_z_max = iConfig.getParameter<double>("Muon_vtx_position_z_max");
}

MuonSelector::~MuonSelector(){
  delete tree_;
}

void MuonSelector::Fill(const edm::Event& iEvent){
  Clear();
  
  int mucoun = 0;
  
  edm::Handle<edm::View<pat::Muon> > muon_h;
  iEvent.getByLabel(_muonToken, muon_h);
  
  edm::Handle<reco::VertexCollection> vtx_h;
  iEvent.getByLabel(_vertexInputTag, vtx_h);
  
  reco::VertexCollection::const_iterator firstGoodVertex = vtx_h->end();
  for (reco::VertexCollection::const_iterator it = vtx_h->begin(); it != firstGoodVertex; it++)
    {
      isGoodVertex(*it);
      firstGoodVertex = it;
      break;
    }
  // require a good vertex 
  if (firstGoodVertex == vtx_h->end()) return;
  
  for(edm::View<pat::Muon>::const_iterator mu = muon_h->begin(); mu != muon_h->end(); mu++){
    if (mu->pt() < _Muon_pt_min || !mu->isLooseMuon()) continue;
    if (fabs(mu->eta()) > _Muon_eta_max) continue;  
    
    reco::TrackRef gtk = mu->globalTrack();
    
    MuonPt.push_back(mu->pt());
    MuonEta.push_back(mu->eta());
    MuonPhi.push_back(mu->phi());
    MuonP.push_back(mu->p());
    
    MuonCharge.push_back(mu->charge());
    if(gtk.isNonnull()){
      MuonChi2.push_back(gtk->normalizedChi2());
      MuonValidHits.push_back(gtk->hitPattern().numberOfValidMuonHits()); 
    } else {                
      MuonChi2.push_back(-9999);
      MuonValidHits.push_back(-9999);
      
    }
    MuonMatchedStat.push_back(mu->numberOfMatchedStations());
    MuonDXY.push_back(mu->muonBestTrack()->dxy(firstGoodVertex->position())); 
    
    if(mu->innerTrack().isNonnull()){
      MuonValidHitsInner.push_back(mu->innerTrack()->hitPattern().numberOfValidPixelHits());
      MuonTLayers.push_back(mu->innerTrack()->hitPattern().trackerLayersWithMeasurement());
    } else {
      MuonValidHitsInner.push_back(-999);
      MuonTLayers.push_back(-999);
    }
    
    MuonDZ.push_back(mu->muonBestTrack()->dz(firstGoodVertex->position()));
    MuonTight.push_back(mu->isTightMuon(*firstGoodVertex));
    MuonSoft.push_back(mu->isSoftMuon(*firstGoodVertex));
    MuonPF.push_back(mu->isPFMuon());   
    MuonEnergy.push_back(mu->energy());
    MuonRel.push_back((mu->trackIso() + mu->ecalIso() + mu->hcalIso()));
    MuonIsoCharParPt.push_back((mu->pfIsolationR04().sumChargedParticlePt));
    MuonIsoCharged.push_back((mu->pfIsolationR04().sumChargedHadronPt));
    MuonIsoNeutralHadron.push_back((mu->pfIsolationR04().sumNeutralHadronEt));
    MuonIsoPhoton.push_back((mu->pfIsolationR04().sumPhotonEt));
    MuonIsoPU.push_back((mu->pfIsolationR04().sumPUPt));
    
    
    mucoun++;
  }
  
  
}

void MuonSelector::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  
  AddBranch(&MuonPt                ,"MuonPt");
  AddBranch(&MuonEta               ,"MuonEta");
  AddBranch(&MuonPhi               ,"MuonPhi");
  AddBranch(&MuonP                 ,"MuonP");
  AddBranch(&MuonEnergy            ,"MuonEnergy");
  AddBranch(&MuonCharge            ,"MuonCharge");
  AddBranch(&MuonChi2              ,"MuonChi2");
  AddBranch(&MuonValidHits         ,"MuonValidHits");
  AddBranch(&MuonValidHitsInner    ,"MuonValidHitsInner");
  AddBranch(&MuonMatchedStat       ,"MuonMatchedStat");
  AddBranch(&MuonDXY               ,"MuonDXY");
  AddBranch(&MuonTLayers           ,"MuonTLayers");
  AddBranch(&MuonDZ                ,"MuonDZ");
  AddBranch(&MuonTight             ,"MuonTight");
  AddBranch(&MuonSoft              ,"MuonSoft");
  AddBranch(&MuonPF                ,"MuonPF");
  AddBranch(&MuonIsoCharged        ,"MuonIsoCharged");
  AddBranch(&MuonRel               ,"MuonRel");
  AddBranch(&MuonIsoCharParPt      ,"MuonIsoCharParPt");
  AddBranch(&MuonIsoNeutralHadron  ,"MuonIsoNeutralHadron");
  AddBranch(&MuonIsoPhoton         ,"MuonIsoPhoton");
  AddBranch(&MuonIsoPU             ,"MuonIsoPU");
  
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

void MuonSelector::Clear(){
  
  MuonPt.clear();
  MuonEta.clear();
  MuonPhi.clear();
  MuonEnergy.clear();
  MuonDZ.clear();
  MuonTight.clear();
  MuonSoft.clear();
  MuonPF.clear();   
  MuonIsoCharged.clear();
  MuonIsoNeutralHadron.clear();
  MuonIsoPhoton.clear();
  MuonIsoPU.clear();
  MuonP.clear(); 
  MuonCharge.clear(); 
  MuonChi2.clear(); 
  MuonValidHits.clear();
  MuonValidHitsInner.clear(); 
  MuonMatchedStat.clear(); 
  MuonDXY.clear(); 
  MuonTLayers.clear(); 
  
}

bool MuonSelector::isGoodVertex(const reco::Vertex& vtx)
{
  if (vtx.isFake()) return false;
  if (vtx.ndof() < _Muon_vtx_ndof_min) return false;
  if (vtx.position().Rho() > _Muon_vtx_rho_max) return false;
  if (fabs(vtx.position().Z()) > _Muon_vtx_position_z_max) return false;
  return true;
}
