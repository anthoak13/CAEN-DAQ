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

#ifndef __MAINFRAME_H
#define __MAINFRAME_H


#include "gui.h"
#include <vector>

class DataProcessor;
class TTimer;

//Definition of MainFrame
class MainFrame {

    RQ_OBJECT("MainFrame");

private:
//constant declarations
    static const Int_t kHistNumber = 6;
#ifndef __CINT__
    const char *drawLabel[kHistNumber] = {
    "signal",
    "trap",
    "deriv",
    "cfd",
    "raw",
    "trap deriv"};
#endif
    
    TGMainFrame* fMain;
    TGCompositeFrame *fCTab1, *fCTab2;
    TGTextButton *bConfig, *bDraw;
    TGTab* fTab;
    TGNumberEntry *fEntryEvent, *fEntryHist, *fEntryCh, *fEntryCh2;
    TGLabel       *fLabelEvent, *fLabelHist, *fLabelCh;
    TGVerticalFrame *f1, *f11;
    TGHorizontalFrame *f2, *f3, *f4, *f5, *f12, *f13, *f14;
    TGLayoutHints *fLTab, *fHLabel, *fLDraw, *fLDrawButton;
    TGGroupFrame *fGDraw;
    TGVerticalFrame *fVertDraw, *fVertHist;
    TGComboBox *fSelectHist[kHistNumber];
    TGCheckButton *fDrawSelect[kHistNumber];
    TRootEmbeddedCanvas *fEmbedded, *fEmbedded2;
    TGComboBox *fComboDisp;
    TGLabel *fLabelDisp, *fLabelCh2;
    TGTextButton *bDrawMult, *bGenerate, *bConfig2;
    TGLabel       *fLabelBin, *fLabelXMin, *fLabelXMax;
    TGNumberEntry *fEntryBin, *fEntryXMin, *fEntryXMax;
    TGHorizontalFrame *f15, *f16, *f17;

    TGMenuBar *fMenuBar;
    TGPopupMenu *fMenuFile, *fMenuAcq, *fMenuSpectra;
    TGLayoutHints *fLMenuBar, *fLMenuBarItem;
    TGStatusBar *fStatusBar;

    
    TString inputTemplate, meta;
    UInt_t numCh, headerLength, interpMult;

    DataProcessor *dataP;
    std::vector< TH1F* > hist;
    TTimer *timer;

    enum EMenIds {
    M_ACQ_START,
    M_ACQ_CONFIG,

    M_FILE_OPEN,
    M_FILE_LINK,
    M_FILE_META,
    M_FILE_EXIT,

    M_SPECTRA_CALIB,
    M_SPECTRA_RESET
};

    
public:

    Double_t slope = 1;
    Double_t intercept = 0;
    
    MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MainFrame();
    
    void closeWindow();
    void DoConfig();
    void DoDraw();
    void welcome();
    void DoGenerate();
    void DoDrawMult();
    void UpdateDataProcessor(TString, TString, UInt_t, UInt_t);
    void DoAcquisitionClosed();
    void HandleMenu(Int_t id);
    void UpdateBoardInfo();
};

#endif
