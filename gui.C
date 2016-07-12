//File for gui, macro should be interpreted
//Adam Anthony 

#include "include/gui/gui.h"

void gui()
{
    #ifndef STANDALONE
    gSystem->Load("lib/libCAENDigitizer.so");
    #endif
    testDigitizer();
    new MainFrame(gClient->GetRoot(), 400, 220);
}

//Main Program *****************
#ifdef stand
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   gui();

   theApp.Run();

   return 0;
}
#endif
