# Analysis_ProtoDUNE_RecoMetrics

This repository contains three executables : 

1) Analysis : Making plots based on the TestBeamEventValidationAlgorithm output root trees.  This algorithm exists within the standard Pandora software packages.

2) DataAnalysis : Making plots based on the ProtoDUNEDataAnalysisAlgorithm output root trees.  This algorithm exists within a feature branch of LArContent (https://github.com/StevenGreen1/LArContent/blob/feature/ProtoDUNEDataAnalysis/larpandoracontent/LArMonitoring/ProtoDUNEAnalysisAlgorithm.cc).

3) Differences : List the differences between root files.   (User has to specify branch names and types.)

# Build Instructions
1) Source a version of root.
2) `mkdir build`
3) `cd build`
4) `cmake ..`
5) `make install`

The executables will be created in the bin directory.
