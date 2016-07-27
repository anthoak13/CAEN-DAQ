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
    d_kl = new Long_t[10];
    _p = new Long_t[10];
}

//Destructor
SignalProcessor::~SignalProcessor()
{
    if(_inter != NULL)
	delete _inter;
}

//Public Functions
Int_t  SignalProcessor::getDecayTime() const{ return _decayTime;}
Double_t SignalProcessor::getFlatMult() const{ return _flatMultiplier;}
Double_t SignalProcessor::getM() const{return  _M;}
Int_t SignalProcessor::getOffset() const{ return _offset;}
Double_t SignalProcessor::getScaling() const{ return _scaling;}
Int_t SignalProcessor::getThreshold() const{ return _threshold;}
UInt_t SignalProcessor::getInterpMult() const{ return _interpMult;}
Double_t SignalProcessor::getPeakThreshold() const{ return _peakThreshold;}

void SignalProcessor::trapFilter(std::vector<Long_t>* signal, const UInt_t start,
				 const UInt_t length) const
{
    if(start + length >= signal->size())
	return;
    trapFilter(&signal->at(start), length);

}

void SignalProcessor::trapFilter(Long_t* signal, const UInt_t signalLength) const
{
    //Loop through the signal to convolve
    Long_t* modSig = new Long_t[signalLength];

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

//Outputs a vector the same size as inputed signal
 std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal, const UInt_t start,
					    const UInt_t length, const Int_t shift) const
{
    //create output and zero fill
    std::vector<Double_t> out;
    out.resize(signal.size(), 0);

    //invert the signal
    for(int i = shift; i < length; i++)
    {
        out[i- shift] = -signal[i+ start];
    }
    
    for(int i = 0; i < (length-shift); i++)
        out[i] += signal[i+start];

    return out;
    
}
std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal, const Int_t shift) const
{
    return CFD(signal, 0, signal.size(), shift);
}
std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal) const
{
    return CFD(signal, 0, signal.size(), _offset);
}

int SignalProcessor::zeroAfterThreshold(const std::vector<double> &signal) const
{
    return SignalProcessor::zeroAfterThreshold(signal, _threshold);
}

//Gets the next zero crossing after the signal reaches a min or max of threshold (determined by sgn of threshold)
//return 0 if no zero is found
int SignalProcessor::zeroAfterThreshold(const std::vector<double> &signal, const int threshold) const
{
    auto it = signal.begin();
    if(threshold < 0)
    {
	//find  the point where the signal first drops below the threshold
	while(*it > threshold && it != signal.end()) { it++; }
	
	//Find the next zero after the threshold
	while(*it <= 0 && it != signal.end()) { it++; }
    } else {
	//Find point where signal first rises above threshold
	while(*it > threshold && it != signal.end()) { it++; }
	
	//Find the next zero after the threshold
	while(*it <= 0 && it != signal.end()) { it++; }
    }

    if(it == signal.end())
	return 0;
    
    return std::distance(signal.begin(), it)/_interpMult;
}
std::vector<double> SignalProcessor::deriv(const std::vector<Int_t> &signal) const
{
    return deriv(std::vector<Long_t>(signal.begin(), signal.end()));
}
std::vector<double> SignalProcessor::deriv(const std::vector<Long_t> &signal) const
{
    if(_interpMult < 2)
	return nonInterpDeriv(signal);

    
    setInter(signal);
    std::vector<double> out;

    //calculate the stepsize
    double stepSize = 1.0 /_interpMult;

    for(double i = 0; i < signal.size() - 1; i += stepSize)
    {
	out.push_back(_inter->Deriv(i));
    }

    return out;
}

std::vector<double> SignalProcessor::secondDeriv(const std::vector<Long_t> &signal) const
{
     if(_interpMult < 2)
	return nonInterpSecondDeriv(signal);

      setInter(signal);
    std::vector<double> out;

    //calculate the stepsize
    double stepSize = 1.0 /_interpMult;

    for(double i = 0; i < signal.size() - 1; i += stepSize)
    {
	out.push_back(_inter->Deriv2(i));
    }

    return out;
}

std::vector<double> SignalProcessor::nonInterpDeriv(const std::vector<Long_t> &signal) const
{
    std::vector<double> out;
    for(int i = 1; i < signal.size(); i++)
	out.push_back(signal[i] - signal[i-1]);

    return out;
}

std::vector<double> SignalProcessor::nonInterpSecondDeriv(const std::vector<Long_t> &signal) const
{
    std::vector<double> out;
    for(int i = 1; i < signal.size()-1; i++)
	out.push_back(signal[i-1] - 2*signal[i] + signal[i+1]);

    return out;
}

