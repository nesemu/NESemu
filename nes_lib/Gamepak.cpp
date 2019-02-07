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
    PRG_ram = new uint8_t[8*KILO];
}

Gamepak::~Gamepak() {
    delete[] rom_data;
    delete[] PRG_ram;
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
    initMemory();

    std::cout << "ROM initialized." << std::endl;
    return EXIT_SUCCESS;
}

int Gamepak::verifyHeaders() {
    headers = (iNES_headers *) rom_data;
    if (strncmp(headers->magic_string, "NES\x1A", 4) != 0) {
        std::cerr << "Error: ROM not in iNES format" << std::endl;
        return EXIT_FAILURE;
    }

    if (headers->byte7.iNES2_id == 0b10) {
    	std::cout << "iNES 2.0 format detected" << std::endl;
    	mapper = headers->byte8.iNES2_mapper_xsb << 8 | headers->byte7.mapper_msb << 4 | headers->byte6.mapper_lsb;
        PRG_blocks = headers->byte9.iNES2_PRG_ROM_size_msb << 8 | headers->PRG_ROM_size_lsb;
        PRG_size = PRG_blocks * 16u * KILO;
        CHR_size = (headers->byte9.iNES2_CHR_ROM_size_msb << 8 | headers->CHR_ROM_size_lsb) * 8u * KILO;
    } else {
        std::cout << "iNES 1.x format detected" << std::endl;
        mapper = headers->byte7.mapper_msb << 4 | headers->byte6.mapper_lsb;
        PRG_blocks = headers->PRG_ROM_size_lsb;
        PRG_size = PRG_blocks * 16u * KILO;
        CHR_size = headers->CHR_ROM_size_lsb * 8u * KILO;
    }

    if (mapper > 1) { // Any mapper other than 0 or 1
        std::cerr << "Invalid or unsupported mapper: " << mapper << std::endl;
        return EXIT_FAILURE;
    } else std::cout << "Mapper " << mapper << " detected" << std::endl;


    /* Handle flags (in bit order) */
    nametable_mirroring_type = headers->byte6.mirroring | 1 ? "vertical" : "horizontal";
    std::cout << "Mirroring type: " << nametable_mirroring_type << std::endl;

    if (headers->byte6.battery) { //battery RAM
        std::cout << "Battery RAM present" << std::endl;
        /* Currently we ignore this flag and assume 8k volatile RAM present */
    }

    if (headers->byte6.trainer) {
	      std::cout << "Trainer present" << std::endl;
		    trainer = (uint8_t *)rom_data + 16;
		    memcpy(&PRG_ram[0x1000],trainer,512);
		    PRG_rom_data = trainer + 512;
    } else {
    	PRG_rom_data = (uint8_t *) rom_data + 16;
    }
    CHR_rom_data = PRG_rom_data + PRG_size;

    return EXIT_SUCCESS;
}

void Gamepak::initMemory() {
    if (mapper == 0) {
        PRG_rom_bank1 = PRG_rom_data;
        if (PRG_size > 16*KILO) {
            PRG_rom_bank2 = PRG_rom_data + 16*KILO;
        } else PRG_rom_bank2 = PRG_rom_bank1;
    } else if (mapper == 1) {
        PRG_rom_bank1 = PRG_rom_data;
        PRG_rom_bank2 = PRG_rom_data + (PRG_blocks-1)*16u*KILO;
        MMC1reg.ctrlreg = 0x0C;
        MMC1reg.shift = 0x00;
    }
}


void Gamepak::write_PRG(uint16_t address, uint8_t value) {
    if (address < 0x6000) return;
    else if (address < 0x8000) { // RAM, same for mapper 0 and 1
        PRG_ram[address % 0x2000] = value;
    } else if (mapper == 1) { // writing to control register
        //TODO: Implement Mapper001
        if (value >> 7) { //reset
            MMC1reg.shift = 0x00;
            MMC1reg.ctrlreg = 0x0C;
            PRG_rom_bank2 = PRG_rom_data + (PRG_blocks-1)*16u*KILO;
        }
    }
}

uint8_t Gamepak::read_PRG(uint16_t address) {
    if (address < 0x6000) return 0;
    else if (address < 0x8000) { // RAM, same between mapper 0 and 1
        return PRG_ram[address % 0x2000];
    }
    else if (address < 0xC000) {
        return PRG_rom_bank1[address % 0x4000];
    } else return PRG_rom_bank2[address % 0x4000];
	return 0;
}

void Gamepak::write_CHR(uint16_t address, uint8_t value) {
 // Do nothing; mapper 000 and 001 only support CHR ROM
}

uint8_t Gamepak::read_CHR(uint16_t address) {
    if (address >= 0x2000) return 0;
    if (mapper == 0) {
        return CHR_rom_data[address];
    }
    // TODO: Implement Mapper001
    return 0;
}

uint16_t Gamepak::translate_nametable_address(uint16_t address) {
    // Assume Mapper000; TODO: Implement Mapper001
    switch (headers->byte6.mirroring) {
        case 0: {
            if ((address >= 0x2400) && (address < 0x2800)) {// Table A
                address = (uint16_t) (address - 0x400);
            } else if (address >= 0x2C00 && address < 0x3000) { // Table B
                address = (uint16_t) (address - 0x400);
            }
            break;
        }
        case 1: {
            if (address >= 0x2800 && address < 0x2C00) {
                address = (uint16_t) (address - 0x800);
            }
            else if (address >= 0x2C00 && address < 0x3000) {
                address = (uint16_t)(address - 0x800);
            }
            break;
        }
        default: {
            address = address;
        }
    }
    return address;
}
