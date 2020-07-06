#include "wave-gen.hpp"

uint16_t AudioGen::squareWave(int sampleRate, int time, int frequency, double amplitude, double duty) {
    uint16_t result;
    int samples = (double) sampleRate * (((2048 - (double) frequency) >> 5 ) / CPU_FREQ * adjust);


    int tpc = samples / frequency;
    int cyclePart = time % tpc;
    int dutyCycle = tpc * duty;
    short int _amplitude = MAX_AMPLITUDE / amplitude;

    result = cyclePart < dutyCycle ? _amplitude : 0;

    return result;
}

std::vector<uint16_t> AudioGen::getSample(AudioGen::wave type, int sampleRate, int frequency, double amplitude, double duty, double adjust=0) {
    std::vector<uint16_t> output;
    int samples = (double) sampleRate * (((2048 - (double) frequency) >> 5 ) / CPU_FREQ * adjust);
    
    switch (type) {
        case Square:
            for (int i = 0; i < samples; i++) 
                output.push_back(AudioGen::squareWave(sampleRate, i, frequency, amplitude, duty));
            break;
        case Noise:
            for (int i = 0; i < samples; i++) 
                output.push_back(AudioGen::noiseWave(sampleRate, i));
            break;
    }
}