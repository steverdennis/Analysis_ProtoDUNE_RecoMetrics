#ifndef LAR_SPACE_CHARGE_HELPER_H
#define LAR_SPACE_CHARGE_HELPER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "TGraph.h"
#include "TFile.h"
#include "TF1.h"

namespace analysis
{

typedef std::vector<float> FloatVector;

class SpaceChargeHelper
{
public:
    static void Configure(const std::string &fileName);
    static FloatVector GetSpaceChargeCorrectedPosition(const FloatVector &inputPoistion);
    static FloatVector GetPositionOffset(const FloatVector &positionOffset);

protected:
    static float GetParametricPositionOffset(const FloatVector &transformedPosition, const std::string &axisLabel);
    static float TransformX(float xPosition);
    static float TransformY(float yPosition);
    static float TransformZ(float zPosition);

    };
}

#endif // LAR_SPACE_CHARGE_HELPER_H
