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

#include "WaveConfigPopout.h"

WaveConfigPopout::WaveConfigPopout(const TGWindow *p, const TGWindow *main, MainFrame *mainFrame)
{
    fMain = new TGTransientFrame(p, main, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "WaveConfigPopout", this, "CloseWindow()");
    fMain->DontCallClose();
    fMain->SetCleanup(kDeepCleanup);
    
    //Layout hints
    fLHardware = new TGLayoutHints(kLHintsNormal, 2,2,3,0);
    f0 = new TGHorizontalFrame(fMain);
    f1 = new TGVerticalFrame(f0);
    fAdcOutput = new TGGroupFrame(f1, "ADC Output", kVerticalFrame);
    fHardware = new TGHorizontalFrame(fAdcOutput);
    fFile = new TGHorizontalFrame(fAdcOutput);
    fHardwareLabel = new TGHorizontalFrame(fAdcOutput);

    //Create USB/PCI things
    fCBHardware = new TGComboBox(fHardware);
    fCBHardware->AddEntry("USB", 1);
    fCBHardware->AddEntry("PCI", 0);
    fCBHardware->Select(0, false);
    fCBHardware->Resize(45, 20);
    fNum1 =  new TGNumberEntry(fHardware, 0, 2, -1,  TGNumberFormat::EStyle::kNESInteger,
			       TGNumberFormat::EAttribute::kNEANonNegative);
    fNum2 =  new TGNumberEntry(fHardware, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
			       TGNumberFormat::EAttribute::kNEANonNegative);
    fNum3 =  new TGNumberEntry(fHardware, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
			       TGNumberFormat::EAttribute::kNEANonNegative);
    
    //Add things to hardware
    fHardwareLabel->AddFrame(new TGLabel(fHardwareLabel, "Hardware: "),
			     new TGLayoutHints(kLHintsNormal, 0, 111, 0, 0 ));
    fHardwareLabel->AddFrame(fBHex = new TGCheckButton(fHardwareLabel, "hex"),
			     fLHardware);
    fHardware->AddFrame(fCBHardware, fLHardware);
    fHardware->AddFrame(fNum1, fLHardware);
    fHardware->AddFrame(fNum2, fLHardware);
    fHardware->AddFrame(fNum3, fLHardware);

    //Add file stuffs
    TGLayoutHints *file1, *file2;
    file1 = new TGLayoutHints(kLHintsNormal, 2, 2, 7, 0); 
    file2 = new TGLayoutHints(kLHintsNormal, 2, 2, 0, 0);
    
    fCBFileType  = new TGComboBox(fFile);
    fCBFileType->AddEntry("Binary", 1);
    fCBFileType->AddEntry("ASCII", 0);
    fCBFileType->Select(0, false);
    fCBFileType->Resize(55, 20);
    //buttons for header, etc
    f2 = new TGVerticalFrame(fFile);
    fBHeader = new TGCheckButton(f2, "Header");
    fBTest   = new TGCheckButton(f2, "Test signal");
    f2->AddFrame(fBHeader, file2);
    f2->AddFrame(fBTest, file2);

    //Add things to hardware
    fFile->AddFrame(fCBFileType, file1);
    fFile->AddFrame(f2, file2);
    
	
    //Adde everything to group frame
    fAdcOutput->AddFrame(fHardwareLabel, fLHardware);
    fAdcOutput->AddFrame(fHardware, fLHardware);
    fAdcOutput->AddFrame(new TGLabel(fAdcOutput, "File: "), fLHardware);
    fAdcOutput->AddFrame(fFile, fLHardware);

    //************* create Channel thing
    fChannels = new TGGroupFrame(f1, "Channels", kVerticalFrame);
    f11 = new TGHorizontalFrame(fChannels);
    fBAddCh = new TGTextButton(f11, "Add Ch:");

    fNEntryCh = new TGNumberEntryField(f11, -1, 0, TGNumberFormat::EStyle::kNESInteger,
			       TGNumberFormat::EAttribute::kNEANonNegative);
    fNEntryCh->Resize(20, fNEntryCh->GetDefaultHeight());
    f15  = new TGVerticalFrame(f11);
    fCBChan = new TGComboBox(f15);
    fCBChan->AddEntry("Global", 0);
    fCBChan->Resize(60, 20);
    f15->AddFrame(new TGLabel(f15, "Selected"), new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
    f15->AddFrame(fCBChan, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
    f11->AddFrame(f15, new TGLayoutHints(kLHintsNormal, 2, 10, 0, 2));
    f11->AddFrame(fBAddCh, new TGLayoutHints(kLHintsNormal, 2, 0, 13, 2));
    f11->AddFrame(fNEntryCh, new TGLayoutHints(kLHintsNormal, 0, 2, 13, 2));
    f12 = new TGHorizontalFrame(fChannels);
    f13 = new TGHorizontalFrame(fChannels);
    f14 = new TGHorizontalFrame(fChannels);
    TGLayoutHints *fLHintsCh = new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2);
    //Add all of the channel things
    fNEntryOffset = new TGNumberEntryField(f12, -1, 0, TGNumberFormat::EStyle::kNESReal,
					   TGNumberFormat::EAttribute::kNEAAnyNumber,
					   TGNumberFormat::ELimit::kNELLimitMinMax, -50, 50);
    fNEntryOffset->Resize(50, fNEntryOffset->GetDefaultHeight());
    f12->AddFrame(fNEntryOffset, fLHardware);
    f12->AddFrame(new TGLabel(f12, "DC Offset"), fLHardware);
    fNEntryChThreshold = new TGNumberEntryField(f13, -1, 0, TGNumberFormat::EStyle::kNESInteger,
					   TGNumberFormat::EAttribute::kNEANonNegative,
					   TGNumberFormat::ELimit::kNELLimitMinMax, 0, 65535);
    fNEntryChThreshold->Resize(50, fNEntryChThreshold->GetDefaultHeight());
    f13->AddFrame(fNEntryChThreshold, fLHardware);
    f13->AddFrame(new TGLabel(f13, "Trigger Threshold"), fLHardware);

    fCBChTrig = new TGComboBox(f14);
    fCBChTrig->AddEntry("DISABLED", 0);
    fCBChTrig->AddEntry("ACQUISITION", 1);
    fCBChTrig->AddEntry("ACQ & TRGOUT", 2);
    fCBChTrig->Resize(100, 20);
    f14->AddFrame(fCBChTrig, fLHardware);
    f14->AddFrame( new TGLabel(f14, "Channel Trigger"), new TGLayoutHints(kLHintsNormal, 2,54,3,0));

    //Add everything to group frame
    fChannels->AddFrame(f11, fLHardware);
    fChannels->AddFrame(fBEnabled = new TGCheckButton(fChannels, "Enabled"), fLHardware);
    fChannels->AddFrame(f12, fLHardware);
    fChannels->AddFrame(f13, fLHardware);
    fChannels->AddFrame(f14, fLHardware);


    //Create vertical frame for right half
    f21 = new TGGroupFrame(f0, "Misc", kVerticalFrame);
    TGLayoutHints *labelHint = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 3, 5, 5);
    for(int i = 0; i < 7; i++)
    {
	fRight[i] = new TGHorizontalFrame(f21);
	fRight[i]->AddFrame(new TGLabel(fRight[i], fLabelRight[i]), labelHint);
	f21->AddFrame(fRight[i], fLHardware);
    }
    fRight[0]->AddFrame(fTEntryGnuPlot = new TGTextEntry(fRight[0], path), new TGLayoutHints(kLHintsNormal, 3, 0, 0, 0));
    fTEntryGnuPlot->SetWidth(150);
    
    fCBExternalTrig = new TGComboBox(fRight[1]);
    fCBExternalTrig->AddEntry("DISABLED", 0);
    fCBExternalTrig->AddEntry("ACQUISITION", 1);
    fCBExternalTrig->AddEntry("ACQ & TRGOUT", 2);
    fCBExternalTrig->Resize(100, 20);
    fRight[1]->AddFrame(fCBExternalTrig, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));

    fCBLogic = new TGComboBox(fRight[2]);
    fCBLogic->AddEntry("NIM", 1);
    fCBLogic->AddEntry("TTL", 0);
    fCBLogic->Resize(50, 20);
    fRight[2]->AddFrame(fCBLogic, new TGLayoutHints(kLHintsNormal, 45, 0, 0, 0));

    for(int i = 3; i < 7; i++)
    {
	fNEntryRight[i-3] = new TGNumberEntryField(fRight[i], -1, 0, TGNumberFormat::EStyle::kNESInteger,
					   TGNumberFormat::EAttribute::kNEANonNegative,
					   TGNumberFormat::ELimit::kNELLimitMinMax, 0, 1023);
	fRight[i]->AddFrame(fNEntryRight[i-3], fLHardware);
    }

    //Create okay and cancel buttons
    fButtons = new TGHorizontalFrame(fMain);
    
    bOk = new TGTextButton(fButtons, "&Accept");
    bOk->Connect("Clicked()", "WaveConfigPopout", this, "DoOk()");
    bCancel = new TGTextButton(fButtons, "&Cancel");
    bCancel->Connect("Clicked()", "WaveConfigPopout", this, "DoCancel()");
    TGLayoutHints *fLButton = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 0, 5);
    fButtons->AddFrame(bOk, fLButton);
    fButtons->AddFrame(bCancel, fLButton);
    
    
    fMain->AddFrame(f0, new TGLayoutHints(kLHintsNormal, 0,0,0,0));
    fMain->AddFrame(fButtons, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 0, 5));

    f1->AddFrame(fAdcOutput, fLHardware);
    f1->AddFrame(fChannels, fLHardware);
    
    f0->AddFrame(f1, fLHardware);
    f0->AddFrame(f21, fLHardware);    
    fMain->SetWindowName("Wavedump Config");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultWidth(), fMain->GetDefaultHeight());
    fMain->MapWindow();
    
    fBAddCh->Connect("Clicked()", "WaveConfigPopout", this, "AddChannel()");
    fCBChan->Connect("Selected(Int_t)", "WaveConfigPopout", this, "SelectChannel(Int_t)");
    LoadConfig();
}

