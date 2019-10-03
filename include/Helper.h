/**
 *  @file   include/Helper.h
 * 
 *  @brief  Header file for helper class.
 * 
 *  $Log: $
 */
#ifndef HELPER_H
#define HELPER_H 1

#include <string>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"

namespace analysis
{

enum Particle {ELECTRON, POSITRON, PHOTON, MUON, ANTIMUON, PROTON, ANTIPROTON, NEUTRON, ANTINEUTRON, NEUTRINO, PIPLUS, PIMINUS, PIZERO, KAON0L, KAON0S, KAONPLUS, KAONMINUS, OTHER};

typedef std::vector<std::string> StringVector;
typedef std::vector<int> IntVector;
typedef std::vector<float> FloatVector;
typedef std::map<std::string, int> StringIntMap;
typedef std::map<std::string, float> StringFloatMap;
typedef std::map<std::string, IntVector*> StringIntVectorMap;
typedef std::map<std::string, FloatVector*> StringFloatVectorMap;
typedef std::map<std::string, std::string> StringMap;

class Helper
{
public:
    /**
     *  @brief Constructor
     */
    Helper();

    /**
     *  @brief Destructor
     */
    ~Helper();

    /**
     *  @brief Calculate cos theta in a 2D projection
     *
     *  @param x1
     *  @param y1
     *  @param x2
     *  @param y2
     */
    static float CalculateCosTheta2D(const float x1, const float y1, const float x2, const float y2);

    /**
     *  @brief Calculate the full width half maximum for a histogram
     *
     *  @param pTH1F target histogram
     */
    static float GetFullWidthAtHalfMaximum(TH1F *pTH1F);

    /**
     *  @brief Return particle enum from pdg
     *
     *  @param pdg code of target particle
     */
    static Particle GetParticleType(int pdg);

    /**
     *  @brief Return string name of particle (root label format)
     *
     *  @param part target particle
     */
    static const char *GetParticleString(Particle part);

    /**
     *  @brief Return string name of particle (pure string, no funny symbols)
     *
     *  @param part target particle
     */
    static const char *GetParticleName(Particle part);

    /**
     *  @brief Return a colour int based on particle type
     *
     *  @param part target particle
     */
    static int ParticleToLineColor(Particle part);

    /**
     *  @brief Save a canvas to pdf and .C with given name and momentum string
     *
     *  @param pTCanvas canvas to save
     *  @param name of output files
     *  @param momentum string for files
     */
    static void Save(TCanvas *pTCanvas, std::string name, int momenta = 0);

    /**
     *  @brief Make a histogram look nice on a logx plot
     *
     *  @param pTH1F initial histogram
     */
    static void BinLogX(TH1F *pTH1F);

    /**
     *  @brief Format a given root histogram
     *
     *  @param pT root histogram
     */
    template<typename T>
    static void Format(T *&pT);

    /**
     *  @brief Make an efficiency plot from two histograms
     *
     *  @param pTH1F_Total historgram of total events 
     *  @param pTH1F_Matched histogram of selected events
     *  @param label for histogram
     *  @param cut for plotting points
     */
    static TGraphErrors *MakeEfficiency(TH1F *pTH1F_Total, TH1F *pTH1F_Matched, const std::string &label, const int cut = 0);

    /**
     *  @brief Get particle type based on cherenkov status and tof
     *
     *  @param ckov0Status status of first cherenkov detector
     *  @param ckov1Status status of second cherenkov detector
     *  @param momentum beam momentum
     *  @param tof time of flight
     */
    static Particle GetParticle(const int ckov0Status, const int ckov1Status, const float momentum, const float tof);

    /**
     *  @brief Make an efficiency plot from two 2D histograms
     *
     *  @param pTH1F_Total historgram of total events 
     *  @param pTH1F_Matched histogram of selected events
     *  @param label for histogram
     *  @param cut min nEntries in histogram to use for calculating efficiency
     */
    static TH2F *MakeEfficiency2D(TH2F *pTH2F_Total, TH2F *pTH2F_Matched, std::string label, const int cut = 0);

    /**
     *  @brief Object to string
     *
     *  @param t object
     */
    template<typename T>
    static std::string ToString(T t);

    /**
     *  @brief Object to string with given precision
     *
     *  @param t object
     *  @param n of decimal places to include
     */
    template<typename T>
    static std::string ToStringPrecision(const T a_value, const int n = 6);

    /**
     *  @brief  Function indicating whether two root files are identical
     *
     *  @param  fileNameA first root file name
     *  @param  fileNameB second root file name
     *  @param  treeName tree name to compare
     *  @param  intStrings list of int variables in root tree
     *  @param  floatStrings list of float variables in root tree
     *  @param  intVectorStrings list of std::vector<int> variables in root tree
     *  @param  floatVectorStrings list of std::vector<float> variables in root tree
     *  @param  nameSwitch optional map of stringA to stringB where stringA/stringB is the variable name in filenameA/filenameB, only used if variable names differ
     *
     *  @return bool are files identical
     */
    static bool CompareFiles(const std::string &fileNameA, const std::string &fileNameB, const std::string &treeName,
        const StringVector &intStrings, const StringVector &floatStrings, const StringVector &intVectorStrings,
        const StringVector &floatVectorStrings, const StringMap &nameSwitch = StringMap());

    /**
     *  @brief  Function to check whether file exists
     *
     *  @param  fileName input file name
     *
     *  @return bool does file exist
     */
    static bool DoesFileExist(const std::string &fileName);

    /**
     *  @brief  Function to check whether two std::vector<floats> are identical
     *
     *  @param  floatVectorA first input vector
     *  @param  floatVectorB second input vector
     *
     *  @return bool are vectors identical
     */
    static bool AreFloatVectorsIdentical(const FloatVector &floatVectorA, const FloatVector &floatVectorB);
};

} // namespace analysis

#endif // #ifndef HELPER_H
