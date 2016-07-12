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