WaveConfigPopout::~WaveConfigPopout()
{
    fMain->DeleteWindow();
}

void WaveConfigPopout::CloseWindow(){ delete this; }

void WaveConfigPopout::DoOk() { WriteConfig(); CloseWindow(); }

void WaveConfigPopout::DoCancel() { CloseWindow(); }

void WaveConfigPopout::LoadConfig()
{
    config.read();

    //setup misc
    fTEntryGnuPlot->SetText(config.getGnuPlotPath());
    fCBExternalTrig->Select(config.getExtTrig());
    fCBLogic->Select(config.getNIM());
    fNEntryRight[0]->SetNumber(config.getRecordLength());
    fNEntryRight[1]->SetNumber(config.getMaxEvents());
    fNEntryRight[2]->SetNumber(config.getPostTrig());
    fNEntryRight[3]->SetNumber(config.getInterrupt());

    //setup output
    fBHex->SetState((EButtonState)config.getHexLocation());
    fBHeader->SetState((EButtonState)config.getHeader());
    fBTest->SetState((EButtonState)config.getTestPattern());
    fCBHardware->Select(config.getUSB());
    fCBFileType->Select(config.getBinary());
    fNum1->SetNumber(config.getDigitizerLoc(0));
    fNum2->SetNumber(config.getDigitizerLoc(1));
    if(config.getUSB())
	fNum2->SetNumber(config.getDigitizerLoc(2));

    //setupChan
    fCBChan->RemoveAll();
    fCBChan->AddEntry("Global", 0);
    for(auto&& chan:config.getChConfigs())
	fCBChan->AddEntry(TString(std::to_string(chan.first)),
			  chan.first+1);

    fCBChan->Select(0, false);
    SetupChannel(0);
    oldCh = 0;
}

