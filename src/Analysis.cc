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

    DrawClass drawClass_DistanceToBeamSpot("Distance To Beam Spot");
    drawClass_DistanceToBeamSpot.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_DistanceToBeamSpot.SetLegend(0.5f, 0.8f, 0.6f, 0.775f);
    drawClass_DistanceToBeamSpot.SetNColumns(1);
//    drawClass_DistanceToBeamSpot.SetLogY(true);

    DrawClass drawClass_SupplementaryAngle("Supplementary Angle To Beam");
    drawClass_SupplementaryAngle.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_SupplementaryAngle.SetLegend(0.5f, 0.775f, 0.6f, 0.775f);
    drawClass_SupplementaryAngle.SetNColumns(1);
//    drawClass_SupplementaryAngle.SetLogY(true);

    DrawClass drawClass_MaxY("Max Y");
    drawClass_MaxY.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_MaxY.SetLegend(0.175f, 0.4f, 0.6f, 0.775f);
    drawClass_MaxY.SetNColumns(1);
//    drawClass_MaxY.SetLogY(true);

    DrawClass drawClass_BDTScore("BDT Score");
    drawClass_BDTScore.SetRange(0.f, 0.f, 0.f, 1.f);
    drawClass_BDTScore.SetLegend(0.5f, 0.8f, 0.6f, 0.775f);
    drawClass_BDTScore.SetNColumns(1);
//    drawClass_BDTScore.SetLogY(true);

/*
43             m_featureVector.push_back(closestDistanceNu);
 44             m_featureVector.push_back(supplementaryAngleToBeamNu);
 45             m_featureVector.push_back(separationNu);
 46             m_featureVector.push_back(eigenValuesNu.GetX());
 47             m_featureVector.push_back(eigenValuesNu.GetY());
 48             m_featureVector.push_back(eigenValuesNu.GetZ());
 49             m_featureVector.push_back(maxYNu);
 50             m_featureVector.push_back(allConnectedPfoListNu.size());
 51 
 52             m_featureVector.push_back(closestDistanceCr);
 53             m_featureVector.push_back(supplementaryAngleToBeamCr);
 54             m_featureVector.push_back(separationCr);
 55             m_featureVector.push_back(eigenValuesCr.GetX());
 56             m_featureVector.push_back(eigenValuesCr.GetY());
 57             m_featureVector.push_back(eigenValuesCr.GetZ());
 58             m_featureVector.push_back(maxYCr);
 59             m_featureVector.push_back(allConnectedPfoListCr.size());
*/

    std::ifstream file("TrainingFileName_RetrainBeamParticleId_5GeV_Concatenated.txt");
    std::ifstream file2("BDTScores.txt");
    std::string str;

    TH1F *pTH1F_DistanceToBeamSpot_TB_RecoNu = new TH1F("DistanceToBeamSpot_TB_RecoNu","",100,0,1000);
    Helper::Format(pTH1F_DistanceToBeamSpot_TB_RecoNu);
    pTH1F_DistanceToBeamSpot_TB_RecoNu->GetXaxis()->SetTitle("Distance To Beam Spot [cm]");

    TH1F *pTH1F_DistanceToBeamSpot_CR_RecoNu = new TH1F("DistanceToBeamSpot_CR_RecoNu","",100,0,1000);
    Helper::Format(pTH1F_DistanceToBeamSpot_CR_RecoNu);
    pTH1F_DistanceToBeamSpot_CR_RecoNu->GetXaxis()->SetTitle("Distance To Beam Spot [cm]");

    TH1F *pTH1F_SupplementaryAngle_TB_RecoNu = new TH1F("SupplementaryAngle_TB_RecoNu","",100,0,3.14);
    Helper::Format(pTH1F_SupplementaryAngle_TB_RecoNu);
    pTH1F_SupplementaryAngle_TB_RecoNu->GetXaxis()->SetTitle("Supplementary Angle To Beam [rad]");

    TH1F *pTH1F_SupplementaryAngle_CR_RecoNu = new TH1F("SupplementaryAngle_CR_RecoNu","",100,0,3.14);
    Helper::Format(pTH1F_SupplementaryAngle_CR_RecoNu);
    pTH1F_SupplementaryAngle_CR_RecoNu->GetXaxis()->SetTitle("Supplementary Angle To Beam [rad]");

    TH1F *pTH1F_BDTScore_Beam = new TH1F("BDTScore_Beam", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Beam);
    pTH1F_BDTScore_Beam->GetXaxis()->SetTitle("BDT Score");

    TH1F *pTH1F_BDTScore_Bkg = new TH1F("BDTScore_Bkg", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Bkg);
    pTH1F_BDTScore_Bkg->GetXaxis()->SetTitle("BDT Score");

