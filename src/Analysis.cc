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
//    TApplication *pTApplication = new TApplication("Analysis", &argc,argv);
//    if (!pTApplication)
//    {
//        std::cout << "TApplication problem." << std::endl;
//    }

    gROOT->SetBatch();
    Style();

    EventClassVector eventClassVector;
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/2GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/3GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/4GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/6GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "Simulation");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/Data_Run_5387_Momentum_1GeV/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/Data_Run_5430_Momentum_2GeV/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/Data_Run_5777_Momentum_3GeV/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/Data_Run_5758_Momentum_6GeV/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/March/ProtoDUNE_Data/AnalysisTag8/Data_Run_5145_Momentum_7GeV/RootFiles/*.root", "Data");

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

    int isTriggered(std::numeric_limits<int>::max()), nBeamPfos(std::numeric_limits<int>::max());
    int ckov0Status(std::numeric_limits<int>::max()), ckov1Status(std::numeric_limits<int>::max());
    float beamMomentum(std::numeric_limits<float>::max());
    float tof(std::numeric_limits<float>::max());
//    float beamPositionX(std::numeric_limits<float>::max()), beamPositionY(std::numeric_limits<float>::max()), beamPositionZ(std::numeric_limits<float>::max());
    float beamDirectionX(std::numeric_limits<float>::max()), beamDirectionY(std::numeric_limits<float>::max()), beamDirectionZ(std::numeric_limits<float>::max());
    std::vector<float> *recoDirectionX(nullptr), *recoDirectionY(nullptr), *recoDirectionZ(nullptr);
//    std::vector<float> *recoDirectionCRX(nullptr), *recoDirectionCRY(nullptr), *recoDirectionCRZ(nullptr);
//    std::vector<int> *nHitsCosmicU(nullptr), *nHitsCosmicV(nullptr), *nHitsCosmicW(nullptr);
    std::vector<int>  *nHitsRecoTotal(nullptr), *nHitsCosmicTotal(nullptr);
//    int nCosmicRayPfos(0);
    std::vector<float> *cosmicRayX0s(nullptr);
//   std::vector<float> *allTestBeamScores(nullptr);
//    std::vector<float> *crStartPointX(nullptr), *crStartPointY(nullptr), *crStartPointZ(nullptr), *crEndPointX(nullptr), *crEndPointY(nullptr), *crEndPointZ(nullptr);

    const float cmPerTick(0.0802814);
    const float nsPerTick(500);
    const int maxBin(8);

    typedef std::map<Particle, DrawClass> ParticleToDrawClassMap;

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs Momentum");
    drawClass_BeamParticleEff.SetRange(0.f, static_cast<float>(maxBin), 0.f, 1.05f);
    drawClass_BeamParticleEff.SetLegend(0.35, 0.65, 0.2, 0.4);
    drawClass_BeamParticleEff.SetNColumns(1);
    drawClass_BeamParticleEff.SetBottomMargin(0.15);
    drawClass_BeamParticleEff.SetTopMargin(0.15);

    std::vector<Particle> particles;
    particles = {POSITRON, PROTON, KAONPLUS, PIPLUS, ANTIMUON, OTHER};
    ParticleToDrawClassMap drawClassMap_BeamParticleEff, drawClassMap_OpeningAngle;

    for (const Particle &particle : particles)
    {
        std::string particleName(Helper::GetParticleName(particle));
        DrawClass drawClass("Beam Particle Efficiency Vs Momentum " + particleName);
        drawClass.SetRange(0.f, 8.f, 0.f, 1.05f);
        drawClass.SetLegend(0.35, 0.65, 0.2, 0.4);
        drawClass.SetNColumns(1);
        drawClass.SetBottomMargin(0.15);
        drawClass.SetTopMargin(0.15);
        drawClassMap_BeamParticleEff.insert(ParticleToDrawClassMap::value_type(particle, drawClass));

        DrawClass drawClass_OpeningAngle1("Beam Particle Opening Angle" + particleName);
        drawClass_OpeningAngle1.SetLegend(0.6, 0.8, 0.6, 0.8);
        drawClass_OpeningAngle1.SetNColumns(1);
        drawClass_OpeningAngle1.SetBottomMargin(0.15);
        drawClass_OpeningAngle1.SetTopMargin(0.15);
        drawClass_OpeningAngle1.SetTitleOffsetY(1.3);
        drawClassMap_OpeningAngle.insert(ParticleToDrawClassMap::value_type(particle, drawClass_OpeningAngle1));
    }

    DrawClass drawClass_OpeningAngle("Beam Particle Opening Angle");
    drawClass_OpeningAngle.SetLegend(0.6, 0.8, 0.6, 0.8);
    drawClass_OpeningAngle.SetNColumns(1);
    drawClass_OpeningAngle.SetBottomMargin(0.15);
    drawClass_OpeningAngle.SetTopMargin(0.15);
    drawClass_OpeningAngle.SetTitleOffsetY(1.3);

    DrawClass drawClass_NCosmicRayPfos("Number of Reconstructed Cosmic Rays");
    drawClass_NCosmicRayPfos.SetRange(0.f, 200.f, 0.f, 0.055f);
    drawClass_NCosmicRayPfos.SetNColumns(1);
    drawClass_NCosmicRayPfos.SetBottomMargin(0.15);
    drawClass_NCosmicRayPfos.SetTopMargin(0.15);
    drawClass_NCosmicRayPfos.SetLegend(0.2, 0.4, 0.6, 0.775);

    DrawClass drawClass_StitchedT0("Stitched T0");
    drawClass_StitchedT0.SetRange(-2750.f, 750.f, 0.f, 0.02f);
    drawClass_StitchedT0.SetLegend(0.35, 0.65, 0.3, 0.5);
    drawClass_StitchedT0.SetNColumns(1);
    drawClass_StitchedT0.SetBottomMargin(0.15);
    drawClass_StitchedT0.SetTopMargin(0.15);
    drawClass_StitchedT0.SetTitleOffsetY(1.25);

    for (EventClass &eventClass : eventClassVector)
    {
        int nBeamParticles(0), nBeamParticleMatches(0);
        int nDetailedTrig(0);

        // Beam Efficiency vs Momentum
        TH1F *pTH1F_BeamMCPrimaryMomentum = new TH1F("BeamMCPrimaryMomentum", "", maxBin, 0.f, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentum);
        pTH1F_BeamMCPrimaryMomentum->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentum->SetLineColor(kRed);

        TH1F *pTH1F_BeamMCPrimaryMomentum_Matched = new TH1F("BeamMCPrimaryMomentum_Matched", "", maxBin, 0.f, maxBin);
        Helper::Format(pTH1F_BeamMCPrimaryMomentum_Matched);
        pTH1F_BeamMCPrimaryMomentum_Matched->GetXaxis()->SetTitle("Beam Momentum [GeV]");
        pTH1F_BeamMCPrimaryMomentum_Matched->SetLineColor(kBlue);

        // Opening Angles
        const int nBinsAngle(100);
        const float lowBinAngle(0.f), highBinAngle(1.57f);
        TH1F *pTH1F_BeamOpeningAngle = new TH1F("Opening_Angle", "", nBinsAngle, lowBinAngle, highBinAngle);
        Helper::Format(pTH1F_BeamOpeningAngle);
        pTH1F_BeamOpeningAngle->GetXaxis()->SetTitle("Opening Angle [rad]");
        pTH1F_BeamOpeningAngle->GetYaxis()->SetTitle("Fraction of Events");

        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryMomentumTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryMomentumTotal_Matched;
        ParticleToHistMap particleToHistMap_BeamOpeningAngle;

        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));

            std::string histName("BeamMCPrimaryMomentumTotal_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Particle = new TH1F(histName.c_str(), "", maxBin, 0.f, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryMomentumTotal_Particle);
            pTH1F_BeamMCPrimaryMomentumTotal_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryMomentumTotal_Particle->SetLineColor(kRed);
            particleToHistMap_BeamMCPrimaryMomentumTotal.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryMomentumTotal_Particle));

            std::string histNameMatched("BeamMCPrimaryMomentumTotal_Matched_" + particleName);
            TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle = new TH1F(histNameMatched.c_str(), "", maxBin, 0.f, maxBin);
            Helper::Format(pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle);
            pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->GetXaxis()->SetTitle("Number of Hits");
            pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->SetLineColor(kBlue);
            particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle));

            std::string histNameAngle("Opening_Angle_" + particleName);
            TH1F *pTH1F_BeamOpeningAngle1 = new TH1F(histNameAngle.c_str(), "", nBinsAngle, lowBinAngle, highBinAngle);
            Helper::Format(pTH1F_BeamOpeningAngle1);
            pTH1F_BeamOpeningAngle1->GetXaxis()->SetTitle("Opening Angle [rad]");
            pTH1F_BeamOpeningAngle1->GetYaxis()->SetTitle("Fraction of Events");
            particleToHistMap_BeamOpeningAngle.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamOpeningAngle1));
        }

        TH1F *pTH1F_NClearCosmicRayPfos = new TH1F("NCosmicRayPfos", "", 100, 0, 100);
        Helper::Format(pTH1F_NClearCosmicRayPfos);
        pTH1F_NClearCosmicRayPfos->GetXaxis()->SetTitle("Number of Reconstructed Cosmic-Ray Muons");
        pTH1F_NClearCosmicRayPfos->GetYaxis()->SetTitle("Fraction of Events");

        TH1F *pTH1F_StitchedT0 = new TH1F("StitchedT0", "", 100, -2750, 750);
        Helper::Format(pTH1F_StitchedT0);
        pTH1F_StitchedT0->GetXaxis()->SetTitle("T_{0} [ns]");
        pTH1F_StitchedT0->GetYaxis()->SetTitle("Fraction of Events");

        TChain *pTChain = eventClass.GetTChain();

        pTChain->SetBranchAddress("isTriggered", &isTriggered);
        pTChain->SetBranchAddress("nBeamPfos", &nBeamPfos);
        pTChain->SetBranchAddress("beamMomentum", &beamMomentum);
        pTChain->SetBranchAddress("tof", &tof);
        pTChain->SetBranchAddress("ckov0Status", &ckov0Status);
        pTChain->SetBranchAddress("ckov1Status", &ckov1Status);
//        pTChain->SetBranchAddress("beamPositionX", &beamPositionX);
//        pTChain->SetBranchAddress("beamPositionY", &beamPositionY);
//        pTChain->SetBranchAddress("beamPositionZ", &beamPositionZ);
        pTChain->SetBranchAddress("beamDirectionX", &beamDirectionX);
        pTChain->SetBranchAddress("beamDirectionY", &beamDirectionY);
        pTChain->SetBranchAddress("beamDirectionZ", &beamDirectionZ);
        pTChain->SetBranchAddress("recoDirectionX", &recoDirectionX);
        pTChain->SetBranchAddress("recoDirectionY", &recoDirectionY);
        pTChain->SetBranchAddress("recoDirectionZ", &recoDirectionZ);
        pTChain->SetBranchAddress("nHitsRecoTotal", &nHitsRecoTotal);
//        pTChain->SetBranchAddress("nCosmicRayPfos", &nCosmicRayPfos);
        pTChain->SetBranchAddress("cosmicRayX0s", &cosmicRayX0s);
//        pTChain->SetBranchAddress("nHitsCosmicU", &nHitsCosmicU);
//        pTChain->SetBranchAddress("nHitsCosmicV", &nHitsCosmicV);
//        pTChain->SetBranchAddress("nHitsCosmicW", &nHitsCosmicW);
        pTChain->SetBranchAddress("nHitsCosmicTotal", &nHitsCosmicTotal);
//        pTChain->SetBranchAddress("allTestBeamScores", &allTestBeamScores);
//        pTChain->SetBranchAddress("recoDirectionCRX", &recoDirectionCRX);
//        pTChain->SetBranchAddress("recoDirectionCRY", &recoDirectionCRY);
//        pTChain->SetBranchAddress("recoDirectionCRZ", &recoDirectionCRZ);
/*
        pTChain->SetBranchAddress("crStartPointX", &crStartPointX);
        pTChain->SetBranchAddress("crStartPointY", &crStartPointY);
        pTChain->SetBranchAddress("crStartPointZ", &crStartPointZ);
        pTChain->SetBranchAddress("crEndPointX", &crEndPointX);
        pTChain->SetBranchAddress("crEndPointY", &crEndPointY);
        pTChain->SetBranchAddress("crEndPointZ", &crEndPointZ);
*/

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

            // Clear Cosmics
            int nClearCosmics(0);

            for (const int nHits : *nHitsCosmicTotal)
            {
                if (nHits > 100)
                    nClearCosmics++;
            }

            pTH1F_NClearCosmicRayPfos->Fill(nClearCosmics);

            // T0s
            for (const float x0 : *cosmicRayX0s)
            {
                const float recoT0(x0 * nsPerTick / (1000.f * cmPerTick)); // T0 in us
                pTH1F_StitchedT0->Fill(recoT0);
            }

            // Cut to make sure only dealing with clean triggered events
            if (beamMomentum > 100.f)
                continue;

            // Beam Reco Eff Vs Momentum and Opening Angles
            if (isTriggered == 1)
            {
                nDetailedTrig++;

                Particle particle(Helper::GetParticle(ckov0Status, ckov1Status, beamMomentum, tof));

                if (particle ==  OTHER && ckov1Status > 1e9 && tof > 1e9)
                    particle = Helper::GetParticleType(static_cast<int>(ckov0Status));

                if (std::find(particles.begin(), particles.end(), particle) == particles.end())
                    continue;

                TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Particle = particleToHistMap_BeamMCPrimaryMomentumTotal.at(particle);
                TH1F *pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle = particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.at(particle);

                pTH1F_BeamMCPrimaryMomentum->Fill(beamMomentum);
                pTH1F_BeamMCPrimaryMomentumTotal_Particle->Fill(beamMomentum);

                if (nBeamPfos > 0)
                {
                    pTH1F_BeamMCPrimaryMomentum_Matched->Fill(beamMomentum);
                    pTH1F_BeamMCPrimaryMomentumTotal_Matched_Particle->Fill(beamMomentum);

                    const float mag1(std::sqrt(beamDirectionX*beamDirectionX + beamDirectionY*beamDirectionY + beamDirectionZ*beamDirectionZ));
                    float cosTheta(0.f);
                    int maxNHits(0);

                    for (unsigned int i = 0; i < nHitsRecoTotal->size(); i++)
                    {
                        const int nHits(nHitsRecoTotal->at(i));
                        if (nHits > maxNHits)
                        {
                            maxNHits = nHits;
                            const float x(recoDirectionX->at(i));
                            const float y(recoDirectionY->at(i));
                            const float z(recoDirectionZ->at(i));
                            const float mag2(std::sqrt(x*x+y*y+z*z));
                            cosTheta = std::fabs(x*beamDirectionX + y*beamDirectionY + z*beamDirectionZ) / (mag1 *mag2);
                        }
                    }

                    pTH1F_BeamOpeningAngle->Fill(std::acos(cosTheta));
                    particleToHistMap_BeamOpeningAngle.at(particle)->Fill(std::acos(cosTheta));
                }
            }
        }

        std::cout << "Event Description      : " << eventClass.GetDescription() << std::endl;
        std::cout << "nBeamParticles         : " << nBeamParticles << std::endl;
        std::cout << "nBeamParticleMatches   : " << nBeamParticleMatches << std::endl;
        const float beamParticleEfficiency(static_cast<float>(nBeamParticleMatches)/static_cast<float>(nBeamParticles));
        std::cout << "Efficiency             : " << beamParticleEfficiency << std::endl;
        const float beamParticleEfficiencyError(std::sqrt(beamParticleEfficiency * (1-beamParticleEfficiency) / static_cast<float>(nBeamParticles)));
        std::cout << "Efficiency Error       : " << beamParticleEfficiencyError << std::endl;
        std::cout << "Efficiency [%]         : " << beamParticleEfficiency*100.f << "+-" << beamParticleEfficiencyError*100.f << std::endl;
        const std::string beamEffStr(Helper::ToStringPrecision(beamParticleEfficiency*100.f, 2) + "#pm" + Helper::ToStringPrecision(beamParticleEfficiencyError*100.f, 2) + "%");
        std::cout << "nDetailedTrig          : " << nDetailedTrig << std::endl;
        std::cout << "Fraction Detailed Trig : " << static_cast<float>(nDetailedTrig)/static_cast<float>(nBeamParticles) << std::endl;

        TGraphErrors *pTGraphErrors_BeamMomentumEfficiency = Helper::MakeEfficiency(pTH1F_BeamMCPrimaryMomentum, pTH1F_BeamMCPrimaryMomentum_Matched, "BeamMomentumEfficiency", 10);
        Helper::Format(pTGraphErrors_BeamMomentumEfficiency);
        pTGraphErrors_BeamMomentumEfficiency->GetXaxis()->SetTitle("Momentum [GeV]");
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetTitle("Reconstruction Efficiency");
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetRangeUser(0,1);
        pTGraphErrors_BeamMomentumEfficiency->GetYaxis()->SetDecimals();
