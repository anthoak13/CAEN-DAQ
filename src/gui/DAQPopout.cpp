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

#include "DAQPopout.h"
#include <iostream>


DAQPopout::DAQPopout(const TGWindow *p, const TGWindow *main,  MainFrame *frame,
		     TString *tempIn, TString *metaIn, UInt_t *numChIn, UInt_t *headerLengthIn)
{

    inputTemplate = tempIn;
    meta = metaIn;
    numCh = numChIn;
    headerLength = headerLengthIn;
    
    //create main frame
    fMain = new TGTransientFrame(p, main, 10, 10, kHorizontalFrame);
    fMain->Connect("CloseWindow()", "DAQPopout", this, "CloseWindow()");
    fMain->DontCallClose();

    //Create layouthints
    TGLayoutHints *fLHintsMain = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
    TGLayoutHints *fLHintsText = new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 0, 2, 3);

    //Create frames
    f2 = new TGVerticalFrame(fMain);
    f4 = new TGVerticalFrame(fMain);
    fMain->AddFrame(f2, fLHintsMain);
    fMain->AddFrame(f4, fLHintsMain);

    //Create labels 
    fLabelIn = new TGLabel(f2, "Input file: ");
    fLabelMeta = new TGLabel(f2, "Meta file: ");
    fEntryIn = new TGTextEntry(f2, *inputTemplate);
    fEntryIn->SetWidth(100);
    fEntryMeta = new TGTextEntry(f2, *meta);
    fEntryMeta->SetWidth(100);
    f2->AddFrame(fLabelIn, fLHintsText);
    f2->AddFrame(fEntryIn, fLHintsText);
    f2->AddFrame(fLabelMeta, fLHintsText);
    f2->AddFrame(fEntryMeta, fLHintsText);


    //Add labels for chan/header
    fLabelCh = new TGLabel(f4, "Num Channels");
    fLabelHeader = new TGLabel(f4, "Header length");
    fEntryCh = new TGNumberEntry(f4, 32, 5, -1, TGNumberFormat::EStyle::kNESInteger,
				    TGNumberFormat::EAttribute::kNEANonNegative,
				    TGNumberFormat::ELimit::kNELLimitMin, 0);
    fEntryHeader = new TGNumberEntry(f4, 6, 5, -1, TGNumberFormat::EStyle::kNESInteger,
				    TGNumberFormat::EAttribute::kNEANonNegative,
				    TGNumberFormat::ELimit::kNELLimitMin, 0);
    f4->AddFrame(fLabelCh, fLHintsText);
    f4->AddFrame(fEntryCh, fLHintsText);
    f4->AddFrame(fLabelHeader, fLHintsText);
    f4->AddFrame(fEntryHeader, fLHintsText);
    
    //Add ok and cancel
    f3  = new TGHorizontalFrame(f4);
    f4->AddFrame(f3, new TGLayoutHints(kLHintsTop | kLHintsRight, 3, 0, 1, 1));
    f3->AddFrame(fBOk = new TGTextButton(f3, "&Accept"), fLHintsMain);
    f3->AddFrame(fBCancel = new TGTextButton(f3, "&Cancel"), fLHintsMain);
    fBOk->Connect("Clicked()", "DAQPopout", this, "DoOk()");
    fBOk->Connect("Clicked()", "MainFrame", frame, "DoAcquisitionClosed()");
    fBCancel->Connect("Clicked()", "DAQPopout", this, "DoCancel()");

    fMain->SetWindowName("Data Acquisition");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultWidth(), fMain->GetDefaultHeight());
    fMain->MapWindow();
}

DAQPopout::~DAQPopout()
{
    fMain->DeleteWindow();
}

void DAQPopout::CloseWindow() { delete this; }

void DAQPopout::DoOk()
{
    //save changes
    *inputTemplate = fEntryIn->GetText();
    *meta = fEntryMeta->GetText();
    *numCh = fEntryCh->GetNumber();
    *headerLength = fEntryHeader->GetNumber();
    CloseWindow();
}
void DAQPopout::DoCancel()
{
    std::cout << "Closing DAQ without updating" << std::endl;
    CloseWindow();
}
