//File for gui, macro should be interpreted
//Adam Anthony 

#include "TGClient.h"
#include "TCanvas.h"
#include "TGNumberEntry.h"
#include "TGWindow.h"
#include "TRootEmbeddedCanvas.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGMenu.h"
#include "TGFrame.h"
#include "TGTab.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TSystem.h"
#include "RQ_OBJECT.h"
#include "TH1.h"
#include "include/DataProcessor.h"
#include "include/WavedumpConfig.h"
#include "include/Digitizer.h"
#include <iostream>
#include <vector>
#include <exception>

const char *fLabel[] = {
    "Rise time",
    "Top",
    "M",
    "Offset",
    "Scaling",
    "Threshold"};

const char *fLabelRight[] = {
    "GNU Plot path",
    "External trigger",
    "Logic",
    "Record length",
    "Max events in buffer",
    "Event post trigger",
    "Inturrupt"};

const char *drawLabel[] = {
    "signal",
    "trap",
    "deriv",
    "cfd",
    "raw" };

enum ECommandIds {
    T_INPUT,
    T_SINGLE,
    T_MULTIPLE
};

enum EMenIds {
    M_ACQ_START,
    M_ACQ_CONFIG,

    M_FILE_OPEN,
    M_FILE_LINK,
    M_FILE_EXIT
};

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

//Definition of Config popout
class ConfigPopout {
    RQ_OBJECT("ConfigPopout");

private:
    TGTransientFrame *fMain;
    TGCompositeFrame *f1, *f2;
    TGGroupFrame *fTrap, *fZero;
    TGTextButton *bCancel, *bOK;
    TGHorizontalFrame *fNumTrap[3], *fNumZero[3];
    TGNumberEntry *fEntryTrap[3], *fEntryZero[3];
    SignalProcessor *signalP;
    
    
public:
    ConfigPopout(const TGWindow*, const TGWindow*, SignalProcessor*);
    virtual ~ConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    
};

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

class WaveConfigPopout {
    RQ_OBJECT("WaveConfigPopout");

private:
    TGTransientFrame *fMain;
    TGHorizontalFrame *f0;
    TGGroupFrame *fAdcOutput, *fChannels;
    TGHorizontalFrame *fHardware, *fFile, *fHardwareLabel;
    TGVerticalFrame *f2, *f1;
    TGNumberEntry *fNum1, *fNum2, *fNum3;
    TGCheckButton *fBHex, *fBHeader, *fBTest;
    TGComboBox *fCBHardware, *fCBFileType;
    TGLayoutHints *fLHardware;
    //Chan
    TGHorizontalFrame *f11, *f12, *f13, *f14;
    TGVerticalFrame *f15;
    TGTextButton *fBAddCh;
    TGNumberEntryField *fNEntryCh, *fNEntryOffset, *fNEntryChThreshold;
    TGComboBox *fCBChan, *fCBChTrig;
    TGCheckButton *fBEnabled;
    //Misc
    TGGroupFrame *f21;
    TGComboBox *fCBLogic, *fCBExternalTrig;
    TGTextEntry *fTEntryGnuPlot;
    TGHorizontalFrame *fRight[7];
    TGNumberEntryField *fNEntryRight[4];

    TGTextButton *bOk, *bCancel;
    TGHorizontalFrame *fButtons;
    WavedumpConfig config;
    TString path;

public:
    WaveConfigPopout(const TGWindow *p, const TGWindow *main, MainFrame *mainFrame);
    virtual ~WaveConfigPopout();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    void LoadConfig();
    void WriteConfig();

};
    

