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
#include "Math/Interpolator.h"
#include "TString.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

ClassImp(SignalProcessor);
//Constructor
SignalProcessor::SignalProcessor(const UInt_t riseTime,       const UInt_t M,
				 const Double_t flatMult,     const UInt_t zeroOffset,
				 const Int_t  zeroThreshold,  const UInt_t interpMult,
				 const Long_t pileHigh,       const Long_t pileLow,
				 const UInt_t peakLength,     const Int_t peakDisplacement,
				 const UInt_t pointsToAvg,    const Int_t peakThreshold)
{
    _riseTime = riseTime;
    _M = M;
    _flatMultiplier = flatMult;

    _zeroOffset = zeroOffset;
    _zeroThreshold = zeroThreshold;
    _interpMult = interpMult;

    _pileHigh = pileHigh;
    _pileLow = pileLow;
    _peakLength = peakLength;
    
    _pointsToAverage = pointsToAvg;
    _peakDisplacement = peakDisplacement;
    _peakThreshold = peakThreshold;

    //Variables not set by user
    _inter = NULL;
    d_kl = new Long_t[10];
    _p = new Long_t[10];
}

//Destructor
SignalProcessor::~SignalProcessor()
{
    if(_inter != nullptr)
	delete _inter;
}

//Public Functions
Int_t  SignalProcessor::getRiseTime() const{ return _riseTime;}
Int_t SignalProcessor::getM() const{return  _M;}
Double_t SignalProcessor::getFlatMult() const{ return _flatMultiplier;}

Int_t SignalProcessor::getZeroOffset() const{ return _zeroOffset;}
Int_t SignalProcessor::getZeroThreshold() const{ return _zeroThreshold;}
UInt_t SignalProcessor::getInterpMult() const{ return _interpMult;}

Long_t SignalProcessor::getPileHigh() const { return _pileHigh; }
Long_t SignalProcessor::getPileLow() const { return _pileLow; }
UInt_t SignalProcessor::getPeakLength() const { return _peakLength; }

UInt_t SignalProcessor::getPointsToAverage() const { return _pointsToAverage; }
Int_t SignalProcessor::getPeakDisplacement() const { return _peakDisplacement; }
Int_t SignalProcessor::getPeakThreshold() const { return _peakThreshold; }


void SignalProcessor::trapFilter(std::vector<Long_t>* signal, const UInt_t startIn,
				 const UInt_t lengthIn) const
{
    UInt_t start = (startIn < signal->size()) ? startIn : signal->size() - 1;
    UInt_t length = (start + lengthIn < signal->size()) ? lengthIn : signal->size() - start;

    trapFilter(&signal->at(start), length);

}

void SignalProcessor::trapFilter(Long_t* signal, const UInt_t signalLength) const
{
    //Get arrays for processing signal
    setD_kl(signal, signalLength);
    p(signal, signalLength);
    
    //get the modified signal;
    for(int n = 0; n < signalLength; n++)
    {
	signal[n] = signal[n-1] + _p[n] + d_kl[n] * _M;
    }
}

//Outputs a vector the same size as inputed signal
 std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal, const UInt_t start,
					    const UInt_t length, const Int_t shift) const
{
    std::vector<Double_t> out;

    //Zero pad until start of shift
    for(int i = 0; i < signal.size(); i++)
	if( i < start + shift)
	    out.push_back(0);
	else if (i < start + length + shift)
	    out.push_back(-signal.at(i - shift));
	else
	    out.push_back(0);

    //Add the two signals together
    for(int i = 0; i < signal.size(); i++)
	out.at(i) += signal.at(i);

    return out;
    
}
std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal, const Int_t shift) const
{
    return CFD(signal, 0, signal.size(), shift);
}
std::vector<Double_t> SignalProcessor::CFD(const std::vector<Double_t> &signal) const
{
    return CFD(signal, 0, signal.size(), _zeroOffset);
}

//Gets the next zero crossing after the signal reaches a min or max of threshold (determined by sgn of threshold)
//return 0 if no zero is found
int SignalProcessor::zeroAfterThreshold(const std::vector<double> &signal, const int threshold) const
{
    //Lambda's for comparison
    auto crossedThreshold = [=] (Long_t in){ return ( threshold < 0 && in < threshold ) ||
				    ( threshold > 0 && in > threshold ); };
    auto crossedZero = [=] (Long_t in){ return ( threshold < 0 && in > 0 ) ||
				      ( threshold > 0 && in < 0 ); };
    
    if(signal.empty() || threshold == 0)
	return -1;

    auto it = signal.begin();
    auto end = signal.end();

    while(!crossedThreshold(*it) && it < end)
    {
	it++;
    }
    //  std::cout << "Looking for 0" << std::endl;
    while(!crossedZero(*it) && it < end)
    {
	it++;
    }

    if(!(it < end))
    {
	return -1;
    }

    return end - it + 1;
}

