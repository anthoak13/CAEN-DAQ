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

void test(TString fileTemplate = "/data/0/adam/wave%d.dat",
	  Int_t numFiles = 1, TString outFile = "test.root")
{

  //  std::cout << Form("%s %d %s", fileTemplate.Data(), numFiles, outFile.Data())
  //	    << std::endl;

   BinaryLoader *loader = new BinaryLoader(fileTemplate, numFiles, outFile);
   loader->print();
   loader->writeTree();
   //delete loader;
  
}


#ifdef STANDALONE
int main(int argc, char *argv[])
{
  if(argc != 4)
    {
      std::cout << "Usage: unpacker dataTempalte numFiles output.root" 
		<< std::endl;
      return 1;
    }
  test(argv[1], atoi(argv[2]), argv[3]);
  
  return 0;
}
#endif
