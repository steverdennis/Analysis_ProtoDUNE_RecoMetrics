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
#include "TH2F.h"
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
    gROOT->SetBatch();
    Style();

    EventClassVector eventClassVector;
    std::string inputFiles("");

    for (int momentum = 1; momentum <= 7; momentum++)
        inputFiles += "/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/September/ProtoDUNE_HierarchyMetrics/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/" + Helper::ToString(momentum) + "GeV/SpaceCharge/RootFiles/*.root:";

    eventClassVector.emplace_back(inputFiles, "Default");

    int mcNuanceCode(0), isCorrectTBHierarchy(0), eventNumber(0);
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryTier(nullptr), *bestMatchPfoPdg(nullptr), *mcPrimaryNHitsTotal(nullptr), *bestMatchPfoNHitsTotal(nullptr), *bestMatchPfoNSharedHitsTotal(nullptr);
    std::vector<float> *mcPrimaryVtxZ(nullptr), *mcPrimaryEndZ(nullptr);

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs nTrack and nShower Daughters");
    drawClass_BeamParticleEff.SetRange(0.f, 10.f, 0.f, 1.f);

    DrawClass drawClass_BeamParticleEff2D("Beam Particle Efficiency Vs nTrack and nShower Daughters 2D");
    drawClass_BeamParticleEff2D.SetRange(0.f, 5.f, 0.f, 5.f);

    DrawClass drawClass("nTrknShw");
    drawClass.SetRange(0.f, 15.f, 0.f, 1.f);

    DrawClass drawClass2("nTrknShw_Matched");
    drawClass2.SetRange(0.f, 15.f, 0.f, 1.f);

    std::vector<Particle> particles = {POSITRON, PROTON, ANTIMUON, KAONPLUS, PIPLUS, ELECTRON, ANTIPROTON, MUON, KAONMINUS, PIMINUS, PHOTON};

    typedef std::map<Particle, DrawClass> ParticleToDrawClassMap;
    ParticleToDrawClassMap drawClassMap_BeamParticleEff;
    ParticleToDrawClassMap drawClassMap_BeamParticleComp;
    ParticleToDrawClassMap drawClassMap_BeamParticlePur;

    const int nBins(20), maxBin(20);
    // Log Scale
    const int nBinHits(100), maxBinHits(5);

    for (const Particle &particle : particles)
    {
        std::string particleName(Helper::GetParticleName(particle));

        DrawClass drawClass("Primary Reco Efficiency Vs NHits " + particleName);
        drawClass.SetLogX(true);
        drawClass.SetRange(0.f, maxBinHits, 0.f, 1.05f);
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

    for (EventClass &eventClass : eventClassVector)
    {
        // Beam Efficiency vs NHits
        TH2F *pTH2F_MCTrkShw = new TH2F("MCTrkShw", "", nBins, 0, maxBin, nBins, 0, maxBin);
        Helper::Format(pTH2F_MCTrkShw);
        pTH2F_MCTrkShw->GetXaxis()->SetTitle("nTrk Daughters");
        pTH2F_MCTrkShw->GetYaxis()->SetTitle("nShw Daughters");

        TH2F *pTH2F_MCTrkShw_Matched = new TH2F("MCTrkShw_Matched", "", nBins, 0, maxBin, nBins, 0, maxBin);
        Helper::Format(pTH2F_MCTrkShw_Matched);
        pTH2F_MCTrkShw_Matched->GetXaxis()->SetTitle("nTrk Daughters");
        pTH2F_MCTrkShw_Matched->GetYaxis()->SetTitle("nShw Daughters");

        // Beam Efficiency vs NHits
        TH1F *pTH1F_MCPrimaries = new TH1F("MCPrimaries", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_MCPrimaries);
        pTH1F_MCPrimaries->GetXaxis()->SetTitle("nPrimaries");

        TH1F *pTH1F_MCPrimaries_Matched = new TH1F("MCPrimaries_Matched", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_MCPrimaries_Matched);
        pTH1F_MCPrimaries_Matched->GetXaxis()->SetTitle("nPrimaries");

        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal_Matched;
        ParticleToHistMap particleToHistMap_BeamParticleCompleteness;
        ParticleToHistMap particleToHistMap_BeamParticlePurity;

        for (const Particle &particle : particles)
        {
            const std::string particleName(Helper::GetParticleName(particle));

            std::string histName("BeamMCPrimaryNHitsTotal_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = new TH1F(histName.c_str(), "", nBinHits, 0, maxBinHits);
            Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            pTH1F_BeamMCPrimaryNHitsTotal_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryNHitsTotal_Particle->SetLineColor(kRed);
            particleToHistMap_BeamMCPrimaryNHitsTotal.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryNHitsTotal_Particle));

            const std::string histNameMatched("BeamMCPrimaryNHitsTotal_Matched_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = new TH1F(histNameMatched.c_str(), "", nBinHits, 0, maxBinHits);
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

        TChain *pTChain = eventClass.GetTChain();
        pTChain->SetBranchAddress("eventNumber", &eventNumber);
        pTChain->SetBranchAddress("isCorrectTBHierarchy", &isCorrectTBHierarchy);
        pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);

        pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);
        pTChain->SetBranchAddress("mcPrimaryNHitsTotal", &mcPrimaryNHitsTotal);
        pTChain->SetBranchAddress("mcPrimaryTier", &mcPrimaryTier);
        pTChain->SetBranchAddress("mcPrimaryVtxZ", &mcPrimaryVtxZ);
        pTChain->SetBranchAddress("mcPrimaryEndZ", &mcPrimaryEndZ);

        pTChain->SetBranchAddress("bestMatchPfoPdg", &bestMatchPfoPdg);

        int nTargetTBHierarchyLosses(0), nTargetTBHierarchySplits(0), nTargetCRMatches(0), nTargetMatches(0), nTargetPrimaries(0), nTargetGoodTBHierarchyMatches(0), nTargetTBHierarchyMatches(0);

        pTChain->SetBranchAddress("nTargetTBHierarchyLosses", &nTargetTBHierarchyLosses);
        pTChain->SetBranchAddress("nTargetTBHierarchySplits", &nTargetTBHierarchySplits);
        pTChain->SetBranchAddress("nTargetCRMatches", &nTargetCRMatches);
        pTChain->SetBranchAddress("nTargetMatches", &nTargetMatches);
        pTChain->SetBranchAddress("nTargetPrimaries", &nTargetPrimaries);
        pTChain->SetBranchAddress("nTargetGoodTBHierarchyMatches", &nTargetGoodTBHierarchyMatches);
        pTChain->SetBranchAddress("nTargetTBHierarchyMatches", &nTargetTBHierarchyMatches);

        pTChain->SetBranchAddress("bestMatchPfoNHitsTotal", &bestMatchPfoNHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoNSharedHitsTotal", &bestMatchPfoNSharedHitsTotal);

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            if (mcNuanceCode != 2001)
                continue;

            unsigned int parentCounter(std::numeric_limits<int>::max());
            for (unsigned int counter = 0; counter < mcPrimaryTier->size(); counter++)
            {
                if (mcPrimaryTier->at(counter) == 0)
                {
                    parentCounter = counter;
                    break;
                }
            }

            if (parentCounter > 1e3)
            {
                std::cout << "Missing parent" << std::endl;
                continue;
            }

            const int mcParentPdg(mcPrimaryPdg->at(parentCounter));
            const int mcParentNHits(mcPrimaryNHitsTotal->at(parentCounter));

            if (mcParentNHits < 1)
                continue;

            if (std::fabs(mcParentPdg) == 11 || mcParentPdg == 22)
            {
                if (mcPrimaryVtxZ->at(parentCounter) < 0)
                    continue;
            }
            else
            {
                if (mcPrimaryEndZ->at(parentCounter) < 0)
                    continue;
            }

            int nTrk(0), nShw(0);
            bool skip(false);
            bool newDefinition(true);

            for (unsigned int counter = 0; counter < mcPrimaryPdg->size(); counter++)
            {
                if (bestMatchPfoPdg->at(counter) == 0)
                    newDefinition = false;

                const int pdg(mcPrimaryPdg->at(counter));
                const int nMCHits(mcPrimaryNHitsTotal->at(counter));

                Particle particle(Helper::GetParticleType(mcPrimaryPdg->at(counter))); // Should check first, but shouldn't break

                if (particleToHistMap_BeamMCPrimaryNHitsTotal.find(particle) != particleToHistMap_BeamMCPrimaryNHitsTotal.end())
                {
                    TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal.at(particle);
                    TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.at(particle);

                    pTH1F_BeamMCPrimaryNHitsTotal_Particle->Fill(nMCHits);

                    const float completeness = (float)(bestMatchPfoNSharedHitsTotal->at(counter))/(float)(mcPrimaryNHitsTotal->at(counter));
                    particleToHistMap_BeamParticleCompleteness.at(particle)->Fill(completeness - std::numeric_limits<float>::epsilon());;

                    const float purity = (float)(bestMatchPfoNSharedHitsTotal->at(counter))/(float)(bestMatchPfoNHitsTotal->at(counter));
                    particleToHistMap_BeamParticlePurity.at(particle)->Fill(purity - std::numeric_limits<float>::epsilon());

                    if (bestMatchPfoPdg->at(counter) != 0)
                        pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->Fill(nMCHits);

                }

                if (counter == parentCounter)
                    continue;

                if (std::fabs(pdg) == 11 || pdg == 22)
                {
                    nShw++;
                }
                else
                {
                    nTrk++;
                }

                if (mcPrimaryNHitsTotal->at(counter) < 100)
                    skip = true;
            }

            if (skip) continue;

            pTH1F_MCPrimaries->Fill(nTrk + nShw);
            pTH2F_MCTrkShw->Fill(nTrk, nShw);

//            if (newDefinition && (nTargetGoodTBHierarchyMatches == nTargetTBHierarchyMatches) && (nTargetGoodTBHierarchyMatches >= nTargetPrimaries) && (nTargetCRMatches == 0) && (nTargetTBHierarchySplits == 0) && (nTargetTBHierarchyLosses == 0)) //isCorrectTBHierarchy)
            if (isCorrectTBHierarchy)
            {
                pTH1F_MCPrimaries_Matched->Fill(nTrk + nShw);
                pTH2F_MCTrkShw_Matched->Fill(nTrk, nShw);
            }
       }

        TGraphErrors *pTGraphErrors_RecoEff = Helper::MakeEfficiency(pTH1F_MCPrimaries , pTH1F_MCPrimaries_Matched, "RecoEff");
        Helper::Format(pTGraphErrors_RecoEff);
        pTGraphErrors_RecoEff->GetXaxis()->SetTitle("nPrimaries");
        pTGraphErrors_RecoEff->GetYaxis()->SetDecimals();
        drawClass_BeamParticleEff.AddGraph(pTGraphErrors_RecoEff, eventClass.GetDescription());

        TH2F *pTH2F_RecoEff = Helper::MakeEfficiency2D(pTH2F_MCTrkShw, pTH2F_MCTrkShw_Matched, "RecoEff2D");
        Helper::Format(pTH2F_RecoEff);
        pTH2F_RecoEff->GetXaxis()->SetTitle("nTrk Daughters");
        pTH2F_RecoEff->GetYaxis()->SetTitle("nShw Daughters");
        pTH2F_RecoEff->GetYaxis()->SetDecimals();
        drawClass_BeamParticleEff2D.Add2DHisto(pTH2F_RecoEff, eventClass.GetDescription());

        for (const Particle &particle : particles)
        {
            const std::string particleName(Helper::GetParticleName(particle));
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

        drawClass.AddHisto(pTH1F_MCPrimaries, eventClass.GetDescription());
        drawClass2.AddHisto(pTH1F_MCPrimaries_Matched, eventClass.GetDescription());

        delete pTH1F_MCPrimaries;
        delete pTH1F_MCPrimaries_Matched;
        delete pTH2F_MCTrkShw;
        delete pTH2F_MCTrkShw_Matched;

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
    drawClass_BeamParticleEff2D.Draw();
    drawClass.Draw();
    drawClass2.Draw();

    for (const auto &iter : drawClassMap_BeamParticleEff)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticleComp)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_BeamParticlePur)
        iter.second.Draw();

    return 0;
}

//==================================================
