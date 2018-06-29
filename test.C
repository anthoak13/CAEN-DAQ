///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                            Copyright (C) 2016                             //
//                    Adam Anthoony : All rights reserved                    //
//                                                                           //
//     This source code is licensed under the GNU GPL v3.0.You have the      //
//     right to modify and/or redistribute this source code under the terms  //
//     specified in the license, which may be found online at                //
//     http://www.gnu.org/licenses.                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

//script for trying PSD
#include "TApplication.h"
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "DataProcessor.h"
#include "SignalProcessor.h"
#include "TMath.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   gStyle->SetLegendBorderSize(0);   
   //genGraphs();
   //NaIPosterGraphs();
   HPGePosterGraphs();
   theApp.Run();

   return 0;
}
#endif
