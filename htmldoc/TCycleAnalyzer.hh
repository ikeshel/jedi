
//
// ? -> i.keshelashvili@fz-juelich.de
//

////////////////////////////////////////////////////////////////
//                                                            //
// TMyClass                                                   //
//                                                            //
// This is the description block.                             //
//                                                            //
////////////////////////////////////////////////////////////////

#pragma once

//c++
#include <iostream>

// ROOT
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TVirtualPad.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLine.h>
#include <TMath.h>
#include <TPostScript.h>
#include <TLatex.h>

// #include <TApplication.h>
// #include <TGClient.h>
// #include <TGButton.h>
// #include <TGFrame.h>
// #include <TFrame.h>
// #include <TRootEmbeddedCanvas.h>

class TCycleAnalyzer 
{
private:
  int ReadFile();

protected:

public:
  enum CycleType {S15, S01, S02};

  //
  TString  strFileName; //
  TFile   *fin;         //

  TVirtualPad *cMain;       //
  void SetPad(TVirtualPad *cc) { cMain = cc; };
  TVirtualPad * GetPad() {return cMain; };

  int      fSelectedHisto; // 0->15; 1->01; 2->02
  void     SelectHisto(int n);

  TH1D    *hAsLR[3];
  TH1D    *hAsUD[3];  
  TH1D    *hBCT[3];  

  TF1     *fAsLR;
  TF1     *fStartPol;
  TF1     *fStopPol;

  // timing
  TLine   *lStart;
  TLine   *lStop;
  void     SetTiming( double StartTime, double DeltaTime );

  TLine   *lThreshold;

  TLatex * tex;

  TCycleAnalyzer(TString strFileName);
  virtual ~TCycleAnalyzer();

  
  double GetSpinFreq() { return fAsLR->GetParameter(2); }; // spin
  double GetSpinFreqError() { return fAsLR->GetParError(2); }; // spin

  double GetDamping() { return fAsLR->GetParameter(4); }; // damping

  Double_t OscFit(Double_t *x, Double_t *par);

  void DoFit();
  void ResetFit();
  void DoDraw();
  void DoReport();
  void DoPrint();

  ClassDef(TCycleAnalyzer, 0)
    };