int SignalProcessor::cfdZero(const std::vector<double> &signal) const
{
    return zeroAfterThreshold(signal, _zeroThreshold)/_interpMult;
}
int SignalProcessor::peakZero(const std::vector<double> &signal) const
{
    return zeroAfterThreshold(signal, _peakThreshold);
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

std::vector<double> SignalProcessor::pileupTrace(const std::vector<Long_t> &signal) const
{
    return pileupTrace(signal, 0, signal.size());
}

std::vector<double> SignalProcessor::pileupTrace(const std::vector<Long_t> &signal, const UInt_t startIn, const UInt_t lengthIn) const
{
    /*std::vector<double> out;
    //validate parameters
    UInt_t start = (signal.size() > startIn) ? startIn : signal.size();
    UInt_t length = (signal.size() > start + lengthIn) ? lengthIn : signal.size() - start;
    
    //pushback zeros to start of signal
    for(int i = 0; i < start; i++) out.push_back(0.0);

    //Found signal so loop though and get curve (3 point moving average of
    // second order centered second derivative, if points can't be used pad with zeros
    for(int i = start; i < length + start; i++)
    {
	//if all the values needed are defined
	if(i > 2 && i < signal.size() - 2)
	    out.push_back( (signal[i-2] - signal[i-1] - signal[i+1] + signal[i+2])/3.0 );
	else
	    out.push_back(0.0);
    }

    //pushback zeros to finish padding
    for(int i = start + length; i < signal.size(); i++) out.push_back(0.0);*/

    return pileupTraceToThreshold(signal, startIn, lengthIn, 0);
}

//gets pileup trace
std::vector<double> SignalProcessor::pileupTraceToThreshold(const std::vector<Long_t> &signal) const
{
    return pileupTraceToThreshold(signal, 0, signal.size()-2, _pileLow);
}
//TODO: make min code work regardless of the sign of threshold
std::vector<double> SignalProcessor::pileupTraceToThreshold(const std::vector<Long_t> &signal,
							    const UInt_t startIn, const UInt_t lengthIn,
							    const Long_t threshold) const
{
    std::vector<double> out;
    std::vector<UInt_t> mins;
    bool reachedThreshold = false;
    int minLoc = 0;
    
    //Lambdas to easily compare. Takes sign of threshold into account
    //true: in < -threshold || in > +threshold; always false if threshold = 0
    auto compare = [=] (Long_t in){ return ( threshold < 0 && in < threshold ) ||
				          ( threshold > 0 && in > threshold ); };
    auto zeroFound = [=] (Long_t in){ return ( threshold < 0 && in > 0 ) ||
				            ( threshold > 0 && in < 0 ); };

    //validate parameters
    UInt_t start = (signal.size() > startIn) ? startIn : signal.size();
    UInt_t length = (signal.size() > start + lengthIn) ? lengthIn : signal.size() - start;
    
    //pushback zeros to start of signal
    for(int i = 0; i < start; i++) out.push_back(0.0);

    //Found signal so loop though and get curve (3 point moving average of
    // second order centered second derivative, if points can't be used pad with zeros
    for(int i = start; i < length + start; i++)
    {
	//if all the values needed are defined
	if(i > 2)
	    out.push_back( signal[i-2] - signal[i-1] - signal[i+1] + signal[i+2] );
	else
	    out.push_back(0.0);

	//Determine if threshold was reached
	reachedThreshold |= compare(out.back());
	
	//return if threshold was reached and we've reached the zero crossing
	if(reachedThreshold && zeroFound(out.back()))
	    return out;
    }

    //pushback zeros to finish padding
    for(int i = start + length; i < signal.size(); i++) out.push_back(0.0);

    return out;
}
std::vector<double> SignalProcessor::pileupTraceToThreshold(const std::vector<Long_t> &signal,
					       const Long_t threshold) const
{
    return pileupTraceToThreshold(signal, 0, signal.size() - 2, threshold);
}


//Uses the change in derivative at the beginning and end of the flat top to
//find the peak value. If the derivative doesn't change as expected, ie it is exeptionally
//smooth, it returns a simple maximum of the trapazoid
Long_t SignalProcessor::peakFind(const std::vector<Long_t>::iterator start,
				 const std::vector<Long_t>::iterator end) const
{
    const double thresh = 0.1;
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
    
    return returnValue;
}

//Threshold low = 0 signifies that the function should to a straight count of the peaks
UInt_t SignalProcessor::peaksPastThreshold(const std::vector<double> &signal, const Long_t threshHigh,
					   const UInt_t distBetweenPeaks) const
{
    return peaksPastThreshold(signal.begin(), signal.end(), threshHigh, distBetweenPeaks);
}
UInt_t SignalProcessor::peaksPastThreshold(std::vector<double>::const_iterator start,
					   std::vector<double>::const_iterator end,
					   const Long_t threshHigh, const UInt_t distBetweenPeaks) const
{
    return peaksPastThreshold(start, end, threshHigh, 0, distBetweenPeaks);
}
UInt_t SignalProcessor::peaksPastThreshold(const std::vector<double> &signal,
					   const Long_t threshHigh, const Long_t threshLow,
					   const UInt_t distBetweenPeaks) const
{
    return peaksPastThreshold(signal.begin(), signal.end(), threshHigh, threshLow, distBetweenPeaks);
}
UInt_t SignalProcessor::peaksPastThreshold(const std::vector<double> &signal) const
{
    return peaksPastThreshold(signal, _pileHigh, _pileLow, _peakLength);
}
UInt_t SignalProcessor::peaksPastThreshold(std::vector<double>::const_iterator start,
					   std::vector<double>::const_iterator end) const
{
    return peaksPastThreshold(start, end, _pileHigh, _pileLow, _peakLength);
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
	if(threshLow != 0 && *it < threshLow)
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
    //std::cout << "min loc: " << minLoc << std::endl;
    UInt_t numPeaks = 0;
    for(auto&& peakLoc:peaks)
	if(peakLoc < minLoc)
	{
		//std::cout << " Peak at: " << num;
	    numPeaks++;
	}
    //std::cout << "num peaks: " << numPeaks<< std::endl;

    return numPeaks;
}

Float_t SignalProcessor::QDC(const std::vector<Long_t> &signal, const UInt_t startIn, const UInt_t lengthIn) const
{
    UInt_t start = (startIn < signal.size()) ? startIn : signal.size() - 1;
    UInt_t length = (start + lengthIn < signal.size()) ? lengthIn : signal.size() - start;
    int Q = 0;
    
    for(int i = 0; i < length; i++)
    {
	Q += signal.at(start+i);
    }
    return Q;
    
}

void SignalProcessor::write(TString fileName) const
{
    std::ofstream file;
    file.open(fileName);

    if(!file.is_open())
	throw std::runtime_error(std::string("Can't open " + fileName));

    file << "# comma deliminated file containing parameters" << std::endl;
    file << "# RiseTime        ,M            ,TopMult" << std::endl;
    file << "# ZeroOffset      ,ZeroThreshold,InterpMult" << std::endl;
    file << "# HighThreshold   ,LowThreshold ,PeakLength" << std::endl;
    file << "# DistanceToSample,PointsToAvg  ,PeakThreshold" << std::endl << std::endl;

    file << _riseTime << "," << _M << "," << _flatMultiplier << std::endl;
    file << _zeroOffset << "," << _zeroThreshold << "," << _interpMult << std::endl;
    file << _pileHigh << "," << _pileLow << "," << _peakLength << std::endl;
    file << _peakDisplacement << "," << _pointsToAverage << "," << _peakThreshold << std::endl;

    file.close();

}

void SignalProcessor::load(TString fileName)
{
    
    //Load file
    std::ifstream file;
    std::vector<Double_t> in;
    file.open(fileName);
    if(!file.is_open())
    {
	std::cout << "Can't open " << fileName << " creating default file" << std:: endl;
	write(fileName);
	return;
    }
    
    //If the file exists read its content 
    while(!file.eof())
    {
	TString temp;
	temp.ReadToken(file);
	
	//If the line should be skipped
	if(temp.Contains("#"))
	{
	    temp.ReadLine(file);
	    continue;
	}
	
	//get the meta data
	TString delim = ",";
	TString token;
	Ssiz_t from = 0;

	while(temp.Tokenize(token, from, delim))
	{
	    in.push_back(token.Atof());
	}
    }

    //Make sure signal is write
    if(in.size() != 12)
	throw std::runtime_error(std::string(fileName + " is formatted incorrectly!"));

    
    //Update signalP
    _riseTime = in.at(0);
    _M = in.at(1);
    _flatMultiplier = in.at(2);

    _zeroOffset = in.at(3);
    _zeroThreshold = in.at(4);
    _interpMult = in.at(5);

    _pileHigh = in.at(6);
    _pileLow = in.at(7);
    _peakLength = in.at(8);
    
    _pointsToAverage = in.at(9);
    _peakDisplacement = in.at(10);
    _peakThreshold = in.at(11);

    std::cout <<"Loaded file";
    

    file.close();
    
}

//*********Private functions ********************

std::vector<double> SignalProcessor::nonInterpDeriv(const std::vector<Long_t> &signal) const
{
    std::vector<double> out;
    out.push_back(0);
    for(int i = 1; i < signal.size(); i++)
	out.push_back(signal[i] - signal[i-1]);

    return out;
}

std::vector<double> SignalProcessor::nonInterpSecondDeriv(const std::vector<Long_t> &signal) const
{
    std::vector<double> out;
    out.push_back(0);
    for(int i = 1; i < signal.size()-1; i++)
	out.push_back(signal[i-1] - 2*signal[i] + signal[i+1]);
    out.push_back(0);

    return out;
}


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
    int k = (int) _riseTime;
    int l = (int) (_flatMultiplier * _riseTime + k);

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
Long_t SignalProcessor::s(const UInt_t n) const
{
    Long_t ret = (_p[n] + d_kl[n] * _M);

    //Base case
    if(n < 1)
	return ret;

    //Not base
    ret += s(n-1);
    
    return ret;
}
