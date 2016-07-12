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
    //std::vector<int> d_kl;
    //std::vector<int> _p;
    ROOT::Math::Interpolator* _inter;

    int* d_kl;
    int* _p;

    //void setD_kl(std::vector<int>*, const int, const int);
    //void setP(const int);
    //int s(const int);
    void setInter(std::vector<int>*);

    void trapFilter(int*, const int);
    void setD_kl(int*, const int);
    int setP(int*, const int);
    void prepP(const int);
    void p(int*, const int);
    int s(int*, const int);

    
public:
    SignalProcessor();
    ~SignalProcessor();

    void setDecayTime(const int);
    void setFlatMult(const double);
    void setM(const double);
    void setOffset(const int);
    void setScaling(const double);
    void setThreshold(const int);
    
    Int_t getDecayTime();
    Double_t getFlatMult();
    Double_t getM();
    Int_t getOffset();
    Double_t getScaling();
    Int_t getThreshold();

    void trapFilter(std::vector<int>*, const int, const int);

    void CFD(std::vector<double>*, const int, const int, const int);
    void CFD(std::vector<double>*, const int);
    void CFD(std::vector<double>*);
    
    int zeroAfterThreshold(std::vector<double>*, const int);
    int zeroAfterThreshold(std::vector<double>*);

    std::vector<double> interpolateDeriv(std::vector<int>*, const int);

    int peakFind(std::vector<int>::iterator, std::vector<int>::iterator);

    ClassDef(SignalProcessor,0)
};

#endif