//Implimentation of MainFrame
MainFrame::MainFrame(const TGWindow* p, UInt_t w, UInt_t h)
{
    
    //Create data processor
    inputTemplate = "data_in%i";
    meta = "";
    numCh = 16;
    headerLength = 6;
    dataP = new DataProcessor("", "", 0, 0);
    fMain = new TGMainFrame(p, w, h);

    //Make sure window closes
    fMain->Connect("CloseWindow()", "MainFrame", this, "closeWindow()");

    //Create menu **********
    fLMenuBar = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
    fLMenuBarItem = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);

    fMenuFile = new TGPopupMenu(gClient->GetRoot());
    fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
    fMenuFile->AddEntry("&Relink", M_FILE_LINK);
    fMenuFile->AddSeparator();
    fMenuFile->AddEntry("&Exit", M_FILE_EXIT);

    fMenuAcq = new TGPopupMenu(gClient->GetRoot());
    fMenuAcq->AddEntry("&Start...", M_ACQ_START);
    fMenuAcq->AddEntry("&Config...", M_ACQ_CONFIG);

    fMenuFile->Connect("Activated(Int_t)", "MainFrame", this,
		       "HandleMenu(Int_t)");
    fMenuAcq->Connect("Activated(Int_t)", "MainFrame", this,
		       "HandleMenu(Int_t)");

    fMenuBar = new TGMenuBar(fMain, 1, 1, kHorizontalFrame);
    fMenuBar->AddPopup("&File", fMenuFile, fLMenuBarItem);
    fMenuBar->AddPopup("&Wavedump", fMenuAcq, fLMenuBarItem);
    fMain->AddFrame(fMenuBar, fLMenuBar);

    
    //Create a tab widget ***************
    fTab = new TGTab(fMain, 400, 200);
    fTab->Connect("Selected(Int_t)", "MainFrame", this, "DoTab(Int_t)");
    fLTab = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
    
    //Create composite frames for the three tabs
    //Add the first Tab
    TGCompositeFrame* tf = fTab->AddTab("Single Event");

    //Create a Horizontal Frame to hold everything 
    fCTab1 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);

    //Create frame that will be the left half
    f1 = new TGVerticalFrame(fCTab1, 60, 20);
    fCTab1->AddFrame(f1, fLTab);
    
    //Create frames fro event num and hist *******
    fHLabel = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 5, 2, 0);
    f2 = new TGHorizontalFrame(f1, 60, 20);
    f3 = new TGHorizontalFrame(f1, 60, 20);
    f5 = new TGHorizontalFrame(f1, 60, 20);
    fEntryEvent = new TGNumberEntry(f2, 0, 10, -1,  TGNumberFormat::kNESInteger,
				    TGNumberFormat::kNEANonNegative,
				    TGNumberFormat::kNELLimitMinMax,
				    0, dataP->getMaxEvents());
    fEntryHist = new TGNumberEntry(f3, 1, 10, -1,  TGNumberFormat::EStyle::kNESInteger,
				   TGNumberFormat::EAttribute::kNEANonNegative,
				   TGNumberFormat::ELimit::kNELLimitMinMax,
				   0, 5);
    fEntryCh = new TGNumberEntry(f5, 0, 10, -1,  TGNumberFormat::EStyle::kNESInteger,
				    TGNumberFormat::EAttribute::kNEANonNegative,
				    TGNumberFormat::ELimit::kNELLimitMinMax,
				    0, dataP->getNumCh());
    fLabelEvent = new TGLabel(f2, "Event #");
    fLabelHist = new TGLabel(f3, "# of Hists");
    fLabelCh = new TGLabel(f5, "Channel");
    f2->AddFrame(fEntryEvent, fHLabel);
    f2->AddFrame(fLabelEvent, fHLabel);
    f3->AddFrame(fEntryHist, fHLabel);
    f3->AddFrame(fLabelHist, fHLabel);
    f5->AddFrame(fEntryCh, fHLabel);
    f5->AddFrame(fLabelCh, fHLabel);
    f1->AddFrame(f2, fHLabel);
    f1->AddFrame(f5, fHLabel);
    f1->AddFrame(f3, fHLabel);


    //Create Frames to select what to draw and on what hist ******
    fGDraw = new TGGroupFrame(f1, "Draw", kHorizontalFrame);
    f1->AddFrame(fGDraw, fLTab);
    fLDraw = new TGLayoutHints(kLHintsNormal, 1, 1, 1, 3);
    fLDrawButton = new TGLayoutHints(kLHintsNormal, 1, 1, 2, 5);
    fVertDraw = new TGVerticalFrame(fGDraw);
    fVertHist = new TGVerticalFrame(fGDraw);
    fGDraw->AddFrame(fVertDraw, fLDraw);
    fGDraw->AddFrame(fVertHist, fLDraw);
    fVertDraw->AddFrame(new TGLabel(fVertDraw, ""));
    fVertHist->AddFrame(new TGLabel(fVertHist, "hist #"));
    for(int i = 0; i < 5; i++)
    {
	fDrawSelect[i] = new TGCheckButton(fVertDraw, drawLabel[i]);
	fSelectHist[i] = new TGComboBox(fVertHist);
	for(int j = 1; j <= fEntryHist->GetNumber(); j++)
	{
	    fSelectHist[i]->AddEntry(Form("%i", j), j);
	}
	fSelectHist[i]->Resize(50, 20);
	fDrawSelect[i]->Resize(10, 40);
	fVertDraw->AddFrame(fDrawSelect[i], fLDrawButton);
	fVertHist->AddFrame(fSelectHist[i], fLDraw);
    }

    //Add config and redraw buttons
    f4 = new TGHorizontalFrame(f1, 60, 20);
    f1->AddFrame(f4, new TGLayoutHints(kLHintsTop | kLHintsRight, 2, 2, 0, 0));
    f4->AddFrame(bConfig = new TGTextButton(f4, "&Config"), fLDraw);
    f4->AddFrame(bDraw = new TGTextButton(f4, "&Draw"), fLDraw);
    bConfig->Connect("Clicked()", "MainFrame", this, "DoConfig()");
    bDraw->Connect("Clicked()", "MainFrame", this, "DoDraw()");

    //Add the embedded canvas
    fEmbedded = new TRootEmbeddedCanvas("c1", fCTab1, 600, 500);
    fCTab1->AddFrame(fEmbedded, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 0, 0, 0));


    tf->AddFrame(fCTab1, new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));


    //Create second tab *******************
    tf = fTab->AddTab("Mult Events");

    //Create horizontal tab for everything
    fCTab2 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);

    //Create frame that will be the left half
    f11 = new TGVerticalFrame(fCTab2, 60, 20);
    fCTab2->AddFrame(f11, new TGLayoutHints(kLHintsNormal, 5, 5, 5, 5));

    //Create frames for channel and display
    f12 = new TGHorizontalFrame(f11, 60, 20);
    f13 = new TGHorizontalFrame(f11, 60, 20);
    f11->AddFrame(f12, fHLabel);
    f11->AddFrame(f13, fHLabel);

    //Create channel drop menu
    fLabelCh2 = new TGLabel(f12, "Channel");
    fEntryCh2 = new TGNumberEntry(f12, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
				    TGNumberFormat::EAttribute::kNEANonNegative,
				    TGNumberFormat::ELimit::kNELLimitMinMax,
				    0, dataP->getNumCh());
    f12->AddFrame(fEntryCh2, fHLabel);
    f12->AddFrame(fLabelCh2, new TGLayoutHints(kLHintsNormal, 0, 38, 2, 0));

    //Create display drop menu
    fLabelDisp = new TGLabel(f13, "Display");
    fComboDisp = new TGComboBox(f13);
    fComboDisp->AddEntry("Q", 0);
    fComboDisp->AddEntry("Zero", 1);
    fComboDisp->AddEntry("Baseline", 2);
    f13->AddFrame(fComboDisp, fHLabel);
    f13->AddFrame(fLabelDisp, fHLabel);
    fComboDisp->Resize(70, 20);

    //Add options for drawing hist
    f15 = new TGHorizontalFrame(f11, 60, 20);
    f16 = new TGHorizontalFrame(f11, 60, 20);
    f17 = new TGHorizontalFrame(f11, 60, 20);
    f11->AddFrame(f15, fHLabel);
    f11->AddFrame(f16, fHLabel);
    f11->AddFrame(f17, fHLabel);

    fLabelBin = new TGLabel(f15, "# bins");
    fLabelXMin = new TGLabel(f16, "x min");
    fLabelXMax = new TGLabel(f17, "x max");
    fEntryBin = new TGNumberEntry(f15, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
				  TGNumberFormat::EAttribute::kNEANonNegative);
    fEntryXMin = new TGNumberEntry(f16, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
				  TGNumberFormat::EAttribute::kNEANonNegative);
    fEntryXMax = new TGNumberEntry(f17, 0, 8, -1,  TGNumberFormat::EStyle::kNESInteger,
				  TGNumberFormat::EAttribute::kNEANonNegative);
    f15->AddFrame(fEntryBin, fHLabel);
    f15->AddFrame(fLabelBin, fHLabel);
    f16->AddFrame(fEntryXMin, fHLabel);
    f16->AddFrame(fLabelXMin, fHLabel);
    f17->AddFrame(fEntryXMax, fHLabel);
    f17->AddFrame(fLabelXMax, fHLabel);

    //Create buttons to draw and generate tree
    f14 = new TGHorizontalFrame(f11, 60, 20);
    f11->AddFrame(f14, new TGLayoutHints(kLHintsTop | kLHintsRight, 2, 2, 10, 0));
    f14->AddFrame(bConfig2 = new TGTextButton(f14, "&Config"), fLDraw);
    f14->AddFrame(bGenerate = new TGTextButton(f14, "&Generate"), fLDraw);
    f14->AddFrame(bDrawMult = new TGTextButton(f14, "&Draw"), fLDraw);
    bGenerate->Connect("Clicked()", "MainFrame", this, "DoGenerate()");
    bDrawMult->Connect("Clicked()", "MainFrame", this, "DoDrawMult()");
    bConfig2->Connect("Clicked()", "MainFrame", this, "DoConfig()");

    fComboDisp->Select(0, false);

    //Create second canvas
    //Add the embedded canvas
    fEmbedded2 = new TRootEmbeddedCanvas("c2", fCTab2, 600, 500);
    fCTab2->AddFrame(fEmbedded2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    //Add Tab...
    tf->AddFrame(fCTab2, new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
    
    //Add Tabs to main frame
    fMain->AddFrame(fTab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX |
					    kLHintsExpandY, 2, 2, 5, 1));

    fMain->SetWindowName("Data Processor");
    fMain->MapSubwindows();
    fMain->Resize(fMain->GetDefaultSize());
    fMain->MapWindow();

    welcome();
}

