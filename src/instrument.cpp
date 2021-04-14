//
//  instrument.cpp
//  vivalpi
//
//  Created by Daan Rongen on 13/04/2021.
//

#include "instrument.h"
#include "ofMain.h"

Instrument::Instrument() {
    vol = 1.0;
    frequency = 0;
    lowpass, highpass, bandpass, notch = 0.5;
}

Instrument::~Instrument() {}

void Instrument::setEnv(int _a, int _d, int _s, int _r) {
    env.setAttack(_a);
    env.setDecay(_d);
    env.setSustain(_s);
    env.setRelease(_r);
}

void Instrument::setFrequency(double _f, float _v) {
    frequency = (_f / _v) + (rand() % static_cast<int>((_f * _v) - (_f / _v) + 1));
}

void Instrument::setVolume(float _vol) {
    vol = _vol;
}

void Instrument::setFilterMix(double _lp, double _hp, double _bp, double _n) {
    lowpass = _lp;
    highpass = _hp;
    bandpass = _bp;
    notch = _n;
}

double Instrument::play() {
    return env.adsr(svf.play(osc.sinewave(frequency), lowpass, highpass, bandpass, notch), env.trigger) * vol;
}

void Instrument::activate() {
    env.trigger = 1;
}

void Instrument::deactivate() {
    env.trigger = 0;
}
