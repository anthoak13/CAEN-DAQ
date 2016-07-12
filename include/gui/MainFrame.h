#ifndef __MAINFRAME_H
#define __MAINFRAME_H

#ifndef __CINT__
#include "gui.h"
#endif
class DataProcessor;
//Definition of MainFrame
class MainFrame {

    RQ_OBJECT("MainFrame");

private:
    DataProcessor *dataP;
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
    TGComboBox *fSelectHist[5];
    TGCheckButton *fDrawSelect[5];
    TRootEmbeddedCanvas *fEmbedded, *fEmbedded2;
    TGComboBox *fComboDisp;
    TGLabel *fLabelDisp, *fLabelCh2;
    TGTextButton *bDrawMult, *bGenerate, *bConfig2;
    TGLabel       *fLabelBin, *fLabelXMin, *fLabelXMax;
    TGNumberEntry *fEntryBin, *fEntryXMin, *fEntryXMax;
    TGHorizontalFrame *f15, *f16, *f17;

    TGMenuBar *fMenuBar;
    TGPopupMenu *fMenuFile, *fMenuAcq;
    TGLayoutHints *fLMenuBar, *fLMenuBarItem;
    
    TString inputTemplate, meta;
    UInt_t numCh, headerLength;

    
    
    const char *drawLabel[5] = {
    "signal",
    "trap",
    "deriv",
    "cfd",
    "raw" };

    enum EMenIds {
    M_ACQ_START,
    M_ACQ_CONFIG,

    M_FILE_OPEN,
    M_FILE_LINK,
    M_FILE_EXIT
};

    
public:
    MainFrame(const TGWindow *,UInt_t , UInt_t);
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
};

#endif