MainFrame::~MainFrame()
{
    fMain->Cleanup();
    delete dataP;
    delete fMain;
}

void MainFrame::closeWindow()
{
    std::cout << "Closing GUI" << std::endl;
    std::cout << "Event num: " << fEntryEvent->GetNumber() << std::endl;
    gApplication->Terminate();
}

void MainFrame::welcome()
{
    std::cout << "Welcome to Data Processor. GUI is ready for use."
	      << std:: endl;
}

void MainFrame::DoConfig()
{
    std::cout << "Config pressed" << std::endl;
    new ConfigPopout(gClient->GetRoot(), fMain, dataP->getSignalP());
}

void MainFrame::DoDraw()
{
    TCanvas *canv = new TCanvas("canv", 600, 500, fEmbedded->GetCanvasWindowId());
    canv->cd();
    
    //Process the event
    dataP->processEvent(fEntryCh->GetNumber(), fEntryEvent->GetNumber());

    std::vector< TH1F* > hist;
    //Loop through the options and create a canvas with all of the checked boxes
    for(int i = 0; i < 5; i++)
    {
	if (fDrawSelect[i]->GetState() == kButtonDown)
	{
	    std::vector<int> output;
	    switch(i)
	    {
	    case 0:
		output = dataP->getSignal();
		hist.push_back(new TH1F("h1", "Signal", output.size(), 0, output.size()));
		break;
	    case 1:
		output = dataP->getTrap();
		hist.push_back( new TH1F("h2", "Trap", output.size(), 0, output.size()));
		break;
	    case 2:
		output = dataP->getDeriv();
		hist.push_back(new TH1F("h3", "Deriv", output.size(), 0, output.size()));
		break;
	    case 3:
		output = dataP->getCFD();
		hist.push_back(new TH1F("h4", "CFD", output.size(), 0, output.size()));
		break;
	    default:
		break;
	    }
	    

	    //Populate the histogram
	    if(i < 4)
	    {
		for(int j = 0; j < output.size(); j++)
		    hist.back()->Fill(j, output[j]);
		
		//Draw hist
		hist.back()->SetLineColor(i+1);
		hist.back()->SetLineWidth(2);
		hist.back()->Draw("hist same");
	    }
	}
    }
    
    canv->Update();
    fEmbedded->AdoptCanvas(canv);
}

