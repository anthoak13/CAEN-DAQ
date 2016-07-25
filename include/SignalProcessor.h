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

    void trapFilter(Long_t*, const int);
    void setD_kl(Long_t*, const int);
    Long_t setP(Long_t*, const int);
    void prepP(const int);
    void p(Long_t*, const int);
    Long_t s(Long_t*, const int);
    std::vector<double> nonInterpDeriv(const std::vector<Long_t>&);

    
public:
    SignalProcessor(const UInt_t riseTime=12,   const UInt_t M=12,
		    const Double_t flatMult=2.0/3.0, const Double_t peakThreshold=0.1,
		    const UInt_t zeroOffset=6, const Int_t zeroThreshold=-20,
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

    void trapFilter(std::vector<Long_t> *signal, const int start, const int length);
    Float_t QDC(const std::vector<int> &signal, const int start, const int length);

    void CFD(std::vector<double>*, const int, const int, const int);
    void CFD(std::vector<double>*, const int);
    void CFD(std::vector<double>*);
    
    int zeroAfterThreshold(std::vector<double> *signal, const int);
    int zeroAfterThreshold(std::vector<double> *signal);

    std::vector<double> deriv(const std::vector<int> &signal);
    std::vector<double> deriv(const std::vector<Long_t> &signal);

    Long_t peakFind(std::vector<Long_t>::iterator, std::vector<Long_t>::iterator);

    ClassDef(SignalProcessor,0)
};

#endif
