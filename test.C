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

#include "BinaryLoader.h"
#include "DataProcessor.h"

#include <fstream>
#include <iostream>

void test()
{

   BinaryLoader *loader = new BinaryLoader("/data/0/adam/wave%d.dat", 1, "test.root");
   loader->print();
   loader->writeTree();
   //delete loader;
  
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


   return 0;
}
#endif
