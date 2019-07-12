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
    TApplication *pTApplication = new TApplication("Analysis", &argc,argv);
    if (!pTApplication)
    {
        std::cout << "TApplication problem." << std::endl;
    }

    gROOT->SetBatch();
    Style();

    SpaceChargeHelper::Configure("/usera/sg568/LAr/Jobs/protoDUNE/2019/April/BDTVertexing/SpaceChargeCorrections/SCEoffsets_ProtoDUNE_E500.root");

    EventClassVector eventClassVector;

//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B+C, BDTVtx : NTree 100, Depth 3");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B+C, BDTVtx : NTree 100, Depth 6");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B, BDTVtx : NTree 100, Depth 6");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_VetoHalo/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_VetoHalo/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_VetoHalo/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B-H, BDTVtx : NTree 100, Depth 6");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/1GeV/NoSpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/5GeV/NoSpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag2/mcc11_Pndr/Beam_Cosmics/7GeV/NoSpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, NoSCE, B+C, BDTVtx : NTree 100, Depth 6");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B+C, Default");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B, Default");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_VetoHalo/1GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_VetoHalo/5GeV/SpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_VetoHalo/7GeV/SpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, SCE, B-H, Default");
//    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/NoSpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/NoSpaceCharge/RootFiles/*.root:/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/NoSpaceCharge/RootFiles/*.root", "1, 5, 7 GeV, NoSCE, B+C, Default");

