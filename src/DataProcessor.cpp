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

#include "DataProcessor.h"
#include "SignalProcessor.h"
#include "TBenchmark.h"

ClassImp(DataProcessor);

DataProcessor::DataProcessor(const TString fileTemplate, const TString meta, const UInt_t numFiles,
			     const UInt_t headerLength)
{
    //Create a new signalProcessor with the default values
    signalProcessor = new SignalProcessor();
    
    //Check if a dummy class should be created
    if(numFiles == 0)
	return;
    
    //set variables
    setHeaderLength(headerLength);
    setNumCh(numFiles);
    
    //load files
    if(!loadFiles(fileTemplate, numFiles))
	throw std::runtime_error("Can't open files");

    //set the metaData
    _meta = meta;
    loadMetaData(numFiles);

    //populate event map
    populateEventMap();

#ifdef DEBUG
    bench = new TBenchmark();
    std::cout << "Making bench: " << bench << std::endl;
#endif
}

DataProcessor::~DataProcessor()
{
#ifdef DEBUG
    if(!bench)
    {
	std::cout << "Bench: " << bench << std::endl;
	delete bench;
    }
#endif
    writeMetaData();
    
    delete signalProcessor;
}
//return 1: event too large
//return 0: success 
Int_t DataProcessor::processEvent(const UInt_t f, const UInt_t event)
{
    if(event >= getNumEvents())
	return 1;
    
    //Reset all variables
    raw.clear();
    signal.clear();
    cfd.clear();
    deriv.clear();
    trap.clear();
    trapDeriv.clear();
    UInt_t header[getHeaderLength()];
    UShort_t voltages[getEventLength()];
    _baseline = 0;
    _timestamp = 0;
    _zero = 0;
    _Q = 0;
    _QDC = 0;

    //Seek to event
    fseek(files[f], eventMap[event], SEEK_SET);
    //Load in the voltages and header
    fread(&header, 4, getHeaderLength(), files[f]);
    fread(&voltages, 2, getEventLength(), files[f]);

    //Get baseline correction
    bool baseValid = calcBaseline(voltages, f, 0.01);

    
    //Populate signal
    for( int i = 0; i < getEventLength(); i++)
    {
	int sig = 0;
	if(metaData[f][0] == 1)
       	    sig = _baseline - voltages[i];
	else
	    sig = voltages[i] - _baseline;

	//Ensure signal is NonNegative
	sig = (sig < 0 ) ? 0 : sig;

	//Push back signal
	signal.push_back(sig);
	trap.push_back(sig);
	raw.push_back(voltages[i]);
    }

    //Get the derivative and perform cfd if the base is valid
    if(baseValid)
    {
	deriv = signalProcessor->deriv(signal);
	cfd   =	signalProcessor->CFD(deriv);
	_zero = signalProcessor->cfdZero(cfd);

	//make sure the zero is valid
	if( (_zero + metaData[f][4]-metaData[f][3]) > trap.size() )
	_zero = metaData[f][3];
    }
    else
	//baseline was sloped so just use user defined sig start
	_zero = metaData[f][3];

    
    //Apply trapazoid filter
    signalProcessor->trapFilter(&trap, _zero, metaData[f][4] - metaData[f][3]);

    //Get moving average of the second derivative (used for pileup ID)
    trapDeriv = signalProcessor->pileupTraceToThreshold(std::vector<Long_t>(trap.begin(), trap.begin() + _zero +
									    metaData[f][4] - metaData[f][3]));

    //Look for pileup
    if(signalProcessor->peaksPastThreshold(trapDeriv) > 1)
    {
//	std::cout << "Failed new method: " << event << std::endl;
	_badEvents++;
	_Q = -1;
    }
    else
    {
	//_Q = signalProcessor->peakFind(trap.begin() + _zero, trap.begin() + _zero +
	//			       metaData[f][4] - metaData[f][3]);
        //Charge should be the value of the Trap function at the zero crossing
	Int_t loc = signalProcessor->peakZero(trapDeriv) + signalProcessor->getPeakDisplacement();
	if(loc >= trap.size())
	    loc = trap.size() -1;
	_Q = trap.at(loc);
    }
    
    //Do old QDC method
    _QDC = signalProcessor->QDC(signal, _zero, metaData[f][4] - metaData[f][3]);

    //Get the timestamp
    _timestamp = header[5];

    return 0;
}

