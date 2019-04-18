/**
 *  @file   src/Analysis.cc
 * 
 *  @brief  Implementation of the int main function for the analysis.
 * 
 *  $Log: $
 */

#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>

#include "TApplication.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMarker.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TVector3.h"

#include "DrawClass.h"
#include "Helper.h"
#include "Style.h"
#include "EventClass.h"

//==================================================

using namespace analysis;

int main(int argc, char *argv[])
{
/*
    TApplication *pTApplication = new TApplication("Analysis", &argc,argv);
    if (!pTApplication)
    {
        std::cout << "TApplication problem." << std::endl;
    }
*/
    gROOT->SetBatch();
    Style();

    EventClassVector eventClassVector;
    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/2GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/3GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/4GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/6GeV/SpaceCharge/RootFiles/*.root:/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_RecoMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "Simulation");

    std::vector<Particle> particles = {POSITRON, PROTON, ANTIMUON, KAONPLUS, PIPLUS};

    int mcNuanceCode(0), isBeamParticle(0), isCosmicRay(0), nNuMatches(0), nCRMatches(0), eventNumber(0);
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryNHitsTotal(nullptr), *bestMatchPfoPdg(nullptr), *bestMatchPfoNHitsTotal(nullptr),*bestMatchPfoNSharedHitsTotal(nullptr), *bestMatchPfoIsTestBeam(nullptr);
    std::vector<float> *mcPrimaryPX(nullptr), *mcPrimaryPY(nullptr), *mcPrimaryPZ(nullptr);

    // Beam
    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs NHits");
    drawClass_BeamParticleEff.SetLogX(true);
    drawClass_BeamParticleEff.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_BeamParticleEff.SetLegend(0.2f, 0.5f, 0.6f, 0.775f);

    DrawClass drawClass_BeamParticleEffP("Beam Particle Efficiency Vs Momentum");
    drawClass_BeamParticleEffP.SetRange(0.f, 8.f, 0.f, 1.05f);
    drawClass_BeamParticleEffP.SetLegend(0.5f, 0.8f, 0.3f, 0.5f);

    DrawClass drawClass_BeamParticleComp("Beam Particle Completeness");
//    drawClass_BeamParticleComp.SetLogY(true);
    drawClass_BeamParticleComp.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
    drawClass_BeamParticleComp.SetLegend(0.2f, 0.5f, 0.6f, 0.775f);

    DrawClass drawClass_BeamParticlePurity("Beam Particle Purity");
//    drawClass_BeamParticlePurity.SetLogY(true);
    drawClass_BeamParticlePurity.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
    drawClass_BeamParticlePurity.SetLegend(0.2f, 0.5f, 0.6f, 0.775f);

    // Cosmics
    DrawClass drawClass_CosmicRayEff("Cosmic Ray Efficiency Vs NHits");
    drawClass_CosmicRayEff.SetLogX(true);
    drawClass_CosmicRayEff.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_CosmicRayEff.SetLegend(0.5f, 0.8f, 0.3f, 0.5f);

    DrawClass drawClass_CosmicRayComp("Cosmic Ray Completeness");
//    drawClass_CosmicRayComp.SetLogY(true);
    drawClass_CosmicRayComp.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
    drawClass_CosmicRayComp.SetLegend(0.2f, 0.5f, 0.6f, 0.775f);

    DrawClass drawClass_CosmicRayPurity("Cosmic Ray Purity");
//    drawClass_CosmicRayPurity.SetLogY(true);
    drawClass_CosmicRayPurity.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
    drawClass_CosmicRayPurity.SetLegend(0.2f, 0.5f, 0.6f, 0.775f);

    DrawClass drawClass_CosmicRayCRMatchesEvent("CR Matches Cosmic Ray Event");
    drawClass_CosmicRayCRMatchesEvent.SetRange(0.f, 125.f, 0.f, 125.f);
    drawClass_CosmicRayCRMatchesEvent.SetLegend(0.3f, 0.45f, 0.7f, 0.85f);
    drawClass_CosmicRayCRMatchesEvent.SquarePlot(true);
    drawClass_CosmicRayCRMatchesEvent.SetLeftMargin(0.2);
    drawClass_CosmicRayCRMatchesEvent.SetRightMargin(0.1);
    drawClass_CosmicRayCRMatchesEvent.SetTopMargin(0.15);
    drawClass_CosmicRayCRMatchesEvent.SetBottomMargin(0.15);
/*
    // Beam Particle Species
    typedef std::map<Particle, DrawClass> ParticleToDrawClassMap;
    ParticleToDrawClassMap drawClassMap_BeamParticleEff;
    ParticleToDrawClassMap drawClassMap_BeamParticleComp;
    ParticleToDrawClassMap drawClassMap_BeamParticlePur;

    for (const Particle &particle : particles)
    {
        std::string particleName(Helper::GetParticleName(particle));

        DrawClass drawClass("Beam Particle Efficiency Vs NHits " + particleName);
        drawClass.SetLogX(true);
        drawClass.SetRange(0.f, 0.f, 0.f, 1.05f);
        drawClassMap_BeamParticleEff.insert(ParticleToDrawClassMap::value_type(particle, drawClass));

        DrawClass drawClass_BeamParticleComp_Particle("Beam Particle Completeness" + particleName);
        drawClass_BeamParticleComp_Particle.SetLogY(true);
        drawClass_BeamParticleComp_Particle.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
        drawClassMap_BeamParticleComp.insert(ParticleToDrawClassMap::value_type(particle, drawClass_BeamParticleComp_Particle));

        DrawClass drawClass_BeamParticlePurity_Particle("Beam Particle Purity" + particleName);
        drawClass_BeamParticlePurity_Particle.SetLogY(true);
        drawClass_BeamParticlePurity_Particle.SetRange(0.f, 1.1f, 0.0001f, 1.05f);
        drawClassMap_BeamParticlePur.insert(ParticleToDrawClassMap::value_type(particle, drawClass_BeamParticlePurity_Particle));
    }
*/
    const int nBins(25), maxBin(4);

    for (EventClass &eventClass : eventClassVector)
    {
        int nBeamParticles(0), nBeamParticleMatches(0);
        int nCosmic(0), nCosmicMatches(0);

        // Beam
        TH1F *pTH1F_BeamMCPrimaryNHitsTotal = new TH1F("BeamMCPrimaryNHitsTotal", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal);
        Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal);
        pTH1F_BeamMCPrimaryNHitsTotal->GetXaxis()->SetTitle("Number of Hits");
        pTH1F_BeamMCPrimaryNHitsTotal->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched = new TH1F("BeamMCPrimaryNHitsTotal_Matched", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Matched);
        Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Matched);
        pTH1F_BeamMCPrimaryNHitsTotal_Matched->GetXaxis()->SetTitle("Number of Hits");
        pTH1F_BeamMCPrimaryNHitsTotal_Matched->SetLineColor(kBlue);

        TH1F *pTH1F_BeamMCPrimaryPTotal = new TH1F("BeamMCPrimaryPTotal", "", 8, 0, 8);
        Helper::Format(pTH1F_BeamMCPrimaryPTotal);
        pTH1F_BeamMCPrimaryPTotal->GetXaxis()->SetTitle("Momentum [GeV]");
        pTH1F_BeamMCPrimaryPTotal->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryPTotal_Matched = new TH1F("BeamMCPrimaryPTotal_Matched", "", 8, 0, 8);
        Helper::Format(pTH1F_BeamMCPrimaryPTotal_Matched);
        pTH1F_BeamMCPrimaryPTotal_Matched->GetXaxis()->SetTitle("Momentum [GeV]");
        pTH1F_BeamMCPrimaryPTotal_Matched->SetLineColor(kBlue);

        TH1F *pTH1F_BeamParticleCompleteness = new TH1F("BeamParticleCompleteness", "", nBins, 0, 1.f);
        Helper::Format(pTH1F_BeamParticleCompleteness);
        pTH1F_BeamParticleCompleteness->GetXaxis()->SetTitle("Completeness");
        pTH1F_BeamParticleCompleteness->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_BeamParticleCompleteness->SetLineColor(kBlue);

        TH1F *pTH1F_BeamParticlePurity = new TH1F("BeamParticlePurity", "", nBins, 0, 1.f);
        Helper::Format(pTH1F_BeamParticlePurity);
        pTH1F_BeamParticlePurity->GetXaxis()->SetTitle("Purity");
        pTH1F_BeamParticlePurity->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_BeamParticlePurity->SetLineColor(kBlue);

        // Cosmics
        TH1F *pTH1F_CosmicMCPrimaryNHitsTotal = new TH1F("CosmicMCPrimaryNHitsTotal", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_CosmicMCPrimaryNHitsTotal);
        Helper::BinLogX(pTH1F_CosmicMCPrimaryNHitsTotal);
        pTH1F_CosmicMCPrimaryNHitsTotal->GetXaxis()->SetTitle("Number of Hits");
        pTH1F_CosmicMCPrimaryNHitsTotal->SetLineColor(kRed);

        TH1F *pTH1F_CosmicMCPrimaryNHitsTotal_Matched = new TH1F("CosmicMCPrimaryNHitsTotal_Matched", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_CosmicMCPrimaryNHitsTotal_Matched);
        Helper::BinLogX(pTH1F_CosmicMCPrimaryNHitsTotal_Matched);
        pTH1F_CosmicMCPrimaryNHitsTotal_Matched->GetXaxis()->SetTitle("Number of Hits");
        pTH1F_CosmicMCPrimaryNHitsTotal_Matched->SetLineColor(kBlue);

        TH1F *pTH1F_CosmicRayCompleteness = new TH1F("CosmicRayCompleteness", "", nBins, 0, 1.f); 
        Helper::Format(pTH1F_CosmicRayCompleteness);
        pTH1F_CosmicRayCompleteness->GetXaxis()->SetTitle("Completeness");
        pTH1F_CosmicRayCompleteness->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_CosmicRayCompleteness->SetLineColor(kBlue);

        TH1F *pTH1F_CosmicRayPurity = new TH1F("CosmicRayPurity", "", nBins, 0, 1.f); 
        Helper::Format(pTH1F_CosmicRayPurity);
        pTH1F_CosmicRayPurity->GetXaxis()->SetTitle("Purity");
        pTH1F_CosmicRayPurity->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_CosmicRayPurity->SetLineColor(kBlue);

        TH2F *pTH2F_CosmicRayCRMatchesEvent = new TH2F("CosmicRayCRMatchesEvent", "", 125, 0.f, 125.f, 125, 0.f, 125.f);
        Helper::Format(pTH2F_CosmicRayCRMatchesEvent);
        pTH2F_CosmicRayCRMatchesEvent->GetXaxis()->SetTitle("Number of Cosmic-Ray Muons");
        pTH2F_CosmicRayCRMatchesEvent->GetYaxis()->SetTitle("#splitline{Number of Reconstructed}{Cosmic-Ray Muons}");
