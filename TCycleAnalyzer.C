
//
// ? -> i.keshelashvili@fz-juelich.de
//

#include "TCycleAnalyzer.hh"


// =============================================================================
TCycleAnalyzer::TCycleAnalyzer(TString strFin):fSelectedHisto(1)
{
  strFileName = strFin;

  ReadFile();
  LRCrossRatio();

  fAsLR = new TF1("fAsLR", this, &TCycleAnalyzer::OscFit, 0, 1, 5, "TCycleAnalyzer", "OscFit");

  fAsLR->SetNpx(2000);
  fAsLR->SetParName(0,"Constant");
  fAsLR->SetParName(1,"Cos.Phase");
  fAsLR->SetParName(2,"Cos.Freq [Hz]");
  fAsLR->SetParName(3,"Exp.Am");
  fAsLR->SetParName(4,"Exp.Sl [s]");

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

  tex = new TLatex();
  tex->SetTextColor(38);
  tex->SetTextSize(0.0297619);
}

// =============================================================================
TCycleAnalyzer::~TCycleAnalyzer() {
}


// =============================================================================
void TCycleAnalyzer::LRCrossRatio()
{
  //
  //   
  for (int i=1; i<=hCR->GetNbinsX(); i++) {
    Double_t l1 = hL01->GetBinContent(i);
    Double_t r1 = hR01->GetBinContent(i);
    Double_t l2 = hL02->GetBinContent(i);
    Double_t r2 = hR02->GetBinContent(i);
  
    if ( !(l1 > 0 && r1 > 0 && l2 > 0 && r2 > 0) ) continue;
    //    if ( l1 && r1 && l2 && r2  ) continue;
  
    Double_t delta = sqrt( l2*r1 / (l1*r2) );
    //    Double_t sigmaDelta = delta/2.*sqrt(1./l1 + 1./r1 + 1./r2 +1./l2);
  
    Double_t cr = (delta - 1)/(delta + 1);
    Double_t sigmaCR = delta/pow(delta+1,2)*sqrt(1./l1+1./l2+1./r1+1./r2);
   
    hCR->SetBinContent(i,cr);
    hCR->SetBinError(i,sigmaCR);
  }
} // end LRCrossRatio()


// =============================================================================
void TCycleAnalyzer::SelectHisto(int n) {
  fSelectedHisto = n;
} // end SelectHisto(int n)


// =============================================================================
void TCycleAnalyzer::SetRebin(int rebin) { 
  // Rebinning all histograms
  hSelected->Rebin( rebin );
  hSelected->Scale( 1.0/rebin );

} // end SetRebin(int in)

// =============================================================================
void TCycleAnalyzer::SetFrequency(double par) { 
  // 
  fFrequency = par;
} // end SetFrequency(double par)

// =============================================================================
void TCycleAnalyzer::SetDamping(double par) { 
  // 
  fDamping = par;
} // end SetDamping(double par)

// =============================================================================
Double_t TCycleAnalyzer::OscFit(Double_t *x, Double_t *par) {
  return ( par[0] + 
	   TMath::Cos(par[1]+ 2.*TMath::Pi()*par[2]*x[0]) *par[3] * 
	   TMath::Exp(-1./par[4]*x[0]) );
} // end OscFit(...)

// =============================================================================
void TCycleAnalyzer::ResetFit() {
  // set the fir parameters on initial values
  //  fAsLR->SetParameters( -0.2, -20.0, fFrequency, 1.e+06, 3.37);
  fAsLR->SetParameters( -0.2, -20.0, fFrequency, 1.e+06, fDamping);
} // ResetFit()

// =============================================================================
void TCycleAnalyzer::SetTiming( double StartTime, double DeltaTime ) {
  // set timing, start and delta T  
  lStart->SetX1( StartTime );
  lStop->SetX1( StartTime + DeltaTime );
  lStart->SetVertical();
  lStop->SetVertical();
} // end SetTiming()

