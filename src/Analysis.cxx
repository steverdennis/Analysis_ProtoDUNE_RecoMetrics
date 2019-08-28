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

    int mcNuanceCode(0), isCorrectTBHierarchy(0), eventNumber(0); //, isBeamParticle(0), isCosmicRay(0), nNuMatches(0), nCRMatches(0), eventNumber(0);
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryTier(nullptr), *bestMatchPfoPdg(nullptr), *mcPrimaryNHitsTotal(nullptr); //*mcPrimaryNHitsTotal(nullptr), *mcPrimaryTier(nullptr), *bestMatchPfoPdg(nullptr), *bestMatchPfoNHitsTotal(nullptr),*bestMatchPfoNSharedHitsTotal(nullptr);
    std::vector<float> *mcPrimaryVtxZ(nullptr), *mcPrimaryEndZ(nullptr);

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs nTrack and nShower Daughters");
    drawClass_BeamParticleEff.SetRange(0.f, 10.f, 0.f, 1.f);

    DrawClass drawClass_BeamParticleEff2D("Beam Particle Efficiency Vs nTrack and nShower Daughters 2D");
    drawClass_BeamParticleEff2D.SetRange(0.f, 5.f, 0.f, 5.f);

    DrawClass drawClass("nTrknShw");
    drawClass.SetRange(0.f, 15.f, 0.f, 1.f);

    DrawClass drawClass2("nTrknShw_Matched");
    drawClass2.SetRange(0.f, 15.f, 0.f, 1.f);

    const int nBins(20), maxBin(20);

    std::ofstream infoFile;
    infoFile.open("InfoFile.txt");

    for (EventClass &eventClass : eventClassVector)
    {
        int nBeamParticles(0), nBeamParticleMatches(0);
        int nCosmic(0), nCosmicMatches(0);

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

        TChain *pTChain = eventClass.GetTChain();
        pTChain->SetBranchAddress("eventNumber", &eventNumber);
        pTChain->SetBranchAddress("isCorrectTBHierarchy", &isCorrectTBHierarchy);
        pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);
//        pTChain->SetBranchAddress("isBeamParticle", &isBeamParticle);
//        pTChain->SetBranchAddress("isCosmicRay", &isCosmicRay);
//        pTChain->SetBranchAddress("nTargetNuMatches", &nNuMatches);
//        pTChain->SetBranchAddress("nTargetCRMatches", &nCRMatches);

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

//        pTChain->SetBranchAddress("bestMatchPfoNHitsTotal", &bestMatchPfoNHitsTotal);
//        pTChain->SetBranchAddress("bestMatchPfoNSharedHitsTotal", &bestMatchPfoNSharedHitsTotal);

std::string oldFileName("");
int counter(0);

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

const TString fileName(pTChain->GetFile()->GetName());
if (fileName != oldFileName)
{
    counter = 0;
    oldFileName = fileName;
}
if (mcNuanceCode == 2001)
    counter++;

//            const int nMCHits(mcPrimaryNHitsTotal->front());
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

                if (counter == parentCounter)
                    continue;

                const int pdg(mcPrimaryPdg->at(counter));
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

            std::string infoPrint(isCorrectTBHierarchy ? "Correctly Reconstructed" : "Incorrectly Reconstructed");
            infoFile << infoPrint << " " << nTrk << " " << nShw << pTChain->GetFile()->GetName() << " Event Number : " << counter << std::endl;

            pTH1F_MCPrimaries->Fill(nTrk + nShw);
            pTH2F_MCTrkShw->Fill(nTrk, nShw);
/*
            bool isGoodReco(true);
            for (unsigned int counter = 0; counter < bestMatchPfoPdg->size(); counter++)
            {
                const int pfoPdg(bestMatchPfoPdg->at(counter));
                if (pfoPdg == 0)
                {
                    isGoodReco = false;
                }
            }

            if (isGoodReco)
                pTH2F_MCTrkShw_Matched->Fill(nTrk, nShw);
*/

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

        drawClass.AddHisto(pTH1F_MCPrimaries, eventClass.GetDescription());
        drawClass2.AddHisto(pTH1F_MCPrimaries_Matched, eventClass.GetDescription());

        delete pTH1F_MCPrimaries;
        delete pTH1F_MCPrimaries_Matched;
        delete pTH2F_MCTrkShw;
        delete pTH2F_MCTrkShw_Matched;
    }

    drawClass_BeamParticleEff.Draw();
    drawClass_BeamParticleEff2D.Draw();
    drawClass.Draw();
    drawClass2.Draw();
    infoFile.close();
    return 0;
}

//==================================================
