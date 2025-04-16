#include <vector>
#include <iostream>
#include "TH2D.h"
#include "TFile.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TStyle.h"

// Include OscProb files

#include "DefaultCanvas.C"

const std::vector<std::string> flavorTransition = {
    "E_to_E", "E_to_Mu", "E_to_Tau", "Mu_to_Mu", "Mu_to_Tau", "Tau_to_Tau"};

const std::vector<std::string> nsiParams = {
    "eps_ee", "eps_emu", "eps_etau", "eps_mumu", "eps_mutau", "eps_tautau"};

const std::vector<std::string> flavorTransitionLatex = {
    "#nu_{e} #rightarrow #nu_{e}", "#nu_{e} #rightarrow #nu_{#mu}",
    "#nu_{e} #rightarrow #nu_{#tau}", "#nu_{#mu} #rightarrow #nu_{#mu}",
    "#nu_{#mu} #rightarrow #nu_{#tau}", "#nu_{#tau} #rightarrow #nu_{#tau}"};

const std::vector<std::string> nsiParamsLatex = {
    "#varepsilon_{ee}", "#varepsilon_{e#mu}", "#varepsilon_{e#tau}",
    "#varepsilon_{#mu#mu}", "#varepsilon_{#mu#tau}", "#varepsilon_{#tau#tau}"};



void DrawDividedNSI(TFile* inputFile, const std::string& transition, const std::string& transitionLatex) {

    std::string outFilePath = "/home/seave/anaconda3/OscProb/tutorial/DividedOscillograms/" + transition + "_Divided.root";
    TFile* outFile = new TFile(outFilePath.c_str(), "RECREATE"); // Open once
    
  
    // Get baseline histogram
     TH2D* baseHist = (TH2D*)inputFile->Get("Baseline;2"); 
    
     for (size_t i = 0; i < nsiParams.size(); ++i) {
    
        // Get NSI histogram
        std::string subfile = nsiParams[i] + ";2";
        TH2D* nsiHist = (TH2D*)inputFile->Get(subfile.c_str());
            
    
        // Create ratio histogram
        TH2D* ratio = (TH2D*)nsiHist->Clone((nsiParams[i] + "_ratio").c_str());
        ratio->Divide(baseHist);
        ratio->SetMinimum(0);
        ratio->SetMaximum(20);
    
        std::string title = nsiParamsLatex[i] + " ratio to baseline for " + transitionLatex;
        ratio->SetTitle(title.c_str());
        
        bool originalCanvas = false;
        
        TCanvas* canvas = DefaultCanvas(ratio, originalCanvas);
    
        // Save to output file
        outFile->cd();  // Make sure you're writing to the correct file
        canvas->Write(nsiParams[i].c_str());
        ratio->Write(nsiParams[i].c_str());
    
        delete ratio;
        delete canvas;
        }
    
        outFile->Close(); 
        delete outFile;
    }
    
    

void DivideOscillogram() {
    
    for (size_t i = 0; i < flavorTransition.size(); ++i) {
        std::string filePath = "/home/seave/anaconda3/OscProb/tutorial/Oscillograms/" + flavorTransition[i] + ".root";
        TFile* inputFile = TFile::Open(filePath.c_str(), "READ");

        DrawDividedNSI(inputFile, flavorTransition[i], flavorTransitionLatex[i]);

        inputFile->Close();
        delete inputFile;
    }
}
