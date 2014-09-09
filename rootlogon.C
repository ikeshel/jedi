
//
// i.keshelashvili@fz-juelich.de
//

{
  // gStyle->SetStatFormat("11100");
  gStyle->SetOptStat(0);
  //  gStyle->SetOptFit();

  char *szLib[] = {"TCycleAnalyzer_C.so", "TResonanceFinder_C.so", "analyse_C.so"};

  for ( int i=0; i< sizeof(szLib) / sizeof(char*); i++) {
    if ( gSystem->Load( Form("lib/%s", szLib[i]) ) ) {
      cerr << " !!! ERROR loading library : " << szLib[i] << "\n"
	   << " Please type >make" << endl;
    } else {
      cout << " Loaded library : " << szLib[i] << endl;
    }
    
  }

  // run analysis
  //
  TTimer tt("analyse()", 1000);
  tt.Start(500, 1);
}
