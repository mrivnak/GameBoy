#include "application.hpp"
#include "window.hpp"
#include "render-device.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <regex>

#include <GL/gl.h>
#include <cxxopts.hpp>

#include "motherboard.hpp"

#include "bitmap.hpp"
#include "texture.hpp"

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	std::string gameFilename;
	bool DEBUG = false;

	cxxopts::Options options("GameBoy", "Nintendo Game Boy emulator built in C++");

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
			gameFilename = result["f"].as<std::string>();
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

	if (!std::regex_match(gameFilename, gb)) {
		std::cerr << "Invalid file extension. Only \".gb\" files are supported." << std::endl;
		return 1;
	}

	// TODO: 
	/*
	std::FILE* game_file = std::fopen(gameFilename.c_str(), "r");

	if (!game_file) {
		std::perror("Failed to open game file");
		std::cerr << "Filename: " << gameFilename << std::endl;
		return 1;
	}

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
	*/

	Motherboard motherboard;

	motherboard.loadCartridge(gameFilename);
	motherboard.loadBootROM();

	std::string gameTitle = motherboard.getTitle();

	Application app;
	Window& window = app.createWindow("WIT GB Emulator - " + gameTitle, 800, 600);
	auto& renderDevice = window.getRenderDevice();

	Bitmap bmp(64, 64);
	auto data = bmp.getData();

	for (int y = 0, i = 0; y < 64; ++y) {
		for (int x = 0; x < 64; ++x, i += 3) {
			data[i] = 4 * x;
			data[i + 1] = 4 * y;
			data[i + 2] = 0;
		}
	}

	Texture tex(bmp);

	while (app.isRunning()) {
		app.pollEvents();

		// processor exec is going in here for now
		motherboard.clock();

		renderDevice.clear();
		renderDevice.drawTexturedQuad(tex);

		window.swapBuffers();
	}

	return 0;
}

