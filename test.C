//script for trying PSD
#include "TApplication.h"
#include "TROOT.h"

void test()
{
    gApplication->Terminate();
}

#ifdef STANDALONE

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   test();

   theApp.Run();

   return 0;
}
#endif
