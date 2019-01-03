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
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5834_Momentum_0p3GeV/RootFiles/*.root", "Run 5834, 0.3 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5826_Momentum_0p5GeV/RootFiles/*.root", "Run 5826, 0.5 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5387_Momentum_1GeV/RootFiles/*.root", "Run 5387, 1 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5430_Momentum_2GeV/RootFiles/*.root", "Run 5430, 2 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5777_Momentum_3GeV/RootFiles/*.root", "Run 5777, 3 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5758_Momentum_6GeV/RootFiles/*.root", "Run 5758, 6 GeV");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/January/ProtoDUNE_Data/AnalysisTag1/Data_Run_5145_Momentum_7GeV/RootFiles/*.root", "Run 5145, 7 GeV");

/*
 isTriggered     = 1
 beamMomentum    = 6.3864
 beamPositionX   = -26.4815
 beamPositionY   = 424.051
 beamPositionZ   = 0
 beamDirectionX  = 0.174434
 beamDirectionY  = 0.193961
 beamDirectionZ  = -0.965376
 tof             = 154.395
 ckov0Status     = -2147483648
 ckov1Status     = -2147483648
 nBeamPfos       = 1
 nShwBeamPfos    = 0
 nTrkBeamPfos    = 1
 nHitsRecoU      = (vector<int>*)0x4ee7e10
 nHitsRecoV      = (vector<int>*)0x5464140
 nHitsRecoW      = (vector<int>*)0x52ae420
 nHitsRecoTotal  = (vector<int>*)0x53302c0
 recoParticleId  = (vector<int>*)0x527ef80
 recoDirectionX  = (vector<float>*)0x5303fc0
 recoDirectionY  = (vector<float>*)0x511d9c0
 recoDirectionZ  = (vector<float>*)0x53040c0
*/

    int isTriggered(std::numeric_limits<int>::max()), nBeamPfos(std::numeric_limits<int>::max()), ckov0Status(std::numeric_limits<int>::max()), ckov1Status(std::numeric_limits<int>::max());
    float beamMomentum(std::numeric_limits<float>::max()), tof(std::numeric_limits<float>::max());
    float beamDirectionX(std::numeric_limits<float>::max()), beamDirectionY(std::numeric_limits<float>::max()), beamDirectionZ(std::numeric_limits<float>::max());
    float beamPositionX(std::numeric_limits<float>::max()), beamPositionY(std::numeric_limits<float>::max()), beamPositionZ(std::numeric_limits<float>::max());
    std::vector<float> *recoDirectionX(nullptr), *recoDirectionY(nullptr), *recoDirectionZ(nullptr);
    std::vector<int> *nHitsRecoTotal(nullptr);

    typedef std::map<Particle, DrawClass> ParticleToDrawClassMap;

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs Momentum");
    drawClass_BeamParticleEff.SetRange(0.f, 7.f, 0.f, 1.05f);

    std::vector<Particle> particles;
    particles = {ELECTRON, PROTON, KAONPLUS, PIPLUS, OTHER};
    ParticleToDrawClassMap drawClassMap_BeamParticleEff;

    for (const Particle &particle : particles)
    {
        std::string particleName(Helper::GetParticleName(particle));
        DrawClass drawClass("Beam Particle Efficiency Vs Momentum " + particleName);
        drawClass.SetRange(0.f, 7.f, 0.f, 1.05f);
        drawClassMap_BeamParticleEff.insert(ParticleToDrawClassMap::value_type(particle, drawClass));
    }

    DrawClass drawClass_OpeningAngle("Beam Particle Opening Angle");
    drawClass_OpeningAngle.SetLogY(true);

    DrawClass drawClass_OpeningAngleProjection("Beam Particle Opening Angle Projection");
    drawClass_OpeningAngleProjection.SetLogY(true);

    DrawClass drawClass("Momentum Vs TOF");
    drawClass.SetRange(0.f, 10.f, 0.f, 300.f);

    DrawClass drawClass_BeamPosition("Beam Position");
    drawClass_BeamPosition.SetRange(-400.f, 400.f, -100.f, 700.f);

    DrawClass drawClass_BeamDirection("Beam Direction");
    drawClass_BeamDirection.SetRange(0.f, 1.f, 0.f, 0.f);

    DrawClass drawClass_BeamMomentum("Beam Momentum");
    drawClass_BeamMomentum.SetRange(0.f, 10.f, 0.f, 0.f);

    DrawClass drawClass_P("Beam Momentum Components");
    drawClass_P.SetRange(0.f, 10.f, 0.f, 0.f);

    const int nBins(100), maxBin(10);

    for (EventClass &eventClass : eventClassVector)
    {
        int nBeamParticles(0), nBeamParticleMatches(0);

        // Beam Efficiency vs Momentum
        TH1F *pTH1F_BeamMCPrimaryMomentum = new TH1F("BeamMCPrimaryMomentum", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentum);
        pTH1F_BeamMCPrimaryMomentum->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentum->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryMomentum_Matched = new TH1F("BeamMCPrimaryMomentum_Matched", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentum_Matched);
        pTH1F_BeamMCPrimaryMomentum_Matched->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentum_Matched->SetLineColor(kBlue);

        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryMomentumTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryMomentumTotal_Matched;

        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));

            std::string histName("BeamMCPrimaryMomentumTotal_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Particle = new TH1F(histName.c_str(), "", nBins, 0, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryMomentumTotal_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryMomentumTotal_Particle);
            pTH1F_BeamMCPrimaryMomentumTotal_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryMomentumTotal_Particle->SetLineColor(kRed);
            particleToHistMap_BeamMCPrimaryMomentumTotal.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryMomentumTotal_Particle));

            std::string histNameMatched("BeamMCPrimaryMomentumTotal_Matched_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle = new TH1F(histNameMatched.c_str(), "", nBins, 0, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle);
            Helper::BinLogX(pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle);
            pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle));
        }

        TH1F *pTH1F_BeamMCPrimaryMomentumX = new TH1F("BeamMCPrimaryMomentumX", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentumX);
        pTH1F_BeamMCPrimaryMomentumX->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentumX->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryMomentumY = new TH1F("BeamMCPrimaryMomentumY", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentumY);
        pTH1F_BeamMCPrimaryMomentumY->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentumY->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryMomentumZ = new TH1F("BeamMCPrimaryMomentumZ", "", nBins, 0, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentumZ);
        pTH1F_BeamMCPrimaryMomentumZ->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentumZ->SetLineColor(kRed);

        // TOF vs Momentum
        TH2F *pTH2F = new TH2F("TOF vs Momentum", "", 100, 0, 7, 100, 90, 500);
        pTH2F->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH2F->GetYaxis()->SetTitle("Time of Flight [ns]");

        // Opening Angles
        const int nBinsAngle(100);
        const float lowBinAngle(0.8f), highBinAngle(1.f);
        TH1F *pTH1F_BeamOpeningAngle = new TH1F("Opening Angle", "", nBinsAngle, lowBinAngle, highBinAngle);
        Helper::Format(pTH1F_BeamOpeningAngle);
        pTH1F_BeamOpeningAngle->GetXaxis()->SetTitle("|cos(#theta)|");
        pTH1F_BeamOpeningAngle->GetYaxis()->SetTitle("Entries");

        TH1F *pTH1F_BeamOpeningAngleXY = new TH1F("Opening Angle XY", "", nBinsAngle, lowBinAngle, highBinAngle);
        Helper::Format(pTH1F_BeamOpeningAngleXY);
        pTH1F_BeamOpeningAngleXY->GetXaxis()->SetTitle("|cos(#theta)|");
        pTH1F_BeamOpeningAngleXY->GetYaxis()->SetTitle("Entries");

        TH1F *pTH1F_BeamOpeningAngleXZ = new TH1F("Opening Angle XZ", "", nBinsAngle, lowBinAngle, highBinAngle);
        Helper::Format(pTH1F_BeamOpeningAngleXZ);
        pTH1F_BeamOpeningAngleXZ->GetXaxis()->SetTitle("|cos(#theta)|");
        pTH1F_BeamOpeningAngleXZ->GetYaxis()->SetTitle("Entries");

        TH1F *pTH1F_BeamOpeningAngleYZ = new TH1F("Opening Angle YZ", "", nBinsAngle, lowBinAngle, highBinAngle);
        Helper::Format(pTH1F_BeamOpeningAngleYZ);
        pTH1F_BeamOpeningAngleYZ->GetXaxis()->SetTitle("|cos(#theta)|");
        pTH1F_BeamOpeningAngleYZ->GetYaxis()->SetTitle("Entries");

        // Beam Position
        TH2F *pTH2F_BeamPosition = new TH2F("Beam Position", "", 100, -50, 0, 100, 400, 450);
        Helper::Format(pTH2F_BeamPosition);
        pTH2F_BeamPosition->GetXaxis()->SetTitle("X [cm]");
        pTH2F_BeamPosition->GetYaxis()->SetTitle("Y [cm]");

        TH1F *pTH1F_BeamDirectionX = new TH1F("Beam Position X", "", 200, -1, 1);
        Helper::Format(pTH1F_BeamDirectionX);
        pTH1F_BeamDirectionX->GetXaxis()->SetTitle("Direction");
        pTH1F_BeamDirectionX->GetYaxis()->SetTitle("Entries");

        TH1F *pTH1F_BeamDirectionY = new TH1F("Beam Position Y", "", 200, -1, 1);
        Helper::Format(pTH1F_BeamDirectionY);
        pTH1F_BeamDirectionY->GetXaxis()->SetTitle("Direction");
        pTH1F_BeamDirectionY->GetYaxis()->SetTitle("Entries");

        TH1F *pTH1F_BeamDirectionZ = new TH1F("Beam Position Z", "", 200, -1, 1);
        Helper::Format(pTH1F_BeamDirectionZ);
        pTH1F_BeamDirectionZ->GetXaxis()->SetTitle("Direction");
        pTH1F_BeamDirectionZ->GetYaxis()->SetTitle("Entries");

        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("isTriggered", &isTriggered);
        pTChain->SetBranchAddress("nBeamPfos", &nBeamPfos);
        pTChain->SetBranchAddress("beamMomentum", &beamMomentum);
        pTChain->SetBranchAddress("tof", &tof);
        pTChain->SetBranchAddress("ckov0Status", &ckov0Status);
        pTChain->SetBranchAddress("ckov1Status", &ckov1Status);
        pTChain->SetBranchAddress("beamPositionX", &beamPositionX);
        pTChain->SetBranchAddress("beamPositionY", &beamPositionY);
        pTChain->SetBranchAddress("beamPositionZ", &beamPositionZ);
        pTChain->SetBranchAddress("beamDirectionX", &beamDirectionX);
        pTChain->SetBranchAddress("beamDirectionY", &beamDirectionY);
        pTChain->SetBranchAddress("beamDirectionZ", &beamDirectionZ);
        pTChain->SetBranchAddress("recoDirectionX", &recoDirectionX);
        pTChain->SetBranchAddress("recoDirectionY", &recoDirectionY);
        pTChain->SetBranchAddress("recoDirectionZ", &recoDirectionZ);
        pTChain->SetBranchAddress("nHitsRecoTotal", &nHitsRecoTotal);

        unsigned int nEntries(pTChain->GetEntries());
        for (unsigned int entry = 0; entry < nEntries; entry++)
        {
            pTChain->GetEntry(entry);

            if (isTriggered == 1)
            {
                nBeamParticles++;
                if (nBeamPfos > 0)
                    nBeamParticleMatches++;
            }

            // Cut to make sure only dealing with clean triggered events
            if (beamMomentum > 100.f)
                continue;

            if (isTriggered == 1)
            {
                Particle particle(Helper::GetParticle(ckov0Status, ckov1Status, beamMomentum, tof));
                TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Particle = particleToHistMap_BeamMCPrimaryMomentumTotal.at(particle);
                TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle = particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.at(particle);

                pTH1F_BeamMCPrimaryMomentum->Fill(beamMomentum);
                pTH1F_BeamMCPrimaryMomentumTotal_Particle->Fill(beamMomentum);
                pTH2F_BeamPosition->Fill(beamPositionX, beamPositionY);
                const float beamDirectionMag(std::sqrt(beamDirectionX*beamDirectionX + beamDirectionY*beamDirectionY + beamDirectionZ*beamDirectionZ));
                pTH1F_BeamMCPrimaryMomentumX->Fill(beamMomentum * std::fabs(beamDirectionX) / beamDirectionMag);
                pTH1F_BeamMCPrimaryMomentumY->Fill(beamMomentum * std::fabs(beamDirectionY) / beamDirectionMag);
                pTH1F_BeamMCPrimaryMomentumZ->Fill(beamMomentum * std::fabs(beamDirectionZ) / beamDirectionMag);

                const float mag(std::sqrt(beamDirectionX*beamDirectionX + beamDirectionY*beamDirectionY + beamDirectionZ*beamDirectionZ));

                pTH1F_BeamDirectionX->Fill(beamDirectionX/mag);
                pTH1F_BeamDirectionY->Fill(beamDirectionY/mag);
                pTH1F_BeamDirectionZ->Fill(beamDirectionZ/mag);

                if (nBeamPfos > 0)
                {
                    pTH1F_BeamMCPrimaryMomentum_Matched->Fill(beamMomentum);
                    pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->Fill(beamMomentum);

                    const float mag1(beamDirectionX*beamDirectionX + beamDirectionY*beamDirectionY + beamDirectionZ*beamDirectionZ);
                    float cosTheta(0.f), cosThetaXY(-1.f), cosThetaXZ(-1.f), cosThetaYZ(-1.f);
                    int largestMomentum(0);

                    for (unsigned int i = 0; i < nHitsRecoTotal->size(); i++)
                    {
                        const int nHits(nHitsRecoTotal->at(i));
                        if (nHits > largestMomentum)
                        {
                            largestMomentum = nHits;
                            const float x(recoDirectionX->at(i));
                            const float y(recoDirectionY->at(i));
                            const float z(recoDirectionZ->at(i));
                            const float mag2(x*x+y*y+z*z);
                            cosTheta = (x*beamDirectionX + y*beamDirectionY + z*beamDirectionZ) / (mag1 *mag2);

                            cosThetaXY = Helper::CalculateCosTheta2D(x, y, beamDirectionX, beamDirectionY);
                            cosThetaXZ = Helper::CalculateCosTheta2D(x, z, beamDirectionX, beamDirectionZ);
                            cosThetaYZ = Helper::CalculateCosTheta2D(y, z, beamDirectionY, beamDirectionZ);
                        }
                    }
                    pTH1F_BeamOpeningAngle->Fill(cosTheta);
                    pTH1F_BeamOpeningAngleXY->Fill(cosThetaXY);
                    pTH1F_BeamOpeningAngleXZ->Fill(cosThetaXZ);
                    pTH1F_BeamOpeningAngleYZ->Fill(cosThetaYZ);
                }

                pTH2F->Fill(beamMomentum, tof - 61.4);
            }
        }

        TGraphErrors *pTGraphErrors_BeamMomentumEfficiency = Helper::MakeEfficiency(pTH1F_BeamMCPrimaryMomentum, pTH1F_BeamMCPrimaryMomentum_Matched, "BeamMomentumEfficiency", 5);
        Helper::Format(pTGraphErrors_BeamMomentumEfficiency);
        pTGraphErrors_BeamMomentumEfficiency->GetXaxis()->SetTitle("Momentum [GeV]");
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetTitle("Efficiency");
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetDecimals();
        const int nEvtsTotal(pTH1F_BeamMCPrimaryMomentum->GetEntries());
        drawClass_BeamParticleEff.AddGraph(pTGraphErrors_BeamMomentumEfficiency, eventClass.GetDescription() + ", NEvts " + Helper::ToString(nEvtsTotal));

        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));
            TGraphErrors *pTGraphErrors_BeamMomentumEfficiency_Particle  = Helper::MakeEfficiency(particleToHistMap_BeamMCPrimaryMomentumTotal.at(particle), particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.at(particle), "BeamMomentumEfficiency" + particleName);
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetXaxis()->SetTitle("Momentum [GeV]");
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetTitle("Efficiency");
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetRangeUser(0,1);
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetDecimals();
            const int nEvts(particleToHistMap_BeamMCPrimaryMomentumTotal.at(particle)->GetEntries());
            drawClassMap_BeamParticleEff.at(particle).AddGraph(pTGraphErrors_BeamMomentumEfficiency_Particle, eventClass.GetDescription() + ", NEvts " + Helper::ToString(nEvts));
            delete pTGraphErrors_BeamMomentumEfficiency_Particle;
        }

        drawClass_OpeningAngle.AddHisto(pTH1F_BeamOpeningAngle, eventClass.GetDescription());
        drawClass_OpeningAngleProjection.AddHisto(pTH1F_BeamOpeningAngleXZ, eventClass.GetDescription() + " XZ Projection");
        drawClass_OpeningAngleProjection.AddHisto(pTH1F_BeamOpeningAngleXY, eventClass.GetDescription() + " XY Projection");
        drawClass_OpeningAngleProjection.AddHisto(pTH1F_BeamOpeningAngleYZ, eventClass.GetDescription() + " YZ Projection");

        drawClass.Add2DHisto(pTH2F, eventClass.GetDescription());

        drawClass_BeamPosition.Add2DHisto(pTH2F_BeamPosition, eventClass.GetDescription());

        drawClass_BeamDirection.AddHisto(pTH1F_BeamDirectionZ, eventClass.GetDescription() + ", along Z, mean " + Helper::ToStringPrecision(pTH1F_BeamDirectionZ->GetMean(), 3));
        drawClass_BeamDirection.AddHisto(pTH1F_BeamDirectionX, eventClass.GetDescription() + ", along X, mean " + Helper::ToStringPrecision(pTH1F_BeamDirectionX->GetMean(), 3));
        drawClass_BeamDirection.AddHisto(pTH1F_BeamDirectionY, eventClass.GetDescription() + ", along Y, mean " + Helper::ToStringPrecision(pTH1F_BeamDirectionY->GetMean(), 3));

        drawClass_BeamMomentum.AddHisto(pTH1F_BeamMCPrimaryMomentum, eventClass.GetDescription());
        drawClass_P.AddHisto(pTH1F_BeamMCPrimaryMomentumX, eventClass.GetDescription() + ", Momentum X Direction");
        drawClass_P.AddHisto(pTH1F_BeamMCPrimaryMomentumY, eventClass.GetDescription() + ", Momentum Y Direction");
        drawClass_P.AddHisto(pTH1F_BeamMCPrimaryMomentumZ, eventClass.GetDescription() + ", Momentum Z Direction");

        delete pTH1F_BeamMCPrimaryMomentum;
        delete pTH1F_BeamMCPrimaryMomentum_Matched;
        delete pTH1F_BeamOpeningAngle;
        delete pTH1F_BeamOpeningAngleXY;
        delete pTH1F_BeamOpeningAngleXZ;
        delete pTH1F_BeamOpeningAngleYZ;
        delete pTH2F;
        delete pTH2F_BeamPosition;
        delete pTH1F_BeamMCPrimaryMomentumX;
        delete pTH1F_BeamMCPrimaryMomentumY;
        delete pTH1F_BeamMCPrimaryMomentumZ;
        delete pTH1F_BeamDirectionX;
        delete pTH1F_BeamDirectionY;
        delete pTH1F_BeamDirectionZ;

        for (const auto &iter : particleToHistMap_BeamMCPrimaryMomentumTotal)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamMCPrimaryMomentumTotal_Matched)
            delete iter.second;

        std::cout << "Event Description    : " << eventClass.GetDescription() << std::endl;
        std::cout << "nBeamParticles       : " << nBeamParticles << std::endl;
        std::cout << "nBeamParticleMatches : " << nBeamParticleMatches << std::endl;
        const float beamParticleEfficiency(static_cast<float>(nBeamParticleMatches)/static_cast<float>(nBeamParticles));
        std::cout << "Efficiency           : " << beamParticleEfficiency << std::endl;
        const float beamParticleEfficiencyError(std::sqrt(beamParticleEfficiency * (1-beamParticleEfficiency) / static_cast<float>(nBeamParticles)));
        std::cout << "Efficiency Error     : " << beamParticleEfficiencyError << std::endl;
        std::cout << "Efficiency [%]       : " << beamParticleEfficiency*100.f << "+-" << beamParticleEfficiencyError*100.f << std::endl;
    }

    drawClass_BeamParticleEff.Draw();

    for (const auto &iter : drawClassMap_BeamParticleEff)
        iter.second.Draw();

    drawClass_OpeningAngle.Draw();
    drawClass_OpeningAngleProjection.Draw();
    drawClass.Draw();
    drawClass_BeamPosition.Draw();
    drawClass_BeamMomentum.Draw();
    drawClass_P.Draw();
    drawClass_BeamDirection.Draw();
    return 0;
}

//==================================================
