/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/////////////////////////////////////////////////////////////////
// SkimmingToolExample.cxx, (c) ATLAS Detector software
///////////////////////////////////////////////////////////////////
// Author: James Catmore (James.Catmore@cern.ch)
// Based on the Integrated Simulation Framework
// This is a trivial example of an implementation of a skimming tool 
// which only passes events with N combined muons passing a pt cut of M

#include "DerivationFrameworkExamples/SkimmingToolExample.h"
#include <vector>
#include <string>

// Constructor
DerivationFramework::SkimmingToolExample::SkimmingToolExample( const std::string& t,
                                                 const std::string& n,
                                                 const IInterface* p ) : 
  AthAlgTool(t,n,p),
  m_ntot(0),
  m_npass(0)
  {
    declareInterface<DerivationFramework::ISkimmingTool>(this);
    //declareProperty("MuonContainerKey", m_muonSGKey);
    //declareProperty("NumberOfMuons", m_nMuons);
    //declareProperty("MuonPtCut", m_muonPtCut);	
  }
  
// Destructor
DerivationFramework::SkimmingToolExample::~SkimmingToolExample() {
}  

// Athena initialize and finalize
StatusCode DerivationFramework::SkimmingToolExample::initialize()
{
     ATH_MSG_VERBOSE("initialize() ...");
     return StatusCode::SUCCESS;
}
StatusCode DerivationFramework::SkimmingToolExample::finalize()
{
     ATH_MSG_VERBOSE("finalize() ...");
     ATH_MSG_INFO("Processed "<< m_ntot <<" events, "<< m_npass<<" events passed filter ");
     return StatusCode::SUCCESS;
}

// The filter itself
bool DerivationFramework::SkimmingToolExample::eventPassesFilter() const
{
     ++m_ntot;

     // Retrieve track container	
     const xAOD::TrackParticleContainer* regularTracks(0);
     CHECK(evtStore()->retrieve(regularTracks,"InDetTrackParticles"),"InDetTrackParticles");

     const xAOD::TrackParticleContainer* largeD0Tracks(0);
     CHECK(evtStore()->retrieve(largeD0Tracks,"InDetLargeD0TrackParticles"),"InDetLargeD0TrackParticles");

     ATH_MSG_INFO("A.X.: Regular particles: " << regularTracks->size());
     ATH_MSG_INFO("A.X.: LargeD0 particles: " << largeD0Tracks->size());

     // Combine two containers
     auto allTracks = std::make_unique<xAOD::TrackParticleContainer>();
     auto allTracksAux = std::make_unique<xAOD::TrackParticleAuxContainer>();
     allTracks->setStore(allTracksAux.get());

     for (auto trkItr : *regularTracks) {
	xAOD::TrackParticle* track = new xAOD::TrackParticle();
	allTracks->push_back(track);
	*track = *trkItr;
     }
     for (auto trkItr : *largeD0Tracks) {
	xAOD::TrackParticle* track = new xAOD::TrackParticle();
	allTracks->push_back(track);
	*track = *trkItr;
     }

     ATH_MSG_INFO("A.X.: Combined particles: " << allTracks->size());

     CHECK(evtStore()->record(allTracks.release(), "InDetCombinedTrackParticles"),"InDetCombinedTrackParticles");
     CHECK(evtStore()->record(allTracksAux.release(), "InDetCombinedTrackParticlesAux."),"InDetCombinedTrackParticlesAux.");

     // everyone passes:)
     ++m_npass;
     return true;
}  
  
