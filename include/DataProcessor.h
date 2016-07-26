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

//Data processor
//Adam Anthony
//6/30/16
#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

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

//Forward Declerations
class TBenchmark;
class SignalProcessor;

//Class def
class DataProcessor : public TObject
{
private:
    std::vector< std::vector<Int_t> > metaData;
    std::vector<UInt_t> eventMap;
    std::vector<FILE*> files;
    UInt_t _max;
    UInt_t _eventLength;
    UInt_t _headerLength;
    UInt_t _numCh;
    SignalProcessor *signalProcessor;

    //Variables to be updated during processEvent
    std::vector<UShort_t> raw;
    std::vector<int> signal;
    std::vector<double> deriv;
    std::vector<double> cfd;
    std::vector<Double_t> trapDeriv;
    std::vector<Long_t> trap;
    Float_t _baseline;
    UInt_t _timestamp;
    Float_t _zero;
    Float_t _Q;
    Float_t _QDC;
    UInt_t _badEvents;
    TString _meta;

    TBenchmark *bench;
    

    bool loadFiles(TString, const UInt_t);
    void populateEventMap();
    void nextEvent(FILE*, const UInt_t);

    void setHeaderLength(const UInt_t);
    void loadMetaData(UInt_t);
    void writeMetaData();
    void setNumCh(const UInt_t);

    bool getBaseline(UShort_t *voltages, const int f, const Double_t tol);

public:
    DataProcessor(TString, TString,  const UInt_t, const UInt_t, const UInt_t);
    ~DataProcessor();

    int processEvent(UInt_t, UInt_t);

    int processFiles(bool = false);

    void setMetaData(UInt_t ch, std::vector<Int_t> data) { metaData[ch] = data; };
    std::vector<Int_t> getMetaData(UInt_t ch) { return metaData[ch]; };
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
    Float_t getQDC();
    UInt_t getBadEvents();
    
    const std::vector<Long_t> getSignal();
    const std::vector<Long_t> getDeriv();
    const std::vector<Long_t> getCFD();
    const std::vector<Long_t> getTrap();
    const std::vector<Long_t> getRaw();
    const std::vector<Long_t> getTrapDeriv();
    
    ClassDef(DataProcessor, 0)
};

#endif
