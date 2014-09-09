
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

  int      fSelectedHisto; // 0->15; 1->01; 2->02
  TH1D    *hAsLR[3];
  TH1D    *hAsUD[3];  
  TH1D    *hBCT[3];  

  TF1     *fAsLR;
  TF1     *fStartPol;
  TF1     *fStopPol;
  TLine   *lStart;
  TLine   *lStop;
  TLine   *lThreshold;

  TCycleAnalyzer(TString strFileName);
  virtual ~TCycleAnalyzer();

  
  double GetSpinFreq() { return fAsLR->GetParameter(2); }; // spin
  double GetDamping() { return fAsLR->GetParameter(4); }; // damping


  Double_t OscFit(Double_t *x, Double_t *par);

  int SelectHisto(int n);

  void DoFit();
  void ResetFit();
  void DoDraw();
  void DoReport();
  void DoPrint();



  ClassDef(TCycleAnalyzer, 0)
    };
