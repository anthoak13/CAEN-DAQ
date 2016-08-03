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
#include "TMath.h"
#include "TGraph.h"
#include <fstream>
#include <iostream>

void WalkData()
{
    const TString dir = "/home/adam/data/AmBeNaI/";
    //Create DataP
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();

    UInt_t riseTime = 10;
    Int_t peakDisplacement = -4;
    std::ofstream outFile;
    outFile.open(TString(dir + "Efficiency.txt"));
    if(!outFile.is_open())
	return;
    
    //loop though and create trees of data
    bool test = true;
    while(riseTime <= 15 && peakDisplacement <=4 && test)
    {
	TString fileName = Form(dir + "tree_%i_%i.root", riseTime, peakDisplacement);
	//update signal P
	delete signalP;
	signalP = new SignalProcessor(riseTime, riseTime, 1.5,
				      10, -30, 1,
				      2200, 0, 10,
				      peakDisplacement, 0, -1000);
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
		  0,
		  1000000);
	
	for(int i = 0; i < numEntries; i++)
	{
	    tree->GetEntry(i);
		h1.Fill(temp);
	}

	//Fit with a gausian
        auto offset = 45000*(riseTime - 10);
	TF1 *gaus = new TF1("gaus", "gaus", 320000 + offset,
			                    340000 + offset);
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
	if(peakDisplacement < 4)
	    peakDisplacement += 2;
	else
	{
	    peakDisplacement = -4;
	    riseTime++;
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
    const int startOffset = -2;
    const int tailStart = 6;
    const int sigEnd = 10;
    
    std::cout << "Starting loop: "<< dataP->getNumEvents() << std::endl;
    for(int i =0; i < dataP->getNumEvents(); i++)
    {
	if(i%10000 == 0 )
	    std::cout << "At event: " << i << std::endl;
		
	dataP->processEvent(0, i);
	auto max = dataP->getSignal().begin()+2;
	auto it = max;

	//get the maximum
	while(it < dataP->getSignal().begin()+25)
	{
	    if(*it > *max)
		max = it;

	    it++;
	}
	
	//std::cout << max << std::endl;
	int maxLoc  = max - dataP->getSignal().begin();

	Float_t totalSig = signalP->QDC(dataP->getSignal(),
					maxLoc + startOffset,
					maxLoc + sigEnd);
	Float_t tailSig = signalP->QDC(dataP->getSignal(),
				       maxLoc +  tailStart,
				       maxLoc + sigEnd);
	//std::cout << "Things processed" << std::endl;
	if(dataP->getZero() < 0)
	    continue;

	//stuff
	out << tailSig << "," << totalSig << std::endl;
	Double_t psdParam = (totalSig - tailSig)/totalSig;
	if(psdParam < 0)
	    std::cout << "Pileup at: " <<  i << std::endl;
	else
	    hist->Fill(psdParam);
    }
    std::cout << "Ending loop" << std::endl;
    
    hist->Draw("hist");
    c1->Update();
    //gApplication->Terminate();
}

void genGraphs()
{
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();
    signalP->load("signal.config");
    std::cout << std::endl;
    dataP->processEvent(0, 7);

    //Create canvas
    TCanvas *canv = new TCanvas("canv", "Pic", 600, 800);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);

    canv->cd();
    TPad *pad1 = new TPad("canv_1", "", 0.01, 0.50 , 0.99, 0.99);
    canv->cd();
    TPad *pad2 = new TPad("canv_2", "", 0.01, 0.01 , 0.99, 0.50);
    canv->cd();
    pad1->Draw();
    pad2->Draw();

    
    //Create graph
    auto rawD = dataP->getRaw();
    auto data = std::vector<double>(rawD.begin(), rawD.end()) ;
    auto x = data;
    x.clear();
    for(int i = 0; i < data.size(); i++) x.push_back(i);
    TGraph *raw = new TGraph(data.size(), x.data(), data.data());
    TGraph *raw2 = new TGraph(*raw);
    
    pad1->cd();
    raw->GetXaxis()->SetLabelOffset(999);
    raw->Draw("AC");
    
    
    pad2->cd();
    raw2->SetTitle("");
    raw2->SetLineWidth(2);
    raw2->Draw("AC");

}
    

#ifdef STANDALONE

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   
   //genGraphs();
   WalkData();
   theApp.Run();

   return 0;
}
#endif
