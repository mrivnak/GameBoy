#include "window.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <regex>

#include <GL/gl.h>
#include <cxxopts.hpp>

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
		std::cerr << "Failed to open Game ROM file" << std::endl;
		return 1;
	}

	std::FILE* boot_file = std::fopen("../res/DMG_ROM.bin", "r");

	if (!boot_file) {
		std::cerr << "Failed to open Boot ROM file" << std::endl;
		return 1;
	}

	uint8_t* mem = static_cast<uint8_t*>(std::malloc(0xFFFF));

	if (std::fread(mem, 1, 0xFF, boot_file) != 0xFF) {
		std::puts("Failed to write boot ROM!");
		std::fclose(boot_file);
		std::free(mem);
		return 1;
	}

	std::fclose(boot_file);
	std::free(mem);


	Window window("My Window", 800, 600);

	while (!window.isCloseRequested()) {
		window.pollEvents();

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

