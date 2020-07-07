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

uint16_t AudioGen::noiseWave(int sampleRate, int time) {
    // TODO: Implement noise wave generator
    
    return 0;
}

std::vector<ALint> AudioGen::getSample(AudioGen::wave type, int sampleRate, int frequency, double amplitude, double duty) {
    std::vector<ALint> output;
    
    switch (type) {
        case Square:
            for (int i = 0; i < sampleRate; i++) 
                output.push_back(AudioGen::squareWave(sampleRate, i, frequency, amplitude, duty));
            break;
        case Noise:
            for (int i = 0; i < sampleRate; i++) 
                output.push_back(AudioGen::noiseWave(sampleRate, i));
            break;
    }

    return output;
}