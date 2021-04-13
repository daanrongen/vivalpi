//
//  instrument.cpp
//  vivalpi
//
//  Created by Daan Rongen on 13/04/2021.
//

#include "instrument.h"
#include "ofMain.h"

Instrument::Instrument() {
    frequency = 200;
    a = 1.0;
    vol = 1.0;
}

Instrument::~Instrument() {}

void Instrument::setEnv(int attack, int decay, int sustain, int release) {
    env.setAttack(attack);
    env.setDecay(decay);
    env.setSustain(sustain);
    env.setRelease(release);
}

void Instrument::setFrequency(double _f, float _range) {
    frequency = (_f / _range) + (rand() % static_cast<int>((_f * _range) - (_f / _range) + 1));
}

void Instrument::setVolume(float _vol) {
    vol = _vol;
}

double Instrument::play() {
    return env.adsr(osc.sinewave(frequency), env.trigger) * vol;
}

void Instrument::activate() {
    env.trigger = 1;
}

void Instrument::deactivate() {
    env.trigger = 0;
}
