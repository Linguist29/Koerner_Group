#include <vector>
#include "TF1.h"
#include "TH2D.h"
#include "TMath.h"
#include "PMNS_NSI.h"    // For NSI-aware oscillations
#include "PremModel.h"
#include "SetNiceStyle.C"  // Optional styling for plots

// Function to create an oscillogram with a given NSI parameter setting
TH2D* GetOscHistWithNSI(std::vector<int> OscPath, int mh, double nsi_value, const std::string& nsi_param) {
  // Initialize the NSI model
  OscProb::PMNS_NSI myPMNS;

  // Set basic oscillation parameters (example values)
  double dm21 = 7.5e-5;
  double dm31 = mh > 0 ? 2.457e-3 : -2.449e-3 + dm21;
  double th12 = asin(sqrt(0.304));
  double th13 = asin(sqrt(mh > 0 ? 0.0218 : 0.0219));
  double th23 = asin(sqrt(mh > 0 ? 0.452 : 0.579));
  double dcp = (mh > 0 ? 306 : 254) * TMath::Pi() / 180;

  // Set oscillation parameters
  myPMNS.SetMix(th12, th23, th13, dcp);
  myPMNS.SetDeltaMsqrs(dm21, dm31);

  // Set NSI parameter
  if (nsi_param == "eps_ee") myPMNS.SetEps_ee(nsi_value);
  else if (nsi_param == "eps_emu") myPMNS.SetEps_emu(nsi_value, 0); // Assume no phase
  else if (nsi_param == "eps_etau") myPMNS.SetEps_etau(nsi_value, 0);
  else if (nsi_param == "eps_mumu") myPMNS.SetEps_mumu(nsi_value);
  else if (nsi_param == "eps_mutau") myPMNS.SetEps_mutau(nsi_value, 0);
  else if (nsi_param == "eps_tautau") myPMNS.SetEps_tautau(nsi_value);

  // Initialize the histogram
  int nbinsx = 200;
  int nbinsy = 200;
  SetNiceStyle(); // Optional: set nice plot style
  TH2D* h2 = new TH2D("", "", nbinsx, 0, 50 * nbinsx, nbinsy, -1, 1);
  // Define Earth model for neutrino path
  OscProb::PremModel prem;

  // Loop over cos(theta_z) and L/E
  for (int ct = 1; ct <= nbinsy; ct++) {
    double cosT = h2->GetYaxis()->GetBinCenter(ct);
    double L = prem.GetTotalL(cosT);
    if (cosT < -1 || cosT > 1) continue;

    prem.FillPath(cosT);
    myPMNS.SetPath(prem.GetNuPath());

    for (int le = 1; le <= nbinsx; le++) {
      double loe = h2->GetXaxis()->GetBinCenter(le);
      double prob = myPMNS.Prob(OscPath[0],OscPath[1], L / loe);
    //OscPath contains start and end flavors; 0 = nue, 1 = numu, 2 = nutau
      h2->SetBinContent(le, ct, prob);
    };
  };
  return h2;
};