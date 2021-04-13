#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "instrument.h"

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
    
    ofxPanel gui;
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
    
    //    ofxFloatSlider attack, decay, sustain, release;
    //    ofxFloatSlider cutoff, resonance;
    
    maxiOsc osc, mod;
    maxiClock clock;
    maxiClock rain_clock_1, rain_clock_2;
    maxiClock lead_clock;
    maxiFilter filter;
    convert mtof;
    maxiMix mix;
    
    Instrument rain;
    Instrument metronome;
    Instrument lead;
    
    float waveform[4096];
    int waveIndex;
    double outputs[2];
    int playhead;
    
    int rain_trigger[128];
    float rain_volume_a, rain_volume_b, rain_feedback_a, rain_feedback_b;
    double rain_frequency, rain_frequency2, rain_modulation_index;
    
    int notes[32];
    int chords[16];
    int lead_chord, lead_prev_chord;
    float lead_volume, lead_feedback;
    double lead_cutoff, lead_resonance;
    bool lead_motif_changed;
};
