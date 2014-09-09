
//
// ? -> i.keshelashvili@fz-juelich.de
//

#include "TCycleAnalyzer.hh"


// =============================================================================
TCycleAnalyzer::TCycleAnalyzer(TString strFin):fSelectedHisto(1)
{
  strFileName = strFin;

  ReadFile();

  fAsLR = new TF1("fAsLR", this, &TCycleAnalyzer::OscFit, 0, 1, 5, "TCycleAnalyzer", "OscFit");

  fAsLR->SetNpx(2000);
  fAsLR->SetParName(0,"Constant");
  fAsLR->SetParName(1,"Cos.Phase");
  fAsLR->SetParName(2,"Cos.Freq [Hz]");
  fAsLR->SetParName(3,"Exp.Am");
  fAsLR->SetParName(4,"Exp.Sl [s]");

  fAsLR->SetParameters( -0.2, -20.0, 1.5, 1.e+06, 3.37);

  fStartPol = new TF1("fStartPol", "pol0");
  fStartPol->SetParameter(0, -0.2 );

  fStopPol  = new TF1("fStopPol",  "pol0");
  fStopPol->SetParameter(0, -0.2 );

  // timing
  lStart     = new TLine();
  lStop      = new TLine();
  lStart->SetX1( 1 );
  lStop->SetX1( 2 );
  lStart->SetVertical();
  lStop->SetVertical();

  lThreshold = new TLine();
}

// =============================================================================
TCycleAnalyzer::~TCycleAnalyzer() {
}

// =============================================================================
void TCycleAnalyzer::SelectHisto(int n) {
  fSelectedHisto = n;
} // end SelectHisto(int n)

// =============================================================================
Double_t TCycleAnalyzer::OscFit(Double_t *x, Double_t *par) {
  return ( par[0] + 
	   TMath::Cos(par[1]+ 2.*TMath::Pi()*par[2]*x[0]) *par[3] * 
	   TMath::Exp(-1./par[4]*x[0]) );
} // end OscFit(...)

// =============================================================================
void TCycleAnalyzer::ResetFit() {
  // set the fir parameters on initial values
  fAsLR->SetParameters( -0.2, -20.0, 1.5, 1.e+06, 3.37);
} // ResetFit()

// =============================================================================
void TCycleAnalyzer::SetTiming( double StartTime, double DeltaTime ) {
  // set timing, start and delta T  
  lStart->SetX1( StartTime );
  lStop->SetX1( StartTime + DeltaTime );
  lStart->SetVertical();
  lStop->SetVertical();
}

// =============================================================================
void TCycleAnalyzer::DoFit() {

  // start pol 
  fStartPol->SetRange( lStart->GetX1()-15, lStart->GetX1()-2 );
  hAsLR[fSelectedHisto]->Fit( fStartPol, "+R0Q", "");

  // end pol
  fStopPol->SetRange( lStop->GetX1()+2, lStop->GetX1()+20 );
  hAsLR[fSelectedHisto]->Fit( fStopPol, "+R0Q", "");

  // main fit of precession
  fAsLR->SetParameter(0, fStopPol->GetParameter(0) ); // 

  fAsLR->SetRange( lStart->GetX1(), lStop->GetX1() );
  hAsLR[fSelectedHisto]->Fit( fAsLR, "+R0", "");
} // DoFit()

