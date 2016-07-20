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

#ifndef __METACONFIGPOPOUT_H
#define __METACONFIGPOPOUT_H

#include "gui.h"
#include <string>

class DataProcessor;

class MetaConfigPopout {
    RQ_OBJECT("MetaConfigPopout");

private:
    TGTransientFrame *fMain;
    //Add Channel
    TGHorizontalFrame *fTop, *fMiddle, *fButtons;
    TGVerticalFrame *f0, *fLabel, *fRight;
    TGTextButton *fBAddCh;
    TGNumberEntryField *fNEntryMeta[5];
    TGComboBox *fCBChan, *fCBChType;
    
    TGTextButton *bOk, *bCancel;
    UInt_t oldCh;
    DataProcessor *dataP;
    
#ifndef __CINT__
    const char *fLabelMeta[5] = {
    "Channel type",
    "Baseline start",
    "Baseline end",
    "Signal start",
    "Signal end" };
#endif

    void SetupChannel(UInt_t);
    void SaveChannel(UInt_t);
    
public:
    MetaConfigPopout(const TGWindow *p, const TGWindow *main,  DataProcessor *dataPIn);
    virtual ~MetaConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    void SelectChannel(Int_t);
    void AddChannel();

};
#endif
