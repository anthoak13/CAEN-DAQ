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
    ROOT::Math::Interpolator* _inter;

    Long_t* d_kl;
    Long_t* _p;

    void setInter(const std::vector<Long_t>&);

    void trapFilter(Long_t*, const UInt_t);
    void setD_kl(Long_t*, const UInt_t);
    Long_t setP(Long_t*, const UInt_t);
    void prepP(const UInt_t);
    void p(Long_t*, const UInt_t);
    Long_t s(Long_t*, const UInt_t);
    std::vector<double> nonInterpDeriv(const std::vector<Long_t>&);
    std::vector<double> nonInterpSecondDeriv(const std::vector<Long_t>&);

    
public:
    SignalProcessor(const UInt_t riseTime=3770,   const UInt_t M=3770,
		    const Double_t flatMult=0.05, const Double_t peakThreshold=0.1,
		    const UInt_t zeroOffset=60, const Int_t zeroThreshold=-5,
		    const UInt_t interpMult=1, const Double_t scalingMult=0.8);
    ~SignalProcessor();
    
    Int_t getDecayTime();
    Double_t getFlatMult();
    Double_t getM();
    Int_t getOffset();
    Double_t getScaling();
    Int_t getThreshold();
    UInt_t getInterpMult() { return _interpMult;}
    Double_t getPeakThreshold() { return _peakThreshold;}

    void trapFilter(std::vector<Long_t> *signal, const UInt_t start, const UInt_t length);
    Float_t QDC(const std::vector<int> &signal, const UInt_t start, const UInt_t length);

    void CFD(std::vector<double> *signal, const UInt_t start, const UInt_t length, const int shift);
    void CFD(std::vector<double>*, const int);
    void CFD(std::vector<double>*);
    
    int zeroAfterThreshold(const std::vector<double> &signal, const int threshold);
    int zeroAfterThreshold(const std::vector<double> &signal);

    std::vector<double> deriv(const std::vector<Long_t> &signal);
    std::vector<double> deriv(const std::vector<Int_t> &signal);

    std::vector<double> secondDeriv(const std::vector<Long_t> &signal);

    Long_t peakFind(const std::vector<Long_t>::iterator start, const std::vector<Long_t>::iterator end);

    UInt_t peaksPastThreshold(std::vector<double>::const_iterator start,
			      std::vector<double>::const_iterator end,
			      const Long_t threshHigh, const Long_t threshLow,
			      const UInt_t distBetweenPeaks);
    
    UInt_t peaksPastThreshold(const std::vector<double> &signal, const Long_t threshHigh,
			      const Long_t threshLow, const UInt_t distBetweenPeaks);

    ClassDef(SignalProcessor,0)
};

#endif
