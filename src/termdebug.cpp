#include "termdebug.hpp"

std::string TermDebug::cursorUp(unsigned int n) {
    return "\u001b[" + n + "A";
}