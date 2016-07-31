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

//script for trying PSD
#include "TApplication.h"
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "DataProcessor.h"
#include "SignalProcessor.h"
#include <fstream>
#include <iostream>

void WalkData()
{
    const TString dir = "/home/adam/data/CoHPGe2/";
    //Create DataP
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();

    UInt_t riseTime = 3700;
    Int_t peakDisplacement = 0;
    std::ofstream outFile;
    outFile.open(TString(dir + "Efficiency.txt"));
    if(!outFile.is_open())
	return;
    //loop though and create trees of data
    bool test = true;
    while(riseTime <= 3800 && peakDisplacement <=100 && test)
    {
	TString fileName = Form(dir + "tree_%i_%i.root", riseTime, peakDisplacement);
	//update signal P
	delete signalP;
	signalP = new SignalProcessor(riseTime, riseTime, 0.05,
				      60, -5, 1,
				      40000, -60000, 30,
				      peakDisplacement, 0, -40000);
	std::cout << "Processing " << fileName << "... " <<std::endl;
	dataP->processFiles(true, fileName);

	//Create histogram
	std::cout << "Creating hist" << std::endl;
	TH1F h1;
	TFile *file = new TFile( fileName, "read");
	TFile *histFile = new TFile( Form(dir + "HIST_%i_%i.root", riseTime, peakDisplacement), "recreate");
	TTree *tree =  NULL;
	file->GetObject("macro_tree", tree);

	if(tree == NULL)
	{
	    std::cout << "Tree not found" << std::endl;
	    return;
	}
	UInt_t numEntries =  tree->GetEntries();
	Float_t temp;    
	tree->SetBranchAddress("Q_0", &temp);
    
	//create hist and fill it
	h1 = TH1F("htemp", "", 1000,
		  1400000000,
		  2200000000);
	
	for(int i = 0; i < numEntries; i++)
	{
	    tree->GetEntry(i);
		h1.Fill(temp);
	}

	//Fit with a gausian
        auto offset = 800000*(riseTime - 3700);
	TF1 *gaus = new TF1("gaus", "gaus", 1730000000 + offset,
			                    1745000000 + offset);
	h1.Fit(gaus,"R same quite");

	//write to file
	outFile << riseTime << "," << peakDisplacement << "," << gaus->GetParameter(1) << "," <<
	    gaus->GetParameter(2) << "," << gaus->GetParError(1) << "," <<
	    gaus->GetParError(2) <<"," <<  gaus->GetParameter(2)*2.355/ gaus->GetParameter(1) <<","<<
	    dataP->getBadEvents() << std::endl;
	
	histFile->cd();
	h1.Write();
	file->Close();
	histFile->Close();

	std::cout << "Done" << std::endl;
	if(peakDisplacement < 100)
	    peakDisplacement += 20;
	else
	{
	    peakDisplacement = 0;
	    riseTime += 10;
	}

    }

    outFile.close();
    gApplication->Terminate();
}

void PSD()
{
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();
    signalP->load("signal.config");
    std::cout << std::endl;

    //open files
    std::ofstream out;
    out.open("psd");
    if(!out.is_open())
    {
	std::cout << "psd not open" << std::endl;
	return;
    }


    //Create hist for psdParam
    TH1F *hist = new TH1F("hist", "PSD", 1000, 0, 1);
    TCanvas *c1=new TCanvas("c1");


    std::cout << "Starting loop: "<< dataP->getNumEvents() << std::endl;
    for(int i =0; i < dataP->getNumEvents(); i++)
    {
	if(i%10000 == 0 )
	    std::cout << "At event: " << i << std::endl;
		
	dataP->processEvent(0, i);
	//Float_t shortSig = signalP->QDC(dataP->getSignal(),
	//				dataP->getZero(),
	//				5) * 5;
	//Float_t longSig = signalP->QDC(dataP->getSignal(),
	////			       dataP->getZero(),
	//			       15) * 15;
	//std::cout << "Things processed" << std::endl;
	if(dataP->getZero() < 0)
	    continue;

	//stuff
	//out << shortSig << "," << longSig << std::endl;
	//Double_t psdParam = (longSig - shortSig)/longSig;
	//if(psdParam < 0)
	//    std::cout << i << std::endl;
	//hist->Fill(psdParam);
    }
    std::cout << "Ending loop" << std::endl;
    
    hist->Draw("hist");
    c1->Update();
    gApplication->Terminate();
}
    

#ifdef STANDALONE

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   
   PSD();

   theApp.Run();

   return 0;
}
#endif
