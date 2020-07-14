#include "display.hpp"

#include "memory-bus.hpp"

Display::Display(MemoryBus& memory)
	: memory(&memory) {}

Display::~Display() {
}
