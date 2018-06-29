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
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
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


void NaIGraphics()
{
    //All of this is set using 2016-07-13-35-19
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();
    //replace signal P with the one for 
    delete signalP;
    signalP = new SignalProcessor(12, 12, 1.5,
				  9, -30, 1,
				  2400, 0, 10,
				  -2, 0, -1000);

    int normalEvent = 7;
    int pileupEvent = 16;

    /****Generate graphs for both events***/
    //Raw and Corrected
    //CFD overlayed on corrected
    //TFA
    //Trap filtered with energy drawn on
    //Second deriv with PileHigh Drawn on and peak Threshold4

    /**setup canvas**/
    TCanvas *canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    
    dataP->processEvent(0, normalEvent);

    //Create multigraph for raw and corrected
    TMultiGraph *multiGraph = new TMultiGraph();

    
    //get raw data and convert to a dounble vector
    auto data = dataP->getRaw();
    auto dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    std::vector<Double_t> x;
    for(int i = 0; i < dataD.size(); i++) x.push_back(i);

    //Create graph Raw and corrected graph
    TGraph *raw = new TGraph(x.size(), x.data(), dataD.data());
    raw->SetFillStyle(0);
    raw->SetLineWidth(2);
    raw->SetLineStyle(2);
    raw->SetName("raw");
    raw->SetTitle("Raw Signal");
    

    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    TGraph *corrected = new TGraph(x.size(), x.data(), dataD.data());
    corrected->SetFillStyle(0);
    corrected->SetLineWidth(2);
    corrected->SetLineColor(2);
    corrected->SetName("corr");
    corrected->SetTitle("Corrected Signal");

    
    canv->cd();
    multiGraph->Add(raw);
    multiGraph->Add(corrected);

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(0);
    multiGraph->SetMaximum(3500);

    //Build the legend
    TLegend *legend = new TLegend(0.50, 0.50, 0.90, 0.82);
    legend->AddEntry(raw);
    legend->AddEntry(corrected);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("rawCorr.png");

    //delete old graphs and make new ones
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *signal = new TGraph(x.size(), x.data(), dataD.data());
    signal->SetFillStyle(0);
    signal->SetLineWidth(2);
    signal->SetLineStyle(2);
    signal->SetName("signal");
    signal->SetTitle("Signal");
    

    data = dataP->getCFD();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    TGraph *cfd = new TGraph(x.size(), x.data(), dataD.data());
    cfd->SetFillStyle(0);
    cfd->SetLineWidth(2);
    cfd->SetLineColor(2);
    cfd->SetName("cfd");
    cfd->SetTitle("CFD");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(-30);
    TGraph *thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(10);
    thresh->SetName("thresh");
    thresh->SetTitle("Zero Threshold");

    
    canv->cd();
    multiGraph->Add(signal);
    multiGraph->Add(cfd);
    multiGraph->Add(thresh);

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,150);
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-500);
    multiGraph->SetMaximum(2400);

    //Build the legend
    delete legend;
    legend = new TLegend(0.55, 0.50, 0.87, 0.87);
    legend->AddEntry(signal);
    legend->AddEntry(cfd);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("CFDandThresh.png");

    /***Make normal trap graph ***/
    //delete old graphs and make new ones
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getTrap();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(2);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 10;
	else
	    dataD.push_back(0.0);
    }
    TGraph *trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(2400*10);
    thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(10);
    thresh->SetName("thresh");
    thresh->SetTitle("Pileup Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-200000);
    multiGraph->SetMaximum(500000);

    //Build the legend
    delete legend;
    legend = new TLegend(0.40, 0.50, 0.87, 0.87);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("Normal_trap.png");


        /***Make normal trap graph ***/
    //delete old graphs and make new ones
    dataP->processEvent(0,16);
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getTrap();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(2);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 10;
	else
	    dataD.push_back(0.0);
    }
    trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(2400*10);
    thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(10);
    thresh->SetName("thresh");
    thresh->SetTitle("Pileup Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-250000);
    multiGraph->SetMaximum(650000);

    //Build the legend
    delete legend;
    legend = new TLegend(0.43, 0.50, 0.87, 0.87);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("Pileup_trap.png");
}

