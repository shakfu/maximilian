#include "maximilian.h"
#include "check.h"

// RMS of a sine at freq after a low-pass SVF, skipping the first second as settling time.
static double lowpassRms(double freq, double cutoff) {
    maxiSVF svf;
    svf.setCutoff(cutoff);
    svf.setResonance(1);
    const int sr = maxiSettings::sampleRate;
    double sumSq = 0;
    for (int n = 0; n < 2 * sr; ++n) {
        double y = svf.play(std::sin(TWOPI * freq * n / sr), 1, 0, 0, 0);
        if (n >= sr) sumSq += y * y;
    }
    return std::sqrt(sumSq / sr);
}

int main() {
    const double inputRms = std::sqrt(0.5);
    const double pass = lowpassRms(50, 1000);
    const double stop = lowpassRms(10000, 1000);

    CHECK(std::isfinite(pass) && std::isfinite(stop));
    // Well below cutoff: close to unity gain.
    CHECK_NEAR(pass / inputRms, 1.0, 0.05);
    // Over 3 octaves above cutoff on a 12 dB/octave slope: below -30 dB.
    CHECK(stop / inputRms < 0.03);

    return checkResult();
}
