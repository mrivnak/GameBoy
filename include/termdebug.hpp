#pragma once 

#include "registers.hpp"

#include <bitset>
#include <iomanip>
#include <iostream>
#include <string>

const std::string BLACK     = "\u001b[30m";
const std::string RED       = "\u001b[31m";
const std::string GREEN     = "\u001b[32m";
const std::string YELLOW    = "\u001b[33m";
const std::string BLUE      = "\u001b[34m";
const std::string MAGENTA   = "\u001b[35m";
const std::string CYAN      = "\u001b[36m";
const std::string WHITE     = "\u001b[37m";

const std::string BRIGHT_BLACK      = "\u001b[30;1m";
const std::string BRIGHT_RED        = "\u001b[31;1m";
const std::string BRIGHT_GREEN      = "\u001b[32;1m";
const std::string BRIGHT_YELLOW     = "\u001b[33;1m";
const std::string BRIGHT_BLUE       = "\u001b[34;1m";
const std::string BRIGHT_MAGENTA    = "\u001b[35;1m";
const std::string BRIGHT_CYAN       = "\u001b[36;1m";
const std::string BRIGHT_WHITE      = "\u001b[37;1m";

const std::string RESET = "\u001b[0m";

class TermDebug {
    public:
        static void printDebug(struct Registers * regs);
        static std::string getDebug(struct Registers * regs);
    private:
        static bool isReset;
        static void resetCursor(unsigned int up, unsigned int down, unsigned int right, unsigned int left);
        static std::string cursorUp(unsigned int n);
        static std::string cursorDown(unsigned int n);
        static std::string cursorRight(unsigned int n);
        static std::string cursorLeft(unsigned int n);

        static std::string registersString(struct Registers * regs);
        static void printRegisters(struct Registers * regs);
};