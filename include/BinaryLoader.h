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

// Binary loader
// Adam Anthony
// 6/28/18

/**
 ** This class loads in binary files generated by wavedump and ouput a root tree with the data
 ** It does some simple data verification, throwing out events that have corrupted data
 ** in one of the channels
 **/

#ifndef BINARYLOADER_H
#define BINARYLOADER_H

#ifndef __CINT__
#include "TObject.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#endif
#include "TTree.h"
#include "TFile.h"
//Class def
class BinaryLoader : public TObject
{

 private:
  UInt_t _eventLength;
  UInt_t _headerLength;
  UInt_t _numFiles;
  UInt_t *_header; //Timestamp, TriggerNum, EventLength
  
  //adc[file][sample]
  UShort_t **_adc; 
  

  TFile *outFile;
  TTree *outTree;
  FILE **files;

  //Attempt to open all of the files
  bool loadFiles(const TString fileTemplate);

  //Load the headers and data about the files
  //Sets the length of the sample as well as the number of event in each file
  //Also includes which channel is in each file
  //Load the size of each window as well as the 
  bool readRunData();
  

 public:

  
  BinaryLoader(const TString fileTemplate, const UInt_t numFiles,
	       const UInt_t headerLength, const TString outFileName);
  ~BinaryLoader();

  void writeTree();

  ClassDef(BinaryLoader,0)

};

#endif
