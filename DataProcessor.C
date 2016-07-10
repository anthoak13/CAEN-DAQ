//Data Processor implimentation
//Adam Anthony 6/30/16

#include "DataProcessor.h"

DataProcessor::DataProcessor(TString fileTemplate, TString meta, const UInt_t numFiles, const UInt_t headerLength)
{
    if(numFiles == 0)
	return;
    //set any variables
    setHeaderLength(headerLength);
    setInterpMult(2);
    setNumCh(numFiles);

    //set the metaData
    setMetaData(meta, numFiles);
    
    //load files
    if(!loadFiles(fileTemplate, numFiles))
	throw std::runtime_error("Can't open files");

    //populate event map
    populateEventMap();

    //set signalProcessor up
    signalProcessor.setDecayTime(13);
    signalProcessor.setFlatMult(2.0/3.0);
    signalProcessor.setOffset(16);
    signalProcessor.setScaling(0.8);
    signalProcessor.setThreshold(-17);
}

DataProcessor::~DataProcessor()
{

}
//return 1: event too large
//return 0: success 
int DataProcessor::processEvent(UInt_t f, UInt_t event)
{
    //make sure the event is valid
    if(event >= getNumEvents())
	return 1;

    //Reset all variables
    signal.clear();
    cfd.clear();
    deriv.clear();
    trap.clear();
    UInt_t header[getHeaderLength()];
    UShort_t voltages[getEventLength()];
    _baseline = 0;
    _timestamp = 0;
    _zero = 0;
    _Q = 0;

    //Seek to event
    fseek(files[f], eventMap[event], SEEK_SET);
    //Load in the voltages and header
    fread(&header, 4, getHeaderLength(), files[f]);
    fread(&voltages, 2, getEventLength(), files[f]);

    //Do baseline correction
    for(int i = metaData[f][1]; i < metaData[f][2]; i++)
    {
	_baseline += voltages[i];
    }
    _baseline /= (metaData[f][2] - metaData[f][1]);

    //Populate signal
    for( int i = 0; i < getEventLength(); i++)
    {
	int sig = 0;
	if(metaData[f][0] == 1)
       	    sig = _baseline - voltages[i];
	else
	    sig = voltages[i] - _baseline;
	
	sig = (sig < 0 ) ? 0 : sig;
	signal.push_back(sig);
	trap.push_back(sig);
    }
	
    //find derivative and cfd
    deriv = signalProcessor.interpolateDeriv(&signal, _interpMult);
    cfd = deriv;
    signalProcessor.CFD(&cfd);
    
    //Variables
    _zero = signalProcessor.zeroAfterThreshold(&cfd)/_interpMult;

    //correct zero
    if(_zero + (metaData[f][4]-metaData[f][3]) > trap.size())
	_zero = metaData[f][3];
    signalProcessor.trapFilter(&trap, _zero, metaData[f][4] - metaData[f][3]);
    _Q = signalProcessor.peakFind(trap.begin() + _zero, trap.begin() + _zero +
				  metaData[f][4] - metaData[f][3]);
    _timestamp = header[5];//TODO
    
    return 0;

}
//return 0: sucess
//return 1: trees wrong size

