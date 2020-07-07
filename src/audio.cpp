#include "audio.hpp"
#include "wave-gen.hpp"

APU::Audio::Audio(MemoryBus * memoryBus) {
    // OpenAL
    device = alcOpenDevice(nullptr);

    if (device) {
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
    }
    else {
        // TODO: Handle errors
        std::cerr << "ERROR: Audio device not loaded!" << std::endl;
    }

    // Clear error code
    alGetError();

    // Set static audio
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);

    square1 = new Square(device, memoryBus, SQUARE_WAVE_REGISTER_1, true);
    square2 = new Square(device, memoryBus, SQUARE_WAVE_REGISTER_2, false);
    // wave = new Wave(device, memoryBus, WAVE_REGISTER);
    // noise = new Noise(device, memoryBus, NOISE_REGISTER);
}

APU::Audio::~Audio(){
    square1->~Square();
    square2->~Square();
    // wave->~Wave();
    // noise->~Noise();

    delete square1;
    delete square2;
    // delete wave;
    // delete noise;

    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void APU::Audio::step() {
    square1->step();
    square2->step();
}

// #### OpenAL Source ####

APU::Source::Source() {
    genBuffers();
    genSources();

    alSource3f(*source, AL_POSITION, 0, 0, 0);
    alSource3f(*source, AL_VELOCITY, 0, 0, 0);

    alcGetIntegerv(device, ALC_FREQUENCY, 1, sampleRate);
}

APU::Source::~Source() {
    alDeleteSources(1, source);
}

void APU::Source::genBuffers() {
    ALenum error;
    
    alGenBuffers(bufferSize, buffers);
    if ((error = alGetError()) != AL_NO_ERROR) {
        std::cerr << "OpenAL Error: alGenBuffers:" << error << std::endl;
        return;
    }
}

void APU::Source::genSources() {
    ALenum error;

    alGenSources(1, source);
    if ((error = alGetError()) != AL_NO_ERROR) {
        std::cerr << "OpenAL Error: alGenSources:" << error << std::endl;
        return;
    }
}

// #### Square Wave Channel ####

APU::Square::Square(ALCdevice * device, MemoryBus * memoryBus, const uint16_t memoryAddress, bool sweep) {
    this->device = device;
    this->memoryBus = memoryBus;
    this->memoryAddress = memoryAddress;
    this->sweep = sweep;

    fiveBitCounter = 0;
    stepCounter = 0;

    getValues();

    timerCounter = 2048 - freq;
    lengthCounter = lengthLoad;
    lengthCounterDisable = false;

    amplitude = 15;
}

APU::Square::~Square() {
    // TODO: Clean up
}

void APU::Square::getValues() {
    sweepPeriod     = (memoryBus->readByte(memoryAddress + 0) & 0b01110000) >> 4;
    negate          = (memoryBus->readByte(memoryAddress + 0) & 0b00001000) >> 3;
    shift           = (memoryBus->readByte(memoryAddress + 0) & 0b00000111) >> 0;
    dutyCode        = (memoryBus->readByte(memoryAddress + 1) & 0b11000000) >> 6;
    lengthLoad      = (memoryBus->readByte(memoryAddress + 1) & 0b00111111) >> 0;
    startVol        = (memoryBus->readByte(memoryAddress + 2) & 0b11110000) >> 4;
    envAddMode      = (memoryBus->readByte(memoryAddress + 2) & 0b00001000) >> 3;
    period          = (memoryBus->readByte(memoryAddress + 2) & 0b00000111) >> 0;
    freqLSB         = (memoryBus->readByte(memoryAddress + 3) & 0b11111111) >> 0;
    trigger         = (memoryBus->readByte(memoryAddress + 4) & 0b10000000) >> 7;
    lengthEnable    = (memoryBus->readByte(memoryAddress + 4) & 0b01000000) >> 6;
    freqMSB         = (memoryBus->readByte(memoryAddress + 4) & 0b00000111) >> 0;

    freq = ((uint16_t) freqMSB << 4) + freqLSB;

    switch (dutyCode) {
        case 0:
            duty = 0.125;
            break;
        case 1:
            duty = 0.25;
            break;
        case 2:
            duty = 0.5;
            break;
        case 3:
            duty = 0.75;
            break;
    }
}

void APU::Square::step() {
    // Timer

    if (fiveBitCounter == 0)
        timer();
    
    fiveBitCounter++;

    if (fiveBitCounter == 32)
        fiveBitCounter = 0;

    // Counters
    if (stepCounter % 16384 == 0)
        lengthClock();
    if ((stepCounter - 2) & 32768 == 0)
        if (sweep)
            sweepClock();
    if ((stepCounter - 1) & 65536 == 0)
        volEnvClock();

    stepCounter++;

    if (stepCounter == 65536)
        stepCounter = 0;
}

void APU::Square::timer() {
    if (timerCounter == 0) {
        timerCounter = 2048 - freq;
    }

    outputClock();

    timerCounter--;
}

void APU::Square::outputClock() {
    if (!lengthCounterDisable) {
        getValues();
        double amp = amplitude / 15.0;

        std::vector<ALint> samples = AudioGen::getSample(AudioGen::Square, sampleRate, freq, amplitude, duty);
    }
}

void APU::Square::lengthClock() {
    getValues();

    if (lengthEnable && lengthCounter != 0) {
        lengthCounter--;
    }

    if (lengthCounter == 0)
        lengthCounterDisable = true;
}

void APU::Square::volEnvClock() {
    // TODO: Implement volume
}

void APU::Square::sweepClock() {
    // TODO: Implement sweep
}

// #### Waveform Channel ####

APU::Wave::Wave(ALCdevice * device, MemoryBus * memoryBus, const uint16_t memoryAddress) {
    this->device = device;
    this->memoryBus = memoryBus;
    this->memoryAddress = memoryAddress;
    
    loadSamples();

    // TODO: Add counters
}

APU::Wave::~Wave() {

}

void APU::Wave::loadSamples(){
    uint8_t sampleByte;
    for (int i = 0; i < 32; i += 2) {
        sampleByte = memoryBus->readByte(WAVE_TABLE_REGISTER + i);
        samples[i] = (sampleByte & 240) >> 4;
        samples[i+1] = sampleByte & 15;
    }
}

void APU::Wave::getValues() {
    DACPower        = (memoryBus->readByte(memoryAddress + 0) & 0b10000000) >> 7;
    lengthLoad      = (memoryBus->readByte(memoryAddress + 1) & 0b11111111) >> 0;
    volumeCode      = (memoryBus->readByte(memoryAddress + 2) & 0b01100000) >> 5;
    freqLSB         = (memoryBus->readByte(memoryAddress + 3) & 0b11111111) >> 0;
    trigger         = (memoryBus->readByte(memoryAddress + 4) & 0b10000000) >> 7;
    lengthEnable    = (memoryBus->readByte(memoryAddress + 4) & 0b01000000) >> 6;
    freqMSB         = (memoryBus->readByte(memoryAddress + 4) & 0b00000111) >> 0;
}

APU::Noise::Noise(ALCdevice * device, MemoryBus * memBus, const uint16_t memAddr) {
    memoryBus = memBus;
    memoryAddress = memAddr;
}

APU::Noise::~Noise() {

}

void APU::Noise::getValues() {
    lengthLoad      = (memoryBus->readByte(memoryAddress + 1) & 0b00111111) >> 0;
    startVol        = (memoryBus->readByte(memoryAddress + 2) & 0b11110000) >> 4;
    envAddMode      = (memoryBus->readByte(memoryAddress + 2) & 0b00001000) >> 3;
    period          = (memoryBus->readByte(memoryAddress + 2) & 0b00000111) >> 0;
    clockShift      = (memoryBus->readByte(memoryAddress + 3) & 0b11110000) >> 4;
    LFSRWidth       = (memoryBus->readByte(memoryAddress + 3) & 0b00001000) >> 3;
    divisorCode     = (memoryBus->readByte(memoryAddress + 3) & 0b00000111) >> 4;
    trigger         = (memoryBus->readByte(memoryAddress + 4) & 0b10000000) >> 7;
    lengthEnable    = (memoryBus->readByte(memoryAddress + 4) & 0b01000000) >> 6;
}