/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                             Copyright (C) 2016                              //
//                     Adam Anthoony : All rights reserved                     //
//                                                                             //
//      This source code is licensed under the GNU GPL v3.0.You have the       //
//      right to modify and/or redistribute this source code under the terms   //      
//      specified in the license, which may be found online at                 //
//      http://www.gnu.org/licenses.                                           //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

//Class to track, read and create a wavedump config file
//Adam Anthony 7/7/16
#ifndef _WAVEDUMPCONFIGH
#define _WAVEDUMPCONFIGH

#ifndef __CINT__
#include "TObject.h"
#include "TString.h"
#include "TRegexp.h"
#include <vector>
#include <fstream>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#endif
#include <map>

enum ChannelTrigger
{
    DSBLD,
    ACQ,
    ACQ_TRGOUT
};
enum class DACtype
{
    x742,
    x731,
    x751
};

class ChannelConfig
{
private:
    Bool_t enabled;
    Float_t DCOffset;
    UInt_t triggerThreshold;
    ChannelTrigger triggerSetting;
public:

    ChannelConfig(const ChannelConfig &obj)
    {
	enabled = obj.getEnabled();
	DCOffset = obj.getDCOffset();
	triggerThreshold = obj.getTrigThreshold();
	triggerSetting = obj.getChanTrig();
    }
    ChannelConfig(Bool_t aIn, Float_t bIn, UInt_t cIn, ChannelTrigger dIn)
    {
	setEnabled(aIn); setDCOffset(bIn);
	setTrigThreshold(cIn); setChanTrig(dIn);
    }
    ChannelConfig() { ChannelConfig(false, 0, 0, DSBLD); }
    ~ChannelConfig() { }

    Bool_t getEnabled() const { return enabled; }
    Float_t getDCOffset() const { return DCOffset; }
    UInt_t getTrigThreshold() const { return triggerThreshold; }
    ChannelTrigger getChanTrig() const {return triggerSetting; }

    void setEnabled(Bool_t in) { enabled = in;};
    void setDCOffset(Float_t in) { if(in < 50 && in > -50 ) DCOffset = in;};
    void setTrigThreshold(UInt_t in) { triggerThreshold = in;};
    void setChanTrig(ChannelTrigger in) { triggerSetting = in;};

    void write(std::ofstream&);
    void read(std::ifstream&);

    ClassDef(ChannelConfig, 0)
};

class WavedumpConfig : public TObject
{
private:
    std::map<UInt_t, ChannelConfig> chConfigs;
    ChannelConfig globalChan;
    
    Bool_t _USB;
    Bool_t _hexLocation;
    std::vector<UInt_t> _digitizerLoc;

    TString gnuPlotPath;

    Bool_t _binary;
    Bool_t _header;
    UShort_t _recordLength;
    Bool_t _testPattern;
    ChannelTrigger _extTrig;
    UShort_t _maxEvents;
    UChar_t _postTrig;
    Bool_t _trigRising;
    UShort_t _interrupt;
    Bool_t _NIM;

    TString fileName;
    
    
public:
    WavedumpConfig(TString name = "WaveDumpConfig.txt");
    ~WavedumpConfig();

    void write();

    void read();
    
    void setInput(Bool_t USB, UInt_t int1, UInt_t int2, Bool_t hex, UInt_t int3 = 0);

    void setGnuPlotPath(TString path) { gnuPlotPath = path; };

    void setOutput(Bool_t binary, Bool_t header, Bool_t testPattern, UShort_t recordLength);

    void setConfig(ChannelTrigger extTrig, UShort_t eventsUntilFlush,
		   UChar_t postTrig, Bool_t trigOnRise, UShort_t interrupt, Bool_t NIM);

    void setGlobalChan(const ChannelConfig &chIn);
    void addChannel(UInt_t chNum, const ChannelConfig &chIn);

    ClassDef(WavedumpConfig, 0)

};

#endif