int DataProcessor::processFiles(bool verbose)
{
    //Variables to store
    Float_t Q[_numCh];
    Float_t zero[_numCh];
    UInt_t timestamp[_numCh];
    Float_t baseline[_numCh];

    //ROOT objects for storage
    TFile* rootFile = new TFile("macro.root", "RECREATE");
    TTree* macro_tree = new TTree("macro_tree", "Macro Tree");
    TTree** tmacro_tree = new TTree*[_numCh];
   

    for( int i = 0; i < _numCh; i++ )
    {
	//Create temp trees
	tmacro_tree[i] = new TTree(Form("tmacro_tree_ch%i", i), "tmacro_tree");

	//Fill the tree
	tmacro_tree[i]->Branch(Form("Q_%i", i), &Q[i], Form("Q_%i/F", i));
	macro_tree->Branch(Form("Q_%i", i), &Q[i], Form("Q_%i/F", i));

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
	//If the channel is unused, skip it
	if(metaData[f][0] == 0)
	{
	    if(verbose)
		std::cout << "Skipping channel: " << f << std::endl;
	    for( int i = 0; i < eventMap.size(); i++)
	    {
		Q[f] = 0.0;
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
	    processEvent(f, event);
	    baseline[f] = _baseline;
	    zero[f] = _zero;

	    Q[f] = _Q;
	    timestamp[f] = _timestamp;

	    //if(verbose && _Q > 570000 && _Q < 580000)
	    //std::cout << _Q << "  " << _zero << "   " << event << std::endl;

	    //Fill the tree
	    tmacro_tree[f]->Fill();
	} //over event, events in file
    } //over f, files


    //verify the trees are the same size
    int treeSize = tmacro_tree[0]->GetEntries();
    bool sameSize = true;
    for (int i = 1; i < _numCh && sameSize; i++)
	sameSize &= (tmacro_tree[i]->GetEntries() == treeSize);

    //If the trees are differnt sizes quit
    if(!sameSize)
	return 1;

    if(verbose)
	std::cout << "Combining and writing tree..." << std::endl;
    
    //Trees are the same size so merge
    for( int i = 0; i < treeSize; i++ )
    {
	for ( int f = 0; f < _numCh; f++ )
	    tmacro_tree[f]->GetEntry(i);

	macro_tree->Fill();
    }

    //Write the tree to a root file
    rootFile->cd();
    macro_tree->Write();
    rootFile->Close();

    if(verbose)
	std::cout << "Done "<< std::endl;
    
    //merge trees
    //delete temp trees
    return 0;
}

UInt_t DataProcessor::getNumEvents() { return eventMap.size(); }
UInt_t DataProcessor::getMaxEvents() { return _max; }
UInt_t DataProcessor::getEventLength() { return _eventLength; }
UInt_t DataProcessor::getHeaderLength() { return _headerLength; }
UInt_t DataProcessor::getTimestamp() { return _timestamp; }
UInt_t DataProcessor::getNumCh() { return files.size(); }
Float_t DataProcessor::getQ() { return _Q; }
Float_t DataProcessor::getZero() { return _zero; }
Float_t DataProcessor::getBaseline() { return _baseline; }
SignalProcessor* DataProcessor::getSignalP() { return &signalProcessor; }
const std::vector<int> DataProcessor::getSignal() { return signal; }
const std::vector<int> DataProcessor::getTrap() { return trap; }
const std::vector<int> DataProcessor::getDeriv()
{
    std::vector<Int_t> out;
    for(std::vector<double>::iterator it = deriv.begin(); it < deriv.end(); it+= _interpMult)
	out.push_back(*it);
    return out;
}
const std::vector<int> DataProcessor::getCFD()
{
    std::vector<Int_t> out;
    for(std::vector<double>::iterator it = cfd.begin(); it < cfd.end(); it+= _interpMult)
	out.push_back(*it);
    return out;
}

//Private **********************************
void DataProcessor::setHeaderLength(const UInt_t in) { _headerLength = in; }
void DataProcessor::setInterpMult(const UInt_t in) { _interpMult = in; }
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
    //Get the max number of events
    UInt_t maxEvents, fileSize, eventSize;
    rewind(files[0]);
    fread(&eventSize, 4, 1, files[1]);
    fseek(files[0], 0, SEEK_END);
    fileSize = ftell(files[0]);
    maxEvents = fileSize/eventSize;
    eventMap.reserve(maxEvents);
    _max = maxEvents;
    _eventLength = (eventSize - 4*_headerLength)/2;

    rewind(files[0]);
    
    //Look though 1st file for each event
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
void DataProcessor::nextEvent(FILE* file, const UInt_t eventSize)
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
    
void DataProcessor::setMetaData(TString meta, UInt_t numFiles)
{
    for(UInt_t i = 0; i < numFiles; i++)
    {
	std::vector<Int_t> temp;
	temp.push_back( ( i == 0 ) ? 1 : 0);
	temp.push_back(0);
	temp.push_back(30);
	temp.push_back(35);
	temp.push_back(100);
	metaData.push_back(temp);
    }
}