//return 0: sucess
//return 1: trees wrong size
Int_t DataProcessor::processFiles(const bool verbose, const TString fileName)
{
    //Variables to store channel data
    Float_t Q[_numCh];
    Float_t QDC[_numCh];
    Float_t zero[_numCh];
    UInt_t timestamp[_numCh];
    Float_t baseline[_numCh];
    _badEvents = 0;

    //ROOT objects for storage
    TFile* rootFile = new TFile(fileName, "RECREATE");
    TTree* macro_tree = new TTree("macro_tree", "Macro Tree");
    TTree** tmacro_tree = new TTree*[_numCh];
   

    for( int i = 0; i < _numCh; i++ )
    {
	//Create temp trees
	tmacro_tree[i] = new TTree(Form("tmacro_tree_ch%i", i), "tmacro_tree");

	//Initialize the trees
	tmacro_tree[i]->Branch(Form("Q_%i", i), &Q[i], Form("Q_%i/F", i));
	macro_tree->Branch(Form("Q_%i", i), &Q[i], Form("Q_%i/F", i));

	tmacro_tree[i]->Branch(Form("QDC_%i", i), &QDC[i], Form("QDC_%i/F", i));
	macro_tree->Branch(Form("QDC_%i", i), &QDC[i], Form("QDC_%i/F", i));

	tmacro_tree[i]->Branch(Form("baseline_%i", i), &baseline[i], Form("baseline_%i/F", i));
	macro_tree->Branch(Form("baseline_%i", i), &baseline[i], Form("baseline_%i/F", i));

	tmacro_tree[i]->Branch(Form("zero_%i",i), &zero[i], Form("zero_%i/F",i));
	macro_tree->Branch(Form("zero_%i",i), &zero[i], Form("zero_%i/F",i));

	tmacro_tree[i]->Branch(Form("timestamp_%i", i), &timestamp[i], Form("timestamp_%i/i", i));
	macro_tree->Branch(Form("timestamp_%i", i), &timestamp[i], Form("timestamp_%i/i", i));
    }

    //Loop though every file
    for(int f = 0; f < _numCh; f++)
    {
	//check to make sure meta data is possible (all less than the total event length
	bool validMeta = true;
	for(int i = 1; i < 5; i++)
	    validMeta &= (metaData[f][i] < getEventLength());
	
	//If the channel is unused or meta is invalid, skip it
	if(metaData[f][0] == 0 || !validMeta)
	{
	    if(verbose)
		std::cout << "Skipping channel: " << f << std::endl;
	    for( int i = 0; i < eventMap.size(); i++)
	    {
		Q[f] = 0.0;
		QDC[f] = 0.0;
		zero[f] = 0;
		timestamp[f] = 0;
		baseline[f] = 0;
		tmacro_tree[f]->Fill();
	    }
	    continue;
	}

        //Channel is in use so process all events in the file
	if(verbose)
		std::cout << "Processing channel: " << f << std::endl;
	
	for(int event = 0; event < eventMap.size(); event++)
	{
	    if(verbose && event%10000 == 0)
		std::cout << "Processing event: " << event << std::endl;
#ifdef DEBUG
	    bench->Start("Process");
	    processEvent(f, event);
	    bench->Stop("Process");
#endif
#ifndef DEBUG
	    processEvent(f, event);
#endif
	    
	    //Populate arrrays with calculated values
	    baseline[f] = _baseline;
	    zero[f] = _zero;
	    Q[f] = _Q;
	    QDC[f] = _QDC;
	    timestamp[f] = _timestamp;

	    //Fill the tree
	    tmacro_tree[f]->Fill();
	} //over event, events in file
    } //over f, files


    //verify the trees are the same size
    int treeSize = tmacro_tree[0]->GetEntries();
    bool sameSize = true;
    for (int i = 1; i < _numCh && sameSize; i++)
	sameSize &= (tmacro_tree[i]->GetEntries() == treeSize);

    //If the trees are differnt sizes return an error
    if(!sameSize)
	return 1;

    if(verbose)
	std::cout << "Combining trees..." << std::endl;
    
    //Trees are the same size so merge
    for( int i = 0; i < treeSize; i++ )
    {
	for ( int f = 0; f < _numCh; f++ )
	    tmacro_tree[f]->GetEntry(i);

	macro_tree->Fill();
    }

    if(verbose)
	std::cout << "Writing tree..." << std::endl;

    //Write the tree to a root file
    rootFile->cd();
    macro_tree->Write();
    rootFile->Close();

    if(verbose)
	std::cout << "Done "<< std::endl;

#ifdef DEBUG
    Float_t a,b;
    bench->Summary(a,b);
#endif
    if(verbose)
	std::cout << "skipped " << _badEvents << " bad events." << std::endl;

    return 0;
}

