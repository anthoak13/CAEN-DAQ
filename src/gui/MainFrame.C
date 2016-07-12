//Implimentation of MainFrame
#include "MainFrame.h"
#include "ConfigPopout.h"
#include "DAQPopout.h"
#include "WaveConfigPopout.h"
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
