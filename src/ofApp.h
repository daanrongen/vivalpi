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
    ofxFloatSlider pitch, distortion;
    
    maxiOsc osc, mod;
    maxiFilter bass_filter, wind_filter;
    convert mtof;
    maxiMix mix;
    
    maxiClock clock;
    maxiClock rain_clock_1, rain_clock_2;
    maxiClock bass_clock;
    maxiClock wind_clock;
    
    maxiKick kick;
    maxiHats snare;
    maxiHats hihat;
    
    Instrument rain;
    Instrument bass;
    Instrument wind;
    
    float waveform[4096];
    int waveIndex;
    double outputs[2];
    int playhead, progression;
    float arrangement;
    
    int notes[16];
    int chords[8];
    ofVec3f chordsRange;
    ofVec3f notesRange;
    
    int rain_trigger[64];
    float rain_volume_a, rain_volume_b, rain_feedback_a, rain_feedback_b;
    double rain_frequency, rain_frequency2, rain_modulation_index;
    bool rain_isPlaying;
    
    int bass_chord;
    float bass_volume, bass_feedback, bass_attack;
    double bass_cutoff, bass_resonance;
    bool bass_isPlaying;
    
    int wind_chord;
    float wind_volume, wind_feedback, wind_speed, wind_stretch;
    double wind_cutoff, wind_resonance;
    bool wind_isPlaying;
    
    bool beat_isPlaying;
    float beat_volume;
    float kick_feedback;
    double kick_pitch, kick_release;
    float hihat_volume, hihat_feedback;
    double hihat_pitch, hihat_release;
    float snare_volume, snare_feedback;
    double snare_pitch, snare_release;
    
};
