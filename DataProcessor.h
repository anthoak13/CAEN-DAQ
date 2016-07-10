//Data processor
//Adam Anthony
//6/30/16
#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include "TObject.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include "SignalProcessor.h"

class DataProcessor : public TObject
{
private:
    std::vector< std::vector<Int_t> > metaData;
    std::vector<UInt_t> eventMap;
    std::vector<FILE*> files;
    UInt_t _max;
    UInt_t _eventLength;
    UInt_t _headerLength;
    UInt_t _interpMult;
    UInt_t _numCh;
    SignalProcessor signalProcessor;

    //Variables to be updated during processEvent
    std::vector<int> signal;
    std::vector<double> deriv;
    std::vector<double> cfd;
    std::vector<int> trap;
    Float_t _baseline;
    UInt_t _timestamp;
    Float_t _zero;
    Float_t _Q;
    

    bool loadFiles(TString, const UInt_t);
    void populateEventMap();
    void nextEvent(FILE*, const UInt_t);

    void setHeaderLength(const UInt_t);
    void setMetaData(TString, UInt_t);
    void setInterpMult(const UInt_t);
    void setNumCh(const UInt_t);

public:
    DataProcessor(TString, TString,  const UInt_t, const UInt_t);
    ~DataProcessor();

    int processEvent(UInt_t, UInt_t);

    int processFiles(bool = false);

    UInt_t getNumEvents();
    UInt_t getMaxEvents();
    UInt_t getEventLength();
    UInt_t getHeaderLength();
    UInt_t getTimestamp();
    UInt_t getNumCh();
    SignalProcessor* getSignalP();
    Float_t getQ();
    Float_t getZero();
    Float_t getBaseline();

    const std::vector<int> getSignal();
    const std::vector<int> getDeriv();
    const std::vector<int> getCFD();
    const std::vector<int> getTrap();
    
    ClassDef(DataProcessor, 0);
};

#endif
