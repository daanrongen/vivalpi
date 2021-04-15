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
    
    ofxFloatSlider attack, decay, sustain, release;
    ofxFloatSlider cutoff, resonance;
    ofxFloatSlider lowpass, highpass, bandpass, notch;
    ofxFloatSlider pitch, kickRelease, distortion;
    
    maxiOsc osc, mod;
    maxiFilter filter;
    convert mtof;
    maxiMix mix;
    
    maxiClock clock;
    maxiClock rain_clock_1, rain_clock_2;
    maxiClock lead_clock;
    maxiClock wind_clock;
    
    maxiKick kick;
    maxiHats snare;
    maxiHats hihat;
    
    Instrument rain;
    Instrument metronome;
    Instrument lead;
    Instrument wind;
    
    float waveform[4096];
    int waveIndex;
    double outputs[2];
    int playhead;
    
    int notes[16];
    int chords[8];
    ofVec3f chordsRange;
    ofVec3f notesRange;
    int notesRangeMin, notesRangeMax;
    
    int rain_trigger[64];
    float rain_volume_a, rain_volume_b, rain_feedback_a, rain_feedback_b;
    double rain_frequency, rain_frequency2, rain_modulation_index;
    
    int lead_chord, lead_prev_chord;
    float lead_volume, lead_feedback, lead_attack;
    double lead_cutoff, lead_resonance;
    bool lead_motif_changed;
    
    int wind_trigger[4] = {1, 0, 0, 0};
    int wind_chord;
    float wind_volume, wind_feedback;
    double wind_cutoff, wind_resonance;
    
    float kick_feedback;
    double kick_pitch, kick_release;
    
    float hihat_volume, hihat_feedback;
    double hihat_pitch, hihat_release;

    float snare_volume, snare_feedback;
    double snare_pitch, snare_release;
};
