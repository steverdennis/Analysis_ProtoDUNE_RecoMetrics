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

namespace analysis
{

enum Particle {ELECTRON, POSITRON, PHOTON, MUON, ANTIMUON, PROTON, ANTIPROTON, NEUTRON, ANTINEUTRON, NEUTRINO, PIPLUS, PIMINUS, PIZERO, KAON0L, KAON0S, KAONPLUS, KAONMINUS, OTHER};

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
};

} // namespave analysis

#endif // #ifndef HELPER_H