/*
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root", "1 GeV, SCE, B+C, BDTVtx");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root", "5 GeV, SCE, B+C, BDTVtx");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_BDTVtx/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "7 GeV, SCE, B+C, BDTVtx");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/1GeV/SpaceCharge/RootFiles/*.root", "1 GeV, SCE, B+C, DefaultVtx");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/5GeV/SpaceCharge/RootFiles/*.root", "5 GeV, SCE, B+C, DefaultVtx");
    eventClassVector.emplace_back("/r05/dune/sg568/LAr/Jobs/protoDUNE/2019/May/ProtoDUNE_RecoMetrics_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/7GeV/SpaceCharge/RootFiles/*.root", "7 GeV, SCE, B+C, DefaultVtx");
*/

    std::vector<Particle> particles;

    if (true)
    {
        particles = {POSITRON, PIPLUS, PROTON, ANTIMUON, KAONPLUS}; //, ANTIMUON, KAONPLUS, PIPLUS};
    }
    else
    {
        particles = {ELECTRON, ANTIPROTON, MUON, KAONMINUS, PIMINUS};
    }

    int mcNuanceCode(0), isBeamParticle(0), isCosmicRay(0), nNuMatches(0), nCRMatches(0), eventNumber(0);
    float recoVertexX(std::numeric_limits<float>::max()), recoVertexY(std::numeric_limits<float>::max()), recoVertexZ(std::numeric_limits<float>::max());
    std::vector<int> *mcPrimaryPdg(nullptr), *mcPrimaryNHitsTotal(nullptr), *bestMatchPfoPdg(nullptr), *bestMatchPfoNHitsTotal(nullptr),*bestMatchPfoNSharedHitsTotal(nullptr);
    std::vector<float> *mcPrimaryVtxX(nullptr), *mcPrimaryVtxY(nullptr), *mcPrimaryVtxZ(nullptr), *mcPrimaryEndX(nullptr), *mcPrimaryEndY(nullptr), *mcPrimaryEndZ(nullptr);

    const int vtxBins(100);
    const float vtxMax(50);
    //const float vtxMax(500);

    DrawClass drawClass_BeamParticleEff("Beam Particle Efficiency Vs NHits");
    drawClass_BeamParticleEff.SetLogX(true);
    drawClass_BeamParticleEff.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_BeamParticleEff.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_CosmicRayEff("Cosmic Ray Efficiency Vs NHits");
    drawClass_CosmicRayEff.SetLogX(true);
    drawClass_CosmicRayEff.SetRange(0.f, 0.f, 0.f, 1.05f);
    drawClass_CosmicRayEff.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_VtxDX("Beam Particle Vertex Delta X");
    drawClass_VtxDX.SetLogY(true);
    drawClass_VtxDX.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
    drawClass_VtxDX.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_VtxDY("Beam Particle Vertex Delta Y");
    drawClass_VtxDY.SetLogY(true);
    drawClass_VtxDY.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
    drawClass_VtxDY.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_VtxDZ("Beam Particle Vertex Delta Z");
    drawClass_VtxDZ.SetLogY(true);
    drawClass_VtxDZ.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
    drawClass_VtxDZ.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_VtxDR("Beam Particle Vertex Delta R");
    drawClass_VtxDR.SetLogY(true);
    drawClass_VtxDR.SetRange(-0.1f*vtxMax, vtxMax, 1e-4, 1.05f);
    drawClass_VtxDR.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    DrawClass drawClass_VtxZ("Beam Particle Vertex Z");
    drawClass_VtxZ.SetLogY(true);
    drawClass_VtxZ.SetRange(-1000, 1000, 1e-4, 1.05f);
    drawClass_VtxZ.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);

    typedef std::map<Particle, DrawClass> ParticleToDrawClassMap;
    ParticleToDrawClassMap drawClassMap_BeamParticleEff;
    ParticleToDrawClassMap drawClassMap_BeamParticleComp;
    ParticleToDrawClassMap drawClassMap_BeamParticlePur;
    ParticleToDrawClassMap drawClassMap_VtxDX;
    ParticleToDrawClassMap drawClassMap_VtxDY;
    ParticleToDrawClassMap drawClassMap_VtxDZ;
    ParticleToDrawClassMap drawClassMap_VtxDR;

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

        DrawClass drawClass_VtxDX("Beam Particle Vertex Delta X " + particleName);
        drawClass_VtxDX.SetLogY(true);
        drawClass_VtxDX.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
        drawClass_VtxDX.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
        drawClassMap_VtxDX.insert(ParticleToDrawClassMap::value_type(particle, drawClass_VtxDX));

        DrawClass drawClass_VtxDY("Beam Particle Vertex Delta Y " + particleName);
        drawClass_VtxDY.SetLogY(true);
        drawClass_VtxDY.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
        drawClass_VtxDY.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
        drawClassMap_VtxDY.insert(ParticleToDrawClassMap::value_type(particle, drawClass_VtxDY));

        DrawClass drawClass_VtxDZ("Beam Particle Vertex Delta Z " + particleName);
        drawClass_VtxDZ.SetLogY(true);
        drawClass_VtxDZ.SetRange(-vtxMax, vtxMax, 1e-4, 1.05f);
        drawClass_VtxDZ.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
        drawClassMap_VtxDZ.insert(ParticleToDrawClassMap::value_type(particle, drawClass_VtxDZ));

        DrawClass drawClass_VtxDR("Beam Particle Vertex Delta R " + particleName);
        drawClass_VtxDR.SetLogY(true);
        drawClass_VtxDR.SetRange(-0.1f*vtxMax, vtxMax, 1e-4, 1.05f);
        drawClass_VtxDR.SetLegend(0.05f, 0.95f, 0.825f, 0.975f);
        drawClassMap_VtxDR.insert(ParticleToDrawClassMap::value_type(particle, drawClass_VtxDR));
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

        TH1F *pTH1F_BeamVtxDX = new TH1F("BeamVtxDX", "", 2*vtxBins, -1.f*vtxMax, vtxMax);
        Helper::Format(pTH1F_BeamVtxDX);
        pTH1F_BeamVtxDX->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaX [cm]");

        TH1F *pTH1F_BeamVtxDY = new TH1F("BeamVtxDY", "", 2*vtxBins, -1.f*vtxMax, vtxMax);
        Helper::Format(pTH1F_BeamVtxDY);
        pTH1F_BeamVtxDY->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaY [cm]");

        TH1F *pTH1F_BeamVtxDZ = new TH1F("BeamVtxDZ", "", 2*vtxBins, -1.f*vtxMax, vtxMax);
        Helper::Format(pTH1F_BeamVtxDZ);
        pTH1F_BeamVtxDZ->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaZ [cm]");

        TH1F *pTH1F_BeamVtxDR = new TH1F("BeamVtxDR", "", 1.1f*vtxBins, -0.1f*vtxMax, vtxMax);
        Helper::Format(pTH1F_BeamVtxDR);
        pTH1F_BeamVtxDR->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");

        TH1F *pTH1F_BeamVtxZ = new TH1F("BeamVtxZ", "", 1000, -1000, 1000);
        Helper::Format(pTH1F_BeamVtxZ);
        pTH1F_BeamVtxZ->GetXaxis()->SetTitle("Beam Particle Vertex Z [cm]");

        typedef std::map<Particle, TH1F*> ParticleToHistMap;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal;
        ParticleToHistMap particleToHistMap_BeamMCPrimaryNHitsTotal_Matched;
        ParticleToHistMap particleToHistMap_BeamParticleCompleteness;
        ParticleToHistMap particleToHistMap_BeamParticlePurity;
        ParticleToHistMap particleToHistMap_BeamVtxDX;
        ParticleToHistMap particleToHistMap_BeamVtxDY;
        ParticleToHistMap particleToHistMap_BeamVtxDZ;
        ParticleToHistMap particleToHistMap_BeamVtxDR;

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

            histName = "BeamVtxDX" + particleName;
            TH1F *pTH1F_BeamVtxDX_Particle = new TH1F(histName.c_str(), "", 2*vtxBins, -1.f*vtxMax, vtxMax);
            Helper::Format(pTH1F_BeamVtxDX_Particle);
            pTH1F_BeamVtxDX_Particle->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaX [cm]");
            particleToHistMap_BeamVtxDX.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamVtxDX_Particle));

            histName = "BeamVtxDY" + particleName;
            TH1F *pTH1F_BeamVtxDY_Particle = new TH1F(histName.c_str(), "", 2*vtxBins, -1.f*vtxMax, vtxMax);
            Helper::Format(pTH1F_BeamVtxDY_Particle);
            pTH1F_BeamVtxDY_Particle->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaY [cm]");
            particleToHistMap_BeamVtxDY.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamVtxDY_Particle));

            histName = "BeamVtxDZ" + particleName;
            TH1F *pTH1F_BeamVtxDZ_Particle = new TH1F(histName.c_str(), "", 2*vtxBins, -1.f*vtxMax, vtxMax);
            Helper::Format(pTH1F_BeamVtxDZ_Particle);
            pTH1F_BeamVtxDZ_Particle->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaZ [cm]");
            particleToHistMap_BeamVtxDZ.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamVtxDZ_Particle));

            histName = "BeamVtxDR" + particleName;
            TH1F *pTH1F_BeamVtxDR_Particle = new TH1F(histName.c_str(), "", 1.1f*vtxBins, -0.1f*vtxMax, vtxMax);
            Helper::Format(pTH1F_BeamVtxDR_Particle);
            pTH1F_BeamVtxDR_Particle->GetXaxis()->SetTitle("Beam Particle Vertex #DeltaR [cm]");
            particleToHistMap_BeamVtxDR.insert(ParticleToHistMap::value_type(particle, pTH1F_BeamVtxDR_Particle));
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
        pTChain->SetBranchAddress("recoVertexX", &recoVertexX);
        pTChain->SetBranchAddress("recoVertexY", &recoVertexY);
        pTChain->SetBranchAddress("recoVertexZ", &recoVertexZ);

        pTChain->SetBranchAddress("mcPrimaryPdg", &mcPrimaryPdg);
        pTChain->SetBranchAddress("mcPrimaryNHitsTotal", &mcPrimaryNHitsTotal);
        pTChain->SetBranchAddress("mcPrimaryVtxX", &mcPrimaryVtxX);
        pTChain->SetBranchAddress("mcPrimaryVtxY", &mcPrimaryVtxY);
        pTChain->SetBranchAddress("mcPrimaryVtxZ", &mcPrimaryVtxZ);
        pTChain->SetBranchAddress("mcPrimaryEndX", &mcPrimaryEndX);
        pTChain->SetBranchAddress("mcPrimaryEndY", &mcPrimaryEndY);
        pTChain->SetBranchAddress("mcPrimaryEndZ", &mcPrimaryEndZ);
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

                if (std::find(particles.begin(), particles.end(), particle) == particles.end()) continue;

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

                const bool isShower(std::fabs(mcPrimaryPdg->at(0)) == 11 || mcPrimaryPdg->at(0) == 22);
                const float vtxX(isShower ? mcPrimaryVtxX->at(0) : mcPrimaryEndX->at(0));
                const float vtxY(isShower ? mcPrimaryVtxY->at(0) : mcPrimaryEndY->at(0));
                const float vtxZ(isShower ? mcPrimaryVtxZ->at(0) : mcPrimaryEndZ->at(0));

                pTH1F_BeamVtxZ->Fill(vtxZ);

                if (vtxZ < -50.f) continue;

                FloatVector position;
                position.push_back(recoVertexX);
                position.push_back(recoVertexY);
                position.push_back(recoVertexZ);

                FloatVector sccPosition = position;

                if (eventClass.GetDescription().find("NoSCE") == std::string::npos)
                    sccPosition = SpaceChargeHelper::GetSpaceChargeCorrectedPosition(position);

                const float vtxDeltaX(sccPosition.at(0) - vtxX);
                const float vtxDeltaY(sccPosition.at(1) - vtxY);
                const float vtxDeltaZ(sccPosition.at(2) - vtxZ);
                const float vtxDeltaR(std::sqrt(vtxDeltaX*vtxDeltaX + vtxDeltaY*vtxDeltaY + vtxDeltaZ*vtxDeltaZ));

                pTH1F_BeamVtxDX->Fill(vtxDeltaX);
                pTH1F_BeamVtxDY->Fill(vtxDeltaY);
                pTH1F_BeamVtxDZ->Fill(vtxDeltaZ);
                pTH1F_BeamVtxDR->Fill(vtxDeltaR);

