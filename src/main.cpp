#include "window.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <regex>

#include <GL/gl.h>
#include <cxxopts.hpp>

#include "processor.hpp"

#define BOOT_ROM_SIZE 0xFF

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	std::string game_filename;
	bool DEBUG = false;

	cxxopts::Options options("Game Boy", "Nintendo Game Boy emulator built in C++");

	try {
		options.add_options()
			("d,debug", "Enable debugging")
			("f,file", "ROM File name", cxxopts::value<std::string>())
			("h,help", "Help menu")
			;

		auto result = options.parse(argc, argv);

		DEBUG = result.count("d");

		if (result.count("h")) {
			std::cout << options.help({"", "Group"}) << std::endl;
			return 0;
		}

		if (result.count("f")) {
			game_filename = result["f"].as<std::string>();
		}
		else {
			std::cerr << "No file specified! Exiting..." << std::endl;
			return 1;
		}
	}
	catch (const cxxopts::OptionException& e) {
		std::cerr << "Error parsing options: " << e.what() << std::endl;
		std::cout << options.help({"", "Group"}) << std::endl;
		return 1;
	}

	// Filename related regex stuff
	std::regex gb(".*.gb");

	if (!std::regex_match(game_filename, gb)) {
		std::cerr << "Invalid file extension. Only \".gb\" files are supported." << std::endl;
		return 1;
	}

	std::FILE* game_file = std::fopen(game_filename.c_str(), "r");

	if (!game_file) {
		std::perror("Failed to open game file");
		std::cerr << "Filename: " << game_filename << std::endl;
		return 1;
	}

	Processor processor;

	std::FILE* boot_file = std::fopen("../res/DMG_ROM.bin", "r");

	if (!boot_file) {
		std::cerr << "Failed to open Boot ROM file" << std::endl;
		return 1;
	}

	if (std::fread(processor.getMemory().getData(), 1, BOOT_ROM_SIZE, boot_file)
			!= BOOT_ROM_SIZE) {
		std::fputs("Failed to write boot ROM!", stderr);
		std::fclose(boot_file);

		return 1;
	}

	std::fclose(boot_file);

	const uint8_t TEST_PROGRAM[] = {
		0x00, // NOP
		0x10, // STOP
		0x20, (uint8_t)(-4), // JR NZ, 1h
	};

	std::memcpy(processor.getMemory().getData(), TEST_PROGRAM, sizeof(TEST_PROGRAM));

	Window window("My Window", 160, 144);

	if (DEBUG) {
		
	}

	while (!window.isCloseRequested()) {
		window.pollEvents();

		// processor exec is going in here for now
		processor.step();

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-1.f, -1.f);
		glVertex2f(0.f, 1.f);
		glVertex2f(1.f, -1.f);
		glEnd();

		window.swapBuffers();
	}

	return 0;
}

