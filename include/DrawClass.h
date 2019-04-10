/**
 *  @file   include/DrawClass.h
 *
 *  @brief  Header file for the event class.
 *
 *  $Log: $
 */

#ifndef DRAW_CLASS_H
#define DRAW_CLASS_H 1

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"

namespace analysis
{

typedef std::map<int, std::string> IntToStringMap;

class DrawClass
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  verboseString dscription
     *  @param  momentum
     */
    DrawClass(const std::string &verboseString, const float momentum = 0.f);

    /**
     *  @brief  Destructor
     */
    ~DrawClass();

    /**
     *  @brief  Copy constructor
     *
     *  @param  rhs the DrawClass to copy
     */
    DrawClass(const DrawClass &rhs);

    /**
     *  @brief  Assignment operator
     *
     *  @param  rhs the DrawClass to assign
     */
    DrawClass &operator=(const DrawClass &rhs);

    /**
     *  @brief  Add graph to draw class
     *
     *  @param  pTGraphErrors to add to class
     *  @param  description
     */
    void AddGraph(TGraphErrors *pTGraphErrors, const std::string &description);

    /**
     *  @brief  Add graph to draw class
     *
     *  @param  pTH1F to add to class
     *  @param  description
     */
    void AddHisto(TH1F *pTH1F, const std::string &description);

    /**
     *  @brief  Add histo to draw class
     *
     *  @param  pTH2F to add to class
     *  @param  description
     */
    void Add2DHisto(TH2F *pTH2F, const std::string &description);

    /**
     *  @brief  Draw all graphs and save plots
     */
    void Draw() const;

    /**
     *  @brief  Set whether using log x axis
     *
     *  @param setLogX
     */
    void SetLogX(bool setLogX);

    /**
     *  @brief  Set whether using log y axis
     *
     *  @param setLogY
     */
    void SetLogY(bool setLogY);

    /**
     *  @brief  Set whether to normalise histograms
     *
     *  @param norm
     */
    void Normalise(bool norm);

    /**
     *  @brief  Set whether to draw a square plot
     *
     *  @param squarePlot
     */
    void SquarePlot(bool squarePlot);

    /**
     *  @brief  Set axis range
     *
     *  @param  minX
     *  @param  maxX
     *  @param  minY
     *  @param  maxY
     */
    void SetRange(float minX, float maxX, float minY, float maxY);

    /**
     *  @brief  Set legend position
     *
     *  @param  minX
     *  @param  maxX
     *  @param  minY
     *  @param  maxY
     */
    void SetLegend(float minX, float maxX, float minY, float maxY);

    /**
     *  @brief  Set nColumns in legend
     *
     *  @param  nCol
     */
    void SetNColumns(int nCol);

    /**
     *  @brief  Add labels to x axis
     *
     *  @param  xAxisLabels map of bin number to label
     */
    void AddXAxisLabels(IntToStringMap xAxisLabels);

private:
    /**
     *  @brief  Graph and additional info class
     */
    class IndexedGraph
    {
    public:
        /**
         *  @brief  Constructor
         *
         *  @param  pTGraphErrors graph
         *  @param  description
         */
        IndexedGraph(TGraphErrors *pTGraphErrors, const std::string &description);

        /**
         *  @brief  Destructor 
         */
        ~IndexedGraph();

        /**
         *  @brief  Copy constructor
         *
         *  @param  rhs the IndexedGraph to copy
         */
        IndexedGraph(const IndexedGraph &rhs);

        /**
         *  @brief  Assignment operator
         *
         *  @param  rhs the IndexedGraph to assign
         */
        IndexedGraph &operator=(const IndexedGraph &rhs);

        /**
         *  @brief  Get graph
         */
        TGraphErrors *GetGraph();

        /**
         *  @brief  Get descirption
         */
        std::string GetDescription();

    private:
        TGraphErrors *m_pTGraphErrors; ///< Graph
        std::string   m_desciption;    ///< Description
    };

    typedef std::vector<IndexedGraph*> GraphVector;

    /**
     *  @brief  Histo and additional info class
     */
    class IndexedHisto
    {
    public:
        /**
         *  @brief  Constructor
         *
         *  @param  pTH1F histo
         *  @param  description
         */
        IndexedHisto(TH1F *pTH1F, const std::string &description);

        /**
         *  @brief  Destructor 
         */
        ~IndexedHisto();

        /**
         *  @brief  Copy constructor
         *
         *  @param  rhs the IndexedHisto to copy
         */
        IndexedHisto(const IndexedHisto &rhs);

        /**
         *  @brief  Assignment operator
         *
         *  @param  rhs the IndexedHisto to assign
         */
        IndexedHisto &operator=(const IndexedHisto &rhs);

        /**
         *  @brief  Get histo
         */
        TH1F *GetHisto();

        /**
         *  @brief  Get descirption
         */
        std::string GetDescription();