void NaIPosterGraphs()
{

    //All of this is set using 2016-07-13-35-19
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();
    //replace signal P with the one for 
    delete signalP;
    signalP = new SignalProcessor(12, 12, 1.5,
				  9, -30, 1,
				  2400, 0, 10,
				  -2, 0, -1000);

    int normalEvent = 7;
    int pileupEvent = 16;

    /****Generate graphs for both events***/
    //Raw and Corrected
    //CFD overlayed on corrected
    //TFA
    //Trap filtered with energy drawn on
    //Second deriv with PileHigh Drawn on and peak Threshold4

    /**setup canvas**/
    TCanvas *canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    
    dataP->processEvent(0, normalEvent);

    //Create multigraph
    TMultiGraph *multiGraph = new TMultiGraph();

    /***For baseline corrected graph ***/
    //get raw data and convert to a dounble vector
    auto data = dataP->getRaw();
    auto dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    std::vector<Double_t> x;
    for(int i = 0; i < dataD.size(); i++) x.push_back(i);

    //Create graph Raw
    TGraph *raw = new TGraph(x.size(), x.data(), dataD.data());
    raw->SetFillStyle(0);
    raw->SetLineWidth(2);
    raw->SetLineStyle(1);
    raw->SetLineColor(2);
    raw->SetName("raw");
    raw->SetTitle("Raw Signal");
    

    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    TGraph *corrected = new TGraph(x.size(), x.data(), dataD.data());
    corrected->SetFillStyle(0);
    corrected->SetLineWidth(2);
    corrected->SetName("corr");
    corrected->SetTitle("Corrected Signal");

    //Add the graphs to the multigraph
    canv->cd();
    multiGraph->Add(raw);
    multiGraph->Add(corrected);
    multiGraph->SetTitle("Raw and Corrected Signal");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(0);
    multiGraph->SetMaximum(3500);

    //Build the legend
    TLegend *legend = new TLegend(0.50, 0.50, 0.90, 0.82);
    legend->AddEntry(raw);
    legend->AddEntry(corrected);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/PosterNaIBaseline.png");

    //****CFD and signal****//
    //delete old graphs and make new ones
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *signal = new TGraph(x.size(), x.data(), dataD.data());
    signal->SetFillStyle(0);
    signal->SetLineWidth(2);
    signal->SetLineStyle(1);
    signal->SetName("signal");
    signal->SetTitle("Signal");
    

    data = dataP->getCFD();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    TGraph *cfd = new TGraph(x.size(), x.data(), dataD.data());
    cfd->SetFillStyle(0);
    cfd->SetLineWidth(2);
    cfd->SetLineColor(2);
    cfd->SetName("cfd");
    cfd->SetTitle("CFD");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(-30);
    TGraph *thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(1);
    thresh->SetName("thresh");
    thresh->SetTitle("Zero Threshold");

    
    canv->cd();
    multiGraph->Add(signal);
    multiGraph->Add(cfd);
    multiGraph->Add(thresh);
    multiGraph->SetTitle("CFD and Signal");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,150);
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-500);
    multiGraph->SetMaximum(2400);

    //Build the legend
    delete legend;
    legend = new TLegend(0.55, 0.50, 0.87, 0.87);
    legend->AddEntry(signal);
    legend->AddEntry(cfd);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/NaICFD.png");

    /***Make normal trap graph ***/
    //delete old graphs and make new ones
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getTrap();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(1);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 10;
	else
	    dataD.push_back(0.0);
    }
    TGraph *trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(-1000*10);
    thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(1);
    thresh->SetName("thresh");
    thresh->SetTitle("Zero Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);
    multiGraph->SetTitle("NaI Trapezoid Filter");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-200000);
    multiGraph->SetMaximum(500000);

    //Build the legend
    delete legend;
    legend = new TLegend(0.40, 0.50, 0.87, 0.87);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/NaITrap.png");


    /***Make NaI TFA ***/
    //delete old graphs and make new ones
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(1);
    trap->SetName("sig");
    trap->SetTitle("Signal");
    

    data = dataP->getDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 1;
	else
	    dataD.push_back(0.0);
    }
    trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("sigDeriv");
    trapDeriv->SetTitle("TFA");

    canv->Clear();
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->SetTitle("NaI TFA");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-300);
    multiGraph->SetMaximum(2500);

    //Build the legend
    delete legend;
    legend = new TLegend(0.60, 0.65, 0.90, 0.82);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/NaITFA.png");

     /***Make normal trap graph ***/
    //delete old graphs and make new ones
    dataP->processEvent(0,16);
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getTrap();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(1);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 10;
	else
	    dataD.push_back(0.0);
    }
    trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(2400*10);
    thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(1);
    thresh->SetName("thresh");
    thresh->SetTitle("Pileup Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);
    multiGraph->SetTitle("NaI Trapezoid with Pileup");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-300000);
    multiGraph->SetMaximum(650000);

    //Build the legend
    delete legend;
    legend = new TLegend(0.45, 0.50, 0.90, 0.87);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/NaITrapPileup.png");

     /***Make NaI TFA ***/
    //delete old graphs and make new ones
    delete canv;
    TCanvas *canv1 = new TCanvas("canv1", "", 800,800);
    canv1->Range(0,0,1,1);
    canv1->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *trap1 = new TGraph(x.size(), x.data(), dataD.data());
    trap1->SetFillStyle(0);
    trap1->SetLineWidth(2);
    trap1->SetLineStyle(1);
    trap->SetName("sig1");
    trap->SetTitle("Signal");

        
    multiGraph->Add(trap1);
    multiGraph->SetTitle("NaI Pileup Waveform");
    canv1->cd();

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    multiGraph->SetMinimum(0);
    multiGraph->SetMaximum(2500);
    canv1->SetBottomMargin(0.13);
    canv1->SetLeftMargin(0.13);

    //Build the legend
    delete legend;
    legend = new TLegend(0.55, 0.65, 0.75, 0.82);
    legend->AddEntry(trap1);
    canv1->cd();

    legend->Draw();
    canv1->Modified();
    canv1->Update();
    canv1->SaveAs("../poster/NaIPileupForm.png");

}

