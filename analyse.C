  
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

// some constant declarations
const int    kMaxFile = 30;       // sets number of max read files
const double kXerror  = 5e-6;     // sets graph x error

char    szFileName[kMaxFile][64]; // used in file list
double   frequency[kMaxFile];     // ...
TString     strRun[kMaxFile];     // ...

int    fNFile = 0;
int    fSelectedFile; // 
int    fFirstCall[kMaxFile] = {0};

TCycleAnalyzer   * fCyclAna[kMaxFile];
double             fStartTime, fDeltaTime; // used for config file
int                fRebin;                 // ...
double             fFreq;                  // ...
double             fDamping;               // ...

TResonanceFinder * fResFinder;
TGraphErrors     * grRes;

TControlBar *CntrBar;

TCanvas *cHisto;
TCanvas *cGraph;

void MakeControlBar();
void ReadFileList(const char *);
void FindMinimum();
void SaveAll();

using namespace std;

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void FindMinimum() {
  //
  //
   
  for( int i=0; i<fNFile; i++) {
    if( !fFirstCall[fSelectedFile] ) {
      int pnt = fResFinder->GetGraph()->GetN();
      fResFinder->GetGraph()->SetPoint(pnt, frequency[i], fCyclAna[i]->GetSpinFreq() );
      fResFinder->GetGraph()->SetPointError(pnt, kXerror, fCyclAna[i]->GetSpinFreqError() );
    }
  }
  
  fResFinder->DoFit();

  //  fResFinder->SetPad( cGraph->GetPad(0) );
  cGraph->cd();
  fResFinder->DoDraw();

  cGraph->SetCrosshair();

} // end FindMinimum()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void ReadConfig(const char fin[]="config.cfg") {
  //
  //
  ifstream infile;
  infile.open(fin);
  
  TString strLine;
  char szFin[64];
  
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
	  
	  else if( strLine.Contains("REBIN:") ) // 
            {
	      strLine.ReplaceAll("REBIN:","");
	      sscanf(strLine.Data(), "%i", &fRebin );
	    }
	  
	  else if( strLine.Contains("FREQ:") ) // 
            {
	      strLine.ReplaceAll("FREQ:","");
	      sscanf(strLine.Data(), "%lf", &fFreq );
	    }
	  
	  else if( strLine.Contains("DAMPING:") ) // 
            {
	      strLine.ReplaceAll("DAMPING:","");
	      sscanf(strLine.Data(), "%lf", &fDamping );
	    }
	  
	  else if( strLine.Contains("FILELIST:") ) //
            {
	      strLine.ReplaceAll("FILELIST:","");
	      sscanf(strLine.Data(), "%s", szFin );
	      cout << Form("Reading file list: %s", szFin) << endl;
	      ReadFileList( szFin );
	    }
	}
    } else {
    cerr << "\n ERROR: opening config file : config.cfg !!!\n\n" << endl;
    gSystem->Exit(0);
  }
  infile.close();
  
} // end ReadConfig()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void ReadFileList(const char *fin) {
  //
  //
  fNFile = 0;
  
  ifstream infile;
  infile.open(fin);
  
  TString strLine;
  if ( infile.is_open() )
    {
      while ( infile.good() )
	{
	  strLine="";
	  strLine.ReadLine(infile);
	  
	  if( strLine.BeginsWith("#") ) // comments
	    continue;

	  else if( strLine.Contains("FILE:") ) // file name 
            {
	      strLine.ReplaceAll("FILE:","");
	      
	      sscanf(strLine.Data(), "%s %lf", szFileName[fNFile], &frequency[fNFile]);
	      TString strTmp = szFileName[fNFile];
	      strRun[fNFile] = strTmp(strTmp.Sizeof()-10, 4);

	      fCyclAna[fNFile] = new TCycleAnalyzer( szFileName[fNFile] );
	      fCyclAna[fNFile]->SetTiming( fStartTime, fDeltaTime );
	      fCyclAna[fNFile]->SetRebin(fRebin);
	      fCyclAna[fNFile]->SetFrequency(fFreq);
	      fCyclAna[fNFile]->SetDamping(fDamping);
	      fNFile++;	      
	    } 
	}
    } else {
    cerr << "\n ERROR: opening config file : filelist !!!\n\n" << endl;
    gSystem->Exit(0);
  }
  infile.close();  
} // end ReadFileList()


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
  
  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Find Minimum", "FindMinimum()", "finding minimum");

  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Report results", "DoReport()", "report information");
  CntrBar->AddButton("Save All", "SaveAll()", "save all canvases in ./pictures directory");

  CntrBar->AddSeparator(); // ------------
  CntrBar->AddSeparator(); // ------------
  CntrBar->AddButton("Close ROOT", "gSystem->Exit(1)", "Exit Button");
  
  CntrBar->Show();
  gROOT->SaveContext();
} // end MakeControlBar()


// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DrawSelected() {
  //

  //
  cHisto->cd();
  fCyclAna[fSelectedFile]->SetPad( cHisto->GetPad(0) );
  fCyclAna[fSelectedFile]->DoDraw();
  fCyclAna[fSelectedFile]->DoReport();
  
  // graph
  cGraph->cd();
  int nn = fResFinder->GetGraph()->GetN();
  
  if ( nn > fSelectedFile ) nn = fSelectedFile;

  fResFinder->GetGraph()->SetPoint( nn, frequency[fSelectedFile], fCyclAna[fSelectedFile]->GetSpinFreq() );
  fResFinder->GetGraph()->SetPointError( nn, kXerror, fCyclAna[fSelectedFile]->GetSpinFreqError() );

  //  fResFinder->SetPad( cGraph->GetPad(0) );
  fResFinder->DoDraw();
} // end DrawSelected()


// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void SelectFile(int n) {
  fSelectedFile = n;

  // check if already fitted
  if( !fFirstCall[fSelectedFile]++ ) fCyclAna[fSelectedFile]->DoFit();
  
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
  //  fCyclAna[fSelectedFile]->DoPrint();  
  gSystem->Exec("mkdir -p pictures"); // creating dir

  TString strCanvasName = Form("pictures/Canvas_Run_%s", strRun[fSelectedFile].Data() ); // extract run number

  cGraph->Draw();
  TPostScript ps(strCanvasName+".ps", 111); // portrait
  ps.Range(10, 10); // x,y
  ps.Close();

  //  cout << " -- Output Canvas Name : " << strCanvasName+".pdf" << endl;
  cHisto->SaveAs(strCanvasName+".pdf");

  //  gSystem->Exec( Form("lpr -o scaling=70 -P %s", (strCanvasName+".ps").Data() ) );

} // end DoPrint()

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void DoReport() {

  fCyclAna[fSelectedFile]->DoReport();  

  for( int i=0; i<fNFile; i++) {
  
    printf(" Pnt( %i )   %f [MHz]  %f [Hz] #pm %f\n", 
	   i, 
	   frequency[i], 
	   fCyclAna[i]->GetSpinFreq(),
	   fCyclAna[i]->GetSpinFreqError() );
  }
}

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void SaveAll() {

  gSystem->Exec("mkdir -p pictures"); // creating dir
  
  //
  //
  TDatime dt;
  cGraph->SaveAs( Form("pictures/cGraph_d%i_t%i.root", dt.GetDate(), dt.GetTime()) );
  cGraph->SaveAs( Form("pictures/cGraph_d%i_t%i.pdf", dt.GetDate(), dt.GetTime()) );
  
  // TString strCanvasName = Form("pictures/Canvas_Run_%s", strRun.Data() ); // extract run number

  // TPostScript ps(strCanvasName+".ps", 111); // portrait
  // ps.Range(16, 10); // x,y

  // DoDraw();
  // ps.Close();

  // //  cout << " -- Output Canvas Name : " << strCanvasName+".pdf" << endl;
  // cMain->SaveAs(strCanvasName+".pdf");
  
}

// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
// + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
void analyse() {
  cHisto = new TCanvas( "cHisto", "Oscillation", 0, 0, 800, 700);
  cHisto->SetRightMargin(0.02);

  cGraph = new TCanvas( "cGraph", "ResonanceFr", 1020, 0, 500, 700);
  cGraph->SetRightMargin(0.02);

  //___ create modules
  //
  if (!fResFinder) fResFinder = new TResonanceFinder();

  ReadConfig(); // read filelist config file

  if ( !fNFile ) {
    cerr << "\n Please add line into : filelist\n"
 	 << "\n example: FILE:	data/Run2911.root		871.4333\n" << endl;
    return;
  }
  
  MakeControlBar();

  SelectFile(0);

} // end analyse()
