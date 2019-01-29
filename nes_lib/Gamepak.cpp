//
// Created by Ethan Williams on 2019-01-28.
//

#include "Gamepak.h"

#include <iostream>
#include <fstream>
#include <cstring>

Gamepak::Gamepak(std::string filename) {
    filename = this->filename;
}

Gamepak::~Gamepak() {
    delete [] rom_data;
}

int Gamepak::initialize() {
    ifstream romfile.open(filename, ios::binary|ios::ate); //open file at end
    if (!romfile.is_open()) {
        std::cerr << "Error: ROM file does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    size_t filesize = romfile.tellg(); // get size from current filepointer location;
    romfile.seekg(0, ios::beg); //reset file pointer to beginning;

    rom_data = new char[filesize];
    romfile.read(rom_data, filesize);

    if (verifyHeaders()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Gamepak::verifyHeaders() {
    auto headers = (struct iNES_headers *) rom_data;
    if (strncmp(headers->magic_string, "NES\x1A", 4) != 0) {
        std::cerr << "Error: invalid rom format" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
