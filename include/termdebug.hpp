#pragma once 

#include "registers.hpp"

#include <string>

class TermDebug {
    public:
        TermDebug();

        void PrintRegisters(struct Registers regs);
        
        ~TermDebug();
    private:
        void resetCursor();
        std::string cursorUp(unsigned int n);
        std::string cursorDown(unsigned int n);
        std::string cursorLeft(unsigned int n);
        std::string cursorRight(unsigned int n);
};