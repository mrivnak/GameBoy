#pragma once

#include <array>
#include <cstdint>

#include <AL/al.h>
#include <AL/alc.h>

#include "memory-bus.hpp"

namespace APU {
    class Audio;
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
        MemoryBus * memoryBus;
        uint16_t memoryAddress;

        Square * square1;
        Square * square2;
        Wave * wave;
        Noise * noise;
};

class APU::Square {
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

        int stepCounter;
        int slowStepCounter
};

class APU::Wave {
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
};

class APU::Noise {
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
