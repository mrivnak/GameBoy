#pragma once

#include "singleton.hpp"

#include <cstdint>

#include <functional>

class Registers;
class MemoryBus;

// https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// http://www.devrs.com/gb/files/opcodes.html
class Instructions final : public Singleton<Instructions> {
    public:
        static constexpr const uint8_t PREFIX_BYTE = 0xCB;

        using Instruction = std::function<uint16_t(uint16_t, Registers&, MemoryBus&)>;

        Instructions();

        Instruction fetchInstruction(uint8_t ib, bool prefixByte);

        ~Instructions();
    private:
        Instruction prefixed[16][16];
        Instruction nonPrefixed[16][16];

        void initPrefixed();
        void initNonPrefixed();
};
