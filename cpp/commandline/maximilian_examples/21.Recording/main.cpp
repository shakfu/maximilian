#include "maximilian.h"

// Here we define a double floating value that will contain our
// frame of lovely maximilian generated audio
double out;

// Our oscillator to fill our frame up with my favourite wave
maxiOsc osc;

// Our ramp to modulate the oscillators
maxiOsc ramp;

// Every sample we play is appended here
vector<double> recording;

// When the program quits, this object's destructor copies the
// recording into a maxiSample and saves it as a wav file in the
// current directory. It is declared after the recording, so it
// is destroyed first and the recording is still intact.
struct SaveOnExit {
    ~SaveOnExit() {
        maxiSample sample;
        sample.setSampleAndRate(recording, maxiSettings::sampleRate);
        if (sample.save("lovesong.wav")) {
            cout << "Saved " << recording.size() << " samples to lovesong.wav" << endl;
        }
    }
} saveOnExit;

void setup() {
}

void play(double *output) {

    // A pulse wave!!! Yay
    out = osc.pulse(90, ramp.phasor(.2));

    // Fill our output buffer
    output[0]=out;
    output[1]=out;

    // Both channels are the same, so record one of them
    recording.push_back(out);
}
