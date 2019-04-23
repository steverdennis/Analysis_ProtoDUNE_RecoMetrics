/**
 *  @file   src/DrawClass.cc
 *
 *  @brief  Implementation of the event class.
 *
 *  $Log: $
 */

#include "DrawClass.h"

#include <algorithm>

#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "Helper.h"

namespace analysis
{

DrawClass::DrawClass(const std::string &verboseString, const float momentum) :
    m_verboseString(verboseString),
    m_momentum(momentum),
    m_setLogX(false),
    m_setLogY(false),
    m_norm(true),
    m_squarePlot(false),
    m_rangeSet(false),
    m_minX(-std::numeric_limits<float>::max()),
    m_maxX(std::numeric_limits<float>::max()),
    m_minY(-std::numeric_limits<float>::max()),
    m_maxY(std::numeric_limits<float>::max()),
    m_legLowX(0.1),
    m_legHighX(0.825),
    m_legLowY(0.9),
    m_legHighY(0.975),
    m_nColumns(2),
    m_leftMargin(0.15f),
    m_rightMargin(0.15f),
    m_topMargin(0.2f),
    m_bottomMargin(0.1f),
    m_titleOffsetXSet(false),
    m_titleOffsetX(1.f),
    m_titleOffsetYSet(false),
    m_titleOffsetY(1.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::~DrawClass()
{
    for (const auto &iter : m_graphs)
    {
        delete iter;
    }
    for (const auto &iter : m_histos)
    {
        delete iter;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::DrawClass(const DrawClass &rhs) :
    m_verboseString(rhs.m_verboseString),
    m_xAxisLabels(rhs.m_xAxisLabels),
    m_momentum(rhs.m_momentum),
    m_setLogX(rhs.m_setLogX),
    m_setLogY(rhs.m_setLogY),
    m_norm(rhs.m_norm),
    m_squarePlot(rhs.m_squarePlot),
    m_rangeSet(rhs.m_rangeSet),
    m_minX(rhs.m_minX),
    m_maxX(rhs.m_maxX),
    m_minY(rhs.m_minY),
    m_maxY(rhs.m_maxY),
    m_legLowX(rhs.m_legLowX),
    m_legHighX(rhs.m_legHighX),
    m_legLowY(rhs.m_legLowY),
    m_legHighY(rhs.m_legHighY),
    m_nColumns(rhs.m_nColumns),
    m_leftMargin(rhs.m_leftMargin),
    m_rightMargin(rhs.m_rightMargin),
    m_topMargin(rhs.m_topMargin),
    m_bottomMargin(rhs.m_bottomMargin),
    m_titleOffsetXSet(rhs.m_titleOffsetXSet),
    m_titleOffsetX(rhs.m_titleOffsetX),
    m_titleOffsetYSet(rhs.m_titleOffsetYSet),
    m_titleOffsetY(rhs.m_titleOffsetY)
{
    for (const auto &iter : rhs.m_graphs)
    {
        IndexedGraph *pIndexedGraph = new IndexedGraph(*iter);
        m_graphs.push_back(pIndexedGraph);
    }

    for (const auto &iter : rhs.m_histos)
    {
        IndexedHisto *pIndexedHisto = new IndexedHisto(*iter);
        m_histos.push_back(pIndexedHisto);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass &DrawClass::operator=(const DrawClass &rhs)
{
    if (this != &rhs)
    {
        m_verboseString = rhs.m_verboseString;
        m_xAxisLabels = rhs.m_xAxisLabels;
        m_momentum = rhs.m_momentum;
        m_setLogX = rhs.m_setLogX;
        m_setLogY = rhs.m_setLogY;
        m_norm = rhs.m_norm;
        m_squarePlot = rhs.m_squarePlot;
        m_rangeSet =  rhs.m_rangeSet;
        m_minX = rhs.m_minX;
        m_maxX = rhs.m_maxX;
        m_minY = rhs.m_minY;
        m_maxY = rhs.m_maxY;
        m_legLowX = rhs.m_legLowX;
        m_legHighX = rhs.m_legHighX;
        m_legLowY = rhs.m_legLowY;
        m_legHighY = rhs.m_legHighY;
        m_nColumns = rhs.m_nColumns;
        m_leftMargin = rhs.m_leftMargin;
        m_rightMargin = rhs.m_rightMargin;
        m_topMargin = rhs.m_topMargin;
        m_bottomMargin = rhs.m_bottomMargin;
        m_titleOffsetXSet = rhs.m_titleOffsetXSet;
        m_titleOffsetX = rhs.m_titleOffsetX;
        m_titleOffsetYSet = rhs.m_titleOffsetYSet;
        m_titleOffsetY = rhs.m_titleOffsetY;

        for (const auto &iter : rhs.m_graphs)
        {
            IndexedGraph *pIndexedGraph = new IndexedGraph(*iter);
            m_graphs.push_back(pIndexedGraph);
        }

        for (const auto &iter : rhs.m_histos)
        {
            IndexedHisto *pIndexedHisto = new IndexedHisto(*iter);
            m_histos.push_back(pIndexedHisto);
        }
    }

    return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawClass::Draw() const
{
    int width(700);
    int height(500);

    if (m_squarePlot)
        height = 700;

    TCanvas *pTCanvas = new TCanvas("Canvas","",width,height);
    pTCanvas->Draw();

    if (m_setLogX)
        pTCanvas->SetLogx();

    if (m_setLogY)
        pTCanvas->SetLogy();

    pTCanvas->SetTopMargin(m_topMargin);
    pTCanvas->SetLeftMargin(m_leftMargin);
    pTCanvas->SetRightMargin(m_rightMargin);
    pTCanvas->SetBottomMargin(m_bottomMargin);

    std::vector<int> colors = {1, 2, 4, 6, 7, 418, 800};
    const int nColors(colors.size());
    std::vector<int> linestyle = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const int nStyles(linestyle.size());

    const int nObjectsPlot(m_graphs.size() + m_histos.size());
    if (nObjectsPlot > nColors * nStyles)
    {
        std::cout << "Add more draw options for disinction." << std::endl;
        return;
    }

    bool initialized(false);
    int counter(0);

    TLegend *pTLegend = new TLegend(m_legLowX, m_legLowY, m_legHighX, m_legHighY);
    pTLegend->SetTextSize(0.025);
    pTLegend->SetNColumns(m_nColumns);

    if (m_2Dhistos.empty())
    {
        for (const auto &iter : m_graphs)
        {
            TGraphErrors *pTGraphErrors(iter->GetGraph());

            if (pTGraphErrors->GetN() == 0)
            {
                counter++;
                continue;
            }

            const int colorInt(counter % nColors);
            const int styleInt(std::floor(counter / nColors));

//            pTGraphErrors->GetXaxis()->SetRangeUser(m_minX,m_maxX);
            pTGraphErrors->GetXaxis()->SetLimits(m_minX,m_maxX);
            pTGraphErrors->GetXaxis()->SetDecimals();
            pTGraphErrors->GetYaxis()->SetRangeUser(m_minY,m_maxY);
            pTGraphErrors->GetYaxis()->SetDecimals();
            pTGraphErrors->SetMarkerColor(colors.at(colorInt));
            pTGraphErrors->SetLineColor(colors.at(colorInt));
            pTGraphErrors->SetLineStyle(linestyle.at(styleInt));

            if (m_titleOffsetXSet)
                pTGraphErrors->GetXaxis()->SetTitleOffset(m_titleOffsetX);

            if (m_titleOffsetYSet)
                pTGraphErrors->GetYaxis()->SetTitleOffset(m_titleOffsetY);

            if (initialized)
            {
                pTGraphErrors->Draw("same");
            }
            else
            {
                pTGraphErrors->Draw("APL");
                initialized = true;
            }

            pTLegend->AddEntry(pTGraphErrors, iter->GetDescription().c_str(), "pl");
            counter++;
        }

        for (const auto &iter : m_histos)
        {
            TH1F *pTH1F(iter->GetHisto());

            if (pTH1F->GetEntries() == 0)
            {
                counter++;
                continue;
            }

            const int colorInt(counter % nColors);
            const int styleInt(std::floor(counter / nColors));

            //pTGraphErrors->GetXaxis()->SetRangeUser(m_minX,m_maxX);
            pTH1F->GetXaxis()->SetDecimals();
            pTH1F->GetYaxis()->SetDecimals();
            pTH1F->SetMarkerColor(colors.at(colorInt));
            pTH1F->SetLineColor(colors.at(colorInt));
            pTH1F->SetLineStyle(linestyle.at(styleInt));

            if (m_titleOffsetXSet)
                pTH1F->GetXaxis()->SetTitleOffset(m_titleOffsetX);

            if (m_titleOffsetYSet)
                pTH1F->GetYaxis()->SetTitleOffset(m_titleOffsetY);

            if (m_norm)
                pTH1F->Scale(1.f/pTH1F->GetEntries());

            if (!m_xAxisLabels.empty())
            {
                for (const auto &labelIter: m_xAxisLabels)
                    pTH1F->GetXaxis()->SetBinLabel(labelIter.first + 1, labelIter.second.c_str());
            }

            if (initialized)
            {
                pTH1F->Draw("same");
            }
            else
            {
                pTH1F->Draw("");

                if (m_rangeSet)
                {
                    pTH1F->SetAxisRange(m_minX,m_maxX, "X");
                    pTH1F->SetAxisRange(m_minY,m_maxY, "Y");
                    pTH1F->Draw("");
                }

                initialized = true;
            }

            pTLegend->AddEntry(pTH1F, iter->GetDescription().c_str(), "l");
            counter++;
        }
        pTLegend->Draw();
    }
    else
    {
//        pTCanvas->Divide(3,2);
//        counter = 1;

        if (m_2Dhistos.size() > 6)
        {
            std::cout << "Need to readjust number of pads in canvas." << std::endl;
            return;
        }

        for (const auto &iter : m_2Dhistos)
        {
//            pTCanvas->cd(counter);
            TH2F *pTH2F(iter->Get2DHisto());

            if (pTH2F->GetEntries() == 0)
            {
                counter++;
                continue;
            }

//            pTH2F->SetTitle(iter->GetDescription().c_str());
            pTH2F->GetXaxis()->SetDecimals();
            pTH2F->GetYaxis()->SetDecimals();

            if (m_titleOffsetXSet)
                pTH2F->GetXaxis()->SetTitleOffset(m_titleOffsetX);

            if (m_titleOffsetYSet)
                pTH2F->GetYaxis()->SetTitleOffset(m_titleOffsetY);

            if (m_squarePlot)
            {
                pTH2F->GetYaxis()->SetTitleOffset(1.4);
                pTH2F->GetYaxis()->SetNdivisions(5);
                pTH2F->GetXaxis()->SetNdivisions(5);
            }

            if (!m_xAxisLabels.empty())
            {
                for (const auto &labelIter: m_xAxisLabels)
                    pTH2F->GetXaxis()->SetBinLabel(labelIter.first + 1, labelIter.second.c_str());
            }

            pTH2F->Draw("COLZ");

            if (m_rangeSet)
            {
                pTH2F->SetAxisRange(m_minX,m_maxX, "X");
                pTH2F->SetAxisRange(m_minY,m_maxY, "Y");
                pTH2F->Draw("COLZ");
            }

            TPaveText *pTPaveText = new TPaveText(m_legLowX, m_legLowY, m_legHighX, m_legHighY, "NDC");
            pTPaveText->SetTextSize(0.04);
            pTPaveText->SetFillStyle(0);
            pTPaveText->SetLineWidth(0);
            pTPaveText->SetShadowColor(0);
            pTPaveText->AddText(iter->GetDescription().c_str());
            pTPaveText->Draw();

            counter++;
        }
    }

    std::string name(m_verboseString);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());

    if (std::fabs(m_momentum) > std::numeric_limits<float>::epsilon())
        name += "_" + Helper::ToStringPrecision(m_momentum, 0) + "_GeV_Beam_Cosmics";

    std::cout << "Saving name : " << name << std::endl;

    std::string pdf(name + ".pdf");
    std::string png(name + ".png");
    std::string dotC(name + ".C");

    pTCanvas->SaveAs(pdf.c_str());
    pTCanvas->SaveAs(png.c_str());
    pTCanvas->SaveAs(dotC.c_str());
    delete pTLegend;
    delete pTCanvas;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedGraph::IndexedGraph(TGraphErrors *pTGraphErrors, const std::string &description) :
    m_pTGraphErrors(new TGraphErrors(*pTGraphErrors)),
    m_desciption(description)
{
    m_pTGraphErrors->GetXaxis()->SetTitle(pTGraphErrors->GetXaxis()->GetTitle());
    m_pTGraphErrors->GetXaxis()->SetTitleSize(0.05);
    m_pTGraphErrors->GetXaxis()->SetLabelSize(0.05);
    m_pTGraphErrors->GetYaxis()->SetTitle(pTGraphErrors->GetYaxis()->GetTitle());
    m_pTGraphErrors->GetYaxis()->SetTitleSize(0.05);
    m_pTGraphErrors->GetYaxis()->SetLabelSize(0.05);
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedGraph::~IndexedGraph()
{
    delete m_pTGraphErrors;
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedGraph::IndexedGraph(const IndexedGraph &rhs) :
    m_desciption(rhs.m_desciption)
{
    m_pTGraphErrors = new TGraphErrors(*(rhs.m_pTGraphErrors));
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedGraph &DrawClass::IndexedGraph::operator=(const IndexedGraph &rhs)
{
    if (this != &rhs)
    {
        m_desciption = rhs.m_desciption;
        m_pTGraphErrors = new TGraphErrors(*(rhs.m_pTGraphErrors));
    }

    return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedHisto::IndexedHisto(TH1F *pTH1F, const std::string &description) :
    m_pTH1F(new TH1F(*pTH1F)),
    m_desciption(description)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedHisto::~IndexedHisto()
{
    delete m_pTH1F;
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedHisto::IndexedHisto(const IndexedHisto &rhs) :
    m_desciption(rhs.m_desciption)
{
    m_pTH1F = new TH1F(*(rhs.m_pTH1F));
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::IndexedHisto &DrawClass::IndexedHisto::operator=(const IndexedHisto &rhs)
{
    if (this != &rhs)
    {
        m_desciption = rhs.m_desciption;
        m_pTH1F = new TH1F(*(rhs.m_pTH1F));
    }

    return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::Indexed2DHisto::Indexed2DHisto(TH2F *pTH2F, const std::string &description) :
    m_pTH2F(new TH2F(*pTH2F)),
    m_desciption(description)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::Indexed2DHisto::~Indexed2DHisto()
{
    delete m_pTH2F;
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::Indexed2DHisto::Indexed2DHisto(const Indexed2DHisto &rhs) :
    m_desciption(rhs.m_desciption)
{
    m_pTH2F = new TH2F(*(rhs.m_pTH2F));
}

//------------------------------------------------------------------------------------------------------------------------------------------

DrawClass::Indexed2DHisto &DrawClass::Indexed2DHisto::operator=(const Indexed2DHisto &rhs)
{
    if (this != &rhs)
    {
        m_desciption = rhs.m_desciption;
        m_pTH2F = new TH2F(*(rhs.m_pTH2F));
    }

    return *this;
}

} // namespace analysis