// =============================================================================
int TCycleAnalyzer::ReadFile() {

  fin = TFile::Open(strFileName.Data());
  
  if (!fin || fin->IsZombie()) {
    cerr << "File " << strFileName << " not found!!!" << endl;
    gSystem->Exit(1);
  } else
    cout << "Reading File : " << strFileName << endl;
    
  fin->cd("Histograms"); // find if file is from Offline or Online analyse
  bool bOnOff = fin->cd("Histograms/Asymmetry");

  gROOT->cd();

  //  hAsLR[0] = (TH1D*) fin->Get("Histograms/Asymmetry/AsLR_15")->Clone();
  
  if ( bOnOff ) {
    hAsLR[1] = (TH1D*) fin->Get("Histograms/Asymmetry/AsLR_01")->Clone("_01"); // online analyse
    hAsLR[2] = (TH1D*) fin->Get("Histograms/Asymmetry/AsLR_02")->Clone("_02");
  } else
    hAsLR[1] = (TH1D*) fin->Get("Histograms/LVDSana/Pol01/AsymmetryVer_01_LR")->Clone(); // offline analyse 

  
//   hAsUD[0] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_15")->Clone();
//   hAsUD[1] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_01")->Clone();
//   hAsUD[2] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_02")->Clone();
  
//   hBCT[0] = (TH1D*) fin->Get("Histograms/BCT/BCT_15")->Clone();
//   hBCT[1] = (TH1D*) fin->Get("Histograms/BCT/BCT_01")->Clone();
//   hBCT[2] = (TH1D*) fin->Get("Histograms/BCT/BCT_02")->Clone();
  
  fin->Close();
  return 0;
} // end ReadFile()

// =============================================================================
void TCycleAnalyzer::DoDraw() {

  if( !cMain ) return;

  strFileName.ReplaceAll("data/","");
  strFileName.ReplaceAll(".root","");
  cMain->SetTitle( Form("c%s", strFileName.Data() ) );
  
  hAsLR[fSelectedHisto]->SetLineColor(kBlue-8);
  hAsLR[fSelectedHisto]->SetMarkerStyle(20);
  
  hAsLR[fSelectedHisto]->GetXaxis()->SetRangeUser(45, 90);
  hAsLR[fSelectedHisto]->Draw();
  //  hAsLR[fSelectedHisto]->Draw("E1");
  
  fAsLR->SetLineColor(kBlue);
  fAsLR->Draw("same");
  
  fStartPol->SetLineWidth(1);
  fStartPol->SetLineColor(kGreen+2);
  fStartPol->Draw("same");
  
  fStopPol->SetLineWidth(1);
  fStopPol->SetLineColor(kRed);
  fStopPol->Draw("same");
  
  
  // start line (green)
  lStart->SetY1( hAsLR[fSelectedHisto]->GetMinimum() );
  lStart->SetY2( hAsLR[fSelectedHisto]->GetMaximum() );

  lStart->SetLineWidth(3);
  lStart->SetLineColor(kGreen+2);
  lStart->Draw();

  // stop line (red)
  lStop->SetY1( hAsLR[fSelectedHisto]->GetMinimum() );
  lStop->SetY2( hAsLR[fSelectedHisto]->GetMaximum() );

  lStop->SetLineWidth(3);
  lStop->SetLineColor(kRed);
  lStop->Draw();

  cMain->Update();

  return;
}

// =============================================================================
void TCycleAnalyzer::DoReport() {
  cout << " * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
  cout << Form(" Input .root file name    : %s ",  strFileName.Data() ) << endl;
  cout << Form(" Spin Resonance Frequency : %12.10f [Hz] ",  fAsLR->GetParameter(2) ) << endl;
  cout << Form(" Damping constant         : %12.10f [s] ",  fAsLR->GetParameter(4) ) << endl;
} // DoReport()

// =============================================================================
void TCycleAnalyzer::DoPrint() {
  gSystem->Exec("mkdir -p pictures"); // creating dir

  int size = strFileName.Sizeof(); // get size

  TString strRun = strFileName(size-10, 4);

  TString strCanvasName = Form("pictures/Canvas_Run_%s", strRun.Data() ); // extract run number

  TPostScript ps(strCanvasName+".ps", 111); // portrait
  ps.Range(16, 10); // x,y

  DoDraw();
  ps.Close();

  //
  //  cout << " -- Output Canvas Name : " << strCanvasName+".pdf" << endl;
  cMain->SaveAs(strCanvasName+".pdf");

  //  gSystem->Exec( Form("lpr -p %s", (strCanvasName+".ps").Data() ) );
} // DoPrint()
