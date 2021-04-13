#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class Instrument {
public:
    maxiOsc osc;
    maxiEnv env;
    maxiFilter filter;
    
    double frequency;
    float a = 1.0;
    float vol = 1.0;
    
    void setEnv(int attack, int decay, int sustain, int release) {
        env.setAttack(attack);
        env.setDecay(decay);
        env.setSustain(sustain);
        env.setRelease(release);
    }
    
    void setFrequency(double _f, float _range = 1.0) {
        frequency = (_f / _range) + (rand() % static_cast<int>((_f * _range) - (_f / _range) + 1));
    }
    
    void setVolume(float _vol) {
        vol = _vol;
    }
    
    double play() {
        return env.adsr(osc.sinewave(frequency), env.trigger) * vol;
    }
    
    void activate() {
        env.trigger = 1;
    }
    
    void deactivate() {
        env.trigger = 0;
    }
};

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void audioOut(ofSoundBuffer& output) override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    ofSoundStream soundStream;
    ofxMaxiFFT fft;
    ofxMaxiFFTOctaveAnalyzer oct;
//    ofFile file;
    
    ofxGuiGroup gui;
    ofxPanel instrumentGui, weatherGui;
    ofxLabel instrumentSettings, weatherSettings;
    ofxFloatSlider metronomeVolume;
    ofxFloatSlider temperature;
    ofxFloatSlider humidity;
    ofxFloatSlider clouds;
    ofxFloatSlider precipitation;
    ofxFloatSlider windSpeed;
    ofxFloatSlider seasonProgression;
    ofxFloatSlider sunPosition;
    ofxToggle bassToggle;
    
    maxiOsc osc, mod;
    maxiClock clock;
    maxiClock rain_clock_1, rain_clock_2;
    convert mtof;
    maxiMix mix;
    
    float waveform[4096];
    int waveIndex;
    double outputs[2];
    int playhead;
    
    Instrument rain;
    Instrument metronome;
    Instrument bass;
    
    int rain_trigger[128];
    float rain_volume_a, rain_volume_b, rain_feedback_a, rain_feedback_b;
    double rain_frequency, rain_frequency2, rain_modulation_index;
};
