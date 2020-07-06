#include "audio.hpp"

const uint16_t volumeRegister       = 0xFF24;
const uint16_t squareWaveRegister1  = 0xFF10;
const uint16_t squareWaveRegister2  = 0xFF15;
const uint16_t waveRegister         = 0xFF1A;
const uint16_t noiseRegister        = 0xFF1F;
const uint16_t waveTableRegister    = 0xFF30;


APU::Audio::Audio(MemoryBus * memoryBus) {
    square1 = new Square(memoryBus, squareWaveRegister1, true);
    square2 = new Square(memoryBus, squareWaveRegister2, false);
    wave = new Wave(memoryBus, waveRegister);
    noise = new Noise(memoryBus, noiseRegister);
}

APU::Audio::~Audio(){
    square1->~Square();
    square2->~Square();
    wave->~Wave();
    noise->~Noise();

    delete square1;
    delete square2;
    delete wave;
    delete noise;
}

APU::Square::Square(MemoryBus * memBus, const uint16_t memAddr, bool swp) {
    memoryBus = memBus;
    memoryAddress = memAddr;
    sweep = swp;

    // clear error code
    alGetError();
    
    alGenBuffers(bufferSize, buffers);
    if ((error = alGetError()) != AL_NO_ERROR) {
        std::cerr << "OpenAL Error: alGenBuffers:" << error << std::endl;
        return;
    }

    // Set static audio
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
}

void APU::Audio::step() {
    square1->step();
    square2->step();
}

// #### OpenAL Source ####

APU::Source::Source() {
    ALenum error;

    alGenSources(1, source);
    if ((error = alGetError()) != AL_NO_ERROR) {
        std::cerr << "OpenAL Error: alGenSources:" << error << std::endl;
        return;
    }

    alcGetIntegerv(device, ALC_FREQUENCY, 1, sampleRate);
}

APU::Source::~Source() {
    alDeleteSources(1, source);
}

// #### Square Wave Channel ####

APU::Square::Square(ALCdevice * device, MemoryBus * memoryBus, const uint16_t memoryAddress, bool sweep) {
    this->device = device;
    this->memoryBus = memoryBus;
    this->memoryAddress = memoryAddress;
    this->sweep = sweep;

    fiveBitCounter = 0;

    getValues();

    timerCounter = 2048 - freq;
}

APU::Square::~Square() {

}

void APU::Square::getValues() {
    sweepPeriod     = (memoryBus->readByte(memoryAddress + 0) & 0b01110000) >> 4;
    negate          = (memoryBus->readByte(memoryAddress + 0) & 0b00001000) >> 3;
    shift           = (memoryBus->readByte(memoryAddress + 0) & 0b00000111) >> 0;
    duty            = (memoryBus->readByte(memoryAddress + 1) & 0b11000000) >> 6;
    lengthLoad      = (memoryBus->readByte(memoryAddress + 1) & 0b00111111) >> 0;
    startVol        = (memoryBus->readByte(memoryAddress + 2) & 0b11110000) >> 4;
    envAddMode      = (memoryBus->readByte(memoryAddress + 2) & 0b00001000) >> 3;
    period          = (memoryBus->readByte(memoryAddress + 2) & 0b00000111) >> 0;
    freqLSB         = (memoryBus->readByte(memoryAddress + 3) & 0b11111111) >> 0;
    trigger         = (memoryBus->readByte(memoryAddress + 4) & 0b10000000) >> 7;
    lengthEnable    = (memoryBus->readByte(memoryAddress + 4) & 0b01000000) >> 6;
    freqMSB         = (memoryBus->readByte(memoryAddress + 4) & 0b00000111) >> 0;

    freq = ((uint16_t) freqMSB << 4) + freqLSB;
}

void APU::Square::step() {
    // Timer

    if (fiveBitCounter == 0)

        timer();
    
    fiveBitCounter++;

    if (fiveBitCounter == 31)
        fiveBitCounter = 0;
}

void APU::Square::timer() {
    if (timerCounter == 0) {
        timerCounter = 2048 - freq;
    }

    outputClock();

    timerCounter--;
}
// #### Waveform Channel ####

APU::Wave::Wave(ALCdevice * device, MemoryBus * memoryBus, const uint16_t memoryAddress) {
    this->device = device;
    this->memoryBus = memoryBus;
    this->memoryAddress = memoryAddress;
    
    loadSamples();
}

APU::Wave::~Wave() {

}

void APU::Wave::loadSamples(){
    uint8_t sampleByte;
    for (int i = 0; i < 32; i += 2) {
        sampleByte = memoryBus->readByte(waveTableRegister + i);
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

APU::Noise::Noise(MemoryBus * memBus, const uint16_t memAddr) {
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