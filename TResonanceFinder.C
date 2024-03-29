
#define TResonanceFinder_cxx

#include "TResonanceFinder.hh"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

//==============================================================================
TResonanceFinder::TResonanceFinder() 
{
  //
  grSpinRes = new TGraphErrors();
  grSpinRes->SetNameTitle("grSpinRes", ";f=(1-G#gamma)f_{cyc} [kHz];Spin Oscilation [Hz]");
  grSpinRes->GetXaxis()->SetNdivisions(505);

  lOffset   = new TLine(); 
  lOffset->SetLineWidth(2);
  lOffset->SetLineColor(kBlue+2);
  lOffset->SetHorizontal();
  lOffset->SetX1( grSpinRes->GetXaxis()->GetXmin() ); 
  lOffset->SetX2( grSpinRes->GetXaxis()->GetXmax() );

  lMeanFreq = new TLine();
  lMeanFreq->SetLineWidth(2);
  lMeanFreq->SetLineColor(kGreen+1);
  lMeanFreq->SetVertical();
  lMeanFreq->SetY1( grSpinRes->GetYaxis()->GetXmin() );
  lMeanFreq->SetY2( grSpinRes->GetYaxis()->GetXmax() );

  // grSpinRes->SetPoint(0,1000*0.871433,2.30328);
  // grSpinRes->SetPointError(0,8.71433e-9,0.230328);
  // grSpinRes->SetPoint(1,1000*0.871432,1.83941);
  // grSpinRes->SetPointError(1,8.71432e-9,0.183941);
  // grSpinRes->SetPoint(2,1000*0.871431,1.53676);
  // grSpinRes->SetPointError(2,8.71431e-9,0.153676);
  // grSpinRes->SetPoint(3,1000*0.87143,1.823);
  // grSpinRes->SetPointError(3,8.7143e-9,0.1823);
  // grSpinRes->SetPoint(4,1000*0.871429,2.54106);
  // grSpinRes->SetPointError(4,8.71429e-9,0.254106);

  //
  fFitFunc = new TF1("fFitFunc", this, &TResonanceFinder::MyPol2, 0, 1, 3, "TResonanceFinder", "MyPol2");

  fFitFunc->SetNpx(2000); // increased resolution
  fFitFunc->SetParName(0,"Curvature");
  fFitFunc->SetParName(1,"Minimum");
  fFitFunc->SetParName(2,"Offset");

} // end TResonanceFinder()

//==============================================================================
TResonanceFinder::~TResonanceFinder()
{
  // virtual kaputmacher
}

//==============================================================================
Double_t TResonanceFinder::MyPol2(Double_t *x, Double_t *par) {
  
  return (par[0]*TMath::Power((par[1]-x[0]),2) + par[2]);

} // end MyPol2

//==============================================================================
void TResonanceFinder::DoFit() {

  fFitFunc->SetRange( grSpinRes->GetXaxis()->GetXmin(),
		      grSpinRes->GetXaxis()->GetXmax() ); // x-axis range

  //  fFitFunc->SetParameters(1.14128e+07, 871.4278, 0.15); //  
  fFitFunc->SetParameters( 8.5e+06, 871.4278, 0.051); // 

  fFitFunc->SetParLimits(0, 1, 1e+11); // parabolic
  fFitFunc->SetParLimits(1, 800, 900); // kHz resonance freq.
  fFitFunc->SetParLimits(2, 0.001, 20); // Hz oscillation

  grSpinRes->Fit(fFitFunc, "+R0", "");

  lOffset->SetY1( fFitFunc->GetParameter(2) );
  lOffset->SetY2( fFitFunc->GetParameter(2) );

  lMeanFreq->SetX1( fFitFunc->GetParameter(1) );
  lMeanFreq->SetX2( fFitFunc->GetParameter(1) );

  grSpinRes->SetTitle( Form("Minimum Freq : %10.6f [kHz];f=(1-G#gamma)f_{cyc} [kHz];Spin Oscilation [Hz]", 
			    fFitFunc->GetParameter(1) ) );

  cout << Form("\n -- Minimum RF-Freq : %15.10f", fFitFunc->GetParameter(1) ) << endl;

} // end DoFit()

//==============================================================================
void TResonanceFinder::DoPrint()
{
  gSystem->Exec("mkdir -p pictures"); // creating dir

  // int size = strFileName.Sizeof(); // get size
  // TString strRun = strFileName(size-10, 4);
  // TString strCanvasName = Form("pictures/Canvas_Run_%s", strRun.Data() ); // extract run number

  TString strCanvasName = Form("pictures/cMinimum" ); // extract run number

  TPostScript ps(strCanvasName+".ps", 111); // portrait
  ps.Range(16, 10); // x,y

  DoDraw();
  ps.Close();
  
  //
  //  cout << " -- Output Canvas Name : " << strCanvasName+".pdf" << endl;
  cMain->SaveAs(strCanvasName+".pdf");
  
  gSystem->Exec( Form("lpr -p %s", (strCanvasName+".ps").Data() ) );

} // end DoPrint()

//==============================================================================
void TResonanceFinder::DoDraw()
{
  //
  grSpinRes->SetMarkerStyle(20);
  grSpinRes->GetXaxis()->SetNdivisions(505);
  grSpinRes->Draw("AP"); 
  fFitFunc->Draw("same");

  // draw lines
  lOffset->SetX1( grSpinRes->GetXaxis()->GetXmin() ); 
  lOffset->SetX2( grSpinRes->GetXaxis()->GetXmax() );
  lOffset->Draw();

  lMeanFreq->SetY1( grSpinRes->GetYaxis()->GetXmin() );
  lMeanFreq->SetY2( grSpinRes->GetYaxis()->GetXmax() );
  lMeanFreq->Draw();

  cout << lOffset->GetX1() << "  "
       << lMeanFreq->GetY1() << endl;

} // end DoDraw()

