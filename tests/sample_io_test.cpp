#include "maximilian.h"
#include "check.h"

int main() {
    // A loaded file survives a save/load round trip unchanged.
    maxiSample original, reloaded;
    CHECK(original.load(MAXIMILIAN_SAMPLES_DIR "/beat2.wav"));
    CHECK(original.getLength() > 0);
    CHECK(original.save("roundtrip.wav"));
    CHECK(reloaded.load("roundtrip.wav"));
    CHECK(reloaded.getLength() == original.getLength());
    CHECK(reloaded.mySampleRate == original.mySampleRate);
    bool identical = reloaded.getLength() == original.getLength();
    for (size_t i = 0; identical && i < original.getLength(); ++i) {
        identical = reloaded.amplitudes[i] == original.amplitudes[i];
    }
    CHECK(identical);

    // A sample that was never loaded writes a valid header; out-of-range values clamp.
    maxiSample fresh, freshReloaded;
    std::vector<double> values {0.0, 0.5, 2.0, -2.0};
    fresh.setSampleAndRate(values, 22050);
    CHECK(fresh.save("fresh.wav"));
    CHECK(freshReloaded.load("fresh.wav"));
    CHECK(freshReloaded.mySampleRate == 22050);
    CHECK(freshReloaded.getLength() == values.size());
    if (freshReloaded.getLength() == values.size()) {
        CHECK_NEAR(freshReloaded.amplitudes[0], 0.0, 1e-4);
        CHECK_NEAR(freshReloaded.amplitudes[1], 0.5, 1e-4);
        CHECK_NEAR(freshReloaded.amplitudes[2], 1.0, 1e-4);
        CHECK_NEAR(freshReloaded.amplitudes[3], -1.0, 1e-4);
    }

    CHECK(!fresh.save("no-such-directory/x.wav"));

    return checkResult();
}
