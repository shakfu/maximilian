#include "maximilian.h"
#include "maxiFFT.h"
#include "check.h"
#include <algorithm>

int main() {
    const int fftSize = 1024;
    const int bin = 20;
    const double freq = bin * double(maxiSettings::sampleRate) / fftSize;

    maxiFFT fft;
    fft.setup(fftSize, fftSize / 4);
    CHECK(fft.getNumBins() == fftSize / 2);

    // Run several frames so the analysis window is full of the sine.
    int frames = 0;
    for (int n = 0; frames < 8; ++n) {
        if (fft.process(std::sin(TWOPI * freq * n / maxiSettings::sampleRate))) ++frames;
    }

    std::vector<float> &mags = fft.getMagnitudes();
    CHECK(int(mags.size()) == fftSize / 2);
    long peak = std::max_element(mags.begin(), mags.end()) - mags.begin();
    CHECK(peak == bin);
    // Energy away from the peak (beyond window leakage) should be negligible.
    CHECK(mags[bin + 10] < 0.01f * mags[bin]);

    return checkResult();
}
