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

//Class to process digital signals
//Include methods for trapazoidal filtering
//and Constant Fraction discrimination
//Adam Anthony 6/30/16

#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

//inlude files

#include "TObject.h"
#include "TMath.h"
#include "Math/Interpolator.h"
#ifndef __CINT__
#include <vector>
#include <iterator>
#endif

class SignalProcessor : public TObject
{
private:
    int _decayTime;
    double _flatMultiplier;
    double _stepSize;
    double _scaling;
    int _offset;
    int _threshold;
    int _M;
    Double_t _peakThreshold;
    UInt_t _interpMult;

    mutable ROOT::Math::Interpolator* _inter;
    mutable Long_t* d_kl;
    mutable Long_t* _p;

    void setInter(const std::vector<Long_t>&) const;

    void trapFilter(Long_t*, const UInt_t) const;
    void setD_kl(Long_t*, const UInt_t) const;
    Long_t setP(Long_t*, const UInt_t) const;
    void prepP(const UInt_t) const;
    void p(Long_t*, const UInt_t) const;
    Long_t s(Long_t*, const UInt_t) const;
    std::vector<double> nonInterpDeriv(const std::vector<Long_t>&) const;
    std::vector<double> nonInterpSecondDeriv(const std::vector<Long_t>&) const;

    
public:
    SignalProcessor(const UInt_t riseTime=3770,   const UInt_t M=3770,
		    const Double_t flatMult=0.05, const Double_t peakThreshold=0.1,
		    const UInt_t zeroOffset=60, const Int_t zeroThreshold=-5,
		    const UInt_t interpMult=1, const Double_t scalingMult=0.8);
    ~SignalProcessor();
    
    Int_t getDecayTime() const;
    Double_t getFlatMult() const;
    Double_t getM() const;
    Int_t getOffset() const;
    Double_t getScaling() const;
    Int_t getThreshold() const;
    UInt_t getInterpMult() const;
    Double_t getPeakThreshold() const;

    void trapFilter(std::vector<Long_t> *signal, const UInt_t start, const UInt_t length) const;
    Float_t QDC(const std::vector<int> &signal, const UInt_t start, const UInt_t length) const;

    std::vector<Double_t> CFD(const std::vector<Double_t> &signal, const UInt_t start,
			      const UInt_t length, const Int_t shift) const;
    std::vector<Double_t> CFD(const std::vector<Double_t> &signal, const Int_t shift) const;
    std::vector<Double_t> CFD(const std::vector<Double_t> &signal) const;
    
    int zeroAfterThreshold(const std::vector<double> &signal, const int threshold) const;
    int zeroAfterThreshold(const std::vector<double> &signal) const;

    std::vector<double> deriv(const std::vector<Long_t> &signal) const;
    std::vector<double> deriv(const std::vector<Int_t> &signal) const;

    std::vector<double> secondDeriv(const std::vector<Long_t> &signal) const;

    std::vector<double> pileupTrace(const std::vector<Long_t> &signal) const;
    std::vector<double> pileupTrace(const std::vector<Long_t> &signal, const UInt_t start,
				    const UInt_t length) const;
    
    std::vector<double> pileupTraceToThreshold(const std::vector<Long_t> &signal,
					       const Long_t threshold) const;
    std::vector<double> pileupTraceToThreshold(const std::vector<Long_t> &signal, const UInt_t start,
					       const UInt_t length, const Long_t threshold) const;

    Long_t peakFind(const std::vector<Long_t>::iterator start,
		    const std::vector<Long_t>::iterator end) const;

    UInt_t peaksPastThreshold(std::vector<double>::const_iterator start,
			      std::vector<double>::const_iterator end,
			      const Long_t threshHigh, const Long_t threshLow,
			      const UInt_t distBetweenPeaks) const;
    UInt_t peaksPastThreshold(const std::vector<double> &signal, const Long_t threshHigh,
			      const Long_t threshLow, const UInt_t distBetweenPeaks) const;
    UInt_t peaksPastThreshold(std::vector<double>::const_iterator start,
			      std::vector<double>::const_iterator end,
			      const Long_t threshHigh,
			      const UInt_t distBetweenPeaks) const;
    UInt_t peaksPastThreshold(const std::vector<double> &signal, const Long_t threshHigh,
			     const UInt_t distBetweenPeaks) const;

    ClassDef(SignalProcessor,0)
};

#endif
