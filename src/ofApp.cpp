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
    float windSpeed = 4.6;
    return windSpeed;
}

float getTemperature() {
    // PERFORM API REQUEST HERE
    float temperature = 39.0;
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

float getSeasonValue() {
    time_t now = time(0);
    tm *local_time = localtime(&now);
    int month = 1 + local_time -> tm_mon;
    int day = local_time -> tm_mday;
    
    int monthProgression = ofMap(day, 1, 31, 0.0, 0.9);
    return (float)month + monthProgression;
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
    gui.add(clouds.setup("Clouds (%)", getCloudiness(), 0.0, 100.0));
    gui.add(precipitation.setup("Rain (mm/s)", getPrecipitation(), 0.0, 300.0));
    gui.add(windSpeed.setup("Wind Speed (m/s)", getWindSpeed(), 0.0, 200.0));
    
    gui.add(cutoff.setup("Cutoff", 40, 0, 100));
    gui.add(resonance.setup("Resonance", 60, 0, 200));
    gui.add(attack.setup("Attack", 1, 0, 1000));
    gui.add(decay.setup("Decay", 1, 0, 1000));
    gui.add(sustain.setup("Sustain", 1, 0, 1000));
    gui.add(release.setup("Release", 1, 0, 2000));
    
    gui.add(lowpass.setup("Low Pass", 0.5, 0.0, 1.0));
    gui.add(highpass.setup("High Pass", 0.5, 0.0, 1.0));
    gui.add(bandpass.setup("Band Pass", 0.5, 0.0, 1.0));
    gui.add(notch.setup("Notch", 0.5, 0.0, 1.0));
    
    gui.add(pitch.setup("Pitch", 12000, 0, 20000));
    gui.add(distortion.setup("Distortion", 0.0, 0.0, 2.0));
    
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
    playhead = 0;
    
//    setMotif(notes, chords, temperature, temperature.getMin(), temperature.getMax());
    setMotif(notes, chords, getTemperature(), -10.0, 40.0);
    
    metronome.setEnv(1, 1, 1, 1000);
    metronome.setFrequency(640);
    
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
    
    lead_clock.setTempo(clock.bpm);
    lead_clock.setTicksPerBeat(clock.ticks);
    lead_chord = lead_prev_chord = 0;
    lead_motif_changed = false;
    lead_attack = 1000;
    lead_cutoff = 50;
    lead_resonance = 65;
    lead.setEnv(lead_attack, 1, 1, 30);
    
    wind_clock.setTempo(clock.bpm);
    wind_clock.setTicksPerBeat(clock.ticks);
    wind.setEnv(1000, 1, 1, 200);
    wind.setFilterMix(0.10, 0.0, 0.5, 0.25);
    
//    kick.setPitch(180);
//    kick.setRelease(650);
    
    kick_pitch = 180;
    kick_release = 650;
    hihat_pitch = 12000;
    hihat_release = 800;
    snare_pitch = 1000;
    snare_release = 140;
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
        lead_clock.ticker();
        wind_clock.ticker();
        
        if (clock.tick) {
            playhead++;
            
            if (playhead % 4 == 0) metronome.activate();
            else metronome.deactivate();
            
            if (playhead % 8 == 0 || randomFloat() > 0.95) {
                kick_pitch = 180;
                kick_release = 650;
                kick_feedback = 1.0 - randomFloat() * 0.0001;
                kick.trigger();
            };
            
            if (playhead % 8 == 2 || randomFloat() > 0.95) {
                snare_volume = 0.6;
                snare_pitch = 1000;
                snare_release = 140;
                snare_feedback = 1.0 - randomFloat() * 0.001;
                snare.trigger();
            }
                
            if (playhead % 4 == 0 || randomFloat() > 0.50) {
                hihat_volume = 0.15;
                hihat_pitch = 12000;
                hihat_release = 800;
                hihat_feedback = 1.0 - randomFloat() * 0.001;
                hihat.trigger();
            };
            
            if (playhead % 512 == 0) {
                setMotif(notes, chords, getTemperature(), -10.0, 40.0);
                cout << "motif changed" << endl;
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
        rain.setVolume(rain_volume_a + 0.5);
        
        if (lead_clock.tick) {
            if (playhead % 32 == 0) lead_chord++;
            
            lead_volume = 0.5;
            lead_feedback = 1.0 - randomFloat() * 0.0001;
            lead.activate();
        }
        
        lead_volume, lead_attack *= lead_feedback;
        lead.setFrequency(filter.hires(mtof.mtof(notes[playhead % 16] + chords[lead_chord % 8]), cutoff, resonance));
        lead.setVolume(lead_volume);
        lead.setEnv(lead_attack + randomInt(20, 200), randomInt(1, 50), 1, 300);
        
        if (wind_clock.tick) {
            if (playhead % 8 == 0) wind.activate();
        }
        
        wind.setFrequency(mtof.mtof(notes[lead_chord % 8]));
        
        
        float beat = kick.play() + (snare.play() * snare_volume) + (hihat.play() * hihat_volume);
        
//        float out = kick.play() + (hihat.play() * hihat_volume) + rain.play() + lead.play();
        float out = lead.play() + beat;
        
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
