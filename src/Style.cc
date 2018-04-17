#include "Style.h"

void Style()
{
  gROOT->SetStyle("Plain"); /*Default white background for all plots*/
  /* set bkg color of all to kWhite: white, but not 0*/
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetFillColor(1);
  gStyle->SetFillStyle(1001);   // solid
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetLegendBorderSize(0);

  // Use times new roman, precision 2 
  Int_t lhcbFont        = 132;  // Old LHCb style: 62;
  // Line thickness
  Double_t lhcbWidth    = 2.00; // Old LHCb style: 3.00;
  // Text size
  Double_t lhcbTSize    = 0.03; 

  // use large fonts
  gStyle->SetTextFont(lhcbFont);
  gStyle->SetTextSize(lhcbTSize);
  gStyle->SetLabelFont(lhcbFont,"x");
  gStyle->SetLabelFont(lhcbFont,"y");
  gStyle->SetLabelFont(lhcbFont,"z");
  gStyle->SetLabelSize(lhcbTSize,"x");
  gStyle->SetLabelSize(lhcbTSize,"y");
  gStyle->SetLabelSize(lhcbTSize,"z");
  gStyle->SetTitleFont(lhcbFont);
  gStyle->SetTitleFont(lhcbFont,"x");
  gStyle->SetTitleFont(lhcbFont,"y");
  gStyle->SetTitleFont(lhcbFont,"z");
  gStyle->SetTitleSize(1.2*lhcbTSize,"x");
  gStyle->SetTitleSize(1.2*lhcbTSize,"y");
  gStyle->SetTitleSize(1.2*lhcbTSize,"z");
  gStyle->SetLegendFont(lhcbFont); 

  // use medium bold lines and thick markers
  gStyle->SetLineWidth(lhcbWidth);
  gStyle->SetFrameLineWidth(lhcbWidth);
  gStyle->SetHistLineWidth(lhcbWidth);
  gStyle->SetFuncWidth(lhcbWidth);
  gStyle->SetGridWidth(lhcbWidth);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.0);

  gStyle->SetTitleFont(lhcbFont,"title");
  gStyle->SetLegendBorderSize(0);
  gStyle->SetOptStat(0);  

  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadRightMargin(0.1); // increase for colz plots (0.2) dhynds increased from 0.05
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.1);

  const Int_t NRGBs = 5;
//  const Int_t NCont = 255;
  const Int_t NCont = 99;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red2[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green2[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue2[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red2, green2, blue2, NCont);
  gStyle->SetNumberContours(NCont);

  Int_t ci = 10001; // color index
  TColor *orange = new TColor(ci, 1, 0.55, 0);
  Int_t ci2 = 10002; // color index
  TColor *violet = new TColor(ci2, 0.62, 0, 1);
  Int_t ci3 = 10003; // color index
  TColor *lightblue = new TColor(ci3, 0, 0.75, 1);
  Int_t ci4 = 10004; // color index
  TColor *gold = new TColor(ci4, 1, 0.843 ,0);

  if (!orange || !violet || !lightblue || !gold)
    std::cout << "Problem setting user-defined colours." << std::endl;

  //set the tick mark style
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  gStyle->SetNdivisions(505,"x");
  gStyle->SetNdivisions(510,"y");

  gROOT->ForceStyle();
}

