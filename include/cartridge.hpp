#pragma once

#include <array>
#include <cstdint>

// TODO: define cartridge structure
struct Header {
    std::array<uint8_t, 16> title;
    uint8_t type;
    uint8_t ROMSize;
    uint8_t RAMSize;
    bool CGB;
};

struct ROMBank {
    uint8_t bankPtr;  // 0-255
    std::array<std::array<uint8_t, 0x4000>, 256> bank;  // TODO: Verify correct arrray dimensions
};

struct RAMBank {
    uint8_t bankPtr;  // 0-15
    std::array<std::array<uint8_t, 0x4000>, 16> bank;  // TODO: Verify correct arrray dimensions
};

struct Cartridge {
    std::array<uint8_t, 0x4000> ROM;
    Header header;
    ROMBank rombank;
    RAMBank rambank;
};