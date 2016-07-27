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
    
    //create main frame
    fMain = new TGTransientFrame(p, main, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "ConfigPopout", this, "CloseWindow()");
    fMain->DontCallClose();
    fMain->SetCleanup(kDeepCleanup);

    //Transient frame has a vertical frame by default (b/c default options)
    //Create 2 verical frames for groups and buttons
    f1 = new TGCompositeFrame(fMain, 60, 20, kHorizontalFrame);
    f11 = new TGCompositeFrame(fMain, 60, 20, kHorizontalFrame);
    f2 = new TGCompositeFrame(fMain, 60, 20, kHorizontalFrame);

    //Add options and buttons
    TGLayoutHints *mainHints =  new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 15, 0);
    fMain->AddFrame(f1, mainHints);
    fMain->AddFrame(f11, mainHints);
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
    fPileup = new TGGroupFrame(f11, "Pileup detection", kVerticalFrame);
    fPeak = new TGGroupFrame(f11, "Peak sampling", kVerticalFrame);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 5, 0);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);
    f1->AddFrame(fTrap, fL1);
    f1->AddFrame(fZero, fL1);
    f11->AddFrame(fPileup, fL1);
    f11->AddFrame(fPeak, fL1);

    auto getFrame = [=](int i) { switch(i/3) { case 0: return fTrap;
	                                       case 1: return fZero;
					       case 2: return fPileup;
	                                       case 3: return fPeak; };};
    for(int i = 0; i < numEntries; i++)
    {
	//Create frames and add to group frame
	fNumFrames[i] = new TGHorizontalFrame(getFrame(i), 60, 20);
	getFrame(i)->AddFrame(fNumFrames[i], fL2);

	//Add num entries //TODO::Load in default values with current values from DataProcessor
	Double_t defaultVal = 0;
	//determine default value
	switch(i) {
	case 0:
	    defaultVal = signalP->getRiseTime();
	    break;
	case 1:
	    defaultVal = signalP->getM();
	    break;
	case 2:
	    defaultVal = signalP->getFlatMult();
	    break;
	case 3:
	    defaultVal = signalP->getZeroOffset();
	    break;
	case 4:
	    defaultVal = signalP->getZeroThreshold();
	    break;
	case 5:
	    defaultVal = signalP->getInterpMult();
	    break;
	case 6:
	    defaultVal = signalP->getPileHigh();
	    break;
	case 7:
	    defaultVal = signalP->getPileLow();
	    break;
	case 8:
	    defaultVal = signalP->getPeakLength();
	    break;
	case 9:
	    defaultVal = signalP->getPeakDisplacement();
	    break;
	case 10:
	    defaultVal = signalP->getPointsToAverage();
	    break;
	}

	fEntry[i] = new TGNumberEntry(fNumFrames[i], defaultVal);
	fNumFrames[i]->AddFrame(fEntry[i], fL2);

	//Add labels
	fNumFrames[i]->AddFrame( new TGLabel(fNumFrames[i], fLabel[i]), fL2);
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
    signalP = new SignalProcessor(fEntry[0]->GetNumber(), fEntry[1]->GetNumber(),
				  fEntry[2]->GetNumber(), fEntry[3]->GetNumber(),
				  fEntry[4]->GetNumber(), fEntry[5]->GetNumber(),
				  fEntry[6]->GetNumber(), fEntry[7]->GetNumber(),
				  fEntry[8]->GetNumber(), fEntry[9]->GetNumber(),
				  fEntry[10]->GetNumber());



    std::cout << "Updated config" << std::endl;
    CloseWindow();
}
void ConfigPopout::DoCancel()
{
    CloseWindow();
}