void MainFrame::DoDrawMult()
{
    TCanvas *canv2 = new TCanvas("canv2", 600, 500, fEmbedded2->GetCanvasWindowId());
    canv2->cd();
    
    std::cout << "Drawing" << std::endl;
    TH1F *h1;
    TFile *file = new TFile("macro.root", "read");
    TTree *tree =  NULL;
    file->GetObject("macro_tree", tree);

    if(tree == NULL)
    {
	std::cout << "Tree not found" << std::endl;
	return;
    }
    UInt_t numEntries =  tree->GetEntries();
    Float_t temp;

    switch(fComboDisp->GetSelected())
    {
    case 0:
	tree->SetBranchAddress(Form("Q_%i", (int)fEntryCh2->GetNumber()), &temp);
	break;
    case 1:
	tree->SetBranchAddress(Form("zero_%i", (int)fEntryCh2->GetNumber()), &temp);
	break;
    case 2:
	tree->SetBranchAddress(Form("baseline_%i", (int)fEntryCh2->GetNumber()), &temp);
	break;
    }
    
    //create hist and fill it
    h1 = new TH1F("htemp", "", fEntryBin->GetNumber(),
		  fEntryXMin->GetNumber(),
		  fEntryXMax->GetNumber());
    
    for(int i = 0; i < numEntries; i++)
    {
	tree->GetEntry(i);
	h1->Fill(temp);
    }

    //draw hist
    h1->Draw();
    canv2->Update();
    fEmbedded2->AdoptCanvas(canv2);
}