//Uses the change in derivative at the beginning and end of the flat top to
//find the peak value. If the derivative doesn't change as expected, ie it is exeptionally
//smooth, it returns a simple maximum of the trapazoid
Long_t SignalProcessor::peakFind(const std::vector<Long_t>::iterator start,
				 const std::vector<Long_t>::iterator end) const
{
    const double thresh = _peakThreshold;
    std::vector<Long_t>::iterator peak1;
    std::vector<Long_t>::iterator mid;
    std::vector<Long_t>::iterator peak2;
    bool posDeriv = (*(start+1) - *start) > 0;
    std::vector<Long_t>::iterator it = start;
    bool midValid = true;
    bool secondValid = true;
    double val = 0.0;

    if(posDeriv)
    {
	//find 1st peak
	while(*(it + 1) >= *it && it != end) { it++; }
	peak1 = it;

	//find mid
	while(*(it + 1) <= *it && it != end) { it++; }
	mid = it;

	//Find second max
	while(*(it + 1) >= *it && it != end) { it++; }
	peak2 = it;
    }
    else
    {
	//find 1st peak
	while(*(it + 1) >= *it && it != end) { it++; }
	peak1 = it;

	//find mid
	while(*(it + 1) <= *it && it != end) { it++; }
	mid = it;

	//Find second max
	while(*(it + 1) >= *it && it != end) { it++; }
	peak2 = it;
    }

    //Check the validity of the peaks
    val = (*peak1 - *mid)/(double) *peak1;
    midValid = ( val < thresh && val > -thresh ); 
    val = (*peak1 - *peak2) / (double) *peak2;
    secondValid = ( val < thresh && val > -thresh );

    
    Long_t returnValue;
    //If the peak is valid get the average
    if(midValid && secondValid)
	
	if(peak1 != peak2)
	    returnValue = TMath::Mean(peak1, peak2);
	else
	    returnValue = *peak1;
    else 
	returnValue = *(TMath::LocMax(start, end));

#ifdef DEBUG
    //  if(returnValue < 0)
	std::cout << "Peak 1: " << *peak1 << " mid: " << *mid << " peak2: " << *peak2 << " mean: " << returnValue << std::endl;
#endif
    
    return returnValue;
}

UInt_t SignalProcessor::peaksPastThreshold(const std::vector<double> &signal,
					   const Long_t threshHigh, const Long_t threshLow,
					   const UInt_t distBetweenPeaks) const
{
    return peaksPastThreshold(signal.begin(), signal.end(), threshHigh, threshLow, distBetweenPeaks);
}
UInt_t SignalProcessor::peaksPastThreshold(std::vector<double>::const_iterator start,
					   std::vector<double>::const_iterator end,
					   const Long_t threshHigh, const Long_t threshLow,
					   const UInt_t distBetweenPeaks) const
{
    std::vector<UInt_t> peaks;
    auto min = start;
    auto lastPeak = start;

    //loop through signal looking for past peak
    for(auto it = start; it < end; it++)
    {
	//if reached top of trap, coresponding negative spike then return the number of peaks
	if(*it < threshLow)
	{
	    //std::cout << "Quiting at: " << std::distance(start, it) << " with " <<
	    //	peaks.size() << " peaks." << std::endl;
	    return peaks.size();
	}
	//found the start of a peak
	if(*it > threshHigh)
	{
	    //if this should be counted as a new peak
	    if(std::distance(lastPeak, it) > distBetweenPeaks || lastPeak == start)
		peaks.push_back(std::distance(start, it));

	    //std::cout << "Peak at: " << std::distance(start, it) << " " << std::distance(lastPeak, it) << " from last."  << std:: endl;
	    
	    //Loop until back under the threshold
	    while(*it > threshHigh  && it != end) it++;

	    //set old peak
	    lastPeak = it;
	}
	else
	{
	    //not in a peak to set the min if necessary
	    if( *it < *min)
		min = it;
	}
    }

    //Looped through all peaks, now return the number of peaks less than the min
    UInt_t minLoc = std::distance(start, min);
    UInt_t numPeaks = 0;
    for(auto&& num:peaks)
	if(num < minLoc)
	{
	    //std::cout << " Peak at: " << num;
	    numPeaks++;
	}
    //std::cout <<  std::endl;
    return numPeaks;

}

Float_t SignalProcessor::QDC(const std::vector<int> &signal, const UInt_t start, const UInt_t length) const
{
    int Q = 0;
    for(int i = 0; i < length; i++)
    {
	Q += signal[start+i];
    }
    return ((Float_t)Q)/length;
    
}

//*********Private functions ********************

//Deletes the old interpolator and creates a new object to be used by other functions
void SignalProcessor::setInter(const std::vector<Long_t> &in) const
{
    //create x array
    std::vector<double> x;
    std::vector<double> y;
    for(unsigned int i = 0; i < in.size(); i++) { x.push_back(i); y.push_back(in[i]); } 

    if(_inter != NULL)
	    delete _inter;
    //std::cout << "Creating interp" << std::endl;
    _inter = new ROOT::Math::Interpolator(x, y, ROOT::Math::Interpolation::kAKIMA);
    //std::cout << "Created" << std::endl;
}

//signal is unmodified
void SignalProcessor::setD_kl(Long_t* signal, const UInt_t length) const
{
    int k = (int) _decayTime;
    int l = (int) (_flatMultiplier * 2 * _decayTime + k);

    //Cleanup memory
    delete [] d_kl;
    d_kl = new Long_t[length];

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

//signal is unmodified, assumes d_kl is propely set
void SignalProcessor::p(Long_t* signal, const UInt_t n) const
{
    prepP(n);
    setP(signal, n-1);
}

Long_t SignalProcessor::setP(Long_t* signal, const UInt_t n) const
{
    //Basecase
    if( n < 1)
	_p[n] = 0;
    else
	_p[n] = (setP(signal, n-1) + d_kl[n]);

    return _p[n];
    
}


void SignalProcessor::prepP(const UInt_t n) const
{
    delete [] _p;
    _p = new Long_t[n];
}


//modifies signal inplace, assumes d_kl and p are properly set
Long_t SignalProcessor::s(Long_t* signal, const UInt_t n) const
{
    Long_t ret = (_p[n] + d_kl[n] * _M);

    //Base case
    if(n < 1)
	return ret;

    //Not base
    ret += s(signal, n-1);
    
    return ret;
}


