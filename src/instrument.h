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
    
    double frequency;
    float vol;

public:
    Instrument();
    ~Instrument();
    
    void setEnv(int attack, int decay, int sustain, int release);
    void setFrequency(double _f, float _range = 1.0);
    void setVolume(float _vol);
    
    double play();
    void activate();
    void deactivate();
};

#endif /* _INSTRUMENT */
