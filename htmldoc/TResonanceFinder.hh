
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

#include <TROOT.h>
#include <TSystem.h>
#include <TVirtualPad.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLine.h>
#include <TPostScript.h>

class TResonanceFinder { 

public :

  TVirtualPad * cMain;       //
  TVirtualPad * GetPad() {return cMain; };
  void SetPad(TVirtualPad *cc) { cMain = cc; };

  TGraphErrors * grSpinRes; // 
  TGraphErrors * GetGraph() { return grSpinRes; }; // 

  TF1 * fFitFunc;

  TLine * lOffset;
  TLine * lMeanFreq;

  //
  TResonanceFinder();
  virtual ~TResonanceFinder();

  Double_t MyPol2(Double_t*, Double_t*);

  void     DoFit();
  void     DoPrint();
  void     DoDraw();

  ClassDef(TResonanceFinder, 0)
};
