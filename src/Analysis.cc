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

    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag2/mcc11_Pndr_T0/Beam_Cosmics/5GeV/FluidFlow/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 Fluid Flow");
    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag2/mcc11_Pndr_T0/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 Space Charge");
    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag2/mcc11_Pndr_T0/Beam_Cosmics/5GeV/NoSpaceCharge/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 No Space Charge");

//    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag4/mcc11_Pndr_T0/Beam_Cosmics/5GeV/NoSpaceCharge/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 No Space Charge, ApplyT0 Calc");
//    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag4/mcc11_Pndr_T0/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 Space Charge, ApplyT0 Calc");
//    eventClassVector.emplace_back("/r06/dune/sg568/LAr/Jobs/protoDUNE/2019/January/T0_Analysis/AnalysisTag4/mcc11_Pndr_T0/Beam_Cosmics/5GeV/FluidFlow/RootFiles/T0_Analysis_Job_Number_*.root", "MCC11 Fluid Flow, ApplyT0 Calc");


    int isCosmicRay(0), canCosmicBeStitched(0), nMCHitsTPC1(0), nMCHitsTPC2(0);
    float mcT0(0.f);
    std::vector<int> *bestMatchPfoHasBeenStitched(nullptr), *bestMatchPfoHitsTPC1(nullptr), *bestMatchPfoHitsTPC2(nullptr);
    std::vector<float> *bestMatchPfoX0(nullptr);

    DrawClass drawClass_T0Resolution("Cosmic Ray T0 Resolustion");
    DrawClass drawClass_T0("Cosmic Ray T0");
    DrawClass drawClass_T0Reco("Cosmic Ray Reconstructed T0");
    DrawClass drawClass_T0Eff("Cosmic Ray T0 Tagging Efficiency");
    DrawClass drawClass_nHits("Cosmic Ray T0 Split Hits");

    const float cmPerTick(0.0802814);
    const float nsPerTick(500);

    for (EventClass &eventClass : eventClassVector)
    {
        int nMC(0), nReco(0), nFakeStitch(0);

        // Cosmic Ray T0 Resolution
        TH1F *pTH1F_T0Resolution = new TH1F("T0Resolution", "", 200, -20, 20);
        Helper::Format(pTH1F_T0Resolution);
        pTH1F_T0Resolution->GetXaxis()->SetTitle("T_{Reco, 0} - T_{MC, 0} [#mus]");

        TH2F *pTH2F_T0Reconstructed = new TH2F("T0", "", 400, -3000, 1000, 400, -3000, 1000);
        Helper::Format(pTH2F_T0Reconstructed);
        pTH2F_T0Reconstructed->GetXaxis()->SetTitle("T_{MC, 0} [#mus]");
        pTH2F_T0Reconstructed->GetYaxis()->SetTitle("T_{Reco, 0}[#mus]");

        TH1F *pTH1F_T0 = new TH1F("T0", "", 4000, -3000, 1000);
        Helper::Format(pTH1F_T0);
        pTH1F_T0->GetXaxis()->SetTitle("T_{Reco, 0} [#mus]");

        TH2F *pTH2F_T0_nMCHits = new TH2F("T0nMCHits", "", 25, 0, 2500, 25, 0, 2500);
        Helper::Format(pTH2F_T0_nMCHits);
        pTH2F_T0_nMCHits->GetXaxis()->SetTitle("nHits Drift Vol 1");
        pTH2F_T0_nMCHits->GetYaxis()->SetTitle("nHits Drift Vol 2");

        TH2F *pTH2F_T0_nMCHits_Matched = new TH2F("T0nMCHitsMatched", "", 25, 0, 2500, 25, 0, 2500);
        Helper::Format(pTH2F_T0_nMCHits_Matched);
        pTH2F_T0_nMCHits_Matched->GetXaxis()->SetTitle("nHits Drift Vol 1");
        pTH2F_T0_nMCHits_Matched->GetYaxis()->SetTitle("nHits Drift Vol 2");

        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("isCosmicRay", &isCosmicRay);
        pTChain->SetBranchAddress("canCosmicBeStitched", &canCosmicBeStitched);
        pTChain->SetBranchAddress("mcT0", &mcT0);
        pTChain->SetBranchAddress("nMCHitsTPC1", &nMCHitsTPC1);
        pTChain->SetBranchAddress("nMCHitsTPC2", &nMCHitsTPC2);
        pTChain->SetBranchAddress("bestMatchPfoX0", &bestMatchPfoX0);
        pTChain->SetBranchAddress("bestMatchPfoHasBeenStitched", &bestMatchPfoHasBeenStitched);
        pTChain->SetBranchAddress("bestMatchPfoHitsTPC1", &bestMatchPfoHitsTPC1);
        pTChain->SetBranchAddress("bestMatchPfoHitsTPC2", &bestMatchPfoHitsTPC2);

        float deltaT0Sum(0.f);
        int nT0s(0);

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            if (!canCosmicBeStitched && bestMatchPfoHasBeenStitched->size() == 1 && bestMatchPfoHasBeenStitched->front() == 1)
                nFakeStitch++;

            // Cosmics
            if (isCosmicRay)
            {
                if (canCosmicBeStitched)
                {
                    nMC++;
                    pTH2F_T0_nMCHits->Fill(std::max(nMCHitsTPC1, nMCHitsTPC2), std::min(nMCHitsTPC1, nMCHitsTPC2));
                }

                if (bestMatchPfoX0->size() != 1)
                {
                    std::cout << "Multiple X0s for a pfo" << std::endl;
                    continue;
                }

                if (bestMatchPfoX0->front() > 1e10)
                    continue;

                if (canCosmicBeStitched)
                {
                    nReco++;
                    pTH2F_T0_nMCHits_Matched->Fill(std::max(nMCHitsTPC1, nMCHitsTPC2), std::min(nMCHitsTPC1, nMCHitsTPC2));
                }

                const float recoT0(bestMatchPfoX0->front() * nsPerTick / (1000.f * cmPerTick)); // T0 in us
                const float deltaT0(recoT0-(mcT0/1000.f));

                nT0s++;
                deltaT0Sum += deltaT0;
                pTH1F_T0->Fill(recoT0);
                pTH1F_T0Resolution->Fill(deltaT0);
                pTH2F_T0Reconstructed->Fill(mcT0/1000.f, recoT0);
            }
        }

        std::cout << eventClass.GetDescription() << std::endl;
        std::cout << "nReco : " << nReco << ", nMC : " << nMC << std::endl;
        const float integratedEff((float)(nReco) / (float)(nMC));
        std::cout << "Eff : " << integratedEff << std::endl;
        std::cout << "Fake Stitches : " << nFakeStitch << std::endl;

        TH2F *pTH2F_Eff(Helper::MakeEfficiency2D(pTH2F_T0_nMCHits, pTH2F_T0_nMCHits_Matched, "CosmicStitchingEfficiencyVsNHits"));
        pTH2F_Eff->GetXaxis()->SetTitle("Max Pfo Hits in TPC");
        pTH2F_Eff->GetYaxis()->SetTitle("Min Pfo Hits in TPC");
        pTH2F_Eff->GetZaxis()->SetTitle("Stitching Efficiency");

