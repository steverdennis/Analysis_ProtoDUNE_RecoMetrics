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

#include "Helper.h"

namespace analysis
{ 

DrawClass::DrawClass(const std::string &verboseString, const float momentum) : 
    m_verboseString(verboseString),
    m_momentum(momentum),
    m_setLogX(false),
    m_setLogY(false),
    m_norm(true),
    m_minX(-std::numeric_limits<float>::max()),
    m_maxX(std::numeric_limits<float>::max()),
    m_minY(-std::numeric_limits<float>::max()),
    m_maxY(std::numeric_limits<float>::max())
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
    m_momentum(rhs.m_momentum),
    m_setLogX(rhs.m_setLogX),
    m_setLogY(rhs.m_setLogY),
    m_norm(rhs.m_norm),
    m_minX(rhs.m_minX),
    m_maxX(rhs.m_maxX),
    m_minY(rhs.m_minY),
    m_maxY(rhs.m_maxY)
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
        m_momentum = rhs.m_momentum;
        m_setLogX = rhs.m_setLogX;
        m_setLogY = rhs.m_setLogY;
        m_norm = rhs.m_norm;
        m_minX = rhs.m_minX;
        m_maxX = rhs.m_maxX;
        m_minY = rhs.m_minY;
        m_maxY = rhs.m_maxY;

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
    TCanvas *pTCanvas = new TCanvas("Canvas","");
    pTCanvas->Draw();

    if (m_setLogX)
        pTCanvas->SetLogx();

    if (m_setLogY)
        pTCanvas->SetLogy();

    pTCanvas->SetTopMargin(0.2);
    pTCanvas->SetLeftMargin(0.15);
    pTCanvas->SetRightMargin(0.15);
    pTCanvas->SetBottomMargin(0.1);

    //std::vector<int> colors = {1, 2, 4, 6, 418, 800, 1, 2};
    std::vector<int> colors = {4, 1, 6, 418, 800, 1, 2};
    std::vector<int> linestyle = {1, 1, 1, 1, 1, 1, 2, 2};

    if (m_graphs.size() > colors.size())
    {
        std::cout << "Please add more colours." << std::endl;
        return;
    }

    bool initialized(false);
    int counter(0);

    TLegend *pTLegend = new TLegend(0.1, 0.825, 0.9, 0.975);
    pTLegend->SetTextSize(0.025);
    pTLegend->SetNColumns(2);

    if (m_2Dhistos.empty())
    {
        for (const auto &iter : m_graphs)
        {
            TGraphErrors *pTGraphErrors(iter->GetGraph());
            //pTGraphErrors->GetXaxis()->SetRangeUser(m_minX,m_maxX);
            pTGraphErrors->GetXaxis()->SetDecimals();
            pTGraphErrors->GetYaxis()->SetRangeUser(m_minY,m_maxY);
            pTGraphErrors->GetYaxis()->SetDecimals();
            pTGraphErrors->SetMarkerColor(colors.at(counter));
            pTGraphErrors->SetLineColor(colors.at(counter));
            pTGraphErrors->SetLineStyle(linestyle.at(counter));

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
            //pTGraphErrors->GetXaxis()->SetRangeUser(m_minX,m_maxX);
            pTH1F->GetXaxis()->SetDecimals();
            pTH1F->GetYaxis()->SetRangeUser(m_minY,m_maxY);
            pTH1F->GetYaxis()->SetDecimals();
            pTH1F->SetMarkerColor(colors.at(counter));
            pTH1F->SetLineColor(colors.at(counter));
            pTH1F->SetLineStyle(linestyle.at(counter));

            if (m_norm)
                pTH1F->Scale(1.f/pTH1F->GetEntries());

            if (initialized)
            {
                pTH1F->Draw("same");
            }
            else
            {
                pTH1F->Draw("");
                initialized = true;
            }

            pTLegend->AddEntry(pTH1F, iter->GetDescription().c_str(), "pl");
            counter++;
        }
        pTLegend->Draw();
    }
    else
    {
//        pTCanvas->Divide(4,2);
        counter = 1;

        if (m_2Dhistos.size() > 8)
        {
            std::cout << "Need to readjust number of pads in canvas." << std::endl;
            return;
        }

        for (const auto &iter : m_2Dhistos)
        {
            pTCanvas->cd(counter);
            TH2F *pTH2F(iter->Get2DHisto());
            pTH2F->SetTitle(iter->GetDescription().c_str());
            pTH2F->GetYaxis()->SetRangeUser(m_minX,m_maxX);
            pTH2F->GetXaxis()->SetDecimals();
            pTH2F->GetYaxis()->SetRangeUser(m_minY,m_maxY);
            pTH2F->GetYaxis()->SetDecimals();
            pTH2F->Draw("COLZ");
            counter++;
        }
    }

    std::string name(m_verboseString);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());

std::cout << "m_momentum " << m_momentum << std::endl;
std::cout << "Helper::ToStringPrecision(m_momentum, 0) " << Helper::ToStringPrecision(m_momentum, 0) << std::endl;

    if (std::fabs(m_momentum) > std::numeric_limits<float>::epsilon())
    {
std::cout << "Non-zero momentum detected" << std::endl;
        name += "_" + Helper::ToStringPrecision(m_momentum, 0) + "_GeV_Beam_Cosmics";
    }

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
