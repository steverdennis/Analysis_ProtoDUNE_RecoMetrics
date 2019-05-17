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
#include "SpaceChargeHelper.h"

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

    SpaceChargeHelper::Configure("/usera/sg568/LAr/Jobs/protoDUNE/2019/April/BDTVertexing/SpaceChargeCorrections/SCEoffsets_ProtoDUNE_E500.root");

    float recoVertexX(std::numeric_limits<float>::max()), recoVertexY(std::numeric_limits<float>::max()), recoVertexZ(std::numeric_limits<float>::max());
    float recoVertexX2(std::numeric_limits<float>::max()), recoVertexY2(std::numeric_limits<float>::max()), recoVertexZ2(std::numeric_limits<float>::max());
    float targetVertexX(std::numeric_limits<float>::max()), targetVertexY(std::numeric_limits<float>::max()), targetVertexZ(std::numeric_limits<float>::max());
    float targetVertexX2(std::numeric_limits<float>::max()), targetVertexY2(std::numeric_limits<float>::max()), targetVertexZ2(std::numeric_limits<float>::max());
    int mcNuanceCode(std::numeric_limits<int>::max());
    std::vector<int> *mcPrimaryPdg(nullptr);

    TChain *pTChain = new TChain("Validation");
    pTChain->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_BDTVtx_Job_Number_*.root");
    pTChain->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/3GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_BDTVtx_Job_Number_*.root");
    pTChain->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_BDTVtx_Job_Number_*.root");
    pTChain->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_BDTVtx_Job_Number_*.root");

    TChain *pTChain2 = new TChain("Validation");
    pTChain2->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_Default_Job_Number_*.root");
    pTChain2->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/3GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_Default_Job_Number_*.root");
    pTChain2->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_Default_Job_Number_*.root");
    pTChain2->Add("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/ProtoDUNE_RecoMetrics_Default_Job_Number_*.root");

    pTChain->SetBranchAddress("mcNuanceCode", &mcNuanceCode);
    pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);

    pTChain->SetBranchAddress("recoVertexX", &recoVertexX);
    pTChain->SetBranchAddress("recoVertexY", &recoVertexY);
    pTChain->SetBranchAddress("recoVertexZ", &recoVertexZ);

    pTChain->SetBranchAddress("targetVertexX", &targetVertexX);
    pTChain->SetBranchAddress("targetVertexY", &targetVertexY);
    pTChain->SetBranchAddress("targetVertexZ", &targetVertexZ);

    pTChain2->SetBranchAddress("recoVertexX", &recoVertexX2);
    pTChain2->SetBranchAddress("recoVertexY", &recoVertexY2);
    pTChain2->SetBranchAddress("recoVertexZ", &recoVertexZ2);

    pTChain2->SetBranchAddress("targetVertexX", &targetVertexX2);
    pTChain2->SetBranchAddress("targetVertexY", &targetVertexY2);
    pTChain2->SetBranchAddress("targetVertexZ", &targetVertexZ2);

    const int vtxBins(100);
    const float vtxMax(250);

    DrawClass drawClass_VtxDR_Changed("Beam Particle Vertex Delta R Changed");
    drawClass_VtxDR_Changed.SetLogY(true);
    drawClass_VtxDR_Changed.SetRange(-0.1f*vtxMax, vtxMax, 1, 1e4f);
    drawClass_VtxDR_Changed.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
    drawClass_VtxDR_Changed.Normalise(false);

    DrawClass drawClass_VtxDR_Unchanged("Beam Particle Vertex Delta R Unchanged");
    drawClass_VtxDR_Unchanged.SetLogY(true);
    drawClass_VtxDR_Unchanged.SetRange(-0.1f*vtxMax, vtxMax, 1, 1e4f);
    drawClass_VtxDR_Unchanged.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
    drawClass_VtxDR_Unchanged.Normalise(false);

    TH1F *pTH1F_Res_Unchanged = new TH1F("DeltaR_Unchanged", "", vtxBins, 0, vtxMax);
    Helper::Format(pTH1F_Res_Unchanged);
    pTH1F_Res_Unchanged->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");

    TH1F *pTH1F_Res_Unchanged_BDT = new TH1F("DeltaR_Unchanged_BDT", "", vtxBins, 0, vtxMax);
    Helper::Format(pTH1F_Res_Unchanged_BDT);
    pTH1F_Res_Unchanged_BDT->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");

    TH1F *pTH1F_Res_Changed = new TH1F("DeltaR_Changed", "", vtxBins, 0, vtxMax);
    Helper::Format(pTH1F_Res_Changed);
    pTH1F_Res_Changed->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");

    TH1F *pTH1F_Res_Changed_BDT = new TH1F("DeltaR_Changed_BDT", "", vtxBins, 0, vtxMax);
    Helper::Format(pTH1F_Res_Changed_BDT);
    pTH1F_Res_Changed_BDT->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");

    unsigned int nEntries(pTChain->GetEntries());
    for (unsigned int entry = 0; entry < nEntries; entry++)
    {
        pTChain->GetEntry(entry);
        pTChain2->GetEntry(entry);

        if (2001 != mcNuanceCode || std::fabs(mcPrimaryPdg->front()) != 211) continue;

        if (std::fabs(targetVertexX - targetVertexX2) > 1e-3) continue;
        if (std::fabs(targetVertexY - targetVertexY2) > 1e-3) continue;
        if (std::fabs(targetVertexZ - targetVertexZ2) > 1e-3) continue;

        if (targetVertexZ < -50.f) continue;

        FloatVector position = {recoVertexX, recoVertexY, recoVertexZ};
        FloatVector position2 = {recoVertexX2, recoVertexY2, recoVertexZ2};

        FloatVector sccPosition = SpaceChargeHelper::GetSpaceChargeCorrectedPosition(position);
        FloatVector sccPosition2 = SpaceChargeHelper::GetSpaceChargeCorrectedPosition(position2);

        const float deltaX(sccPosition.at(0) - targetVertexX);
        const float deltaY(sccPosition.at(1) - targetVertexY);
        const float deltaZ(sccPosition.at(2) - targetVertexZ);
        const float deltaR(std::sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ));

        const float deltaX2(sccPosition2.at(0) - targetVertexX2);
        const float deltaY2(sccPosition2.at(1) - targetVertexY2);
        const float deltaZ2(sccPosition2.at(2) - targetVertexZ2);
        const float deltaR2(std::sqrt(deltaX2*deltaX2 + deltaY2*deltaY2 + deltaZ2*deltaZ2));

        const bool hasDeltaXChanged(std::fabs(recoVertexX - recoVertexX2) > 2);
        const bool hasDeltaYChanged(std::fabs(recoVertexY - recoVertexY2) > 2);
        const bool hasDeltaZChanged(std::fabs(recoVertexZ - recoVertexZ2) > 2);

        if (deltaR < 5 && deltaR2 > 10)
            std::cout << "Root File Name : " << pTChain->GetCurrentFile()->GetName() << ", " << pTChain2->GetCurrentFile()->GetName() << ", DeltaDeltaR : " << (deltaR2 - deltaR) << std::endl;

        if (hasDeltaXChanged || hasDeltaYChanged || hasDeltaZChanged)
        {
            pTH1F_Res_Changed_BDT->Fill(deltaR2);
            pTH1F_Res_Changed->Fill(deltaR);
        }
        else
        {
            pTH1F_Res_Unchanged_BDT->Fill(deltaR2);
            pTH1F_Res_Unchanged->Fill(deltaR);
        }
    }

    drawClass_VtxDR_Changed.AddHisto(pTH1F_Res_Changed, "Default Vertex");
    drawClass_VtxDR_Changed.AddHisto(pTH1F_Res_Changed_BDT, "BDT Vertex");

    drawClass_VtxDR_Unchanged.AddHisto(pTH1F_Res_Unchanged, "Default Vertex");
    drawClass_VtxDR_Unchanged.AddHisto(pTH1F_Res_Unchanged_BDT, "BDT Vertex");

    drawClass_VtxDR_Changed.Draw();
    drawClass_VtxDR_Unchanged.Draw();

    return 0;
}
