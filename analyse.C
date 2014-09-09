
//
// i.keshelashvili@fz-juelich.de
//

// c
#include <cstdio>

// c++
#include <iostream>
#include <fstream>

// ROOT
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLine.h>
#include <TMath.h>
#include <TPostScript.h>
#include <TControlBar.h>

#include <TCycleAnalyzer.hh>
#include <TResonanceFinder.hh>


const int kMaxFile = 30; // 

char   szFileName[kMaxFile][64];
double frequency[kMaxFile];

int fNFile = 0;
int fSelectedFile; // 

TCycleAnalyzer   * fCyclAna[kMaxFile];
double fStartTime, fDeltaTime;

TResonanceFinder * fResFinder;
TGraphErrors     * grRes;

TControlBar *CntrBar;

TCanvas *cHisto;
TCanvas *cGraph;

void MakeControlBar();
void ReadConfig();
void FindMinimum();


// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void FindMinimum() {

  //
  //   
  for( int i=0; i<fNFile; i++) {
  
    printf(" Resonance ( %i )   %f [MHz]  %f [Hz] #pm %f\n", 
	   i, 
	   frequency[i], 
	   fCyclAna[i]->GetSpinFreq(),
	   fCyclAna[i]->GetSpinFreqError() );
    
    fResFinder->GetGraph()->SetPoint(i, frequency[i], fCyclAna[i]->GetSpinFreq() );
    fResFinder->GetGraph()->SetPointError(i, 0.0001, fCyclAna[i]->GetSpinFreqError() );
  }
  
  fResFinder->DoFit();

  fResFinder->SetPad( cGraph->GetPad(0) );
  fResFinder->DoDraw();

  cGraph->SetCrosshair();

} // end FindMinimum()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void ReadConfig() {

  //
  //
  fNFile = 0;
  
  ifstream infile;
  infile.open("filelist");
  
  TString strLine;
    
  if ( infile.is_open() )
    {
      while ( infile.good() )
	{
	  strLine="";
	  strLine.ReadLine(infile);
	  
	  if( strLine.BeginsWith("#") ) // comments
	    continue;

	  else if( strLine.Contains("TIMING:") ) // start time and delta time for fitting
            {
	      strLine.ReplaceAll("TIMING:","");
	      sscanf(strLine.Data(), "%lf %lf", &fStartTime, &fDeltaTime);
	    }

	  else if( strLine.Contains("FILE:") ) // file name 
            {
	      strLine.ReplaceAll("FILE:","");
	      
	      sscanf(strLine.Data(), "%s %lf", szFileName[fNFile], &frequency[fNFile]);
	      
	      fCyclAna[fNFile] = new TCycleAnalyzer(szFileName[fNFile]);
	      fCyclAna[fNFile]->SetTiming( fStartTime, fDeltaTime );
	      fNFile++;	      
	    } 
	}
    } else {
    cerr << "\n ERROR: opening config file : filelist !!!\n\n" << endl;
    gSystem->Exit(0);
  }
  infile.close();

  if ( !fNFile ) {
    cerr << "\n Please add line into : filelist\n"
	 << "\n example: FILE:	data/Run2911.root		871.4333\n" << endl;
    return;
  }
  
  MakeControlBar();
} // end ReadConfig()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void MakeControlBar() {

  // control panel
  //
  if( !CntrBar ) CntrBar = new TControlBar("vertical", "Control Panel");
  
  CntrBar->AddButton("___Help to run___","gSystem->Exec(\"cat HELP\")", "Read the HELP file");

  CntrBar->AddButton("ReRead Configuration", "ReadConfig()", "Reading configuration");

  CntrBar->AddSeparator(); // ------------
  
  for( int i=0; i<fNFile; i++)
    CntrBar->AddButton( Form("File %i", i+1), 
			Form("SelectFile(%i)", i), 
			Form("Read file %s", szFileName[i]) );
  
  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Fit the Histogram", "DoFit()", "You can change the range by moving the red and green lines and run the fit function with new range");
  CntrBar->AddButton("Reset fit parameters", "ResetFit()", "Reset all fitting parameters tu initial values");
  CntrBar->AddButton("Report results", "DoReport()", "report information");
  CntrBar->AddButton("Print all", "DoPrint()", "printing canvas & report");
  
  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Find Minimum", "FindMinimum()", "finding minimum");

  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Close ROOT", "gSystem->Exit(1)", "Exit Button");
  
  CntrBar->Show();
  gROOT->SaveContext();
} // end MakeControlBar()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DrawSelected() {
  cHisto->cd();
  fCyclAna[fSelectedFile]->SetPad( cHisto->GetPad(0) );
  fCyclAna[fSelectedFile]->DoDraw();
  fCyclAna[fSelectedFile]->DoReport();
  
  // graph
  cGraph->cd();
  fResFinder->GetGraph()->SetPoint(fSelectedFile, frequency[fSelectedFile], fCyclAna[fSelectedFile]->GetSpinFreq() );
  fResFinder->GetGraph()->SetPointError(fSelectedFile, 0.0001, fCyclAna[fSelectedFile]->GetSpinFreqError() );

  fResFinder->SetPad( cGraph->GetPad(0) );
  fResFinder->DoDraw();
} // end DrawSelected()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void SelectFile(int n) {
  fSelectedFile = n;
  fCyclAna[fSelectedFile]->DoFit();
  
  DrawSelected();
} // end SelectFile()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DoFit() {
  fCyclAna[fSelectedFile]->DoFit();  
  DrawSelected();
} // end DoFit()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void ResetFit() {
  fCyclAna[fSelectedFile]->ResetFit();  
} // end ResetFit()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DoPrint() {
  fCyclAna[fSelectedFile]->DoPrint();  
} // end DoPrint()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DoReport() {
  fCyclAna[fSelectedFile]->DoReport();  
}

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void analyse() {
  cHisto = new TCanvas( "cHisto", "Oscillation", 0, 10, 800, 700);
  cHisto->SetRightMargin(0.02);

  cGraph = new TCanvas( "cGraph", "ResonanceFr", 1020, 10, 500, 700);
  cGraph->SetRightMargin(0.02);

  //___ create modules
  //
  if (!fResFinder) fResFinder = new TResonanceFinder();

  ReadConfig(); // read filelist config file

  SelectFile(1);

} // end analyse()