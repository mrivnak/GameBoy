#pragma once

#include <array>
#include <cstdint>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "memory-bus.hpp"

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
    private:
        // OpenAL
        ALuint * source;
};

class APU::Square : private APU::Source {
    public:
        Square(MemoryBus * memoryBus, const uint16_t memoryAddress, bool sweep);
        ~Square();

        void step();
    private:
        MemoryBus * memoryBus;
        uint16_t memoryAddress;
        bool sweep;

        void getValues();

        bool negate;
        bool envAddMode;
        bool trigger;
        bool lengthEnable;

        uint8_t
            sweepPeriod,
            shift, 
            duty,
            lengthLoad,
            startVol,
            period,
            freqLSB,
            freqMSB
        ;

        void step512Hz();
        void clockLengthCtr();
        void clockSweep();
        void clockVolEnv();

        unsigned int stepCounter;
        unsigned int slowStepCounter;
};

class APU::Wave : private APU::Source {
    public:
        Wave(MemoryBus * memoryBus, const uint16_t memoryAddress);
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

        void step512Hz();
        void clockLengthCtr();

        unsigned int stepCounter;
        unsigned int slowStepCounter;
};

class APU::Noise : private APU::Source {
    public:
        Noise(MemoryBus * memoryBus, const uint16_t memoryAddress);
        ~Noise();

        void step();
    private:
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
