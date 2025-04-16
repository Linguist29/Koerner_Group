#include "TCanvas.h"
#include "TH2D.h"
#include "TPaletteAxis.h"
#include "SetNiceStyle.C"


TCanvas* DefaultCanvas(TH2D* hist, bool originalCanvas){
    TCanvas* canvas = new TCanvas("canvas", "Oscillogram", 800, 700);

    if (!originalCanvas) {
        //erase old settings
        TH2D* cleanHist = (TH2D*)hist->Clone();
        cleanHist->SetDirectory(0);
        cleanHist->GetListOfFunctions()->Clear();
        hist = cleanHist; 
    }
    hist->SetStats(0); // Remove stats box
    canvas->cd();
    hist->Draw("colz");


     // Adjust margins explicitly for consistency
     gPad->SetLeftMargin(0.12);   // Adjust if needed
     gPad->SetRightMargin(0.18);  // Make room for Z-axis
     gPad->SetTopMargin(0.1);     // Space between title and plot
     gPad->SetBottomMargin(0.12); // Adjust if needed
 
    hist->SetXTitle("L/E (km/GeV)");
    hist->SetYTitle("cos#theta_{z}");

    hist->GetXaxis()->CenterTitle(); 
    hist->GetYaxis()->CenterTitle(); 
    
    gStyle->SetPalette(57);
    gPad->Update();

    return canvas;
}

