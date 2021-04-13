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
    float temperature = 12.0;
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

float getSunPosition() {
    // PERFORM API REQUEST HERE
    int sunrise = 1560343627;
    int sunset = 1560396563;
    int now = 1560392563;
    int dayPeak = sunrise + ((sunset - sunrise) / 2);
    int nightPeak = (sunrise + 86400) - sunset;
    
    if (now < sunset && now < dayPeak) return ofMap(now, sunrise, dayPeak, 0.0, 1.0); // between sunrise and daypeak
    else if (now < sunset && now > dayPeak) return ofMap(now, dayPeak, sunset, 1.0, 0.0); // between daypeak and sunset
    else if (now > sunset && now < nightPeak) return ofMap(now, sunset, nightPeak, 0.0, -1.0); // between sunset and nightpeak
    else if (now > sunset && now > nightPeak) return ofMap(now, nightPeak, sunrise + 86400, -1.0, 0.0); // between nightpeak and sunrise
    else return NULL;
}

float getSeasonValue() {
    time_t now = time(0);
    tm *local_time = localtime(&now);
    int month = 1 + local_time -> tm_mon;
    int day = local_time -> tm_mday;
    
    int monthProgression = ofMap(day, 1, 31, 0.0, 0.9);
    return (float)month + monthProgression;
}

/*
 wind sound: noise with and the resonant (resonance / cue) filter peak wind sound
 */

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetupScreen();
    ofSetFrameRate(60);
    
    gui.setup();
    gui.setBackgroundColor(255);
    gui.setHeaderBackgroundColor(255);
    gui.add(instrumentSettings.setup("Instrument Settings", ""));
    gui.add(metronomeVolume.setup("Metronome Volume", 0.0, 0.0, 0.3));
    
    gui.add(weatherSettings.setup("Weather Settings", ""));
    gui.add(temperature.setup("Temperature (ºC)", getTemperature(), -20.0, 50.0));
    gui.add(humidity.setup("Humidity (%)", getHumidity(), 0.0, 100.0));
    gui.add(clouds.setup("Clouds (%)", getCloudiness(), 0.0, 100.0));
    gui.add(precipitation.setup("Rain (mm/s)", getPrecipitation(), 0.0, 300.0));
    gui.add(windSpeed.setup("Wind Speed (m/s)", getWindSpeed(), 0.0, 200.0));
    
    //    gui.add(cutoff.setup("Cutoff", 2, 0, 40));
    //    gui.add(resonance.setup("Resonance", 20, 0, 40));
    //    gui.add(attack.setup("Attack", 0, 0, 10000));
    //    gui.add(decay.setup("Decay", 0, 0, 10000));
    //    gui.add(sustain.setup("Sustain", 0, 0, 10000));
    //    gui.add(release.setup("Release", 1000, 0, 10000));
    
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
    
    metronome.setEnv(1, 1, 1, 1000);
    metronome.setFrequency(640);
    
    rain.setEnv(1000, 1, 1, 300);
    rain.setFrequency(320);
    rain_clock_1.setTempo(clock.bpm);
    rain_clock_1.setTicksPerBeat(clock.ticks);
    rain_volume_a = 0.5;
    rain_volume_b = 0.5;
    rain_frequency = 350;
    rain_frequency2 = 50;
    rain_modulation_index = 650;
    rain_feedback_a = 0.9999;
    rain_feedback_b = 0.9999;
    
    lead.setEnv(10, 1, 1, 1000);
    lead_clock.setTempo(clock.bpm);
    lead_clock.setTicksPerBeat(clock.ticks);
    lead_chord = lead_prev_chord = 0;
    lead_cutoff = 0;
    lead_resonance = 3;
    lead_motif_changed = false;
    
    //    for (int i = 0; i < 32; i++) notes[i] = randomInt(48, 83);
    //    for (int i = 0; i < 16; i++) chords[i] = randomInt(0, 6);
}

void ofApp::update(){
    //    ofBackground(ofMap(sunPosition, -1.0, 1.0, 0, 255));
    ofBackground(255);
    metronome.setVolume(metronomeVolume);
    
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
        
        if (clock.tick) {
            playhead++;
            
            if (playhead % 4 == 0) metronome.activate();
            else metronome.deactivate();
            
            rain_clock_1.setTicksPerBeat(floor(1 + randomFloat() * 5));
        }
        
        if (rain_clock_1.tick) {
            if (rain_trigger[playhead % 64] == 1) rain.activate();
            else rain.deactivate();
            
            rain_frequency = randomInt(400, 900);
            rain_frequency2 = randomInt(0, 10);
            rain_modulation_index = randomInt(0, 600);
            rain_volume_a = 0.5;
            rain_feedback_a = 1.0 - randomFloat() * 0.001;
        }
        
        if (rain_clock_2.tick) {
            rain_frequency = 400;
            rain_frequency2 = 50;
            rain_modulation_index = 10;
            rain_volume_a = 1.0;
            rain_volume_b = 0.5;
            
            if (randomFloat() > 0.5) rain_feedback_b = 1.00001;
            else rain_feedback_b = 0.9999;
        }
        
        rain_volume_a *= rain_feedback_a;
        rain_volume_b *= rain_feedback_b;
        
        rain.setFrequency((rain_frequency * rain_volume_b) + (mod.sinewave(rain_frequency2 * rain_feedback_b) * rain_modulation_index));
        rain.setVolume(rain_volume_a);
        
        if (lead_clock.tick) {
            if (playhead % 32 == 0) lead_chord++;
            
            lead_volume = 0.5;
            lead_feedback = 1.0 - randomFloat() * 0.0001;
            
            if (randomFloat() > 0.95) {
                lead_cutoff = 35;
                lead_resonance = 40;
                lead_prev_chord = lead_chord;
                
                cout << "filter params changed" << endl;
            }
            
            if (lead_chord > lead_prev_chord) {
                lead_cutoff = 0;
                lead_resonance = 2;
            }
            
            if (lead_chord % 4 == 0 && !lead_motif_changed) {
                for (int i = 0; i < 32; i++) notes[i] = randomInt(48, 83);
                for (int i = 0; i < 16; i++) chords[i] = randomInt(0, 6);
                cout << "motif changed" << endl;
                
                lead_motif_changed = true;
            }
            
            cout << lead_motif_changed << endl;
            
            lead.activate();
        } else lead.deactivate();
        
        lead_volume *= lead_feedback;
        lead.setFrequency(filter.hires(mtof.mtof(notes[playhead % 32] + chords[lead_chord % 16]), lead_cutoff, lead_resonance));
        lead.setVolume(lead_volume);
        lead.setEnv(lead_volume * 1000, 1, 1, 300);
        
        // hires filter bass: cutoff: 0, resonance: 2.5
        // hires filter pitch: cutoff: 35, resonance: 40
        
        
        float out = rain.play() + metronome.play() + lead.play();
        
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
