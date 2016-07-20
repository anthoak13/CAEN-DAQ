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

#include "MetaConfigPopout.h"
#include "../DataProcessor.h"

MetaConfigPopout::MetaConfigPopout(const TGWindow *p, const TGWindow *main, DataProcessor *dataPIn)
{
    dataP = dataPIn;
    fMain = new TGTransientFrame(p, main, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "MetaConfigPopout", this, "CloseWindow()");
    fMain->DontCallClose();


    //Create top frame and vertical frame for combo b0x
    fTop = new TGHorizontalFrame(fMain);
    f0 = new TGVerticalFrame(fTop);
    //Create button and selection box for top frame
    fCBChan = new TGComboBox(f0);
    fCBChan->Resize(60, 20);
    //Add everything to vertical frame
    f0->AddFrame(new TGLabel(f0, "Channel"), new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
    f0->AddFrame(fCBChan, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
    //Add everything to the top frame
    fTop->AddFrame(f0, new TGLayoutHints(kLHintsNormal, 2, 10, 0, 2));

    
    //Create Frames for the middle section
    fMiddle = new TGHorizontalFrame(fMain);
    fLabel = new TGVerticalFrame(fMiddle);
    fRight = new TGVerticalFrame(fMiddle);
    fMiddle->AddFrame(fLabel, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
    fMiddle->AddFrame(fRight, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));

    //Loop through and populate middle frames
    TGLayoutHints *fLabelHints = new TGLayoutHints(kLHintsNormal, 2, 2, 3, 7);
    TGLayoutHints *fNumHints   = new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2);
    for(int  i = 0; i < 5; i++)
    {
	fLabel->AddFrame(new TGLabel(fLabel, fLabelMeta[i]), fLabelHints);
	if( i == 0)
	{
	    fCBChType = new TGComboBox(fRight);
	    fCBChType->AddEntry("Disabled", 0);
	    fCBChType->AddEntry("Normal", 1);
	    fCBChType->AddEntry("TAC", 2);
	    fCBChType->Resize(75, 20);
	    fCBChType->Select(0);
	    fRight->AddFrame(fCBChType, fNumHints);
	}
	else
	{
	    fNEntryMeta[i-1] = new TGNumberEntryField(fRight, -1, 0, TGNumberFormat::EStyle::kNESInteger,
						      TGNumberFormat::EAttribute::kNEANonNegative);
	    fNEntryMeta[i-1]->Resize(75, fNEntryMeta[i-1]->GetDefaultHeight());
	    fRight->AddFrame(fNEntryMeta[i-1], fNumHints);
	}
    }


    //Create okay and cancel buttons
    fButtons = new TGHorizontalFrame(fMain);
    
    bOk = new TGTextButton(fButtons, "&Accept");
    bOk->Connect("Clicked()", "MetaConfigPopout", this, "DoOk()");
    TGLayoutHints *fLButton = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 0, 0, 2);
    fButtons->AddFrame(bOk, fLButton);

    fMain->AddFrame(fTop, new TGLayoutHints(kLHintsNormal, 5, 0, 0, 0));
    fMain->AddFrame(fMiddle, new TGLayoutHints(kLHintsNormal, 5, 0, 0, 0));
    fMain->AddFrame(fButtons, new TGLayoutHints(kLHintsTop | kLHintsRight, 0, 2, 5, 2));

    
    fMain->SetWindowName("Meta Config");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultWidth(), fMain->GetDefaultHeight());
    fMain->MapWindow();
    
    fCBChan->Connect("Selected(Int_t)", "MetaConfigPopout", this, "SelectChannel(Int_t)");

    _oldCh = -1;

    //Load in previous meta data
    for(int i = 0; i < dataP->getNumCh(); i++)
	fCBChan->AddEntry(TString(std::to_string(i)), i);

    
    fCBChan->Select(0);
}

MetaConfigPopout::~MetaConfigPopout()
{
    fMain->DeleteWindow();
}

void MetaConfigPopout::CloseWindow(){ delete this; }

void MetaConfigPopout::DoOk()
{
    //Update old channel
    if(_oldCh >= 0)
    {
	std::vector<Int_t> data;
	data.push_back(fCBChType->GetSelected());
	data.push_back(fNEntryMeta[0]->GetNumber());
	data.push_back(fNEntryMeta[1]->GetNumber());
	data.push_back(fNEntryMeta[2]->GetNumber());
	data.push_back(fNEntryMeta[3]->GetNumber());
	dataP->setMetaData(_oldCh, data);
    }
    CloseWindow();
}

void MetaConfigPopout::SelectChannel(Int_t chNum)
{
    //Update old channel
    if(_oldCh >= 0)
    {
	std::vector<Int_t> data;
	data.push_back(fCBChType->GetSelected());
	data.push_back(fNEntryMeta[0]->GetNumber());
	data.push_back(fNEntryMeta[1]->GetNumber());
	data.push_back(fNEntryMeta[2]->GetNumber());
	data.push_back(fNEntryMeta[3]->GetNumber());
	dataP->setMetaData(_oldCh, data);
    }

    _oldCh = chNum;

    //Load in values for new channel
    std::vector<Int_t> data;
    if(chNum < dataP->getNumCh())
	data = dataP->getMetaData(chNum);
    else
	data = {0, 0, 0, 0, 0};
    
    fCBChType->Select(data[0]);
    fNEntryMeta[0]->SetNumber(data[1]);
    fNEntryMeta[1]->SetNumber(data[2]);
    fNEntryMeta[2]->SetNumber(data[3]);
    fNEntryMeta[3]->SetNumber(data[4]);
}