//        const int nEvtsTotal(pTH1F_BeamMCPrimaryMomentum->GetEntries());
        drawClass_BeamParticleEff.AddGraph(pTGraphErrors_BeamMomentumEfficiency, eventClass.GetDescription());

        for (const Particle &particle : particles)
        {
            std::string particleName(Helper::GetParticleName(particle));
            TGraphErrors *pTGraphErrors_BeamMomentumEfficiency_Particle  = Helper::MakeEfficiency(particleToHistMap_BeamMCPrimaryMomentumTotal.at(particle), particleToHistMap_BeamMCPrimaryMomentumTotal_Matched.at(particle), "BeamMomentumEfficiency" + particleName, 10);
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetXaxis()->SetTitle("Momentum [GeV]");
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetTitle("Reconstruction Efficiency");
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetRangeUser(0,1);
            pTGraphErrors_BeamMomentumEfficiency_Particle->GetYaxis()->SetDecimals();
            drawClassMap_BeamParticleEff.at(particle).AddGraph(pTGraphErrors_BeamMomentumEfficiency_Particle, eventClass.GetDescription());
            delete pTGraphErrors_BeamMomentumEfficiency_Particle;

            drawClassMap_OpeningAngle.at(particle).AddHisto(particleToHistMap_BeamOpeningAngle.at(particle), eventClass.GetDescription());
        }

        drawClass_OpeningAngle.AddHisto(pTH1F_BeamOpeningAngle, eventClass.GetDescription());
        drawClass_NCosmicRayPfos.AddHisto(pTH1F_NClearCosmicRayPfos, eventClass.GetDescription());
        drawClass_StitchedT0.AddHisto(pTH1F_StitchedT0, eventClass.GetDescription());

        delete pTH1F_BeamMCPrimaryMomentum;
        delete pTH1F_BeamMCPrimaryMomentum_Matched;
        delete pTH1F_BeamOpeningAngle;
        delete pTH1F_NClearCosmicRayPfos;
        delete pTH1F_StitchedT0;

        for (const auto &iter : particleToHistMap_BeamMCPrimaryMomentumTotal)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamMCPrimaryMomentumTotal_Matched)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamOpeningAngle)
            delete iter.second;
    }

    drawClass_BeamParticleEff.Draw();
    drawClass_OpeningAngle.Draw();

    for (const auto &iter : drawClassMap_BeamParticleEff)
        iter.second.Draw();

    for (const auto &iter : drawClassMap_OpeningAngle)
        iter.second.Draw();

    drawClass_NCosmicRayPfos.Draw();
    drawClass_StitchedT0.Draw();

    return 0;
}

//==================================================
