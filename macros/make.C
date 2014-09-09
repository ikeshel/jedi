
//
// ? -> i.keshelashvili@fz-juelich.de
//

{
  gSystem->SetBuildDir("./lib", 1);

  gSystem->CompileMacro("TCycleAnalyzer.C", "kfO-");
  gSystem->CompileMacro("TResonanceFinder.C", "kfO-");
  gSystem->CompileMacro("analyse.C", "kfO-");
}
