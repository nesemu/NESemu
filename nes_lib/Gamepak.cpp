//
// Created by Ethan Williams on 2019-01-28.
//

#include "Gamepak.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>

Gamepak::Gamepak(std::string filename) {
    this->filename = std::move(filename);
    rom_data = nullptr;
    trainer = nullptr;
    headers = nullptr;
    mapper = 0;
}

Gamepak::~Gamepak() {
    delete[] rom_data;
}

int Gamepak::initialize() {
    std::ifstream romfile(filename, std::ios::binary | std::ios::ate); //open file at end
    if (!romfile.is_open()) {
        std::cerr << "Error: ROM file does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    std::streampos filesize = romfile.tellg(); // get size from current file pointer location;
    romfile.seekg(0, std::ios::beg); //reset file pointer to beginning;

    rom_data = new char[filesize];
    romfile.read(rom_data, filesize);

    if (verifyHeaders()) {
        return EXIT_FAILURE;
    }

    //std::cout << "ROM initialized." << std::endl;
    return EXIT_SUCCESS;
}

int Gamepak::verifyHeaders() {
    headers = (struct iNES_headers *) rom_data;
    if (strncmp(headers->magic_string, "NES\x1A", 4) != 0) {
        std::cerr << "Error: invalid rom format" << std::endl;
        return EXIT_FAILURE;
    }

    PRG_size = headers->PRG_ROM_size * (unsigned) 16384;
    CHR_size = headers->CHR_ROM_size * (unsigned) 8192;

    /* Handle flags (in bit order) */
    nametable_mirroring_type = headers->flags[0] | 1 ? "vertical" : "horizontal";

    if (headers->flags[0] >> 1 & 1) { //battery RAM
        std::cout << "Battery RAM present" << std::endl;
        /* Currently we ignore this flag and assume no ram present */
    }

    if (headers->flags[0] >> 2 & 1) {
	      std::cout << "Trainer present" << std::endl;
		    trainer = rom_data + 16;
		    PRG_rom_data = trainer + 512;
    } else {
    	PRG_rom_data = rom_data + 16;
    }
    CHR_rom_data = PRG_rom_data + PRG_size;

    disable_mirroring = (headers->flags[0] >> 3) | (uint8_t) 1;

    mapper = headers->flags[0] >> 4;

    if (headers->flags[1]) {
        std::cerr << "Error: flag byte 7 nonzero" << std::endl;
//        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

int Gamepak::loadPRG(NesMemory *memory) {
    if (trainer) {
        memory->map_memory(0x7000, trainer, 512);
    }
    memory->map_memory(0x8000, PRG_rom_data, PRG_size);
    if (PRG_size <= 16*1024) { // if only 16KiB of ROM, mirror in upper address range
        memory->map_memory(0xC000, PRG_rom_data, PRG_size);
    }
    return 0;
}
