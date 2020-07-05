#pragma once

#include <cstdint>
#include <vector>

const short MAX_AMPLITUDE = 32767;

namespace AudioGen {
    uint16_t squareWave(int sampleRate, int time, int frequency, double amplitude, double duty);
    uint16_t noiseWave(int sampleRate, int time, int frequency, double amplitude, double duty);

    enum wave{Square, Noise};

    std::vector<short> getSamples(wave type, int sampleRate);
}