// =============================================================================
void TCycleAnalyzer::DoFit() {
  //
  //

  // find avarage value of last bins
  const int delta = 4;

  int nbins = hSelected->GetNbinsX() -4;
  double offset = hSelected->Integral(nbins-delta, nbins)/(delta+1);

  // main fit of precession
  //                    const  phase  freq        exp     damping
  fAsLR->SetRange( lStart->GetX1(), lStop->GetX1() );

  fAsLR->SetParameters( offset, 117.0, fFrequency, 1.e+19, fDamping);
  fAsLR->SetParLimits(0, -30, 30); // more than 1 is needet if you use rebin!
  fAsLR->SetParLimits(1, -180, 180);
  fAsLR->SetParLimits(2, fFrequency-3, fFrequency+3);
  fAsLR->SetParLimits(3, -100, 100);
  fAsLR->SetParLimits(4, fDamping-10, fDamping+10);

  hSelected->Fit( fAsLR, "+R0Q", "");

  hSelected->Fit( fAsLR, "+R0", "");

  // end pol
  fStopPol->SetRange( lStop->GetX1()+2, lStop->GetX1()+20 );
  hSelected->Fit( fStopPol, "+R0Q", "");

  // start pol 
  fStartPol->SetRange( lStart->GetX1()-15, lStart->GetX1()-2 );
  hSelected->Fit( fStartPol, "+R0Q", "");

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

  int size = strFileName.Sizeof(); // get size
  TString strRun = strFileName(size-10, 4);
  
  if ( bOnOff ) {
    hAsLR[1] = (TH1D*) fin->Get("Histograms/Asymmetry/AsLR_01")->Clone("hAsLR_Run"+strRun+"_01"); // online analyse
    hAsLR[2] = (TH1D*) fin->Get("Histograms/Asymmetry/AsLR_02")->Clone("hAsLR_Run"+strRun+"_02");
  } else {
    hAsLR[1] = (TH1D*) fin->Get("Histograms/LVDSana/Pol01/AsymmetryVer_01_LR")->Clone("hAsLR_Run"+strRun+"_01"); // offline analyse 
    hAsLR[2] = (TH1D*) fin->Get("Histograms/LVDSana/Pol01/AsymmetryVer_02_LR")->Clone("hAsLR_Run"+strRun+"_02");
  }

  //
  // Cross Ratio  
  //
  hCR = new TH1D( *hAsLR[1] );
  hCR->Reset();
  hCR->SetName("hCR");

  hR01 = (TH1D*) fin->Get("Histograms/Scaler/Sc25_01")->Clone("hSc25_Run"+strRun+"_01");
  hL01 = (TH1D*) fin->Get("Histograms/Scaler/Sc27_01")->Clone("hSc27_Run"+strRun+"_01");
  hR02 = (TH1D*) fin->Get("Histograms/Scaler/Sc25_02")->Clone("hSc25_Run"+strRun+"_02");
  hL02 = (TH1D*) fin->Get("Histograms/Scaler/Sc27_02")->Clone("hSc27_Run"+strRun+"_02");
  // TH1D * hR15= fin->Get("Histograms/Scaler/Sc25_15");
  // TH1D * hL15= fin->Get("Histograms/Scaler/Sc27_15");
  
  //   hAsUD[0] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_15")->Clone();
  //   hAsUD[1] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_01")->Clone();
  //   hAsUD[2] = (TH1D*) fin->Get("Histograms/Asymmetry/AsUD_02")->Clone();
  
  //   hBCT[0] = (TH1D*) fin->Get("Histograms/BCT/BCT_15")->Clone();
  //   hBCT[1] = (TH1D*) fin->Get("Histograms/BCT/BCT_01")->Clone();
  //   hBCT[2] = (TH1D*) fin->Get("Histograms/BCT/BCT_02")->Clone();
  
  fin->Close();

  //  hSelected =  hAsLR[1];
  hSelected = hCR;
  return 0;
} // end ReadFile()

// =============================================================================
void TCycleAnalyzer::DoDraw() {

  if( !cMain ) return;

  strFileName.ReplaceAll("data/","");
  strFileName.ReplaceAll(".root","");
  cMain->SetTitle( Form("c%s", strFileName.Data() ) );
  
  hSelected->SetLineColor(kBlue-8);
  hSelected->SetMarkerStyle(20);
  
  hSelected->GetXaxis()->SetRangeUser(lStart->GetX1()-10, lStop->GetX1()+10);
  hSelected->SetTitle(  Form("%s |  f: %6.4f [Hz] #tau: %6.4f [s];Time [s]; CR", 
			     strFileName.Data(), 
			     fAsLR->GetParameter(2),
			     fAsLR->GetParameter(4) ) );
  hSelected->Draw();
  
  fAsLR->SetLineColor(kBlue);
  fAsLR->Draw("same");
  
  fStartPol->SetLineWidth(1);
  fStartPol->SetLineColor(kGreen+2);
  fStartPol->Draw("same");
  
  fStopPol->SetLineWidth(1);
  fStopPol->SetLineColor(kRed);
  fStopPol->Draw("same");
    
  // start line (green)
  lStart->SetY1( hSelected->GetMinimum() );
  lStart->SetY2( hSelected->GetMaximum() );

  lStart->SetLineWidth(3);
  lStart->SetLineColor(kGreen+2);
  lStart->Draw();

  // stop line (red)
  lStop->SetY1( hSelected->GetMinimum() );
  lStop->SetY2( hSelected->GetMaximum() );

  lStop->SetLineWidth(3);
  lStop->SetLineColor(kRed);
  lStop->Draw();

  //  tex->SetText(hSelected->GetMaximumX(), hSelected->GetMaximum(), "ddd");
  //  tex->Draw();

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

  //  cout << " -- Output Canvas Name : " << strCanvasName+".pdf" << endl;
  cMain->SaveAs(strCanvasName+".pdf");

  //  gSystem->Exec( Form("lpr -p %s", (strCanvasName+".ps").Data() ) );
} // DoPrint()
