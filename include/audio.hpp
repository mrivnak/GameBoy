#pragma once

#include <array>
#include <cstdint>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "memory-bus.hpp"

const uint16_t VOLUME_REGISTER          = 0xFF24;
const uint16_t SQUARE_WAVE_REGISTER_1   = 0xFF10;
const uint16_t SQUARE_WAVE_REGISTER_2   = 0xFF15;
const uint16_t WAVE_REGISTER            = 0xFF1A;
const uint16_t NOISE_REGISTER           = 0xFF1F;
const uint16_t WAVE_TABLE_REGISTER      = 0xFF30;

namespace APU {
    class Audio;
    class Source;
    class Square;
    class Wave;
    class Noise;
}

class APU::Audio {
    public:
        Audio(MemoryBus * memoryBus);
        ~Audio();

        void step();
    private:
        // OpenAL
        ALCdevice * device;
        ALCcontext * context;
        ALsizei bufferSize = 4096;
        ALuint * buffers;
        ALuint * listener;

        void genBuffers();

        MemoryBus * memoryBus;
        uint16_t memoryAddress;

        Square * square1;
        Square * square2;
        Wave * wave;
        Noise * noise;
};

class APU::Source {
    public:
        Source();
        ~Source();
    protected:
        // OpenAL
        ALCdevice * device;
        ALuint * source;
        ALint * sampleRate;
};

class APU::Square : private APU::Source {
    public:
        Square(ALCdevice * device, MemoryBus * memoryBus, const uint16_t memoryAddress, bool sweep);
        ~Square();

        void step();
    private:
        unsigned int sampleRate;
        MemoryBus * memoryBus;
        uint16_t memoryAddress;
        bool sweep;

        void getValues();
        void timer();
        void outputClock();
        void lengthClock();

        bool negate;
        bool envAddMode;
        bool trigger;
        bool lengthEnable;

        uint8_t
            sweepPeriod,
            shift, 
            dutyCode,
            lengthLoad,
            startVol,
            period,
            freqLSB,
            freqMSB
        ;

        uint16_t freq;
        double duty;
        bool lengthCtrDisable;

        unsigned int stepCounter;
        unsigned int fiveBitCounter;
        unsigned int timerCounter;
        unsigned int lengthCounter
};

class APU::Wave : private APU::Source {
    public:
        Wave(ALCdevice * device, MemoryBus * memBus, const uint16_t memAddr);
        ~Wave();

        void step();
    private:
        MemoryBus * memoryBus;
        uint16_t memoryAddress;

        std::array<uint8_t, 32> samples;
        void loadSamples();

        void getValues();

        bool DACPower;
        bool trigger;
        bool lengthEnable;

        uint8_t
            lengthLoad,
            volumeCode,
            freqLSB,
            freqMSB
        ;

        uint16_t freq;

        void stepFrameCounter();
        void clockLengthCtr();

        unsigned int stepCounter;
        unsigned int slowStepCounter;
};

class APU::Noise : private APU::Source {
    public:
        Noise(ALCdevice * device, MemoryBus * memBus, const uint16_t memAddr);
        ~Noise();

        void step();
    private:
        unsigned int sampleRate;
        MemoryBus * memoryBus;
        uint16_t memoryAddress;

        void getValues();

        bool envAddMode;
        bool LFSRWidth;
        bool trigger;
        bool lengthEnable;

        uint8_t
            lengthLoad,
            startVol,
            period,
            clockShift,
            divisorCode
        ;
};