void MainFrame::DoGenerate()
{
    std::cout << " Generating new tree..." << std::endl;
    dataP->processFiles(true);
}

void MainFrame::DoAcquisitionClosed()
{
    std::cout << "updating DAQ info" << std::endl;
    
    UpdateDataProcessor(inputTemplate, meta, numCh, headerLength);

    std::cout << "DAQ updated" << std::endl;
}

void MainFrame::UpdateDataProcessor(TString templateIn, TString meta, UInt_t numCh, UInt_t headerLength)
{
    delete dataP;
    try {
	dataP = new DataProcessor(templateIn, meta, numCh, headerLength);
    } catch(std::exception &e) {
	std::cout << e.what() << std::endl;
	dataP = new DataProcessor("", "", 0, 0);
    }
	

    //Need to update limits on entry event/ch
    if(dataP->getNumCh() == 0)
    {
	fEntryCh->SetLimits(TGNumberFormat::ELimit::kNELLimitMinMax,
			    0, 0);
	fEntryCh2->SetLimits(TGNumberFormat::ELimit::kNELLimitMinMax,
			    0, 0);
    }
    else
    {
	fEntryCh->SetLimits(TGNumberFormat::ELimit::kNELLimitMinMax,
			    0, dataP->getNumCh()-1);
	fEntryCh2->SetLimits(TGNumberFormat::ELimit::kNELLimitMinMax,
			    0, dataP->getNumCh()-1);
    }
    fEntryEvent->SetLimits(TGNumberFormat::ELimit::kNELLimitMinMax,
				    0, dataP->getMaxEvents());
    

}

