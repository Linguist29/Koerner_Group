#include <vector>
#include "TString.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"

// Ensure OscProb library and includes are loaded
#include "GetOscHistWithNSI.C"
#include "DefaultCanvas.C"

double nsiValue = 0.25;

const std::vector<std::vector<int>> oscPathVector= {{0,0}, {0,1}, {0,2}, {1,1}, {1,2}, {2,2}};

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

void MakeBaseHist(int transitionIndex, const std::string& transitionLatex, TFile* outFile){
    
    TH2D* baseHist = GetOscHistWithNSI(oscPathVector[transitionIndex], 1, 0, "");
    
    std::string baseTitle = transitionLatex + " Baseline ";
    
    baseHist->SetTitle(baseTitle.c_str());

    TCanvas* baseCanvas = DefaultCanvas(baseHist, true);

    outFile->cd();
    baseCanvas->Write("Baseline");
    baseHist->Write("Baseline");

    delete baseCanvas;
}

void LoopOverNSIParams(int transitionIndex, const std::string& transitionLatex, TFile* outFile) {

    MakeBaseHist(transitionIndex, transitionLatex, outFile);
    
    for (size_t i = 0; i < nsiParams.size(); ++i) {
    
        TH2D* nsiHist = GetOscHistWithNSI(oscPathVector[transitionIndex], 1, nsiValue, nsiParams[i]);
      
        std::string nsiTitle = transitionLatex + " With " + nsiParamsLatex[i] + " = 0.25";
      
        nsiHist->SetTitle(nsiTitle.c_str());
      
        // Get default canvas and draw
        TCanvas* nsiCanvas = DefaultCanvas(nsiHist, true);
      
        outFile->cd(); 
        nsiCanvas->Write(nsiParams[i].c_str());
        nsiHist->Write(nsiParams[i].c_str());
      
        delete nsiCanvas;
        }
      }
      

void RunNSIOscillograms() {
    for (size_t i = 0; i < oscPathVector.size(); ++i) {

        std::string transition = flavorTransition[i];
        std::string outFilePath = "/home/seave/anaconda3/OscProb/tutorial/Oscillograms/" + transition + ".root";
      
        TFile* outFile = new TFile(outFilePath.c_str(), "RECREATE"); 
      
        LoopOverNSIParams(i, flavorTransitionLatex[i], outFile);
      
        outFile->Close();
        delete outFile;
        }
      }
  

