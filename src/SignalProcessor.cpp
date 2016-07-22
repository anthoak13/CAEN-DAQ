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

//Class implementation to process digital signals
//Contains methods to apply a trapazoidal filter
//and constant fraction discrimination
//Adam Anthony 6/30/16

//include files
#include "SignalProcessor.h"
#include <iostream>

ClassImp(SignalProcessor);
//Constructor
SignalProcessor::SignalProcessor(const UInt_t riseTime,   const UInt_t M,
		    const Double_t flatMult, const Double_t peakThreshold,
		    const UInt_t zeroOffset, const Int_t zeroThreshold,
		    const UInt_t interpMult, const Double_t scalingMult)
{
    _decayTime = riseTime;
    _M = M;
    _flatMultiplier = flatMult;
    _peakThreshold = peakThreshold;
    _offset = zeroOffset;
    _scaling = scalingMult;
    _threshold = zeroThreshold;
    _interpMult = interpMult;

    _inter = NULL;
    d_kl = new int[10];
    _p = new int[10];
}

//Destructor
SignalProcessor::~SignalProcessor()
{
    if(_inter != NULL)
	delete _inter;
}

//Public Functions
Int_t  SignalProcessor::getDecayTime() { return _decayTime;}
Double_t SignalProcessor::getFlatMult() { return _flatMultiplier;}
Double_t SignalProcessor::getM() {return  _M;}
Int_t SignalProcessor::getOffset() { return _offset;}
Double_t SignalProcessor::getScaling() { return _scaling;}
Int_t SignalProcessor::getThreshold() { return _threshold;}

void SignalProcessor::trapFilter(std::vector<int>* signal, const int start, const int length)
{
    if(start + length >= signal->size())
	return;
    trapFilter(&signal->at(start), length);

}

void SignalProcessor::trapFilter(int* signal, const int signalLength)
{
    //Loop through the signal to convolve
    int* modSig = new int[signalLength];

    //Get arrays for processing signal
    setD_kl(signal, signalLength);
    p(signal, signalLength);
    

    //get the modified signal;
    for(int n = 0; n < signalLength; n++)
    {
	modSig[n] = s(signal, n);
    }

    //replace signal
    for(int n = 0; n <signalLength; n++)
    {
	signal[n] = modSig[n];
    }
    
    delete [] modSig;
}


void SignalProcessor::CFD(std::vector<double>* signal, const int start, const int length,
			  const int shift)
{
    //invert signal and shift
    double* signalInv = new double[length];
    for(int i = shift; i < length; i++)
    {
	signalInv[i- shift] = -signal->at(i+ start);
    }
    
    for(int i = 0; i < (length-shift); i++)
	signal->at(i+start) += signalInv[i];

    delete [] signalInv;
}
void SignalProcessor::CFD(std::vector<double>* signal, const int shift)
{
    CFD(signal, 0, signal->size(), shift);
}
void SignalProcessor::CFD(std::vector<double>* signal)
{
    CFD(signal, 0, signal->size(), _offset);
}

int SignalProcessor::zeroAfterThreshold(std::vector<double>* signal)
{
    return SignalProcessor::zeroAfterThreshold(signal, _threshold);
}
//Gets the next zero crossing after the signal reaches a min or max of threshold (determined by sgn of threshold)
//return 0 if no zero is found
int SignalProcessor::zeroAfterThreshold(std::vector<double>* signal, const int threshold)
{
    std::vector<double>::iterator it = signal->begin();
    if(threshold < 0)
    {
	//find  the point where the signal first drops below the threshold
	while(*it > threshold && it != signal->end()) { it++; }
	
	//Find the next zero after the threshold
	while(*it <= 0 && it != signal->end()) { it++; }
    } else {
	//Find point where signal first rises above threshold
	while(*it > threshold && it != signal->end()) { it++; }
	
	//Find the next zero after the threshold
	while(*it <= 0 && it != signal->end()) { it++; }
    }

    if(it == signal->end())
	return 0;
    
    return std::distance(signal->begin(), it)/_interpMult;
}

std::vector<double> SignalProcessor::deriv(std::vector<int>* signal)
{
    if(_interpMult < 2)
	return nonInterpDeriv(signal);

    
    setInter(signal);
    std::vector<double> out;

    //calculate the stepsize
    double stepSize = 1.0 /_interpMult;

    for(double i = 0; i < signal->size() - 1; i += stepSize)
    {
	out.push_back(_inter->Deriv(i));
    }

    return out;
}

std::vector<double> SignalProcessor::nonInterpDeriv(std::vector<int>* signal)
{
    std::vector<double> out;
    for(int i = 1; i < signal->size(); i++)
	out.push_back(signal->at(i) - signal->at(i-1));

    return out;
}