UInt_t DataProcessor::getNumEvents() const { return eventMap.size(); }
UInt_t DataProcessor::getMaxEvents() const { return _max; }
UInt_t DataProcessor::getEventLength() const { return _eventLength; }
UInt_t DataProcessor::getHeaderLength() const { return _headerLength; }
UInt_t DataProcessor::getTimestamp() const { return _timestamp; }
UInt_t DataProcessor::getNumCh() const { return files.size(); }
Float_t DataProcessor::getQDC() const { return _QDC; }
Float_t DataProcessor::getQ() const { return _Q; }
Float_t DataProcessor::getZero() const { return _zero; }
Float_t DataProcessor::getBaseline() const { return _baseline; }
UInt_t DataProcessor::getBadEvents() const { return _badEvents; }
SignalProcessor* DataProcessor::getSignalP() const { return signalProcessor; }

 std::vector<Long_t> DataProcessor::getTrap() const { return trap; }
 std::vector<Long_t> DataProcessor::getSignal() const
{
    std::vector<Long_t> out;
    out.reserve(signal.size());
    for(auto &&num:signal)
	out.push_back(num);
    
    return out;
}
 std::vector<Long_t> DataProcessor::getDeriv() const
{
    std::vector<Long_t> out;
    for(auto it = deriv.begin(); it < deriv.end(); it+= signalProcessor->getInterpMult())
	out.push_back(*it);
    return out;
}
 std::vector<Long_t> DataProcessor::getTrapDeriv() const
{
    std::vector<Long_t> out;
    for(auto it = trapDeriv.begin(); it < trapDeriv.end(); it+= signalProcessor->getInterpMult())
	out.push_back(*it);
    return out;
}
 std::vector<Long_t> DataProcessor::getCFD() const
{
    std::vector<Long_t> out;
    for(auto it = cfd.begin(); it < cfd.end(); it+= signalProcessor->getInterpMult())
	out.push_back(*it);
    return out;
}
 std::vector<Long_t> DataProcessor::getRaw() const
{
    std::vector<Long_t> out;
    for(auto &&num:raw)
	out.push_back(num);
    return out;
}

//Private **********************************
void DataProcessor::setHeaderLength(const UInt_t in) { _headerLength = in; }
void DataProcessor::setNumCh(const UInt_t in) { _numCh = in; }

//Load in files
bool DataProcessor::loadFiles(TString fileTemplate, const UInt_t numFiles)
{
    bool filesLoaded = true;
    for(UInt_t i = 0; i < numFiles; i++)
    {
	files.push_back(fopen(Form(fileTemplate, i), "rb"));
	filesLoaded &= (files[i] != NULL);
    }

    return filesLoaded;
}