//        const float meanT0Res(deltaT0Sum/static_cast<float>(nT0s));
        const float meanT0Res(pTH1F_T0Resolution->GetMean());
        const float fwhm(Helper::GetFullWidthAtHalfMaximum(pTH1F_T0Resolution));

        const std::string description(", Mean " + Helper::ToStringPrecision(meanT0Res, 2) + " #mus, FWHM " +
            Helper::ToStringPrecision(fwhm, 2) + " #mus, NCosmcis " + Helper::ToStringPrecision(nT0s,0));

        drawClass_T0.AddHisto(pTH1F_T0, eventClass.GetDescription());
        drawClass_T0Resolution.AddHisto(pTH1F_T0Resolution, eventClass.GetDescription() + description);
        drawClass_T0Reco.Add2DHisto(pTH2F_T0Reconstructed, eventClass.GetDescription() + description);

        const std::string effDescription(", Integrated Eff : " + Helper::ToStringPrecision(100.f * integratedEff,3) + "%");
        drawClass_T0Eff.Add2DHisto(pTH2F_Eff, eventClass.GetDescription() + effDescription);

        drawClass_nHits.Add2DHisto(pTH2F_T0_nMCHits, eventClass.GetDescription());

        delete pTH1F_T0;
        delete pTH1F_T0Resolution;
        delete pTH2F_T0Reconstructed;
        delete pTH2F_Eff;
        delete pTH2F_T0_nMCHits;
        delete pTH2F_T0_nMCHits_Matched;
    }

    drawClass_T0.Draw();
    drawClass_T0Resolution.Draw();
    drawClass_T0Reco.Draw();
    drawClass_T0Eff.Draw();
    drawClass_nHits.Draw();
    return 0;
}

//==================================================