/*
        // Beam Particle Species
        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal_Matched;
        ParticleToHistMap particleToHistMap_BeamParticleCompleteness;
        ParticleToHistMap particleToHistMap_BeamParticlePurity;

        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));

            std::string histName("BeamMCPrimaryNHitsTotal_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = new TH1F(histName.c_str(), "", nBins, 0, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            pTH1F_BeamMCPrimaryNHitsTotal_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryNHitsTotal_Particle->SetLineColor(kRed);
            particleToHistMap_BeamMCPrimaryNHitsTotal.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryNHitsTotal_Particle));

            std::string histNameMatched("BeamMCPrimaryNHitsTotal_Matched_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = new TH1F(histNameMatched.c_str(), "", nBins, 0, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle);
            pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle));

            histName = "BeamParticleCompleteness_" + particleName;
            TH1F *pTH1F_BeamParticleCompleteness_Particle = new TH1F(histName.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticleCompleteness_Particle);
            pTH1F_BeamParticleCompleteness_Particle->GetXaxis()->SetTitle("Completeness");
            pTH1F_BeamParticleCompleteness_Particle->GetYaxis()->SetTitle("Fraction of Events");
            pTH1F_BeamParticleCompleteness_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamParticleCompleteness.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticleCompleteness_Particle));

            histName = "BeamParticlePurity_" + particleName;
            TH1F *pTH1F_BeamParticlePurity_Particle = new TH1F(histName.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticlePurity_Particle);
            pTH1F_BeamParticlePurity_Particle->GetXaxis()->SetTitle("Purity");
            pTH1F_BeamParticlePurity_Particle->GetYaxis()->SetTitle("Fraction of Events");
            pTH1F_BeamParticlePurity_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamParticlePurity.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticlePurity_Particle));
        }
*/
        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("eventNumber", &eventNumber);
        pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);
        pTChain->SetBranchAddress("isBeamParticle", &isBeamParticle);
        pTChain->SetBranchAddress("isCosmicRay", &isCosmicRay);
        pTChain->SetBranchAddress("nTargetNuMatches", &nNuMatches);
        pTChain->SetBranchAddress("nTargetCRMatches", &nCRMatches);

        pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);
        pTChain->SetBranchAddress("mcPrimaryNHitsTotal", &mcPrimaryNHitsTotal);
        pTChain->SetBranchAddress("mcPrimaryPX", &mcPrimaryPX);
        pTChain->SetBranchAddress("mcPrimaryPY", &mcPrimaryPY);
        pTChain->SetBranchAddress("mcPrimaryPZ", &mcPrimaryPZ);
        pTChain->SetBranchAddress("bestMatchPfoPdg", &bestMatchPfoPdg);
        pTChain->SetBranchAddress("bestMatchPfoNHitsTotal", &bestMatchPfoNHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoNSharedHitsTotal", &bestMatchPfoNSharedHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoIsTestBeam", &bestMatchPfoIsTestBeam);

        int previousEventNumber(0);
        int nCosmicRayMatchesPerEvent(0), nCosmicRaysPerEvent(0);
        std::vector<int> nCosmicRayMatchesPerEventVec, nCosmicRayPerEventVec;

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            if (eventNumber != previousEventNumber && entry > 0)
            {
                pTH2F_CosmicRayCRMatchesEvent->Fill(nCosmicRaysPerEvent, nCosmicRayMatchesPerEvent);
                nCosmicRayPerEventVec.push_back(nCosmicRaysPerEvent);
                nCosmicRayMatchesPerEventVec.push_back(nCosmicRayMatchesPerEvent);
                nCosmicRaysPerEvent = 0;
                nCosmicRayMatchesPerEvent = 0;
                previousEventNumber = eventNumber;
            }

            if (bestMatchPfoPdg->size() != 1)
                continue;

            const int nMCHits(mcPrimaryNHitsTotal->front());
            const float p(std::sqrt(mcPrimaryPX->front() * mcPrimaryPX->front() + mcPrimaryPY->front() * mcPrimaryPY->front() + mcPrimaryPZ->front() * mcPrimaryPZ->front()));

            if (nMCHits > 100)
                nCosmicRayMatchesPerEvent += nCRMatches;

            // Beam - must access via nuance code to get target beam (2001) as opposed to background beam (2000)
            if (2001 == mcNuanceCode)
            {
                nBeamParticles++;

                pTH1F_BeamMCPrimaryNHitsTotal->Fill(nMCHits);
                pTH1F_BeamMCPrimaryPTotal->Fill(p);
/*
                Particle particle(Helper::GetParticleType(mcPrimaryPdg->at(0))); // Should check first, but shouldn't break
                TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal.at(particle);
                TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.at(particle);
                pTH1F_BeamMCPrimaryNHitsTotal_Particle->Fill(nMCHits);
*/
                if (nNuMatches > 0)
                {
                    nBeamParticleMatches++;
                    pTH1F_BeamMCPrimaryNHitsTotal_Matched->Fill(nMCHits);
                    pTH1F_BeamMCPrimaryPTotal_Matched->Fill(p);
//                    pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->Fill(nMCHits);
                }

                if (bestMatchPfoIsTestBeam->at(0))
                {
                    float completeness(0.f), purity(0.f);
                    if (1 == bestMatchPfoNSharedHitsTotal->size())
                    {
                        completeness = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(mcPrimaryNHitsTotal->at(0));
                        pTH1F_BeamParticleCompleteness->Fill(completeness - std::numeric_limits<float>::epsilon());
//                        particleToHistMap_BeamParticleCompleteness.at(particle)->Fill(completeness - std::numeric_limits<float>::epsilon());;

                        purity = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(bestMatchPfoNHitsTotal->at(0));
                        pTH1F_BeamParticlePurity->Fill(purity - std::numeric_limits<float>::epsilon());
//                        particleToHistMap_BeamParticlePurity.at(particle)->Fill(purity - std::numeric_limits<float>::epsilon());
                    }
                    else if (0 == bestMatchPfoNSharedHitsTotal->size())
                    {
                        pTH1F_BeamParticleCompleteness->Fill(completeness);
                        pTH1F_BeamParticlePurity->Fill(purity);
                    }
                    else
                    {
                        std::cout << "Problem matching beam particle to PFO." << std::endl;
                    }
                }
            }
            // Cosmics
            else if (isCosmicRay)
            {
                nCosmic++;

                if (nMCHits > 100)
                    nCosmicRaysPerEvent++;

                pTH1F_CosmicMCPrimaryNHitsTotal->Fill(nMCHits);

                if (nCRMatches > 0)
                {
                    nCosmicMatches++;
                    pTH1F_CosmicMCPrimaryNHitsTotal_Matched->Fill(nMCHits);
                }

                if (!bestMatchPfoIsTestBeam->at(0))
                {
                    float completeness(0.f), purity(0.f);
                    if (1 == bestMatchPfoNSharedHitsTotal->size())
                    {
                        completeness = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(mcPrimaryNHitsTotal->at(0));
                        pTH1F_CosmicRayCompleteness->Fill(completeness - std::numeric_limits<float>::epsilon());
                        purity = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(bestMatchPfoNHitsTotal->at(0));
                        pTH1F_CosmicRayPurity->Fill(purity - std::numeric_limits<float>::epsilon());
                    }
                    else if (0 == bestMatchPfoNSharedHitsTotal->size())
                    {
                        pTH1F_CosmicRayCompleteness->Fill(completeness);
                        pTH1F_CosmicRayPurity->Fill(purity);
                    }
                    else
                    {
                        std::cout << "Problem matching beam particle to PFO." << std::endl;
                    }
                }
            }
        }

        std::cout << "Event Description    : " << eventClass.GetDescription() << std::endl;
        std::cout << "nBeamParticles       : " << nBeamParticles << std::endl;
        std::cout << "nBeamParticleMatches : " << nBeamParticleMatches << std::endl;
        const float beamParticleEfficiency(static_cast<float>(nBeamParticleMatches)/static_cast<float>(nBeamParticles));
        std::cout << "Efficiency           : " << beamParticleEfficiency << std::endl;
        const float beamParticleEfficiencyError(std::sqrt(beamParticleEfficiency * (1-beamParticleEfficiency) / static_cast<float>(nBeamParticles)));
        std::cout << "Efficiency Error     : " << beamParticleEfficiencyError << std::endl;
        std::cout << "Efficiency [%]       : " << beamParticleEfficiency*100.f << "+-" << beamParticleEfficiencyError*100.f << std::endl;
        const std::string beamEffStr(Helper::ToStringPrecision(beamParticleEfficiency*100.f, 2) + "#pm" + Helper::ToStringPrecision(beamParticleEfficiencyError*100.f, 2) + "%");

        std::cout << "nCosmic              : " << nCosmic << std::endl;
        std::cout << "nCosmicMatches       : " << nCosmicMatches << std::endl;
        const float cosmicRayEfficiency(static_cast<float>(nCosmicMatches)/static_cast<float>(nCosmic));
        std::cout << "Efficiency           : " << cosmicRayEfficiency << std::endl;
        const float cosmicRayEfficiencyError(std::sqrt(beamParticleEfficiency * (1-beamParticleEfficiency) / static_cast<float>(nCosmic)));
        std::cout << "Efficiency Error     : " << cosmicRayEfficiencyError << std::endl;
        std::cout << "Efficiency [%]       : " << cosmicRayEfficiency*100.f << "+-" << cosmicRayEfficiencyError*100.f << std::endl;
        const std::string cosmicEffStr(Helper::ToStringPrecision(cosmicRayEfficiency*100.f, 2) + "#pm" + Helper::ToStringPrecision(cosmicRayEfficiencyError*100.f, 2) + "%");

        float nCosmicRayMatchesPerEventAvg(0.f), nCosmicRayMatchesPerEventStdDev(0.f);
        Helper::GetAverage(nCosmicRayMatchesPerEventVec, nCosmicRayMatchesPerEventAvg, nCosmicRayMatchesPerEventStdDev);
        std::cout << "Cosmic-Ray Muon Matches Per Event : " << nCosmicRayMatchesPerEventAvg << "+-" << nCosmicRayMatchesPerEventStdDev << std::endl;

        float nCosmicRayPerEventAvg(0.f), nCosmicRayPerEventStdDev(0.f);
        Helper::GetAverage(nCosmicRayPerEventVec, nCosmicRayPerEventAvg, nCosmicRayPerEventStdDev);
        std::cout << "Cosmic-Ray Muons Per Event : " << nCosmicRayPerEventAvg << "+-" << nCosmicRayPerEventStdDev << std::endl;

        TGraphErrors *pTGraphErrors_BeamNHitsEfficiency = Helper::MakeEfficiency(pTH1F_BeamMCPrimaryNHitsTotal, pTH1F_BeamMCPrimaryNHitsTotal_Matched, "BeamNHitsEfficiency");
        Helper::Format(pTGraphErrors_BeamNHitsEfficiency);
        pTGraphErrors_BeamNHitsEfficiency->GetXaxis()->SetTitle("Number of Hits");
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetTitle("Reconstruction Efficiency");
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetDecimals();
        drawClass_BeamParticleEff.AddGraph(pTGraphErrors_BeamNHitsEfficiency, eventClass.GetDescription()); // + ", Eff : " + beamEffStr);

        TGraphErrors *pTGraphErrors_BeamPEfficiency = Helper::MakeEfficiency(pTH1F_BeamMCPrimaryPTotal, pTH1F_BeamMCPrimaryPTotal_Matched, "BeamPEfficiency");
        Helper::Format(pTGraphErrors_BeamPEfficiency);
        pTGraphErrors_BeamPEfficiency->GetXaxis()->SetTitle("Momentum [GeV]");
        pTGraphErrors_BeamPEfficiency->GetYaxis()->SetTitle("Reconstruction Efficiency");
        pTGraphErrors_BeamPEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_BeamPEfficiency->GetYaxis()->SetDecimals();
        drawClass_BeamParticleEffP.AddGraph(pTGraphErrors_BeamPEfficiency, eventClass.GetDescription());

        drawClass_BeamParticleComp.AddHisto(pTH1F_BeamParticleCompleteness, eventClass.GetDescription());
        drawClass_BeamParticlePurity.AddHisto(pTH1F_BeamParticlePurity, eventClass.GetDescription());

        TGraphErrors *pTGraphErrors_CosmicNHitsEfficiency = Helper::MakeEfficiency(pTH1F_CosmicMCPrimaryNHitsTotal, pTH1F_CosmicMCPrimaryNHitsTotal_Matched, "CosmicNHitsEfficiency");
        Helper::Format(pTGraphErrors_CosmicNHitsEfficiency);
        pTGraphErrors_CosmicNHitsEfficiency->GetXaxis()->SetTitle("Number of Hits");
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetTitle("Reconstruction Efficiency");
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetDecimals();
        drawClass_CosmicRayEff.AddGraph(pTGraphErrors_CosmicNHitsEfficiency, eventClass.GetDescription()); // + ", Eff : " + cosmicEffStr);
        drawClass_CosmicRayComp.AddHisto(pTH1F_CosmicRayCompleteness, eventClass.GetDescription());
        drawClass_CosmicRayPurity.AddHisto(pTH1F_CosmicRayPurity, eventClass.GetDescription());

        drawClass_CosmicRayCRMatchesEvent.Add2DHisto(pTH2F_CosmicRayCRMatchesEvent, eventClass.GetDescription());
