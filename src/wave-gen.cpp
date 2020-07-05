#include "wave-gen.hpp"

uint16_t AudioGen::squareWave(int sampleRate, int time, int frequency, double amplitude, double duty) {
    uint16_t result;

    int tpc = sampleRate / frequency;
    int cyclePart = time % tpc;
    int dutyCycle = tpc * duty;
    short int _amplitude = MAX_AMPLITUDE / amplitude;

    result = cyclePart < dutyCycle ? _amplitude : 0;

    return result;
}

std::vector<short> AudioGen::getSamples(AudioGen::wave type, int sampleRate) {

}