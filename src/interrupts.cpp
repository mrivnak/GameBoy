#include "interrupts.hpp"

struct Interrupts {
    uint8_t IME;
    uint8_t IF;
    uint8_t IE;
    bool IMEDelay;
}