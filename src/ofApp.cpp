#include "ofApp.h"
#include "maximilian.h"
#include "time.h"

//--------------------------------------------------------------
int randomInt(int min, int max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

float randomFloat() {
    return (float)rand() / RAND_MAX;
}

void setMotif(int notes[16], int chords[8], float customizer, float cMin, float cMax) {
    ofVec3f chordsRange = {floor(ofMap(customizer, cMin, cMax, 2, 9)), floor(ofMap(customizer, cMin, cMax, 5, 12))};
    ofVec3f notesRange = {floor(ofMap(customizer, cMin, cMax, 36, 48)), floor(ofMap(customizer, cMin, cMax, 59, 83))};
    
    for (int i = 0; i < 16; i++) notes[i] = randomInt(notesRange.x, notesRange.y);
    for (int i = 0; i < 8; i++) chords[i] = randomInt(chordsRange.x, chordsRange.y);
}

float getPrecipitation() {
    // PERFORM API REQUEST HERE
    int precipitation = 160;
    return precipitation;
}

float getWindSpeed() {
    // PERFORM API REQUEST HERE
    float windSpeed = 15.6;
    return windSpeed;
}

float getTemperature() {
    // PERFORM API REQUEST HERE
    float temperature = 15.0;
    return temperature;
}

float getHumidity() {
    // PERFORM API REQUEST HERE
    float humidity = 70.0;
    return humidity;
}

float getCloudiness() {
    // PERFORM API REQUEST HERE
    float cloudiness = 10.0;
    return cloudiness;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetupScreen();
    ofSetFrameRate(60);
    
    gui.setup();
    gui.setBackgroundColor(255);
    gui.setHeaderBackgroundColor(255);
    gui.add(instrumentSettings.setup("Instrument Settings", ""));
    gui.add(weatherSettings.setup("Weather Settings", ""));
    gui.add(temperature.setup("Temperature (ºC)", getTemperature(), -10.0, 40.0));
    gui.add(humidity.setup("Humidity (%)", getHumidity(), 0.0, 100.0));
    gui.add(precipitation.setup("Rain (mm/s)", getPrecipitation(), 0.0, 300.0));
    gui.add(windSpeed.setup("Wind Speed (m/s)", getWindSpeed(), 0.0, 150.0));
    
    gui.add(cutoff.setup("Cutoff", 0.0, 0.0, 1.0));
    gui.add(resonance.setup("Resonance", 8, 0, 50));
    
    gui.add(attack.setup("Attack", 1000, 0, 5000));
    gui.add(decay.setup("Decay", 1, 0, 10));
    gui.add(sustain.setup("Sustain", 1, 0, 10));
    gui.add(release.setup("Release", 200, 0, 5000));
    
    gui.add(lowpass.setup("Low Pass", 0.10, 0.0, 1.0));
    gui.add(highpass.setup("High Pass", 0.0, 0.0, 1.0));
    gui.add(bandpass.setup("Band Pass", 0.5, 0.0, 1.0));
    gui.add(notch.setup("Notch", 0.25, 0.0, 1.0));
    
    gui.add(pitch.setup("Pitch", 0.14, 0.0, 5.0));
    gui.add(distortion.setup("Distortion", 2000, 0, 4000));
    
    int sampleRate = 44100;
    int bufferSize= 512;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    fft.setup(ofGetWidth(), bufferSize, 256);
    oct.setup(sampleRate, ofGetWidth(), 10);
    
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    clock.setTempo(120);
    clock.setTicksPerBeat(4);
    setMotif(notes, chords, temperature, temperature.getMin(), temperature.getMax());
    playhead, progression = 0;
    
    arrangement = 0;
    wind_isPlaying, rain_isPlaying = true;
    bass_isPlaying, beat_isPlaying = false;
        
    rain_clock_1.setTempo(clock.bpm);
    rain_clock_1.setTicksPerBeat(clock.ticks);
    rain_volume_a = 0.5;
    rain_volume_b = 0.5;
    rain_frequency = 600;
    rain_frequency2 = 150;
    rain_modulation_index = 650;
    rain_feedback_a = 0.9999;
    rain_feedback_b = 0.9999;
    rain.setEnv(1000, 1, 1, 300);
    
    bass_clock.setTempo(clock.bpm);
    bass_clock.setTicksPerBeat(clock.ticks);
    bass_chord = 0;
    bass_attack = 1000;
    bass_cutoff = 50;
    bass_resonance = 65;
    bass.setEnv(bass_attack, 1, 1, 30);
    
    wind_clock.setTempo(clock.bpm);
    wind_clock.setTicksPerBeat(clock.ticks);
    wind.setFilterMix(0.6, 0.4, 0.5, 0.85);
    wind.setEnv(1000, 1, 1, 200);
    wind_cutoff = 0;
    wind_resonance = 8;
    wind_stretch = 3750;
    wind_speed = 0.15;

    kick_pitch = 180;
    kick_release = 650;
    hihat_pitch = 12000;
    hihat_release = 800;
    snare_pitch = 1000;
    snare_release = 140;
    beat_volume = 1.0;
}

void ofApp::update(){
    ofBackground(255);
    
    for (int i = 0; i < 64; i++) {
        if (i <= floor(ofMap(precipitation, 0, 800, 0, 64))) rain_trigger[i] = 1;
        else rain_trigger[i] = 0;
    }
    random_shuffle(&rain_trigger[0], &rain_trigger[63]);
}

void ofApp::draw(){
    gui.draw();
    
    for (int i = 0; i < oct.nAverages; i++) {
        ofColor color;
        color.setHsb((int)ofMap(i, 0, oct.nAverages, 0, 255), 255, 255);
        ofSetColor(color);
        float size = 1 + oct.averages[i] / 10;
        int x = (int) ((ofGetWidth() / 2) * i / oct.nAverages) + ofGetWidth() / 4;
        ofDrawCircle(x, ofGetHeight() / 2, size);
    }
}

void ofApp::audioOut(ofSoundBuffer& output){
    std::size_t outChannels = output.getNumChannels();
    
    for (int i = 0; i < output.getNumFrames(); i++) {
        clock.ticker();
        rain_clock_1.ticker();
        rain_clock_2.ticker();
        bass_clock.ticker();
        wind_clock.ticker();
        
        if (clock.tick) {
            playhead++;
            
            if (playhead % 8 == 0 || randomFloat() > 0.95) {
                kick_pitch = 180;
                kick_release = 650;
                kick_feedback = 1.0 - randomFloat() * 0.001;
                kick.trigger();
            };
            
            if (playhead % 8 == 2 || randomFloat() > 0.95) {
                snare_volume = 0.5;
                snare_pitch = 1000;
                snare_release = 140;
                snare_feedback = 1.0 - randomFloat() * 0.001;
                snare.trigger();
            }
                
            if (playhead % 4 == 0 || randomFloat() > 0.50) {
                hihat_volume = 0.12;
                hihat_pitch = 12000;
                hihat_release = 800;
                hihat_feedback = 1.0 - randomFloat() * 0.001;
                hihat.trigger();
            };
            
            if (playhead % 256 == 0) {
                setMotif(notes, chords, temperature, temperature.getMin(), temperature.getMax());
                cout << "motif changed" << endl;
            }
            
            if (playhead % 64 == 0 && randomFloat() > 0.15) {
                wind_isPlaying = randomFloat() > 0.3 ? true : false;
                rain_isPlaying = randomFloat() > 0.5 ? true : false;
                bass_isPlaying = randomFloat() > 0.3 ? true : false;
                beat_isPlaying = randomFloat() > 0.4 ? true : false;
                cout << "arrangement changed" << endl;
            }
            
            rain_clock_1.setTicksPerBeat(floor(1 + randomFloat() * 5));
        }
        
        kick_pitch, kick_release *= kick_feedback;
        kick.setRelease(kick_release);
        kick.setPitch(kick_pitch);
        
        hihat_volume, hihat_pitch, hihat_release *= hihat_feedback;
        hihat.setRelease(hihat_release);
        hihat.setPitch(hihat_pitch);
        
        snare_volume, snare_pitch, snare_release *= snare_feedback;
        snare.setPitch(snare_pitch);
        snare.setRelease(snare_release);
        
        if (rain_clock_1.tick) {
            if (rain_trigger[playhead % 64] == 1) rain.activate();
            else rain.deactivate();
            
            rain_frequency = randomInt(700, 1000);
            rain_frequency2 = randomInt(0, 10);
            rain_modulation_index = randomInt(0, 600);
            rain_volume_a = 0.6;
            rain_feedback_a = 1.0 - randomFloat() * 0.001;
        }
        
        if (rain_clock_2.tick) {
            rain_frequency = 600;
            rain_frequency2 = 150;
            rain_modulation_index = 10;
            rain_volume_a = 1.0;
            rain_volume_b = 0.5;
            
            if (randomFloat() > 0.5) rain_feedback_b = 1.00001;
            else rain_feedback_b = 0.9999;
        }
        
        rain_volume_a *= rain_feedback_a;
        rain_volume_b *= rain_feedback_b;
        
        rain.setFrequency((rain_frequency * rain_volume_b) + (mod.sinewave(rain_frequency2 * rain_feedback_b) * rain_modulation_index));
        rain.setVolume(rain_volume_a + 0.75);
        
        if (bass_clock.tick) {
            if (playhead % 32 == 0) bass_chord++;
            
            bass_volume = 0.4;
            bass_feedback = 1.0 - randomFloat() * 0.0001;
            bass_cutoff = ofMap(temperature, temperature.getMin(), temperature.getMax(), 3, 60);
            bass_resonance = ofMap(temperature, temperature.getMin(), temperature.getMax(), 5, 80);
            bass.activate();
        }
        
        bass_volume, bass_attack *= bass_feedback;
        bass.setFrequency(bass_filter.hires(mtof.mtof(notes[playhead % 16] + chords[bass_chord % 8]), bass_cutoff, bass_resonance));
        bass.setVolume(bass_volume);
        bass.setEnv(bass_attack + randomInt(20, 200), randomInt(1, 50), 1, 300);
        
        if (wind_clock.tick) {
            if (playhead % 8 == 0) {
                wind_volume = ofMap(windSpeed, windSpeed.getMin(), windSpeed.getMax(), 0.0, 0.3);
                wind_stretch = ofMap(windSpeed, windSpeed.getMin(), windSpeed.getMax(), 1750, 4000);
                wind_speed = ofMap(windSpeed, windSpeed.getMin(), windSpeed.getMax(), 0.1, 0.5);
                wind.activate();
            }
        }
        
        wind.setFrequency(wind_filter.lores(mtof.mtof(randomInt(0, 11) + 84), pow(((osc.coswave(wind_speed) + 1) / 2), 2) * wind_stretch + 100, wind_resonance));
        wind.setVolume(wind_volume);
        
        float beat = (kick.play() + (snare.play() * snare_volume) + (hihat.play() * hihat_volume)) * beat_volume;
        
        arrangement = (beat_isPlaying * beat) + (wind_isPlaying * wind.play()) + (rain_isPlaying * rain.play()) + (bass_isPlaying * bass.play());
        
        float out = arrangement;
        
        if (fft.process(out)) oct.calculate(fft.magnitudes);
        mix.stereo(out, outputs, 0.5);
        output[i * outChannels] = outputs[0];
        output[i * outChannels + 1] = outputs[1];
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
