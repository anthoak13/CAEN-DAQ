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
    TGNumberEntryField *fNEntryMeta[4];
    TGComboBox *fCBChan;
    TGComboBox *fCBChType, *fCBProcessingType;
    
    TGTextButton *bOk;
    Int_t _oldCh;
    DataProcessor *dataP;
    
#ifndef __CINT__
    const char *fLabelMeta[6] = {
    "Channel type",
    "Proccesing type",
    "Baseline start",
    "Baseline end",
    "Signal start",
    "Signal end" };
#endif
    
public:
    MetaConfigPopout(const TGWindow *p, const TGWindow *main,  DataProcessor *dataPIn);
    virtual ~MetaConfigPopout();
    void CloseWindow();
    void DoOk();
    void SelectChannel(Int_t);

};
#endif
