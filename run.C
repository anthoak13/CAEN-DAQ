void run()
{
    gInterpreter->AddIncludePath("include");
    gInterpreter->AddIncludePath("include/gui");
    gSystem->Load("lib/libCAENDigitizer.so");
    gROOT->ProcessLine(".x gui.C");
}
