//
//  instrument.h
//  vivalpi
//
//  Created by Daan Rongen on 13/04/2021.
//

#ifndef _INSTRUMENT
#define _INSTRUMENT

#include "ofMain.h"
#include "ofxMaxim.h"
#include <stdio.h>

class Instrument {
private:
    maxiOsc osc;
    maxiEnv env;
    maxiFilter filter;
    maxiSVF svf;
    
    double frequency;
    float vol;
    double lowpass, highpass, bandpass, notch;

public:
    Instrument();
    ~Instrument();
    
    void setEnv(int _a, int _d, int _s, int _r);
    void setFrequency(double _f, float _v = 1.0);
    void setVolume(float _vol);
    void setFilterMix(double _lp, double _hp, double _bp, double _n);
    
    double play();
    void activate();
    void deactivate();
};

#endif /* _INSTRUMENT */