//std::cout << "vtxX, vtxY, vtxZ : " << vtxX << ", " << vtxY << ", " << vtxZ << std::endl;
//std::cout << "DeltaR : " << vtxDeltaR << std::endl;
                TH1F *pTH1F_BeamVtxDX_Particle = particleToHistMap_BeamVtxDX.at(particle);
                TH1F *pTH1F_BeamVtxDY_Particle = particleToHistMap_BeamVtxDY.at(particle);
                TH1F *pTH1F_BeamVtxDZ_Particle = particleToHistMap_BeamVtxDZ.at(particle);
                TH1F *pTH1F_BeamVtxDR_Particle = particleToHistMap_BeamVtxDR.at(particle);

                pTH1F_BeamVtxDX_Particle->Fill(vtxDeltaX);
                pTH1F_BeamVtxDY_Particle->Fill(vtxDeltaY);
                pTH1F_BeamVtxDZ_Particle->Fill(vtxDeltaZ);
                pTH1F_BeamVtxDR_Particle->Fill(vtxDeltaR);
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

        drawClass_VtxDX.AddHisto(pTH1F_BeamVtxDX, eventClass.GetDescription());
        drawClass_VtxDY.AddHisto(pTH1F_BeamVtxDY, eventClass.GetDescription());
        drawClass_VtxDZ.AddHisto(pTH1F_BeamVtxDZ, eventClass.GetDescription());
        drawClass_VtxDR.AddHisto(pTH1F_BeamVtxDR, eventClass.GetDescription());
        drawClass_VtxZ.AddHisto(pTH1F_BeamVtxZ, eventClass.GetDescription());

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
            drawClassMap_VtxDX.at(particle).AddHisto(particleToHistMap_BeamVtxDX.at(particle), eventClass.GetDescription());
            drawClassMap_VtxDY.at(particle).AddHisto(particleToHistMap_BeamVtxDY.at(particle), eventClass.GetDescription());
            drawClassMap_VtxDZ.at(particle).AddHisto(particleToHistMap_BeamVtxDZ.at(particle), eventClass.GetDescription());
            drawClassMap_VtxDR.at(particle).AddHisto(particleToHistMap_BeamVtxDR.at(particle), eventClass.GetDescription());
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
        delete pTH1F_BeamVtxDX;
        delete pTH1F_BeamVtxDY;
        delete pTH1F_BeamVtxDZ;
        delete pTH1F_BeamVtxDR;
        delete pTH1F_BeamVtxZ;
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
        for (const auto &iter : particleToHistMap_BeamVtxDX)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamVtxDY)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamVtxDZ)
            delete iter.second;
        for (const auto &iter : particleToHistMap_BeamVtxDR)
            delete iter.second;
    }

    drawClass_BeamParticleEff.Draw();
    drawClass_VtxDX.Draw();
    drawClass_VtxDY.Draw();
    drawClass_VtxDZ.Draw();
    drawClass_VtxDR.Draw();
    drawClass_VtxZ.Draw();

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

    for (const auto &iter : drawClassMap_VtxDX)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_VtxDY)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_VtxDZ)
        iter.second.Draw();
    for (const auto &iter : drawClassMap_VtxDR)
        iter.second.Draw();
    return 0;
}

//==================================================
