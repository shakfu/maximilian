// Runs a player program without an audio device: setup(), then play() for a number of
// frames (argv[1], default 10 seconds). Exits 1 if any output sample is NaN or infinite.
#include "maximilian.h"

void setup();
void play(double *output);

int main(int argc, char **argv) {
    const long frames = argc > 1 ? std::atol(argv[1]) : 10L * maxiSettings::sampleRate;
    std::vector<double> output(maxiSettings::channels, 0);
    setup();
    for (long i = 0; i < frames; ++i) {
        play(output.data());
        for (double x : output) {
            if (!std::isfinite(x)) {
                std::cerr << "non-finite output at frame " << i << std::endl;
                return 1;
            }
        }
    }
    return 0;
}
