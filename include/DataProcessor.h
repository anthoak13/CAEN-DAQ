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
#include <stdexcept>
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
    std::vector<Int_t> signal;
    std::vector<Double_t> deriv;
    std::vector<Double_t> cfd;
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
    

    bool loadFiles(const TString fileTemplate, const UInt_t numFiles);
    void populateEventMap();
    void nextEvent(FILE *file, const UInt_t eventSize) const;

    void setHeaderLength(const UInt_t length);
    void setNumCh(const UInt_t );

    void loadMetaData(const UInt_t numFiles);
    void writeMetaData() const;

    bool calcBaseline(UShort_t *voltages, const int f, const Double_t tol);

    Float_t doTrap(UInt_t file);

public:
    DataProcessor(const TString fileTemplate, const TString meta, const UInt_t numFiles,
		  const UInt_t headerLength);
    ~DataProcessor();

    Int_t processEvent(const UInt_t file, const UInt_t event);

    Int_t processFiles(const bool verbose = false, const TString fileName = "macro.root");

    void setMetaData(UInt_t ch, std::vector<Int_t> data) { metaData[ch] = data; };
    std::vector<Int_t> getMetaData(const UInt_t ch) const { return metaData[ch]; };
    UInt_t getNumEvents() const;
    UInt_t getMaxEvents() const;
    UInt_t getEventLength() const;
    UInt_t getHeaderLength() const;
    UInt_t getTimestamp() const;
    UInt_t getNumCh() const;
    SignalProcessor* getSignalP() const;
    Float_t getQ() const;
    Float_t getZero() const;
    Float_t getBaseline() const;
    Float_t getQDC() const;
    UInt_t getBadEvents() const;
    
    std::vector<Long_t> getSignal() const;
    std::vector<Long_t> getDeriv() const;
    std::vector<Long_t> getCFD() const;
    std::vector<Long_t> getTrap() const;
    std::vector<Long_t> getRaw() const;
    std::vector<Long_t> getTrapDeriv() const;
    
    ClassDef(DataProcessor, 0)
};

#endif
