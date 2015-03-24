#include "NtupleMaker/BSM3G_TNT_Maker/interface/PVSelector.h"

PVSelector::PVSelector(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in PVSelector constructor"<<std::endl;
  if(debug) std::cout<<"in pileup constructor: calling SetBrances()"<<std::endl;
  SetBranches();

  _Pvtx_ndof_min   = iConfig.getParameter<double>("Pvtx_ndof_min");
  _Pvtx_vtx_max    = iConfig.getParameter<double>("Pvtx_vtx_max");
  _Pvtx_vtxdxy_max = iConfig.getParameter<double>("Pvtx_vtxdxy_max");
}

PVSelector::~PVSelector(){
  delete tree_;
}

void PVSelector::Fill(const edm::Event& iEvent){
  
  if(debug_)    std::cout<<"getting pileup info"<<std::endl;
  Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(std::string("addPileupInfo"), PupInfo);
  
  edm::Handle<reco::VertexCollection> vtx;
  iEvent.getByLabel("offlineSlimmedPrimaryVertices",vtx);
  
  nBestVtx = 0; 
  for( reco::VertexCollection::const_iterator vtxIt = vtx->begin(); vtxIt!= vtx->end(); ++vtxIt){
    
    if((vtxIt->isValid()) && !(vtxIt->isFake())) {
      
      if(vtxIt->ndof() < _Pvtx_ndof_min) continue; 
      if(abs(vtxIt->z()) >= _Pvtx_vtx_max) continue;
      double vtxdxy = sqrt((vtxIt->x()*vtxIt->x()) + (vtxIt->y()*vtxIt->y() ));
      if(vtxdxy >=  _Pvtx_vtxdxy_max) continue; 
      nBestVtx++;
    }
  }
  
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  ootnpuVertices   = 0;
  npuVertices      = 0;
  npuVerticesm1    = 0;
  npuVerticesp1    = 0;
  trueInteractions = 0.;
  
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
    if(PVI->getBunchCrossing()==  0){ npuVertices   += PVI->getPU_NumInteractions();  }
    if(PVI->getBunchCrossing()== -1){ npuVerticesm1 += PVI->getPU_NumInteractions(); }
    if(PVI->getBunchCrossing()==  1){ npuVerticesp1 += PVI->getPU_NumInteractions(); }
    if(abs(PVI->getBunchCrossing()) >= 2){
      ootnpuVertices += PVI->getPU_NumInteractions();
    }
    
    if(PVI->getBunchCrossing() == 0)trueInteractions = PVI->getTrueNumInteractions();
    
    if(debug_)std::cout << " Pileup Information: bunchXing, nvtx,true nvtx: " << PVI->getBunchCrossing() << " " << PVI->getPU_NumInteractions()<< " "<< PVI->getTrueNumInteractions()<< std::endl;
    
  }//loop over pileup info
  
  if(debug_)    std::cout<<"got pileup info"<<std::endl;
}

void PVSelector::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&ootnpuVertices  ,"ootnpuVertices");
  AddBranch(&npuVertices     ,"npuVertices");
  AddBranch(&npuVerticesm1   ,"npuVerticesm1");
  AddBranch(&npuVerticesp1   ,"npuVerticesp1");
  AddBranch(&trueInteractions,"trueInteractions");
  AddBranch(&nBestVtx        ,"bestVertices"); 
  if(debug_)    std::cout<<"set branches"<<std::endl;
}

