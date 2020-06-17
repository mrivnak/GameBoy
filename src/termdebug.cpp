#include "termdebug.hpp"

bool TermDebug::isReset = true;

std::string TermDebug::cursorUp(unsigned int n) {
    return "\u001b[" + std::to_string(n) + "A";
}

std::string TermDebug::cursorDown(unsigned int n) {
    return "\u001b[" + std::to_string(n) + "B";
}

std::string TermDebug::cursorRight(unsigned int n) {
    return "\u001b[" + std::to_string(n) + "C";
}

std::string TermDebug::cursorLeft(unsigned int n) {
    return "\u001b[" + std::to_string(n) + "D";
}

void TermDebug::resetCursor(unsigned int up=0, unsigned int down=0, unsigned int right=0, unsigned int left=0) {
    std::cout << cursorLeft(left);
    std::cout << cursorUp(up);
    std::cout << cursorDown(down);
    std::cout << cursorRight(right);
}

std::string TermDebug::registersString(struct Registers * regs) {
    return "";
}

void TermDebug::printRegisters(struct Registers * regs) {
    if (!isReset)
        resetCursor(10, 0, 0, 128);
        isReset = true;
    
    std::cout << \
        "Game Boy Registers:" << std::endl << \
        "  AF: 0x" << std::setfill('0') << std::setw(4) << std::hex << regs->getAF() << std::endl << \
        "  BC: 0x" << std::setfill('0') << std::setw(4) << std::hex << regs->getBC() << std::endl << \
        "  DE: 0x" << std::setfill('0') << std::setw(4) << std::hex << regs->getDE() << std::endl << \
        "  HL: 0x" << std::setfill('0') << std::setw(4) << std::hex << regs->getHL() << std::endl << \
        "Flags:" << std::endl << \
        "  Zero: " << std::bitset<1>(regs->isZero()) << std::endl << \
        "  Subtract: " << std::bitset<1>(regs->isSubtract()) << std::endl << \
        "  Half Carry: " << std::bitset<1>(regs->isHalfCarry()) << std::endl << \
        "  Carry: " << std::bitset<1>(regs->isCarry()) << std::endl;

    isReset = false;
}