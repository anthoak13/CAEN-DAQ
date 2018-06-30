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


BinaryLoader::BinaryLoader(const TString fileTemplate, const UInt_t numFiles,
			   const TString outFileName)
{
  _numFiles = numFiles;
  _headerLength = 6;
  _chMap  = new UInt_t[numFiles];
  _header = new UInt_t[3];

  if( !loadFiles(fileTemplate))
    throw  std::runtime_error(TString("Can't open files: " + fileTemplate).Data());
  
  //Load the headers and data about the files
  //Sets the length of the sample as well as the number of event in each file
  //Also includes which channel is in each file
  if(!readRunData())
    throw  std::runtime_error("Can't load information from binary files");

  //Files have been successfully loaded and data about run loaded
  //So now we can initialize the arrays to hold the adc values
  _adc = new UShort_t*[_numFiles];
  for(int i = 0; i < _numFiles; i++)
      _adc[i] = new UShort_t[_eventLength];


  //Now generate the tree structure to store the events
  outFile = new TFile(outFileName, "RECREATE");
  outTree = new TTree("BinaryTree", "BinaryTree");
  
  //Create header information
  outTree->Branch("header", _header, "Timestamp/i:TriggerNum/i:EventLength/i");

  //Populate the tree with the waveforms
  for(int i = 0; i < numFiles; i++)
    outTree->Branch(Form("ch_%d", _chMap[i]), _adc[i], Form("wave[%d]/s",_eventLength));
		   
}

BinaryLoader::~BinaryLoader()
{
  if(_header != nullptr)
    delete[] _header;
  if(_chMap != nullptr)
    delete[] _chMap;

  if(_adc != nullptr)
    {
      for(int i = 0; i < _numFiles; i++)
	if(_adc[i] != nullptr)
	  delete[] _adc[i];
      delete[] _adc;
    }
  
  if(outFile->IsOpen())
    {
      outFile->Write();
      outFile->Close();
    }
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
      UInt_t eventSize, ch;
      rewind(files[i]);

      //Read in the size of an event and ch
      fread(&eventSize, 4, 1, files[i]);     
      fseek(files[i], 3*4, SEEK_SET);
      fread(&ch, 4, 1, files[i]);
      _chMap[i] = ch;


      //Get the file size and caclulate the number of events
      fseek(files[i], 0, SEEK_END);
      UInt_t fileSize = ftell(files[i]);
      
      //Update the number of events in the run
      minEvents = (minEvents > fileSize/eventSize) ? 
	fileSize/eventSize : minEvents;
      
      rewind(files[i]);
      
      if(i == 0)
	{
	  _eventLength = (eventSize - 4*_headerLength)/2;
	  _eventSize = eventSize;
	}
    }
  
  _numEvents = minEvents;

  return true;
}

void BinaryLoader::writeTree()
{
  //_numEvents = 10;
  for(int ievt = 0; ievt < _numEvents; ievt++)
    {
      if(ievt % 10000 == 0)
	std::cout << Form("Processing event %d of %d", ievt, _numEvents) << std::endl;
      
      //Create variables to update along version
      bool validEvent = true;
      UInt_t *tempHeader = new UInt_t[_headerLength];
      fseek(files[0], ievt*_eventSize, SEEK_SET);
      readHeader(files[0], _header);
      fseek(files[0], ievt*_eventSize, SEEK_SET);
      
      for(int i = 0; i < _numFiles && validEvent; i++)
	{
	  //Make sure the events match
	  fseek(files[i], ievt*_eventSize, SEEK_SET);
	  readHeader(files[i], tempHeader);
	  for(int j = 1; j < 3; j++)
	      validEvent &= (_header[j] == tempHeader[j]);

	  /*std::cout << Form("%u %u %d\n%u %u %d\n%u %u %d",
			    _header[0], tempHeader[0], _header[0]-tempHeader[0],
			    _header[1], tempHeader[1], _header[1]-tempHeader[1],
			    _header[2], tempHeader[2], _header[2]-tempHeader[2])
		    << std::endl << std::endl;
	  */
	  fread(_adc[i], 2, _eventLength, files[i]);

	}

      if(validEvent)
	outTree->Fill();
      else
	std::cout << "Skipping event " << ievt << std::endl;
    }
       
  outFile->Write();
  outFile->Close();
}

void BinaryLoader::readHeader(FILE* file, UInt_t*header)
{
  //Get event size
  UInt_t eventSize;
  fread(&eventSize, 4, 1, file);     
  //Seek and get trigger number
  fseek(file, 4*4, SEEK_CUR);
  fread(&header[1], 4, 1, file);
  //Seek and het timestamp
  fread(&header[0], 4, 1, file);
  header[2] = (eventSize - _headerLength*4)/2;
}
void BinaryLoader::print()
{
  std::cout << Form("EventLength: %d\nNumberEvents : %d",
		    _eventLength, _numEvents)
	    << std::endl;
}
