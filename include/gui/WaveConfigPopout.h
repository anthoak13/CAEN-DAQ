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

#ifndef __WAVECONFIGPOPOUT_H
#define __WAVECONFIGPOPOUT_H

#include "gui.h"
class MainFrame;
class WaveConfigPopout {
    RQ_OBJECT("WaveConfigPopout");

private:
    TGTransientFrame *fMain;
    TGHorizontalFrame *f0;
    TGGroupFrame *fAdcOutput, *fChannels;
    TGHorizontalFrame *fHardware, *fFile, *fHardwareLabel;
    TGVerticalFrame *f2, *f1;
    TGNumberEntry *fNum1, *fNum2, *fNum3;
    TGCheckButton *fBHex, *fBHeader, *fBTest;
    TGComboBox *fCBHardware, *fCBFileType;
    TGLayoutHints *fLHardware;
    //Chan
    TGHorizontalFrame *f11, *f12, *f13, *f14;
    TGVerticalFrame *f15;
    TGTextButton *fBAddCh;
    TGNumberEntryField *fNEntryCh, *fNEntryOffset, *fNEntryChThreshold;
    TGComboBox *fCBChan, *fCBChTrig;
    TGCheckButton *fBEnabled;
    //Misc
    TGGroupFrame *f21;
    TGComboBox *fCBLogic, *fCBExternalTrig;
    TGTextEntry *fTEntryGnuPlot;
    TGHorizontalFrame *fRight[7];
    TGNumberEntryField *fNEntryRight[4];

    TGTextButton *bOk, *bCancel;
    TGHorizontalFrame *fButtons;
    WavedumpConfig config;
    TString path;
#ifndef __CINT__
    const char *fLabelRight[7] = {
    "GNU Plot path",
    "External trigger",
    "Logic",
    "Record length",
    "Max events in buffer",
    "Event post trigger",
    "Inturrupt"};
#endif
    
public:
    WaveConfigPopout(const TGWindow *p, const TGWindow *main, MainFrame *mainFrame);
    virtual ~WaveConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    void LoadConfig();
    void WriteConfig();

};
#endif
