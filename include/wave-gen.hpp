#pragma once

#include <cstdint>
#include <vector>

const short MAX_AMPLITUDE = 32767;
const int CPU_FREQUENCY = 4194304;

namespace AudioGen {
    uint16_t squareWave(int sampleRate, int time, int frequency, double amplitude, double duty);
    uint16_t noiseWave(int sampleRate, int time);

    enum wave{Square, Noise};

    std::vector<uint16_t> getSample(wave type, int sampleRate, int frequency, double amplitude, double duty);
}