void WaveConfigPopout::SetupChannel(UInt_t chNum)
{
    ChannelConfig chan = (chNum == 0) ?
	config.getGlobalChan() : config.getChan(chNum-1);

    fBEnabled->SetState((EButtonState)chan.getEnabled());
    fNEntryOffset->SetNumber(chan.getDCOffset());
    fNEntryChThreshold->SetNumber(chan.getTrigThreshold());
    fCBChTrig->Select((UInt_t)chan.getChanTrig());
}

void WaveConfigPopout::SelectChannel(Int_t chNum)
{
    SaveChannel(oldCh);
    oldCh = chNum;
    SetupChannel(chNum);
}

void WaveConfigPopout::SaveChannel(UInt_t chNum)
{
    ChannelConfig chan(fBEnabled->GetState(),
		       fNEntryOffset->GetNumber(),
		       fNEntryChThreshold->GetNumber(),
		       (ChannelTrigger)fCBChTrig->GetSelected());
    if(chNum == 0)
	config.setGlobalChan(chan);
    else
	config.addChannel(chNum-1, chan);
}

void WaveConfigPopout::AddChannel()
{
    TString chanName =std::to_string((UInt_t)fNEntryCh->GetNumber());
    std::cout << "Adding... " << chanName << std::endl;
    if(!fCBChan->FindEntry(chanName))
	fCBChan->AddEntry(chanName,(UInt_t)fNEntryCh->GetNumber()+1);

    fCBChan->Select(fNEntryCh->GetNumber()+1, true);
}

void WaveConfigPopout::WriteConfig()
{
    config.setInput(fCBHardware->GetSelected(),
		    fNum1->GetNumber(),
		    fNum2->GetNumber(),
		    fBHex->GetState(),
		    fNum3->GetNumber());
    
    config.setGnuPlotPath(fTEntryGnuPlot->GetText());
    
    config.setOutput(fCBFileType->GetSelected(),
		     fBHeader->GetState(),
		     fBTest->GetState(),
		     fNEntryRight[0]->GetNumber());
    
    config.setConfig((ChannelTrigger)fCBExternalTrig->GetSelected(),
		     fNEntryRight[1]->GetNumber(),
		     fNEntryRight[2]->GetNumber(),
		     true,
		     fNEntryRight[3]->GetNumber(),
		     fCBLogic->GetSelected());

    config.write();
}