    private:
        TH1F         *m_pTH1F;         ///< Histo
        std::string   m_desciption;    ///< Description
    };

    typedef std::vector<IndexedHisto*> HistoVector;

    /**
     *  @brief  Histo and additional info class
     */
    class Indexed2DHisto
    {
    public:
        /**
         *  @brief  Constructor
         *
         *  @param  pTH2F histo
         *  @param  description
         */
        Indexed2DHisto(TH2F *pTH2F, const std::string &description);

        /**
         *  @brief  Destructor 
         */
        ~Indexed2DHisto();

        /**
         *  @brief  Copy constructor
         *
         *  @param  rhs the Indexed2DHisto to copy
         */
        Indexed2DHisto(const Indexed2DHisto &rhs);

        /**
         *  @brief  Assignment operator
         *
         *  @param  rhs the Indexed2DHisto to assign
         */
        Indexed2DHisto &operator=(const Indexed2DHisto &rhs);

        /**
         *  @brief  Get histo
         */
        TH2F *Get2DHisto();

        /**
         *  @brief  Get descirption
         */
        std::string GetDescription();

    private:
        TH2F         *m_pTH2F;         ///< Histo
        std::string   m_desciption;    ///< Description
    };

    typedef std::vector<Indexed2DHisto*> Histo2DVector;

    HistoVector     m_histos;        ///< Vector of pointers to IndexedHisto
    Histo2DVector   m_2Dhistos;      ///< Vector of pointers to Indexed2DHisto
    GraphVector     m_graphs;        ///< Vector of pointers to IndexedGraph
    std::string     m_verboseString; ///< Description of plot
    IntToStringMap  m_xAxisLabels;   ///< X axis labels
    float           m_momentum;      ///< Momentum
    bool            m_setLogX;       ///< Is log X axis
    bool            m_setLogY;       ///< Is log Y axis
    bool            m_norm;          ///< Whether to normalise histograms when plotting
    bool            m_squarePlot;    ///< Draw square plot
    float           m_minX;          ///< Min X
    float           m_maxX;          ///< Max X
    float           m_minY;          ///< Min Y
    float           m_maxY;          ///< Max Y
    float           m_legLowX;       ///< Low X Legend position
    float           m_legHighX;      ///< High X Legend position
    float           m_legLowY;       ///< Low Y Legend position
    float           m_legHighY;      ///< High Y Legend position
    int             m_nColumns;      ///< NColumns in legend
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::AddGraph(TGraphErrors *pTGraphErrors, const std::string &description)
{
    IndexedGraph *pIndexedGraph = new IndexedGraph(pTGraphErrors, description);
    m_graphs.push_back(pIndexedGraph);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::AddHisto(TH1F *pTH1F, const std::string &description)
{
    IndexedHisto *pIndexedHisto = new IndexedHisto(pTH1F, description);
    m_histos.push_back(pIndexedHisto);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::Add2DHisto(TH2F *pTH2F, const std::string &description)
{
    Indexed2DHisto *pIndexed2DHisto = new Indexed2DHisto(pTH2F, description);
    m_2Dhistos.push_back(pIndexed2DHisto);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::SetLogX(bool setLogX)
{
    m_setLogX = setLogX;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::SetLogY(bool setLogY)
{
    m_setLogY = setLogY;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::Normalise(bool norm)
{
    m_norm = norm;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::SquarePlot(bool squarePlot)
{
    m_squarePlot = squarePlot;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::SetRange(float minX, float maxX, float minY, float maxY)
{
   m_minX = minX;
   m_maxX = maxX;
   m_minY = minY;
   m_maxY = maxY;
}

//------------------------------------------------------------------------------------------------------------------------------------------ 

inline void DrawClass::SetLegend(float minX, float maxX, float minY, float maxY)
{
    m_legLowX = minX;
    m_legHighX = maxX;
    m_legLowY = minY;
    m_legHighY = maxY;
}

//------------------------------------------------------------------------------------------------------------------------------------------ 

inline void DrawClass::SetNColumns(int nCol)
{
    m_nColumns = nCol;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void DrawClass::AddXAxisLabels(IntToStringMap xAxisLabels)
{
    m_xAxisLabels = xAxisLabels;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline TGraphErrors *DrawClass::IndexedGraph::GetGraph()
{
    return m_pTGraphErrors;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string DrawClass::IndexedGraph::GetDescription()
{
    return m_desciption;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline TH1F *DrawClass::IndexedHisto::GetHisto()
{
    return m_pTH1F;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string DrawClass::IndexedHisto::GetDescription()
{
    return m_desciption;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline TH2F *DrawClass::Indexed2DHisto::Get2DHisto()
{
    return m_pTH2F;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string DrawClass::Indexed2DHisto::GetDescription()
{
    return m_desciption;
}

} // namespace analysis

#endif // #ifndef DRAW_CLASS_H
