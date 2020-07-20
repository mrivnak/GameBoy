#include "audio.hpp"

APU::Sound::Sound(MemoryBus * memoryBus) {
    SDL_Init(SDL_INIT_AUDIO);
    // Open audio device

    SDL_memset(&want, 0, sizeof(want));

    want.freq = 32768;
    want.format = AUDIO_S8;
    want.channels = 2;
    want.samples = 64;

    have.freq = 0;
    have.format = 0;
    have.channels = 0;
    have.samples = 0;
    device = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);

    sampleRate = have.freq;
    sampleClocks = CPU_FREQ / sampleRate;

    for (auto item : audioBuffer) {
        item = 0;
    }

    // TODO: do this
    // audiobuffer_p = c_void_p(self.audiobuffer.buffer_info()[0])

    clock = 0;

    sweep = new Sweep(memoryBus);
    square = new Square(memoryBus);
    wave = new Wave(memoryBus);
    noise = new Noise(memoryBus);

    SDL_PauseAudioDevice(device, 0);
}

APU::Sound::~Sound() {
    delete sweep;
    delete square;
    delete wave;
    delete noise;

    SDL_CloseAudioDevice(device);
}

void APU::Sound::sync() {
    int numSamples = clock / sampleClocks;
    int sample;

    for (int i = 0; i < std::min(2048, numSamples); i++) {
        sweep->run(sampleClocks);
        square->run(sampleClocks);
        wave->run(sampleClocks);
        noise->run(sampleClocks);

        sample = std::max(
            std::min({
                sweep->sample(),
                square->sample(),
                wave->sample(),
                noise->sample(),
                (uint16_t) 64
            }, std::less<uint16_t>()),
            (uint16_t) 0
        );

        audioBuffer[2 * i] = sample;
        audioBuffer[2*i + 1] = sample;
        clock -= sampleClocks;

        uint32_t queuedTime = SDL_GetQueuedAudioSize(device);
        int samplesPerFrame = (sampleRate / 60) * 2;
        if (queuedTime > samplesPerFrame * SOUND_DESYNC_THRESHOLD)
            SDL_ClearQueuedAudio(device);

        SDL_QueueAudio(device, &audioBuffer, 2 * numSamples);
        clock %= sampleClocks;
    }
}

APU::Square::Square(MemoryBus * memoryBus) {
    waveSelect = 0;
    length = 0;
    startVol = 0;
    envAddMode = false;
    envelopePeriod = 0;
    sendPeriod = 0;
    lengthEnable = false;

    enable = false;
    lengthTimer = 64;
    periodTimer = 0;
    envelopeTimer = 0;
    period = 4;
    waveFrame = 0;
    frameTimer = 0x2000;
    frame = 0;
    volume = 0;
    
}

void APU::Square::getValues() {
    // TODO: square wave get
}

void APU::Square::run(int clocks) {
    periodTimer -= clocks;
    while (periodTimer <= 0) {
        periodTimer += period;
        waveFrame = (waveFrame + 1) % 8;  // advance the wave 
    }

    frameTimer -+ clocks;
    while (frameTimer <= 0) {
        frameTimer += 0x2000;
        tickFrame();
    }
}

void APU::Square::tickFrame() {
    frame = (frame + 1) % 8;

    if (lengthEnable && frame % 2 == 0 && lengthTimer > 0) {
        lengthTimer--;
        if (lengthTimer == 0)
            enable = false;
    }

    if (frame == 7 && envelopeTimer != 0) {
        envelopeTimer--;
        if (envelopeTimer == 0) {
            int newVolume = volume + (envAddMode ? 1 : -1);
            if (newVolume < 0 || newVolume > 15) {
                envelopeTimer = 0;
            }
            else {
                envelopeTimer = envelopePeriod;
                volume = newVolume;
            }
        }
    }
}

int APU::Square::sample() {
    return enable ? volume * waveTables[waveSelect][waveFrame] : 0;
}

void APU::Square::triggerEvent() {
    enable = true;
    lengthTimer = lengthTimer || 64;
    periodTimer = period;
    envelopeTimer = envelopePeriod;
    volume = startVol;
}

APU::Sweep::Sweep(MemoryBus * memoryBus) : APU::Square{memoryBus} {
    sweepPeriod = 0;
    sweepDir = 0;
    sweepShift = 0;

    sweepTimer = 0;
    sweepEnable = false;
    sweepShadow = 0;
}

void APU::Sweep::getValues() {
    // TODO: sweep channel get
}

