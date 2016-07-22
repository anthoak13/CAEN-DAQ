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

#ifndef __ECALIBRATIONPOPOUT_H
#define __ECALIBRATIONPOPOUT_H

#include "gui.h"
#include <vector>

class MainFrame;

class ECalibrationPopout {
    RQ_OBJECT("ECalibrationPopout");
    
private:
    TGTransientFrame *fMain;
    TGHorizontalFrame *fFrameLabel, *fFrameButtons;
    TGHorizontalFrame *fFrameNumber[2];
    TGTextButton *fBOk, *fBCancel;
    TGNumberEntryField *fNEntryRaw[2], *fNEntryCalibrated[2];
    MainFrame *mainFrame;
    std::vector<Double_t> raw;
    std::vector<Double_t> calibrated;

    void loadFile();
    void writeFile();
    
public:
    ECalibrationPopout(const TGWindow*, const TGWindow*,  MainFrame*);
    virtual ~ECalibrationPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
};
#endif
