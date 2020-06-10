#pragma once 

#include "registers.hpp"

#include <curses.h>

class TermDebug {
    public:
        TermDebug();

        void PrintRegisters(struct Registers regs);
        
        ~TermDebug();
};