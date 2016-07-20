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

#include "ConfigPopout.h"
#include "../DataProcessor.h"
#include "../SignalProcessor.h"
#include <iostream>


//ConfigPopout implementaion********************
ConfigPopout::ConfigPopout(const TGWindow *p, const TGWindow *main, DataProcessor *dataP)
{
    signalP = dataP->getSignalP();
    //create main framw
    fMain = new TGTransientFrame(p, main, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "ConfigPopout", this, "CloseWindow()");
    fMain->DontCallClose();

    //Transient frame has a vertical frame by default (b/c default options)
    //Create 2 verical frames for groups and buttons
    f1 = new TGCompositeFrame(fMain, 60, 20, kHorizontalFrame);
    f2 = new TGCompositeFrame(fMain, 60, 20, kHorizontalFrame);

    //Add options and buttons
    TGLayoutHints *mainHints =  new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 15, 0);
    fMain->AddFrame(f1, mainHints);
    fMain->AddFrame(f2, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 0, 5));


    //Create the buttons
    bOK = new TGTextButton(f2, "&Accept");
    bOK->Connect("Clicked()", "ConfigPopout", this, "DoOk()");
    bCancel = new TGTextButton(f2, "&Cancel");
    bCancel->Connect("Clicked()", "ConfigPopout", this, "DoCancel()");
    TGLayoutHints *fLButton = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 0, 5);
    f2->AddFrame(bOK, fLButton);
    f2->AddFrame(bCancel, fLButton);    
    

    //Create group frames
    fTrap = new TGGroupFrame(f1, "Trap Filter", kVerticalFrame);
    fZero = new TGGroupFrame(f1, "Zero Filter", kVerticalFrame);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 5, 0);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);
    f1->AddFrame(fTrap, fL1);
    f1->AddFrame(fZero, fL1);

    for(int i = 0; i < 4; i++)
    {
	//Create frames and add to group frame
	fNumTrap[i] = new TGHorizontalFrame(fTrap, 60, 20);
	fNumZero[i] = new TGHorizontalFrame(fZero, 60, 20);
	fTrap->AddFrame(fNumTrap[i], fL2);
	fZero->AddFrame(fNumZero[i], fL2);

	//Add num entries //TODO::Load in default values with current values from DataProcessor
	Double_t default1, default2;
	switch(i) {
	case 0:
		default1 = signalP->getDecayTime();
	        default2 = signalP->getOffset();
		break;
	case 1:
		default1 = signalP->getFlatMult();
	        default2 = signalP->getScaling();
		break;
	case 2:
		default1 = signalP->getM();
	        default2 = signalP->getThreshold();
		break;
	case 3:
	    default1 = signalP->getPeakThreshold();
	    default2 = signalP->getInterpMult();
	    break;
	}
	fEntryTrap[i] = new TGNumberEntry(fNumTrap[i], default1);
	fEntryZero[i] = new TGNumberEntry(fNumZero[i], default2);
	fNumTrap[i]->AddFrame(fEntryTrap[i], fL2);
	fNumZero[i]->AddFrame(fEntryZero[i], fL2);

	//Add labels
	fNumTrap[i]->AddFrame( new TGLabel(fNumTrap[i], fLabel[i]), fL2);
	fNumZero[i]->AddFrame( new TGLabel(fNumZero[i], fLabel[i+4]), fL2);
    }

    fMain->SetWindowName("Config");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultWidth(), fMain->GetDefaultHeight());
    fMain->MapWindow();

    
}

ConfigPopout::~ConfigPopout()
{
    fMain->DeleteWindow();
}


void ConfigPopout::CloseWindow() { delete this; }
void ConfigPopout::DoOk()
{
    std::cout << "Saving changes to config" << std::endl;
    
    //update signal processor with new info
    delete signalP;
    signalP = new SignalProcessor(fEntryTrap[0]->GetNumber(), fEntryTrap[2]->GetNumber(),
				  fEntryTrap[1]->GetNumber(), fEntryTrap[3]->GetNumber(),
				  fEntryZero[0]->GetNumber(), fEntryZero[2]->GetNumber(),
				  fEntryZero[3]->GetNumber(), fEntryZero[1]->GetNumber());

    std::cout << "Updated config" << std::endl;
    CloseWindow();
}
void ConfigPopout::DoCancel()
{
    CloseWindow();
}