void APU::Sweep::tickFrame() {
    APU::Square::tickFrame();

    if (sweepEnable && sweepPeriod && frame & 3 == 2) {
        sweepTimer--;
        if (sweepTimer == 0) {
            if (sweep(true)) {
                sweepTimer = sweepPeriod;
                sweep(false);
            }
        }
    }
}

void APU::Sweep::triggerEvent() {
    APU::Square::triggerEvent();

    sweepShadow = sendPeriod;
    sweepTimer = sweepPeriod;
    sweepEnable = sweepPeriod || sweepShift ? true : false;

    if (sweepShift) {
        sweep(false);
    }
}

bool APU::Sweep::sweep(bool save) {
    int newPeriod;
    if (sweepDir == 0) {
        newPeriod = sweepShadow + (sweepShadow >> sweepShift);
    }
    else {
        newPeriod = sweepShadow - (sweepShadow >> sweepShift);
    }

    if (newPeriod >= 0x800) {
        enable = false;
        return false;
    }
    else if (save && sweepShift) {
        sendPeriod = sweepShadow = newPeriod;
        period = 4 * (0x800 - sendPeriod);
        return true;
    }
    else {  // TODO: make sure this is correct or unreachable
        return false;
    }
}

APU::Wave::Wave(MemoryBus * memoryBus) {
    getWaveTable();

    DACPower = 0;
    length = 0;
    volumeCode = 0;
    sendPeriod = 0;
    lengthEnable = true;

    enable = false;
    lengthTimer = 256;
    periodTimer = 0;
    period = 4;
    waveFrame = 0;
    frameTimer = 0x2000;
    frame = 0;
    volumeShift = 0;
}

void APU::Wave::getValues() {
    // TODO: wave channel get
}

void APU::Wave::getWaveTable() {
    // TODO: get wave table
}

void APU::Wave::run(int clocks) {
    periodTimer -= clocks;
    while (periodTimer <= 0) {
        periodTimer += period;
        waveFrame++;
        waveFrame %= 32;
    }

    frameTimer -= clocks;
    while (frameTimer <= 0) {
        frameTimer += 0x2000;
        tickFrame();
    }    
}

void APU::Wave::tickFrame() {
    frame = (frame + 1) % 8;
    if (lengthEnable && frame % 2 == 0 && lengthTimer > 0) {
        lengthTimer--;
        if (lengthTimer == 0) {
            enable = false;
        }
    }
}

int APU::Wave::sample() {
    if (enable && DACPower) {
        int sample = waveTable[(int) waveFrame / 2] >> ((waveFrame % 2 == 1) ? 0 : 4) & 0x0F;
        return sample >> volumeShift;
    }
    else {
        return 0;
    }
}

void APU::Wave::triggerEvent() {
    enable = true;
    lengthTimer = lengthTimer || 256;
    periodTimer = period;
}

APU::Noise::Noise(MemoryBus * memoryBus) {
    length = 0;
    startVol = 0;
    envAddMode = false;
    envelopePeriod = 0;
    clockShift = 0;
    LFSRWidth = 0;
    divisor = 0;
    lengthEnable = false;

    enable = false;
    lengthTimer = 64;
    periodTimer = 0;
    envelopeTimer = 0;
    period = 8;
    shiftRegister = 1;
    LFSRFeed = 0x4000;
    frameTimer = 0x2000;
    frame = 0;
    volume = 0;

}

void APU::Noise::getValues() {
    // TODO: noise channel get
}

void APU::Noise::run(int clocks) {
    periodTimer -= clocks;
    while (periodTimer <= 0) {
        // TODO:: figure out noise channel
    }

    frameTimer -= clocks;
    while (frameTimer <= 0) {
        frameTimer += 0x2000;
        tickFrame();
    }
}

void APU::Noise::tickFrame() {
    frame = (frame + 1) % 8;
    if (lengthEnable && frame % 2 == 0 && lengthTimer > 0) {
        lengthTimer--;
        if (lengthTimer == 0) {
            enable = false;
        }
    }

    if (frame == 7 && envelopeTimer != 0) {
        envelopeTimer--;
        if (envelopeTimer == 0) {
            int newVolume = volume + (envAddMode ? 1 : -1);
            if (newVolume < 0 || newVolume > 15) {
                envelopeTimer = 0;
            }
            else {
                envelopeTimer = envelopePeriod;
                volume = newVolume;
            }
        }
    }
}

int APU::Noise::sample() {
    // TODO: figure out noise channel
    return 0;
}

void APU::Noise::triggerEvent() {
    enable = true;
    lengthTimer = lengthTimer || 64;
    periodTimer = period;
    envelopeTimer = envelopePeriod;
    volume = startVol;
}