void MainFrame::HandleMenu(Int_t id)
{
    std::cout << id << std::endl;
    switch(id) {
    case M_FILE_OPEN:
	std::cout << " Opening DAQ window" << std::endl;
	new DAQPopout(gClient->GetRoot(), fMain, this, &inputTemplate, &meta, &numCh, &headerLength);
	break;
    case M_FILE_LINK:
	gROOT->ProcessLine(".! . ./link.sh");
	break;
    case M_FILE_EXIT:
	closeWindow();
	break;
    case M_ACQ_START:
	gROOT->ProcessLine(".! gnome-terminal -x wavedump WaveDumpConfig.txt");
	break;
    case M_ACQ_CONFIG:
	new WaveConfigPopout(gClient->GetRoot(), fMain, this);
	break;
    }
	
}
    

//ConfigPopout implementaion********************
ConfigPopout::ConfigPopout(const TGWindow *p, const TGWindow *main, SignalProcessor *sigP)
{
    signalP = sigP;
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

    for(int i = 0; i < 3; i++)
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
	    
	}
	fEntryTrap[i] = new TGNumberEntry(fNumTrap[i], default1);
	fEntryZero[i] = new TGNumberEntry(fNumZero[i], default2);
	fNumTrap[i]->AddFrame(fEntryTrap[i], fL2);
	fNumZero[i]->AddFrame(fEntryZero[i], fL2);

	//Add labels
	fNumTrap[i]->AddFrame( new TGLabel(fNumTrap[i], fLabel[i]), fL2);
	fNumZero[i]->AddFrame( new TGLabel(fNumZero[i], fLabel[i+3]), fL2);
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
    //update shit
    signalP->setDecayTime(fEntryTrap[0]->GetNumber());
    signalP->setFlatMult(fEntryTrap[1]->GetNumber());
    signalP->setM(fEntryTrap[2]->GetNumber());
    signalP->setOffset(fEntryZero[0]->GetNumber());
    signalP->setScaling(fEntryZero[1]->GetNumber());
    signalP->setThreshold(fEntryZero[2]->GetNumber());
    std::cout << "Updated config" << std::endl;
    CloseWindow();
}
void ConfigPopout::DoCancel()
{
    CloseWindow();
}

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

WaveConfigPopout::WaveConfigPopout(const TGWindow *p, const TGWindow *main, MainFrame *mainFrame)
{
    fMain = new TGTransientFrame(p, main, 10, 10, kVerticalFrame);
    fMain->Connect("CloseWindow()", "WaveConfigPopout", this, "CloseWindow()");
    fMain->DontCallClose();

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
    fCBHardware->AddEntry("USB", 0);
    fCBHardware->AddEntry("PCI", 1);
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
    fCBFileType->AddEntry("Binary", 0);
    fCBFileType->AddEntry("ASCII", 1);
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
    fCBLogic->AddEntry("NIM", 0);
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
    bOk->Connect("Clicked()", "ConfigPopout", this, "DoOk()");
    bCancel = new TGTextButton(fButtons, "&Cancel");
    bCancel->Connect("Clicked()", "ConfigPopout", this, "DoCancel()");
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
}

WaveConfigPopout::~WaveConfigPopout()
{
    fMain->DeleteWindow();
}

void WaveConfigPopout::CloseWindow(){ delete this; }

void WaveConfigPopout::DoOk() { CloseWindow(); }

void WaveConfigPopout::DoCancel() { CloseWindow(); }

void WaveConfigPopout::LoadConfig() { }
void WaveConfigPopout::WriteConfig() { }


void gui()
{
    gSystem->Load("libMathMore.so");
    gSystem->Load("lib/libCAENDigitizer.so");

    //    testDigitizer();
    new MainFrame(gClient->GetRoot(), 400, 220);
}

//Main Program *****************
#ifdef stand
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   gui();

   theApp.Run();

   return 0;
}
#endif