/*
        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));
            TGraphErrors *pTGraphErrors_BeamNHitsEfficiency_Particle  = Helper::MakeEfficiency(particleToHistMap_BeamMCPrimaryNHitsTotal.at(particle), particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.at(particle), "BeamNHitsEfficiency" + particleName);
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetTitle("Efficiency");
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetRangeUser(0,1);
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetDecimals();
            drawClassMap_BeamParticleEff.at(particle).AddGraph(pTGraphErrors_BeamNHitsEfficiency_Particle, eventClass.GetDescription());
            delete pTGraphErrors_BeamNHitsEfficiency_Particle;

            drawClassMap_BeamParticleComp.at(particle).AddHisto(particleToHistMap_BeamParticleCompleteness.at(particle), eventClass.GetDescription());
            drawClassMap_BeamParticlePur.at(particle).AddHisto(particleToHistMap_BeamParticlePurity.at(particle), eventClass.GetDescription());
        }
*/
        delete pTH1F_BeamMCPrimaryNHitsTotal;
        delete pTH1F_BeamMCPrimaryNHitsTotal_Matched;
        delete pTGraphErrors_BeamNHitsEfficiency;
        delete pTH1F_BeamMCPrimaryPTotal;
        delete pTH1F_BeamMCPrimaryPTotal_Matched;
        delete pTGraphErrors_BeamPEfficiency;
        delete pTH1F_BeamParticleCompleteness;
        delete pTH1F_BeamParticlePurity;
        delete pTH1F_CosmicMCPrimaryNHitsTotal;
        delete pTH1F_CosmicMCPrimaryNHitsTotal_Matched;
        delete pTGraphErrors_CosmicNHitsEfficiency;
        delete pTH1F_CosmicRayCompleteness;
        delete pTH1F_CosmicRayPurity;
        delete pTH2F_CosmicRayCRMatchesEvent;
/*
        for (const auto &iter : particleToHistMap_BeamMCPrimaryNHitsTotal)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamMCPrimaryNHitsTotal_Matched)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamParticleCompleteness)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamParticlePurity)
            delete iter.second;
*/
    }

    drawClass_BeamParticleEff.Draw();
    drawClass_BeamParticleEffP.Draw();
    drawClass_BeamParticleComp.Draw();
    drawClass_BeamParticlePurity.Draw();
    drawClass_CosmicRayEff.Draw();
    drawClass_CosmicRayComp.Draw();
    drawClass_CosmicRayPurity.Draw();
    drawClass_CosmicRayCRMatchesEvent.Draw();
/*
    for (const auto &iter : drawClassMap_BeamParticleEff)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticleComp)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticlePur)
        iter.second.Draw();
*/
    return 0;
}

//==================================================
