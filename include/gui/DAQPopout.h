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

#ifndef __DAQPOPOUT_H
#define __DAQPOPOUT_H

#include "gui.h"
class MainFrame;
class DAQPopout {
    RQ_OBJECT("DAQPopout");
    
private:
    TGTransientFrame *fMain;
    TGVerticalFrame *f2, *f4;
    TGHorizontalFrame *f3;
    TGLabel *fLabelIn, *fLabelMeta, *fLabelCh, *fLabelHeader;
    TGTextEntry *fEntryIn, *fEntryMeta;
    TGTextButton *fBOk, *fBCancel;
    TGNumberEntry *fEntryCh, *fEntryHeader;

    TString *inputTemplate, *meta;
    UInt_t *numCh, *headerLength;

public:
    DAQPopout(const TGWindow*, const TGWindow*,  MainFrame*, TString*, TString*, UInt_t*, UInt_t*);
    virtual ~DAQPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
};
#endif
