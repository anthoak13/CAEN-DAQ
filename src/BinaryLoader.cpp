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

//Data Processor implimentation
//Adam Anthony 6/30/16

#include "BinaryLoader.h"

ClassImp(BinaryLoader)

BinaryLoader::BinaryLoader(const TString fileTemplate, const UInt_t numFiles,
			   const UInt_t headerLength, const TString outFileName)
{
  _numFiles = numFiles;
  _headerLength = headerLength;
  
  if( !loadFiles(fileTemplate))
    throw  std::runtime_error(TString("Can't open files: " + fileTemplate).Data());
  
  //Load the headers and data about the files
  //Sets the length of the sample as well as the number of event in each file
  //Also includes which channel is in each file
  if(!readRunData())
    throw  std::runtime_error("Can't load information from binary files");
  //Files have been successfully loaded and data about run loaded
  
  //Now generate the tree structure to store the events
  outFile = new TFile(outFileName, "RECREATE");
  outTree = new TTree("BinaryTree", "BinaryTree");
  
  //Create header information
  outTree->Branch("header", &_header, "i/Timestamp:i/TriggerNum:i/EventLength");

  //Populate the tree with the waveforms
  for(int i = 0; i < numFiles; i++)
    outTree->Branch(Form("ch_%d", i), _adc[i], Form("s/wave[%d]",_eventLength));
		   
}

BinaryLoader::~BinaryLoader()
{
  if(_header != nullptr)
    delete _header;
  if(_adc != nullptr)
    {
      for(int i = 0; i < _numFiles; i++)
	if(_adc[i] != nullptr)
	  delete _adc[i];
      delete _adc;
    }
  
  if(outFile->IsOpen())
    {
      outFile->Write();
      outFile->Close();
    }
  if(outFile != nullptr)
    delete outFile; 
  if(outTree != nullptr)
    delete outTree;
  
  
  
}

bool BinaryLoader::loadFiles(const TString fileTemplate)
{
  bool filesLoaded = true;

  files = new FILE*[_numFiles];
  for(int i = 0 ; i < _numFiles; i++)
    {
      files[i] = fopen(Form(fileTemplate, i), "rb");
      filesLoaded &= ( files[i] != nullptr );
    }
  return filesLoaded;
}

bool BinaryLoader::readRunData()
{
  //Set the min events to the maximum possible
  UInt_t minEvents = 0;
  minEvents = ~minEvents;

  //Loop through each file and find the number of events
  for(int i = 0; i < _numFiles; i++)
    {
      rewind(files[i]);
      UInt_t eventSize = 0;
      //TODO:This might be readin it in backwards. It's either this or skip one byte and then read in 3
      fread(&eventSize, 3, 1, files[0]);
      

    }

  return true;
}

void BinaryLoader::writeTree()
{

}