void HPGePosterGraphs()
{
    //All of this is set using 2016-07-29-31-55
    DataProcessor *dataP = new DataProcessor("data_in%i", "meta.config", 1, 6);
    SignalProcessor* signalP = dataP->getSignalP();
    //replace signal P with the one for 
    delete signalP;
    signalP = new SignalProcessor(3720, 3720, 0.05,
				  8, -4, 1,
				  50000, 0, 50,
				  0, 0, -100000);

    int normalEvent = 6;
    int pileupEvent = 7;

    /***Make normal trap graph ***/
    TCanvas *canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    TMultiGraph *multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    dataP->processEvent(0,6);
    auto data = dataP->getTrap();
    auto dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    std::cout << "Trap length: " << dataD.size() << std::endl;
    auto x = dataD;
    x.clear();
    for(int i = 0; i < data.size(); i++) x.push_back(i);
    
    //Create graph Raw and corrected graph
    TGraph *trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(1);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 5000;
	else
	    dataD.push_back(0.0);
    }
    TGraph *trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(-100000*5000);
    TGraph *thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(1);
    thresh->SetName("thresh");
    thresh->SetTitle("Zero Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);
    multiGraph->SetTitle("HPGe Trapezoid Filter");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetNdivisions(508);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->SetMinimum(-800000000);
    multiGraph->SetMaximum(2000000000);

    //Build the legend
    TLegend *legend = new TLegend(0.13, 0.51, 0.50, 0.88);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    multiGraph->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/HPGeTrap.png");

     /***Make pileup trap graph ***/
    //delete old graphs and make new ones
    dataP->processEvent(0,7);
    delete canv;
    canv = new TCanvas("canv", "", 800,600);
    canv->Range(0,0,1,1);
    canv->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getTrap();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    trap = new TGraph(x.size(), x.data(), dataD.data());
    trap->SetFillStyle(0);
    trap->SetLineWidth(2);
    trap->SetLineStyle(1);
    trap->SetName("trap");
    trap->SetTitle("Trapazoid Filter");
    

    data = dataP->getTrapDeriv();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());
    for(int i = 0; i < x.size(); i++)
    {
	if(i < dataD.size())
	    dataD[i] *= 5000;
	else
	    dataD.push_back(0.0);
    }
    trapDeriv = new TGraph(x.size(), x.data(), dataD.data());
    trapDeriv->SetFillStyle(0);
    trapDeriv->SetLineWidth(2);
    trapDeriv->SetLineColor(2);
    trapDeriv->SetName("trapDeriv");
    trapDeriv->SetTitle("Trapezoid 2nd Deriv");

    dataD.clear();
    for(int i = 0; i < x.size(); i++) dataD.push_back(50000*5000);
    thresh = new TGraph(x.size(), x.data(), dataD.data());
    thresh->SetFillStyle(0);
    thresh->SetLineWidth(2);
    thresh->SetLineColor(3);
    thresh->SetLineStyle(1);
    thresh->SetName("thresh");
    thresh->SetTitle("Pileup Threshold");

    
    canv->cd();
    multiGraph->Add(trap);
    multiGraph->Add(trapDeriv);
    multiGraph->Add(thresh);
    multiGraph->SetTitle("HPGe Trapezoid with Pileup");

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    canv->SetBottomMargin(0.13);
    multiGraph->GetXaxis()->SetNdivisions(508);
    multiGraph->SetMinimum(-600000000);
    multiGraph->SetMaximum(2500000000);

    //Build the legend
    delete legend;
    legend = new TLegend(0.13, 0.51, 0.50, 0.88);
    legend->AddEntry(trap);
    legend->AddEntry(trapDeriv);
    legend->AddEntry(thresh);
    legend->Draw();
    multiGraph->Draw();
    canv->Modified();
    canv->Update();
    canv->SaveAs("../poster/HPGeTrapPileup.png");

     /***Make NaI TFA ***/
    //delete old graphs and make new ones
    delete canv;
    TCanvas *canv1 = new TCanvas("canv1", "", 800,800);
    canv1->Range(0,0,1,1);
    canv1->SetFrameBorderMode(0);
    delete multiGraph;
    multiGraph = new TMultiGraph();

    //get raw data and convert to a dounble vector
    data = dataP->getSignal();
    dataD = std::vector<Double_t>(data.cbegin(), data.cend());

    //Create graph Raw and corrected graph
    TGraph *trap1 = new TGraph(x.size(), x.data(), dataD.data());
    trap1->SetFillStyle(0);
    trap1->SetLineWidth(2);
    trap1->SetLineStyle(1);
    trap->SetName("sig1");
    trap->SetTitle("Signal");

        
    multiGraph->Add(trap1);
    multiGraph->SetTitle("HPGe Pileup Waveform");
    canv1->cd();

    multiGraph->Draw("AC");
    multiGraph->GetXaxis()->SetTitle("Sample #");
    multiGraph->GetXaxis()->SetLimits(0,x.size());
    multiGraph->GetXaxis()->SetLabelSize(0.06);
    multiGraph->GetXaxis()->SetTitleSize(0.06);
    multiGraph->GetYaxis()->SetLabelSize(0.06);
    std::cout << multiGraph->GetXaxis()->GetNdivisions() << std::endl;
    multiGraph->GetXaxis()->SetNdivisions(508);
    multiGraph->SetMinimum(0);
    multiGraph->SetMaximum(180);
    canv1->SetBottomMargin(0.13);
    canv1->SetLeftMargin(0.13);

    //Build the legend
    delete legend;

    legend = new TLegend(0.6, 0.75, 0.85, 0.89);
    legend->AddEntry(trap1);
    canv1->cd();

    legend->Draw();
    canv1->Modified();
    canv1->Update();
    canv1->SaveAs("../poster/HPGePileupForm.png");

}
#ifdef STANDALONE

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   gStyle->SetLegendBorderSize(0);   
   //genGraphs();
   //NaIPosterGraphs();
   HPGePosterGraphs();
   theApp.Run();

   return 0;
}
#endif