/*
    TH1F *pTH1F_BDTScore_Beam_Test = new TH1F("BDTScore_Beam_Test", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Beam_Test);
    pTH1F_BDTScore_Beam_Test->GetXaxis()->SetTitle("BDT Score");

    TH1F *pTH1F_BDTScore_Beam_Train = new TH1F("BDTScore_Beam_Train", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Beam_Train);
    pTH1F_BDTScore_Beam_Train->GetXaxis()->SetTitle("BDT Score");

    TH1F *pTH1F_BDTScore_Bkg_Test = new TH1F("BDTScore_Bkg_Test", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Bkg_Test);
    pTH1F_BDTScore_Bkg_Test->GetXaxis()->SetTitle("BDT Score");

    TH1F *pTH1F_BDTScore_Bkg_Train = new TH1F("BDTScore_Bkg_Train", "", 100, -1, 1);
    Helper::Format(pTH1F_BDTScore_Bkg_Train);
    pTH1F_BDTScore_Bkg_Train->GetXaxis()->SetTitle("BDT Score");
*/
    TH1F *pTH1F_Beam_MaxY = new TH1F("Beam_MaxY", "", 100, 0, 1000);
    Helper::Format(pTH1F_Beam_MaxY);
    pTH1F_Beam_MaxY->GetXaxis()->SetTitle("Max Y 3D Hit [cm]");

    TH1F *pTH1F_Bkg_MaxY = new TH1F("Bkg_MaxY", "", 100, 0, 1000);
    Helper::Format(pTH1F_Bkg_MaxY);
    pTH1F_Bkg_MaxY->GetXaxis()->SetTitle("Max Y 3D Hit [cm]");

    while (std::getline(file, str))
    {
        StringVector info;
        Helper::TokenizeString(str, info, ",");

        bool isTriggeredBeam(std::stoi(info.back()) == 1 ? true : false);
        float distanceToBeamSpotNu(std::stof(info.at(1))); //, distanceToBeamSpotCr(std::stof(info.at(9)));
        float supplementaryAngleToBeamNu(M_PI - std::stof(info.at(2))); //, supplementaryAngleToBeamCr(std::stof(info.at(10)));
        float maxYNu(std::stof(info.at(7)));

        if (isTriggeredBeam)
        {
            pTH1F_SupplementaryAngle_TB_RecoNu->Fill(supplementaryAngleToBeamNu);
            pTH1F_DistanceToBeamSpot_TB_RecoNu->Fill(distanceToBeamSpotNu);
            pTH1F_Beam_MaxY->Fill(maxYNu);
        }
        else
        {
            pTH1F_SupplementaryAngle_CR_RecoNu->Fill(supplementaryAngleToBeamNu);
            pTH1F_DistanceToBeamSpot_CR_RecoNu->Fill(distanceToBeamSpotNu);
            pTH1F_Bkg_MaxY->Fill(maxYNu);
        }
    }

    while (std::getline(file2, str))
    {
        StringVector info;
        Helper::TokenizeString(str, info, ",");
//        const bool isTest(info.back() == "test" ? true : false);
        const bool isSignal(std::stoi(info.at(0)) == 1 ? true : false);
        const float bdtScore(std::stof(info.at(1)));

        if (isSignal)
        {
            pTH1F_BDTScore_Beam->Fill(bdtScore);
        }
        else
        {
            pTH1F_BDTScore_Bkg->Fill(bdtScore);
        }
/*
        if (isTest && isSignal)
        {
            pTH1F_BDTScore_Beam_Test->Fill(bdtScore);
        }
        else if (isTest && !isSignal)
        {
            pTH1F_BDTScore_Bkg_Test->Fill(bdtScore);
        }
        else if (!isTest && isSignal)
        {
            pTH1F_BDTScore_Beam_Train->Fill(bdtScore);
        }
        else if (!isTest && !isSignal)
        {
            pTH1F_BDTScore_Bkg_Train->Fill(bdtScore);
        }
*/
    }

    drawClass_DistanceToBeamSpot.AddHisto(pTH1F_DistanceToBeamSpot_TB_RecoNu, "Triggered Test Beam Particles");
    drawClass_DistanceToBeamSpot.AddHisto(pTH1F_DistanceToBeamSpot_CR_RecoNu, "Remaining Particles");

    drawClass_SupplementaryAngle.AddHisto(pTH1F_SupplementaryAngle_TB_RecoNu, "Triggered Test Beam Particles");
    drawClass_SupplementaryAngle.AddHisto(pTH1F_SupplementaryAngle_CR_RecoNu, "Remaining Particles");

    drawClass_MaxY.AddHisto(pTH1F_Beam_MaxY, "Triggered Test Beam Particles");
    drawClass_MaxY.AddHisto(pTH1F_Bkg_MaxY, "Remaining Particles");

    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Bkg, "Remaining Particles");
    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Beam, "Triggered Test Beam Particles");

/*
    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Beam_Train, "Triggered Particles, Training Sample");
    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Bkg_Train, "Remaining Particles, Training Sample");
    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Beam_Test, "Triggered Particles, Test Sample");
    drawClass_BDTScore.AddHisto(pTH1F_BDTScore_Bkg_Test, "Remaining Particles, Test Sample");
*/

    delete pTH1F_DistanceToBeamSpot_TB_RecoNu;
    delete pTH1F_DistanceToBeamSpot_CR_RecoNu;
    delete pTH1F_SupplementaryAngle_TB_RecoNu;
    delete pTH1F_SupplementaryAngle_CR_RecoNu;
    delete pTH1F_BDTScore_Beam;
    delete pTH1F_BDTScore_Bkg;
/*
    delete pTH1F_BDTScore_Beam_Test;
    delete pTH1F_BDTScore_Bkg_Test;
    delete pTH1F_BDTScore_Beam_Train;
    delete pTH1F_BDTScore_Bkg_Train;
*/
    delete pTH1F_Beam_MaxY;
    delete pTH1F_Bkg_MaxY;

    drawClass_DistanceToBeamSpot.Draw();
    drawClass_SupplementaryAngle.Draw();
    drawClass_MaxY.Draw();
    drawClass_BDTScore.Draw();
    return 0;
}

//==================================================
