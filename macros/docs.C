{
  THtml html;
  html.SetProductName("Spin Resonance Locator");
  html.SetAuthorTag("i.keshelashvili(at)fz-juelich.de");

  //  html.SetOutputDir("docs");
 
  html.SetIncludePath(".\/");
  html.SetSourceDir(".\/");

  html.MakeAll();
}
