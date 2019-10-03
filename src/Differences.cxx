/**
 *  @file   src/Differences.cc
 *
 *  @brief  Implementation of the int main function for code designed to compare root files in bulk.
 *
 *  $Log: $
 */
#include <iostream>

#include "Helper.h"

using namespace analysis;

int main(int argc, char *argv[])
{
    StringVector intStrings = {"fileIdentifier", "eventNumber", "mcNuanceCode", "isBeamParticle", "isCosmicRay", "nTargetPrimaries",
                               "nTargetMatches", "nTargetCRMatches", "isCorrectTB", "isCorrectCR", "nTargetTBHierarchyMatches",
                               "isFakeCR", "isSplitCR", "isSplitTB", "isLost", "isFakeTB", "nTargetTBMatches", "interactionType",
                               "nTargetGoodTBHierarchyMatches", "nTargetTBHierarchySplits", "nTargetTBHierarchyLosses",
                               "isCorrectTBHierarchy", "isFakeTBHierarchy", "isSplitTBHierarchy"};

    StringVector floatStrings = {"targetVertexX", "targetVertexY", "targetVertexZ"}; // "recoVertexX", "recoVertexY", "recoVertexZ"

    StringVector intVectorStrings = {"mcPrimaryId", "mcPrimaryPdg", "mcPrimaryNHitsTotal", "mcPrimaryNHitsU", "mcPrimaryNHitsV",
                                     "mcPrimaryNHitsW", "nPrimaryMatchedPfos", "nPrimaryMatchedCRPfos",
                                     "bestMatchPfoPdg", "bestMatchPfoNHitsTotal", "bestMatchPfoNHitsU",
                                     "bestMatchPfoNHitsV", "bestMatchPfoNHitsW", "bestMatchPfoNSharedHitsTotal", "bestMatchPfoNSharedHitsU",
                                     "bestMatchPfoNSharedHitsV", "bestMatchPfoNSharedHitsW", "bestMatchPfoIsTB",
                                     "nPrimaryMatchedTBHierarchyPfos", "bestMatchPfoTier"};
                                     // "mcPrimaryTier", "bestMatchPfoIsTestBeam", "bestMatchPfoIsTestBeamHierarchy", "bestMatchPfoRecoTBId", "bestMatchPfoId"

    StringVector floatVectorStrings = {"mcPrimaryE", "mcPrimaryPX", "mcPrimaryPY", "mcPrimaryPZ", "mcPrimaryVtxX", "mcPrimaryVtxY",
                                       "mcPrimaryVtxZ", "mcPrimaryEndX", "mcPrimaryEndY", "mcPrimaryEndZ"};

// Differeces : "recoVertexX", "recoVertexY", "recoVertexZ", "mcPrimaryTier", "bestMatchPfoIsTestBeam", "bestMatchPfoIsTestBeamHierarchy", "bestMatchPfoRecoTBId", "bestMatchPfoId"

//    StringMap nameSwitch;

    for (int momentum = 1; momentum <=7; momentum = momentum + 2)
    {
        for (int jobNumber = 1; jobNumber <= 100; jobNumber++)
        {
            const std::string fileNameA("/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/October/Vertexing/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/" + std::to_string(momentum) + "GeV/SpaceCharge/RootFiles/Vertexing_Job_Number_" + std::to_string(jobNumber) + ".root");
            const std::string fileNameB("/r07/dune/sg568/LAr/Jobs/protoDUNE/2019/October/Vertexing_Default/AnalysisTag1/mcc11_Pndr/Beam_Cosmics/" + std::to_string(momentum) + "GeV/SpaceCharge/RootFiles/Vertexing_Default_Job_Number_" + std::to_string(jobNumber) + ".root");

            if (!Helper::DoesFileExist(fileNameA))
            {
                std::cout << "File does not exist : " << fileNameA << std::endl;
                continue;
            }

            if (!Helper::DoesFileExist(fileNameB))
            {
                std::cout << "File does not exist : " << fileNameB << std::endl;
                continue;
            }

            if (!Helper::CompareFiles(fileNameA, fileNameB, "Validation", intStrings, floatStrings, intVectorStrings, floatVectorStrings))
            {
                std::cout << "Files differ : " << std::endl;
                std::cout << fileNameA << std::endl;
                std::cout << fileNameB << std::endl;
            }
        }
    }
}
