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

    getValues();
}

APU::Square::~Square() {

}

void APU::Square::getValues() {
    sweepPeriod     = (memoryBus->readByte(memoryAddress) & 112) >> 4;
    negate          = (memoryBus->readByte(memoryAddress) & 8) >> 3;
    shift           = (memoryBus->readByte(memoryAddress) & 7);
    duty            = (memoryBus->readByte(memoryAddress + 1) & 192) >> 6;
    lengthLoad      = (memoryBus->readByte(memoryAddress + 1) & 63);
    startVol        = (memoryBus->readByte(memoryAddress + 2) & 240) >> 4;
    envAddMode      = (memoryBus->readByte(memoryAddress + 2) & 8) >> 3;
    period          = (memoryBus->readByte(memoryAddress + 2) & 7);
    freqLSB         = (memoryBus->readByte(memoryAddress + 3));
    trigger         = (memoryBus->readByte(memoryAddress + 4) & 128) >> 7;
    lengthEnable    = (memoryBus->readByte(memoryAddress + 4) & 64) >> 6;
    freqMSB         = (memoryBus->readByte(memoryAddress + 4) & 7);
}

APU::Wave::Wave(MemoryBus * memBus, const uint16_t memAddr) {
    memoryBus = memBus;
    memoryAddress = memAddr;
    
    loadSamples();
}

APU::Wave::~Wave() {

}

void APU::Wave::loadSamples(){
    uint8_t sampleByte;
    for (int i = 0; i < 32; i += 2) {
        sampleByte = memoryBus->readByte(memoryAddress + i);
        samples[i] = (sampleByte & 240) >> 4;
        samples[i+1] = sampleByte & 15;
    }
}

APU::Noise::Noise(MemoryBus * memBus, const uint16_t memAddr) {

}

APU::Noise::~Noise() {

}
