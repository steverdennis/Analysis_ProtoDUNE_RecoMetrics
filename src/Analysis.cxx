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

    // Particles to make plots for
    std::vector<Particle> particles = {PROTON, PHOTON, PIPLUS, PIMINUS, KAONPLUS, POSITRON};

    // Input files
    EventClassVector eventClassVector;
    std::string inputFiles("");

    for (int momentum = 1; momentum <= 7; momentum++)
        inputFiles += "/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/October/Vertexing_Default/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/" + Helper::ToString(momentum) + "GeV/SpaceCharge/RootFiles/Vertexing_Default_Hierarchy_Job_Number_*.root:";

    eventClassVector.emplace_back(inputFiles, "Default", "ValidationHierarchy");

    inputFiles = "";

    for (int momentum = 1; momentum <= 7; momentum++)
        inputFiles += "/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/October/Vertexing/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/" + Helper::ToString(momentum) + "GeV/SpaceCharge/RootFiles/Vertexing_Hierarchy_Job_Number_*.root:";

    eventClassVector.emplace_back(inputFiles, "Updated Vertexing", "ValidationHierarchy");

    // Tree variables
    int mcNuanceCode(0), isCorrectTBHierarchy(0), nTargetTBHierarchyLosses(0), nTargetTBHierarchySplits(0), nTargetCRMatches(0), nTargetMatches(0), nTargetPrimaries(0), nTargetGoodTBHierarchyMatches(0), nTargetTBHierarchyMatches(0);
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryTier(nullptr), *bestMatchPfoPdg(nullptr), *mcPrimaryNHitsTotal(nullptr), *bestMatchPfoNHitsTotal(nullptr), *bestMatchPfoNSharedHitsTotal(nullptr);
    std::vector<float> *mcPrimaryVtxZ(nullptr), *mcPrimaryEndZ(nullptr);

    float recoVertexX(std::numeric_limits<float>::max()), recoVertexY(std::numeric_limits<float>::max()), recoVertexZ(std::numeric_limits<float>::max());
    float targetVertexX(std::numeric_limits<float>::max()), targetVertexY(std::numeric_limits<float>::max()), targetVertexZ(std::numeric_limits<float>::max());

    // Scale Setting
    const int nBins(20), maxBin(20);
    const int nBinHits(100), maxBinHits(5); // maxBinHits is logX (1e5)

    // Cumulative Plots
    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs nTrack and nShower Daughters");
    drawClass_BeamParticleEff.SetRange(0.f, 10.f, 0.f, 1.f);

    DrawClass drawClass_BeamParticleEff2D("Beam Particle Efficiency Vs nTrack and nShower Daughters 2D");
    drawClass_BeamParticleEff2D.SetRange(0.f, 5.f, 0.f, 5.f);

    DrawClass drawClass("nTrknShw");
    drawClass.SetRange(0.f, 15.f, 0.f, 1.f);

    DrawClass drawClass_Matched("nTrknShw_Matched");
    drawClass_Matched.SetRange(0.f, 15.f, 0.f, 1.f);

    DrawClass drawClass_PrimaryEff("Primary Reco Efficiency Vs NHits");
    drawClass_PrimaryEff.SetLogX(true);
    drawClass_PrimaryEff.SetRange(0.f, maxBinHits, 0.f, 1.05f);
    drawClass_PrimaryEff.SetLegend(0.1,0.9,0.825,0.975);

    DrawClass drawClass_PrimaryComp("Primary Completeness");
    drawClass_PrimaryComp.SetLogY(true);
    drawClass_PrimaryComp.SetRange(0.f, 1.1f, 0.001f, 1.05f);
    drawClass_PrimaryComp.SetLegend(0.1,0.9,0.825,0.975);

    DrawClass drawClass_PrimaryPur("Primary Purity");
    drawClass_PrimaryPur.SetLogY(true);
    drawClass_PrimaryPur.SetRange(0.f, 1.1f, 0.001f, 1.05f);
    drawClass_PrimaryPur.SetLegend(0.1,0.9,0.825,0.975);

    DrawClass drawClass_VtxDeltaR("Vertex Delta R");
    drawClass_VtxDeltaR.SetRange(0.f, 100.f, 0.f, 1.0f);

    for (EventClass &eventClass : eventClassVector)
    {
        // Histograms for a given data set
        TH2F *pTH2F_MCTrkShw = new TH2F("MCTrkShw", "", nBins, 0, maxBin, nBins, 0, maxBin);
        Helper::Format(pTH2F_MCTrkShw);
        pTH2F_MCTrkShw->GetXaxis()->SetTitle("nTrk Daughters");
        pTH2F_MCTrkShw->GetYaxis()->SetTitle("nShw Daughters");

        TH2F *pTH2F_MCTrkShw_Matched = new TH2F("MCTrkShw_Matched", "", nBins, 0, maxBin, nBins, 0, maxBin);
        Helper::Format(pTH2F_MCTrkShw_Matched);
        pTH2F_MCTrkShw_Matched->GetXaxis()->SetTitle("nTrk Daughters");
        pTH2F_MCTrkShw_Matched->GetYaxis()->SetTitle("nShw Daughters");

        TH1F *pTH1F_MCPrimaries = new TH1F("MCPrimaries", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_MCPrimaries);
        pTH1F_MCPrimaries->GetXaxis()->SetTitle("nPrimaries");

        TH1F *pTH1F_MCPrimaries_Matched = new TH1F("MCPrimaries_Matched", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_MCPrimaries_Matched);
        pTH1F_MCPrimaries_Matched->GetXaxis()->SetTitle("nPrimaries");

        TH1F *pTH1F_VtxDeltaR = new TH1F("VtxDeltaR", "", 1000, 0, 100);
        Helper::Format(pTH1F_MCPrimaries_Matched);
        pTH1F_VtxDeltaR->GetXaxis()->SetTitle("Delta R [cm]");

        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal_Matched;
        ParticleToHistMap particleToHistMap_BeamParticleCompleteness;
        ParticleToHistMap particleToHistMap_BeamParticlePurity;

        for (const Particle &particle : particles)
        {
            const std::string particleName(Helper::GetParticleName(particle));

            const std::string histName("BeamMCPrimaryNHitsTotal_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Particle = new TH1F(histName.c_str(), "", nBinHits, 0, maxBinHits);
            Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Particle);
            pTH1F_BeamMCPrimaryNHitsTotal_Particle->GetXaxis()->SetTitle("Number of Hits");
            particleToHistMap_BeamMCPrimaryNHitsTotal.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryNHitsTotal_Particle));

            const std::string histNameMatched("BeamMCPrimaryNHitsTotal_Matched_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle = new TH1F(histNameMatched.c_str(), "", nBinHits, 0, maxBinHits);
            Helper::Format(pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle);
            pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle->GetXaxis()->SetTitle("Number of Hits");
            particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryNHitsTotal_Matched_Particle));

            const std::string histNameComp("BeamParticleCompleteness_" + particleName);
            TH1F *pTH1F_BeamParticleCompleteness_Particle = new TH1F(histNameComp.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticleCompleteness_Particle);
            pTH1F_BeamParticleCompleteness_Particle->GetXaxis()->SetTitle("Completeness");
            pTH1F_BeamParticleCompleteness_Particle->GetYaxis()->SetTitle("Fraction of Events");
            particleToHistMap_BeamParticleCompleteness.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticleCompleteness_Particle));

            const std::string histNamePur("BeamParticlePurity_" + particleName);
            TH1F *pTH1F_BeamParticlePurity_Particle = new TH1F(histNamePur.c_str(), "", nBins, 0, 1.f);
            Helper::Format(pTH1F_BeamParticlePurity_Particle);
            pTH1F_BeamParticlePurity_Particle->GetXaxis()->SetTitle("Purity");
            pTH1F_BeamParticlePurity_Particle->GetYaxis()->SetTitle("Fraction of Events");
            particleToHistMap_BeamParticlePurity.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamParticlePurity_Particle));
        }

        // Load Chain
        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("nTargetTBHierarchyLosses", &nTargetTBHierarchyLosses);
        pTChain->SetBranchAddress("nTargetTBHierarchySplits", &nTargetTBHierarchySplits);
        pTChain->SetBranchAddress("nTargetCRMatches", &nTargetCRMatches);
        pTChain->SetBranchAddress("nTargetMatches", &nTargetMatches);
        pTChain->SetBranchAddress("nTargetPrimaries", &nTargetPrimaries);
        pTChain->SetBranchAddress("nTargetGoodTBHierarchyMatches", &nTargetGoodTBHierarchyMatches);
        pTChain->SetBranchAddress("nTargetTBHierarchyMatches", &nTargetTBHierarchyMatches);
        pTChain->SetBranchAddress("isCorrectTBHierarchy", &isCorrectTBHierarchy);
        pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);

        pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);
        pTChain->SetBranchAddress("mcPrimaryNHitsTotal", &mcPrimaryNHitsTotal);
        pTChain->SetBranchAddress("mcPrimaryTier", &mcPrimaryTier);
        pTChain->SetBranchAddress("mcPrimaryVtxZ", &mcPrimaryVtxZ);
        pTChain->SetBranchAddress("mcPrimaryEndZ", &mcPrimaryEndZ);

        pTChain->SetBranchAddress("bestMatchPfoPdg", &bestMatchPfoPdg);
        pTChain->SetBranchAddress("bestMatchPfoNHitsTotal", &bestMatchPfoNHitsTotal);
        pTChain->SetBranchAddress("bestMatchPfoNSharedHitsTotal", &bestMatchPfoNSharedHitsTotal);

        pTChain->SetBranchAddress("recoVertexX", &recoVertexX);
        pTChain->SetBranchAddress("recoVertexY", &recoVertexY);
        pTChain->SetBranchAddress("recoVertexZ", &recoVertexZ);
        pTChain->SetBranchAddress("targetVertexX", &targetVertexX);
        pTChain->SetBranchAddress("targetVertexY", &targetVertexY);
        pTChain->SetBranchAddress("targetVertexZ", &targetVertexZ);

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            // Only consider test beam interactions
            if (mcNuanceCode != 2001)
                continue;

            const float vtxDeltaX(recoVertexX - targetVertexX);
            const float vtxDeltaY(recoVertexY - targetVertexY);
            const float vtxDeltaZ(recoVertexZ - targetVertexZ);
            const float vtxDeltaR(std::sqrt(vtxDeltaX*vtxDeltaX + vtxDeltaY*vtxDeltaY + vtxDeltaZ*vtxDeltaZ));

            pTH1F_VtxDeltaR->Fill(vtxDeltaR);

            // Find parent
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

            // Veto events if parent produces no hits, or if primary interaction vertex is outside the TPC
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

            // Loop over the primaries
            int nTrk(0), nShw(0);

            for (unsigned int counter = 0; counter < mcPrimaryPdg->size(); counter++)
            {
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

                // Count daughter particle types
                if (std::fabs(pdg) == 11 || pdg == 22)
                {
                    nShw++;
                }
                else
                {
                    nTrk++;
                }
            }

            pTH1F_MCPrimaries->Fill(nTrk + nShw);
            pTH2F_MCTrkShw->Fill(nTrk, nShw);

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
            TGraphErrors *pTGraphErrors_BeamNHitsEfficiency_Particle  = Helper::MakeEfficiency(particleToHistMap_BeamMCPrimaryNHitsTotal.at(particle), particleToHistMap_BeamMCPrimaryNHitsTotal_Matched.at(particle), "BeamNHitsEfficiency" + particleName, 10);
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetTitle("Efficiency");
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetRangeUser(0,1);
            pTGraphErrors_BeamNHitsEfficiency_Particle->GetYaxis()->SetDecimals();

            drawClass_PrimaryEff.AddGraph(pTGraphErrors_BeamNHitsEfficiency_Particle, eventClass.GetDescription() + " " + Helper::GetParticleString(particle));
            drawClass_PrimaryComp.AddHisto(particleToHistMap_BeamParticleCompleteness.at(particle), eventClass.GetDescription() + " " + Helper::GetParticleString(particle));
            drawClass_PrimaryPur.AddHisto(particleToHistMap_BeamParticlePurity.at(particle), eventClass.GetDescription() + " " + Helper::GetParticleString(particle));

            delete pTGraphErrors_BeamNHitsEfficiency_Particle;
        }

        drawClass.AddHisto(pTH1F_MCPrimaries, eventClass.GetDescription());
        drawClass_Matched.AddHisto(pTH1F_MCPrimaries_Matched, eventClass.GetDescription());

        drawClass_VtxDeltaR.AddHisto(pTH1F_VtxDeltaR, eventClass.GetDescription());

        delete pTH1F_MCPrimaries;
        delete pTH1F_MCPrimaries_Matched;
        delete pTH2F_MCTrkShw;
        delete pTH2F_MCTrkShw_Matched;
        delete pTH1F_VtxDeltaR;

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
    drawClass_Matched.Draw();
    drawClass_VtxDeltaR.Draw();

    drawClass_PrimaryEff.Draw();
    drawClass_PrimaryComp.Draw();
    drawClass_PrimaryPur.Draw();

    return 0;
}

//==================================================
