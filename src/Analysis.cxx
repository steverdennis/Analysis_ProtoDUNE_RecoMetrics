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
    TApplication *pTApplication = new TApplication("Analysis", &argc,argv);
    if (!pTApplication)
    {
        std::cout << "TApplication problem." << std::endl;
    }

    gROOT->SetBatch();
    Style();

    EventClassVector eventClassVector;
    std::string inputFiles("");

    for (int momentum = 1; momentum <= 7; momentum++)
        inputFiles += "/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/September/ProtoDUNE_HierarchyMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/" + Helper::ToString(momentum) + "GeV/SpaceCharge/RootFiles/*.root:";

    eventClassVector.emplace_back(inputFiles, "Default");

    std::vector<Particle> particles = {POSITRON, PROTON, ANTIMUON, KAONPLUS, PIPLUS};

    int mcNuanceCode(0), isBeamParticle(0), isCosmicRay(0), nNuMatches(0), nCRMatches(0), eventNumber(0);
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryNHitsTotal(nullptr), *bestMatchPfoPdg(nullptr), *bestMatchPfoNHitsTotal(nullptr),*bestMatchPfoNSharedHitsTotal(nullptr);

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs NHits");
    drawClass_BeamParticleEff.SetLogX(true);
    drawClass_BeamParticleEff.SetRange(0.f, 0.f, 0.f, 1.05f);

    DrawClass drawClass_CosmicRayEff("Cosmic Ray Efficiency Vs NHits");
    drawClass_CosmicRayEff.SetLogX(true);
    drawClass_CosmicRayEff.SetRange(0.f, 0.f, 0.f, 1.05f);

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

    DrawClass drawClass_BeamParticleNuMatches("Nu Matches Beam Particle");
    drawClass_BeamParticleNuMatches.SetRange(0.f, 5.f, 0.1f, 10000.f);
    drawClass_BeamParticleNuMatches.SetLogY(true);
    drawClass_BeamParticleNuMatches.Normalise(false);

    DrawClass drawClass_BeamParticleNuMatchesEvent("Nu Matches Beam Particle Event");
    drawClass_BeamParticleNuMatchesEvent.SetRange(0.f, 5.f, 0.f, 5.f);

    DrawClass drawClass_CosmicRayCRMatches("CR Matches Cosmic Ray");
    drawClass_CosmicRayCRMatches.SetRange(0.f, 5.f, 0.1f, 1e6);
    drawClass_CosmicRayCRMatches.SetLogY(true);
    drawClass_CosmicRayCRMatches.Normalise(false);

    DrawClass drawClass_CosmicRayCRMatchesEvent("CR Matches Cosmic Ray Event");
    drawClass_CosmicRayCRMatchesEvent.SetRange(0.f, 125.f, 0.f, 125.f);
    drawClass_CosmicRayCRMatchesEvent.SquarePlot(true);

    DrawClass drawClass_BeamParticleComp("Beam Particle Completeness");
    drawClass_BeamParticleComp.SetLogY(true);
    drawClass_BeamParticleComp.SetRange(0.f, 1.1f, 0.0001f, 1.05f);

    DrawClass drawClass_BeamParticlePurity("Beam Particle Purity");
    drawClass_BeamParticlePurity.SetLogY(true);
    drawClass_BeamParticlePurity.SetRange(0.f, 1.1f, 0.0001f, 1.05f);

    DrawClass drawClass_CosmicRayComp("Cosmic Ray Completeness");
    drawClass_CosmicRayComp.SetLogY(true);
    drawClass_CosmicRayComp.SetRange(0.f, 1.1f, 0.0001f, 1.05f);

    DrawClass drawClass_CosmicRayPurity("Cosmic Ray Purity");
    drawClass_CosmicRayPurity.SetLogY(true);
    drawClass_CosmicRayPurity.SetRange(0.f, 1.1f, 0.0001f, 1.05f);

    DrawClass drawClass_BeamParticleCompPurity("Beam Particle Completeness Purity");
    drawClass_BeamParticleCompPurity.SetRange(0.f, 1.f, 0.f, 1.f);
    drawClass_BeamParticleCompPurity.SquarePlot(true);

    DrawClass drawClass_CosmicRayCompPurity("Cosmic Ray Completeness Purity");
    drawClass_CosmicRayCompPurity.SetRange(0.f, 1.f, 0.f, 1.f);
    drawClass_CosmicRayCompPurity.SquarePlot(true);

    const int nBins(25), maxBin(4);

    for (EventClass &eventClass : eventClassVector)
    {
        int nBeamParticles(0), nBeamParticleMatches(0);
        int nCosmic(0), nCosmicMatches(0);

        // Beam Efficiency vs NHits
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

            // Beam Completeness
            histName = "BeamParticleCompleteness_" + particleName;
            TH1F *pTH1F_BeamParticleCompleteness_Particle = new TH1F(histName.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticleCompleteness_Particle);
            pTH1F_BeamParticleCompleteness_Particle->GetXaxis()->SetTitle("Completeness");
            pTH1F_BeamParticleCompleteness_Particle->GetYaxis()->SetTitle("Fraction of Events");
            pTH1F_BeamParticleCompleteness_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamParticleCompleteness.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticleCompleteness_Particle));

            // Beam Purity
            histName = "BeamParticlePurity_" + particleName;
            TH1F *pTH1F_BeamParticlePurity_Particle = new TH1F(histName.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticlePurity_Particle);
            pTH1F_BeamParticlePurity_Particle->GetXaxis()->SetTitle("Purity");
            pTH1F_BeamParticlePurity_Particle->GetYaxis()->SetTitle("Fraction of Events");
            pTH1F_BeamParticlePurity_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamParticlePurity.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticlePurity_Particle));
        }

        // Cosmic Efficiency vs NHits
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

        // Beam Completeness
        TH1F *pTH1F_BeamParticleCompleteness = new TH1F("BeamParticleCompleteness", "", nBins, 0, 1.f);
        Helper::Format(pTH1F_BeamParticleCompleteness);
        pTH1F_BeamParticleCompleteness->GetXaxis()->SetTitle("Completeness");
        pTH1F_BeamParticleCompleteness->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_BeamParticleCompleteness->SetLineColor(kBlue);

        // Beam Purity
        TH1F *pTH1F_BeamParticlePurity = new TH1F("BeamParticlePurity", "", nBins, 0, 1.f);
        Helper::Format(pTH1F_BeamParticlePurity);
        pTH1F_BeamParticlePurity->GetXaxis()->SetTitle("Purity");
        pTH1F_BeamParticlePurity->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_BeamParticlePurity->SetLineColor(kBlue);

        // Cosmic Completeness
        TH1F *pTH1F_CosmicRayCompleteness = new TH1F("CosmicRayCompleteness", "", nBins, 0, 1.f); 
        Helper::Format(pTH1F_CosmicRayCompleteness);
        pTH1F_CosmicRayCompleteness->GetXaxis()->SetTitle("Completeness");
        pTH1F_CosmicRayCompleteness->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_CosmicRayCompleteness->SetLineColor(kBlue);

        // Cosmic Purity
        TH1F *pTH1F_CosmicRayPurity = new TH1F("CosmicRayPurity", "", nBins, 0, 1.f); 
        Helper::Format(pTH1F_CosmicRayPurity);
        pTH1F_CosmicRayPurity->GetXaxis()->SetTitle("Purity");
        pTH1F_CosmicRayPurity->GetYaxis()->SetTitle("Fraction of Events");
        pTH1F_CosmicRayPurity->SetLineColor(kBlue);

        // Beam #Nu Matches
        TH1F *pTH1F_BeamParticleNuMatches = new TH1F("BeamParticleNuMatches", "", 5, 0.f, 5.f);
        Helper::Format(pTH1F_BeamParticleNuMatches);
        pTH1F_BeamParticleNuMatches->GetXaxis()->SetTitle("Number of Nu Matches");
        pTH1F_BeamParticleNuMatches->GetYaxis()->SetTitle("Entries");

        TH2F *pTH2F_BeamParticleNuMatchesEvent = new TH2F("BeamParticleNuMatchesEvent", "", 5, 0.f, 5.f, 5, 0.f, 5.f);
        Helper::Format(pTH2F_BeamParticleNuMatchesEvent);
        pTH2F_BeamParticleNuMatchesEvent->GetXaxis()->SetTitle("Number of Beam Particles");
        pTH2F_BeamParticleNuMatchesEvent->GetYaxis()->SetTitle("Number of Nu Matches");

        // Cosmic #CR Matches
        TH1F *pTH1F_CosmicRayCRMatches = new TH1F("CosmicRayCRMatches", "", 5, 0.f, 5.f);
        Helper::Format(pTH1F_CosmicRayCRMatches);
        pTH1F_CosmicRayCRMatches->GetXaxis()->SetTitle("Number of CR Matches");
        pTH1F_CosmicRayCRMatches->GetYaxis()->SetTitle("Entries");

        TH2F *pTH2F_CosmicRayCRMatchesEvent = new TH2F("CosmicRayCRMatchesEvent", "", 125, 0.f, 125.f, 125, 0.f, 125.f);
        Helper::Format(pTH2F_CosmicRayCRMatchesEvent);
        pTH2F_CosmicRayCRMatchesEvent->GetXaxis()->SetTitle("Number of Cosmic-Rays");
        pTH2F_CosmicRayCRMatchesEvent->GetYaxis()->SetTitle("Number of Reconstructed Cosmic-Ray");

        // Beam Completeness Purity
        TH2F *pTH2F_BeamParticleCompletenessPurity = new TH2F("BeamParticleCompletenessPurity","", 20, 0, 1, 20, 0, 1);
        Helper::Format(pTH2F_BeamParticleCompletenessPurity);
        pTH2F_BeamParticleCompletenessPurity->GetXaxis()->SetTitle("Purity");
        pTH2F_BeamParticleCompletenessPurity->GetYaxis()->SetTitle("Completeness");

        // Cosmic Completeness Purity
        TH2F *pTH2F_CosmicRayCompletenessPurity = new TH2F("CosmicRayCompletenessPurity","", 20, 0, 1, 20, 0, 1);
        Helper::Format(pTH2F_CosmicRayCompletenessPurity);
        pTH2F_CosmicRayCompletenessPurity->GetXaxis()->SetTitle("Purity");
        pTH2F_CosmicRayCompletenessPurity->GetYaxis()->SetTitle("Completeness");

        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("eventNumber", &eventNumber);
        pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);
        pTChain->SetBranchAddress("isBeamParticle", &isBeamParticle);
        pTChain->SetBranchAddress("isCosmicRay", &isCosmicRay);
        pTChain->SetBranchAddress("nTargetNuMatches", &nNuMatches);
        pTChain->SetBranchAddress("nTargetCRMatches", &nCRMatches);

        pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);
        pTChain->SetBranchAddress("mcPrimaryNHitsTotal", &mcPrimaryNHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoPdg", &bestMatchPfoPdg);
        pTChain->SetBranchAddress("bestMatchPfoNHitsTotal", &bestMatchPfoNHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoNSharedHitsTotal", &bestMatchPfoNSharedHitsTotal);

        int previousEventNumber(0);
        int nNuMatchesEvent(0), nCrMatchesEvent(0), nBeamParticlesEvent(0), nCosmicRayEvent(0);

        //int nEntries(10000);
        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            if (eventNumber != previousEventNumber && entry > 0)
            {
                pTH2F_BeamParticleNuMatchesEvent->Fill(nBeamParticlesEvent, nNuMatchesEvent);
                pTH2F_CosmicRayCRMatchesEvent->Fill(nCosmicRayEvent, nCrMatchesEvent);
                nBeamParticlesEvent = 0;
                nNuMatchesEvent = 0;
                nCosmicRayEvent = 0;
                nCrMatchesEvent = 0;
                previousEventNumber = eventNumber;
            }

            if (2001 == mcNuanceCode)
            {
                nBeamParticlesEvent++;
            }
            else if (isCosmicRay)
            {
                nCosmicRayEvent++;
            }

            nNuMatchesEvent += nNuMatches;
            nCrMatchesEvent += nCRMatches;

            if (entry == nEntries - 1)
            {
                pTH2F_BeamParticleNuMatchesEvent->Fill(nBeamParticlesEvent, nNuMatchesEvent);
                pTH2F_CosmicRayCRMatchesEvent->Fill(nCosmicRayEvent, nCrMatchesEvent);
            }

            if (bestMatchPfoPdg->size() != 1)
                continue;

            const int nMCHits(mcPrimaryNHitsTotal->front());

            // Beam - must access via nuance code to get target beam (2001) as opposed to background beam (2000)
            if (2001 == mcNuanceCode)
            {
                nBeamParticles++;

                pTH1F_BeamMCPrimaryNHitsTotal->Fill(nMCHits);
                pTH1F_BeamParticleNuMatches->Fill(nNuMatches);

                Particle particle(Helper::GetParticleType(mcPrimaryPdg->at(0))); // Should check first, but shouldn't break
                TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal.at(particle);
                TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.at(particle);
                pTH1F_BeamMCPrimaryNHitsTotal_Particle->Fill(nMCHits);

                if (nNuMatches > 0)
                {
                    nBeamParticleMatches++;
                    pTH1F_BeamMCPrimaryNHitsTotal_Matched->Fill(nMCHits);
                    pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->Fill(nMCHits);
                }

                float completeness(0.f), purity(0.f);
                if (1 == bestMatchPfoNSharedHitsTotal->size())
                {
                    completeness = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(mcPrimaryNHitsTotal->at(0));
                    pTH1F_BeamParticleCompleteness->Fill(completeness - std::numeric_limits<float>::epsilon());
                    particleToHistMap_BeamParticleCompleteness.at(particle)->Fill(completeness - std::numeric_limits<float>::epsilon());;

                    purity = (float)(bestMatchPfoNSharedHitsTotal->at(0))/(float)(bestMatchPfoNHitsTotal->at(0));
                    pTH1F_BeamParticlePurity->Fill(purity - std::numeric_limits<float>::epsilon());
                    particleToHistMap_BeamParticlePurity.at(particle)->Fill(purity - std::numeric_limits<float>::epsilon());
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
                pTH2F_BeamParticleCompletenessPurity->Fill(purity - std::numeric_limits<float>::epsilon(), completeness - std::numeric_limits<float>::epsilon());
            }
            // Cosmics
            else if (isCosmicRay)
            {
                nCosmic++;

                pTH1F_CosmicMCPrimaryNHitsTotal->Fill(nMCHits);
                pTH1F_CosmicRayCRMatches->Fill(nCRMatches);

                if (nCRMatches > 0)
                {
                    nCosmicMatches++;
                    pTH1F_CosmicMCPrimaryNHitsTotal_Matched->Fill(nMCHits);
                }

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
                pTH2F_CosmicRayCompletenessPurity->Fill(purity - std::numeric_limits<float>::epsilon(), completeness - std::numeric_limits<float>::epsilon());
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

        TGraphErrors *pTGraphErrors_BeamNHitsEfficiency = Helper::MakeEfficiency(pTH1F_BeamMCPrimaryNHitsTotal, pTH1F_BeamMCPrimaryNHitsTotal_Matched, "BeamNHitsEfficiency");
        Helper::Format(pTGraphErrors_BeamNHitsEfficiency);
        pTGraphErrors_BeamNHitsEfficiency->GetXaxis()->SetTitle("Number of Hits");
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetTitle("Efficiency");
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_BeamNHitsEfficiency->GetYaxis()->SetDecimals();
        drawClass_BeamParticleEff.AddGraph(pTGraphErrors_BeamNHitsEfficiency, eventClass.GetDescription() + ", Eff : " + beamEffStr);

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

        TGraphErrors *pTGraphErrors_CosmicNHitsEfficiency = Helper::MakeEfficiency(pTH1F_CosmicMCPrimaryNHitsTotal, pTH1F_CosmicMCPrimaryNHitsTotal_Matched, "CosmicNHitsEfficiency");
        Helper::Format(pTGraphErrors_CosmicNHitsEfficiency);
        pTGraphErrors_CosmicNHitsEfficiency->GetXaxis()->SetTitle("Number of Hits");
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetTitle("Efficiency");
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_CosmicNHitsEfficiency->GetYaxis()->SetDecimals();
        drawClass_CosmicRayEff.AddGraph(pTGraphErrors_CosmicNHitsEfficiency, eventClass.GetDescription() + ", Eff : " + cosmicEffStr);

        drawClass_BeamParticleComp.AddHisto(pTH1F_BeamParticleCompleteness, eventClass.GetDescription());
        drawClass_BeamParticlePurity.AddHisto(pTH1F_BeamParticlePurity, eventClass.GetDescription());

        drawClass_CosmicRayComp.AddHisto(pTH1F_CosmicRayCompleteness, eventClass.GetDescription());
        drawClass_CosmicRayPurity.AddHisto(pTH1F_CosmicRayPurity, eventClass.GetDescription());

        drawClass_BeamParticleNuMatches.AddHisto(pTH1F_BeamParticleNuMatches, eventClass.GetDescription());
        drawClass_BeamParticleNuMatchesEvent.Add2DHisto(pTH2F_BeamParticleNuMatchesEvent, eventClass.GetDescription());
        drawClass_CosmicRayCRMatches.AddHisto(pTH1F_CosmicRayCRMatches, eventClass.GetDescription());
        drawClass_CosmicRayCRMatchesEvent.Add2DHisto(pTH2F_CosmicRayCRMatchesEvent, eventClass.GetDescription());

        drawClass_BeamParticleCompPurity.Add2DHisto(pTH2F_BeamParticleCompletenessPurity, eventClass.GetDescription());
        drawClass_CosmicRayCompPurity.Add2DHisto(pTH2F_CosmicRayCompletenessPurity, eventClass.GetDescription());

        delete pTH1F_BeamMCPrimaryNHitsTotal;
        delete pTH1F_BeamMCPrimaryNHitsTotal_Matched;
        delete pTH1F_CosmicMCPrimaryNHitsTotal;
        delete pTH1F_CosmicMCPrimaryNHitsTotal_Matched;
        delete pTGraphErrors_BeamNHitsEfficiency;
        delete pTGraphErrors_CosmicNHitsEfficiency;
        delete pTH1F_BeamParticleCompleteness;
        delete pTH1F_BeamParticlePurity;
        delete pTH1F_CosmicRayCompleteness;
        delete pTH1F_CosmicRayPurity;
        delete pTH1F_BeamParticleNuMatches;
        delete pTH2F_BeamParticleNuMatchesEvent;
        delete pTH1F_CosmicRayCRMatches;
        delete pTH2F_CosmicRayCRMatchesEvent;
        delete pTH2F_BeamParticleCompletenessPurity;
        delete pTH2F_CosmicRayCompletenessPurity;

        for (const auto &iter : particleToHistMap_BeamMCPrimaryNHitsTotal)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamMCPrimaryNHitsTotal_Matched)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamParticleCompleteness)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamParticlePurity)
            delete iter.second;
    }

    drawClass_BeamParticleEff.Draw();
    drawClass_CosmicRayEff.Draw();
    drawClass_BeamParticleComp.Draw();
    drawClass_BeamParticlePurity.Draw();
    drawClass_CosmicRayComp.Draw();
    drawClass_CosmicRayPurity.Draw();
    drawClass_BeamParticleNuMatches.Draw();
    drawClass_CosmicRayCRMatches.Draw();
    drawClass_BeamParticleCompPurity.Draw();
    drawClass_CosmicRayCompPurity.Draw();
    drawClass_BeamParticleNuMatchesEvent.Draw();
    drawClass_CosmicRayCRMatchesEvent.Draw();

    for (const auto &iter : drawClassMap_BeamParticleEff)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticleComp)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticlePur)
        iter.second.Draw();

    return 0;
}

//==================================================