//Uses the change in derivative at the beginning and end of the flat top to
//find the peak value. If the derivative doesn't change as expected, ie it is exeptionally
//smooth, it returns a simple maximum of the trapazoid
int SignalProcessor::peakFind(std::vector<int>::iterator start, std::vector<int>::iterator end)
{
    const double thresh = _peakThreshold;
    std::vector<int>::iterator peak1;
    std::vector<int>::iterator mid;
    std::vector<int>::iterator peak2;
    bool posDeriv = (*(start+1) - *start) > 0;
    std::vector<int>::iterator it = start;
    bool midValid = true;
    bool secondValid = true;
    double val = 0.0;

    if(posDeriv)
    {
	//find 1st peak
	while(*(it + 1) > *it && it != end) { it++; }
	peak1 = it;

	//find mid
	while(*(it + 1) < *it && it != end) { it++; }
	mid = it;

	//Find second max
	while(*(it + 1) > *it && it != end) { it++; }
	peak2 = it;
    }
    else
    {
	//find 1st peak
	while(*(it + 1) > *it && it != end) { it++; }
	peak1 = it;

	//find mid
	while(*(it + 1) < *it && it != end) { it++; }
	mid = it;

	//Find second max
	while(*(it + 1) > *it && it != end) { it++; }
	peak2 = it;
    }

    //Check the validity of the peaks
    val = (*peak1 - *mid)/(double) *peak1;
    midValid = ( val < thresh && val > -thresh ); 
    val = (*peak1 - *peak2) / (double) *peak2;
    secondValid = ( val < thresh && val > -thresh );

    
    int returnValue;
    //If the peak is valid get the average
    if(midValid && secondValid)
	
	if(peak1 != peak2)
	    returnValue = TMath::Mean(peak1, peak2);
	else
	    returnValue = *peak1;
    
    //XOR midValue and secondValue -> pileup
    else if(midValid != secondValid)
    {
	returnValue = -1; //Detect pileup
	
    }
    else
	returnValue = *(TMath::LocMax(start, end));

#ifdef DEBUG
    if(returnValue < 0)
	std::cout << "Peak 1: " << *peak1 << " mid: " << *mid << " peak2: " << *peak2 << " mean: " << returnValue << std::endl;
#endif
    
    return returnValue;
}

//*********Private functions ********************

//Deletes the old interpolator and creates a new object to be used by other functions
void SignalProcessor::setInter(std::vector<int>* in)
{
    //create x array
    std::vector<double> x;
    std::vector<double> y;
    for(unsigned int i = 0; i < in->size(); i++) { x.push_back(i); y.push_back(in->at(i)); } 

    if(_inter != NULL)
	    delete _inter;
    //std::cout << "Creating interp" << std::endl;
    _inter = new ROOT::Math::Interpolator(x, y, ROOT::Math::Interpolation::kAKIMA);
    //std::cout << "Created" << std::endl;
}

//signal is unmodified
void SignalProcessor::setD_kl(int* signal, const int length)
{
    int k = (int) _decayTime;
    int l = (int) (_flatMultiplier * 2 * _decayTime + k);

    //Cleanup memory
    delete [] d_kl;
    d_kl = new int[length];

    //assumes k < l
    for(int n = 0; n <length; n++)
    {
	d_kl[n] = signal[n];

	if(n-k >= 0)
	    d_kl[n] -= signal[n-k];

	if(n-l >= 0)
	    d_kl[n] -= signal[n-l];

	if(n-k-l >= 0)
	    d_kl[n] += signal[n-k-l];
    }
}

//signal is unmodified, assumes d_kl is properly set
void SignalProcessor::p(int* signal, const int n)
{
    prepP(n);
    setP(signal, n-1);
}

int SignalProcessor::setP(int* signal, const int n)
{
    //Basecase
    if( n < 1)
	_p[n] = 0;
    else
	_p[n] = (setP(signal, n-1) + d_kl[n]);

    return _p[n];
    
}


void SignalProcessor::prepP(const int n)
{
    delete [] _p;
    _p = new int[n];
}


//modifies signal inplace, assumes d_kl and p are properly set
int SignalProcessor::s(int* signal, const int n)
{
    int ret = (_p[n] + d_kl[n] * _M);

    //Base case
    if(n < 1)
	return ret;

    //Not base
    ret += s(signal, n-1);
    
    return ret;
}

Float_t SignalProcessor::QDC(std::vector<int> *signal, const int start, const int length)
{
    int Q = 0;
    for(int i = 0; i < length; i++)
    {
	Q += (*signal)[start+i];
    }
    return ((Float_t)Q)/length;
    
}
