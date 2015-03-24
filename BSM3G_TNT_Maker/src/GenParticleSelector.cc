#include "NtupleMaker/BSM3G_TNT_Maker/interface/GenParticleSelector.h"

GenParticleSelector::GenParticleSelector(std::string name, TTree* tree, bool debug, const pset& iConfig):baseTree(name,tree,debug){
  if(debug) std::cout<<"in GenParticleSelector constructor"<<std::endl;
  if(debug) std::cout<<"in pileup constructor: calling SetBrances()"<<std::endl;
  SetBranches();
}

GenParticleSelector::~GenParticleSelector(){
  delete tree_;
}

void GenParticleSelector::Fill(const edm::Event& iEvent){
  Clear(); 
  if(debug_)    std::cout<<"getting gen particle info"<<std::endl;
  
  Handle< reco::GenParticleCollection > _genParticles;
  iEvent.getByLabel("prunedGenParticles", _genParticles);
  
  std::cout<<"muons:"<<std::endl;
  // getting muons
  for ( reco::GenParticleCollection::const_iterator genparticles = _genParticles->begin(); 
        genparticles !=  _genParticles->end(); ++genparticles){
    
    genParticlept.push_back(genparticles->pt());
    genParticleeta.push_back(genparticles->eta()); 
    genParticlephi.push_back(genparticles->phi());
    genParticlestatus.push_back(genparticles->status());
    genParticlepdg_id.push_back(genparticles->pdgId());
    genParticlemotherpdg_id.push_back(genparticles->numberOfMothers() > 0 ? genparticles->mother(0)->pdgId() : -999999);
    genParticlep.push_back(genparticles->p());
    genParticleenergy.push_back(genparticles->energy());
    genParticlevx.push_back(genparticles->vx());
    genParticlevy.push_back(genparticles->vy());
    genParticlevz.push_back(genparticles->vz());
    genParticlecharge.push_back(genparticles->charge());
    
    genParticlenumDaught.push_back(genparticles->numberOfDaughters());
    genParticlenumMother.push_back(genparticles->numberOfMothers());
    
    int idx = -1;
    for (reco::GenParticleCollection::const_iterator mit = _genParticles->begin();mit != _genParticles->end(); ++mit ) {
      if ( genparticles->mother() == &(*mit) ) {
	idx = std::distance(_genParticles->begin(),mit);
	break;
      }
    }
    
    genParticleBmotherIndex = idx;
    
    for (size_t j = 0; j < genparticles->numberOfMothers(); ++j) {
      const reco::Candidate* m = genparticles->mother(j);
      for (reco::GenParticleCollection::const_iterator mit = _genParticles->begin();  mit != _genParticles->end(); ++mit) {
	if (m == &(*mit) ) { 
	  int idx = std::distance(_genParticles->begin(), mit);
	  genParticleBmotherIndices.push_back(idx);
	  break;
	}
	
      }
    }
    
    for (size_t j = 0; j < genparticles->numberOfDaughters(); ++j) {
      const reco::Candidate* d = genparticles->daughter(j);
      for (reco::GenParticleCollection::const_iterator mit = _genParticles->begin();  mit != _genParticles->end(); ++mit) {
	if (d == &(*mit) ) { 
	  int idx = std::distance(_genParticles->begin(), mit);
	  genParticleBdaughtIndices.push_back(idx);
	  break;
	}
      }
    }
    
  }
  if(debug_)    std::cout<<"got gen particle  info"<<std::endl;
}

void GenParticleSelector::SetBranches(){
  if(debug_)    std::cout<<"setting branches: calling AddBranch of baseTree"<<std::endl;
  AddBranch(&genParticlept               ,"genParticlept");
  AddBranch(&genParticleeta              ,"genParticleeta");
  AddBranch(&genParticlephi              ,"genParticlephi");
  AddBranch(&genParticlestatus           ,"genParticlestatus");
  AddBranch(&genParticlepdg_id           ,"genParticlepdg_id");
  AddBranch(&genParticlemotherpdg_id     ,"genParticlemotherpdg_id");
  AddBranch(&genParticlep                ,"genParticlep");
  AddBranch(&genParticleenergy           ,"genParticleenergy");
  AddBranch(&genParticlevx               ,"genParticlevx");
  AddBranch(&genParticlevy               ,"genParticlevy");
  AddBranch(&genParticlevz               ,"genParticlevz");
  AddBranch(&genParticlecharge           ,"genParticlecharge");
  AddBranch(&genParticlenumDaught        ,"genParticlenumDaught");
  AddBranch(&genParticlenumMother        ,"genParticlenumMother");
  AddBranch(&genParticleBmotherIndices   ,"genParticleBmotherIndices");
  AddBranch(&genParticleBdaughtIndices   ,"genParticleBdaughtIndices");
  AddBranch(&genParticleBmotherIndex     ,"genParticleBmotherIndex");
  if(debug_)    std::cout<<"set branches genparticle"<<std::endl;
}

void GenParticleSelector::Clear(){
  
  genParticlept.clear();
  genParticleeta.clear();
  genParticlephi.clear();
  genParticlestatus.clear();
  genParticlepdg_id.clear();
  genParticlemotherpdg_id.clear();
  genParticlep.clear();
  genParticleenergy.clear();
  genParticlevx.clear();
  genParticlevy.clear();
  genParticlevz.clear();
  genParticlecharge.clear();
  genParticlenumDaught.clear();
  genParticlenumMother.clear();
  genParticleBmotherIndices.clear();
  genParticleBdaughtIndices.clear();
  genParticleBmotherIndex = 0;
  
}
