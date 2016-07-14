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


#include "ECalibrationPopout.h"
#include "MainFrame.h"
#include <iostream>

ECalibrationPopout::ECalibrationPopout(const TGWindow *p, const TGWindow *w, MainFrame *main)
{
    mainFrame = main;
    fMain = new TGTransientFrame(p, w, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "DAQPopout", this, "CloseWindow()");
    fMain->DontCallClose();

    TGLayoutHints *fLHintLabel = new TGLayoutHints(kLHintsNormal, 2, 65, 2, 2);
    TGLayoutHints *fLHintEntry = new TGLayoutHints(kLHintsNormal, 2, 3, 2, 2);

    //Create Frame for label
    fFrameLabel = new TGHorizontalFrame(fMain);
    fFrameLabel->AddFrame(new TGLabel(fFrameLabel, "Raw"), fLHintLabel);
    fFrameLabel->AddFrame(new TGLabel(fFrameLabel, "Calibrated"), fLHintEntry);
    fMain->AddFrame(fFrameLabel);
    
    //Create frames for data
    for(int i = 0; i < 2; i++)
    {
	fFrameNumber[i] = new TGHorizontalFrame(fMain);
	fNEntryRaw[i] = new TGNumberEntryField(fFrameNumber[i]);
	fNEntryRaw[i]->Resize(80, 20);
	fNEntryCalibrated[i] = new TGNumberEntryField(fFrameNumber[i]);
	fNEntryCalibrated[i]->Resize(80, 20);
	fFrameNumber[i]->AddFrame(fNEntryRaw[i], fLHintEntry);
	fFrameNumber[i]->AddFrame(fNEntryCalibrated[i], fLHintEntry);
	fMain->AddFrame(fFrameNumber[i]);
    }

    //Create frame for buttons
    fFrameButtons = new TGHorizontalFrame(fMain);
    fFrameButtons->AddFrame(fBOk = new TGTextButton(fFrameButtons, "&Accept"),
			    new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2));
    fFrameButtons->AddFrame(fBCancel = new TGTextButton(fFrameButtons, "&Cancel"),
			    new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2));
    fBOk->Connect("Clicked()", "ECalibrationPopout", this, "DoOk()");
    fBCancel->Connect("Clicked()", "ECalibrationPopout", this, "DoCancel()");

    fMain->AddFrame(fFrameButtons, new TGLayoutHints(kLHintsTop |
						     kLHintsRight, 2, 2, 2, 2));

    //Resize ect
    fMain->SetWindowName("Calibration");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultWidth(), fMain->GetDefaultHeight());
    fMain->MapWindow();
}

ECalibrationPopout::~ECalibrationPopout()
{
    fMain->DeleteWindow();
}

void ECalibrationPopout::CloseWindow() { delete this; }

void ECalibrationPopout::DoOk()
{
    mainFrame->slope = (fNEntryCalibrated[0]->GetNumber() -
		   fNEntryCalibrated[1]->GetNumber())/
	(fNEntryRaw[0]->GetNumber() - fNEntryRaw[1]->GetNumber());
    mainFrame->intercept = - mainFrame->slope * fNEntryRaw[0]->GetNumber() +
	fNEntryCalibrated[0]->GetNumber();

    std::cout << mainFrame->slope << " " << mainFrame->intercept << std::endl;
    CloseWindow();
}

void ECalibrationPopout::DoCancel() { CloseWindow(); }
