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

#ifndef __CONFIGPOPOUT_H
#define __CONFIGPOPOUT_H

#include "gui.h"
class SignalProcessor;
class DataProcessor;
//Definition of Config popout
class ConfigPopout {
    RQ_OBJECT("ConfigPopout");

private:
    static const UInt_t numEntries = 12;
    TGTransientFrame *fMain;
    TGCompositeFrame *f1,*f11, *f2;
    TGGroupFrame *fTrap, *fZero, *fPileup, *fPeak;
    TGTextButton *bCancel, *bOK;
    TGHorizontalFrame *fNumFrames[numEntries];
    TGNumberEntry *fEntry[numEntries];
    SignalProcessor *signalP;

#ifndef __CINT__
    const char *fLabel[numEntries] = {
    "Rise time         ",
    "M",
    "Top Multiplier    ",

    "Zero Offset",
    "Zero Threshold",
    "Interpolation Mult ",

    "High Threshold   ",
    "Low Threshold",
    "Peak Length",

    "Distance to sample",
    "Points to average ",
    "Peak Threshold"};
#endif
    
    
public:
    ConfigPopout(const TGWindow*, const TGWindow*, DataProcessor*);
    virtual ~ConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    
};
#endif