//Populate event map and set number of samples in an event
void DataProcessor::populateEventMap()
{
    //Get the max number of events and event length
    UInt_t maxEvents, fileSize, eventSize;
    rewind(files[0]);
    fread(&eventSize, 4, 1, files[0]);
    fseek(files[0], 0, SEEK_END);
    fileSize = ftell(files[0]);
    maxEvents = fileSize/eventSize;
    eventMap.reserve(maxEvents);
    _max = maxEvents;
    _eventLength = (eventSize - 4*_headerLength)/2;

    rewind(files[0]);
    
    //Look though 1st file and create mapping of events
    UInt_t event = 0;
    while(ftell(files[0]) + eventSize < fileSize)
    {
	//add current event to map
	eventMap.push_back(ftell(files[0]));
	nextEvent(files[0], eventSize);
    }
}

//Finds the next event in file using the supplied event size and looking for
//The 1st element of the header
void DataProcessor::nextEvent(FILE* file, const UInt_t eventSize) const
{
    //Move forward an event
    fseek(file, eventSize, SEEK_CUR);

    //Look for the next valid header
    bool foundheader = false;
    UShort_t prevBytes = 0;
    UShort_t bytes = 0;
    while(bytes != 0 || prevBytes != (UShort_t)eventSize)
    {
	prevBytes = bytes;
	fread(&bytes, 2, 1, file);
    }
    
    //rewind 2 bytes to get file pointing to the begininning of header
    fseek(file, -4, SEEK_CUR);
}


void DataProcessor::loadMetaData(const UInt_t numFiles)
{
    //CLear out the old meta data
    metaData.clear();

    
    //Load file
    std::ifstream file;
    file.open(_meta);
    if(file.is_open())
    {
    //If the file exists read its content 
    while(!file.eof())
    {
	TString temp;
	temp.ReadToken(file);
	
	//If the line should be skipped
	if(temp.Contains("#"))
	{
	    temp.ReadLine(file);
	    continue;
	}

	//get the meta data
	std::vector<Int_t> tempVec;
	for(int i = 0; i < 5; i++)
	{
	    temp.ReadToken(file);
	    tempVec.push_back(temp.Atoi());
	}
	metaData.push_back(tempVec);
    }

    file.close();
    }

    //Fill in the meta data for the rest of the channels with
    //default values
    for(UInt_t i = metaData.size(); i < numFiles; i++)
    {
	std::vector<Int_t> temp;
	temp.push_back(0);
	temp.push_back(0);
	temp.push_back(30);
	temp.push_back(35);
	temp.push_back(100);
	metaData.push_back(temp);
    }
}

void DataProcessor::writeMetaData() const
{
#ifdef DEBUG
    std::cout << "Writing meta: " << _meta << std::endl;
#endif
    std::ofstream file;
    file.open(_meta);

    if(!file.is_open())
	return;

    //Print header for file
    file << "# Ch Type BaselineStart BaselineEnd SigStart SigEnd" << std::endl;
    for(int i = 0; i < metaData.size(); i++)
    {
	file << i;
	for(auto&& num:metaData[i])
	    file << " " << num;
	file <<std::endl;
    }

    file.close();
}

bool DataProcessor::calcBaseline(UShort_t *sig, const int f, const Double_t tol)
{
    //Do baseline correction
    for(int i = metaData[f][1]; i < metaData[f][2]; i++)
    {
	_baseline += sig[i];
    }
    _baseline /= (metaData[f][2] - metaData[f][1]);

    //Get variables to determine slope
    const Double_t base1 = (sig[metaData[f][1]] - _baseline)/_baseline;
    const Double_t base2 = (sig[metaData[f][2]] - _baseline)/_baseline;
    const bool baseValid = !((base1 > tol) && ( base2 < -tol));

    //Check basline for slope
    _baseline = baseValid ? _baseline : sig[metaData[f][2]];
#ifdef DEBUG
    //if(!baseValid)
    //std::cout << "Baseline sloped at: " << _baseline << std::endl;
#endif

    return baseValid;
}
				 
