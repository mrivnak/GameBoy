#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include "memory-bus.hpp"

const short SOUND_DESYNC_THRESHOLD = 5;
const int CPU_FREQ = 4213440;

namespace APU {
    class Sound;
    class Square;
    class Sweep;
    class Wave;
    class Noise;
}

class APU::Sound {
    public:
        Sound(MemoryBus * memoryBus);
        ~Sound();

        void sync();

    private:
        SDL_AudioSpec want;
        SDL_AudioSpec have;
        SDL_AudioDeviceID device;

        int sampleRate;
        int sampleClocks;
        std::array<uint8_t, 4096> audioBuffer;
        int clock;

        Sweep * sweep;
        Square * square;
        Wave * wave;
        Noise * noise;
};

class APU::Square {
    public:
        Square(MemoryBus * memoryBus);
        ~Square();

        void getValues();

        void run(int clocks);
        void tickFrame();

        int sample();
        void triggerEvent();

    protected:
        const std::array<std::array<int, 8>, 4> waveTables {{
            {0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 1, 1},
            {0, 1, 1, 1, 1, 1, 1, 0}
        }};
        int
            waveSelect,
            length,
            startVol,
            envelopePeriod,
            sendPeriod
        ;

        bool
            envAddMode,
            trigger,
            lengthEnable
        ;

        int
            enable,
            lengthTimer,
            periodTimer,
            envelopeTimer,
            period,
            waveFrame,
            frameTimer,
            frame,
            volume
        ;
};

class APU::Sweep : public APU::Square {
    public:
        Sweep(MemoryBus * memoryBus);
        ~Sweep();

        bool sweep(bool save);

        void getValues();
        void tickFrame();
        void triggerEvent();
    
    private:
        int
            sweepPeriod,
            sweepDir,
            sweepShift
        ;

        int
            sweepTimer,
            sweepEnable,
            sweepShadow
        ;
};

class APU::Wave {
    public:
        Wave(MemoryBus * memoryBus);
        ~Wave();

        void getValues();

        void getWaveTable();

        void run(int clocks);
        void tickFrame();

        int sample();
        void triggerEvent();

    private:
        std::array<uint8_t, 16> waveTable;

        int
            length,
            volumeCode,
            sendPeriod
        ;

        bool
            DACPower,
            trigger,
            lengthEnable
        ;

        int
            lengthTimer,
            periodTimer,
            period,
            waveFrame,
            frameTimer,
            frame,
            volumeShift
        ;

        bool enable;
};

class APU::Noise {
    public:
        Noise(MemoryBus * memoryBus);
        ~Noise();

        const std::array<int, 8> DIV_TABLE {{8, 16, 32, 48, 64, 80, 96, 112}};

        void getValues();

        void run(int clocks);
        void tickFrame();

        uint16_t sample();
        void triggerEvent();

    private:
        int
            length,
            startVol,
            envelopePeriod,
            clockShift,
            divisor
        ;
            
        bool
            envAddMode,
            LFSRWidth,
            trigger,
            lengthEnable
        ;
        
        int
            lengthTimer,
            periodTimer,
            envelopeTimer,
            period,
            shiftRegister,
            LFSRFeed,
            frameTimer,
            frame,
            volume
        ;

        bool enable;
};
