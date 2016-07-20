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
    TGTransientFrame *fMain;
    TGCompositeFrame *f1, *f2;
    TGGroupFrame *fTrap, *fZero;
    TGTextButton *bCancel, *bOK;
    TGHorizontalFrame *fNumTrap[4], *fNumZero[4];
    TGNumberEntry *fEntryTrap[4], *fEntryZero[4];
    SignalProcessor *signalP;

#ifndef __CINT__
    const char *fLabel[8] = {
    "Rise time",
    "Top",
    "M",
    "Peak Threshold",
    "Offset",
    "Scaling",
    "Zero Threshold",
    "Interpolation mult"};
#endif
    
    
public:
    ConfigPopout(const TGWindow*, const TGWindow*, DataProcessor*);
    virtual ~ConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    
};
#endif
