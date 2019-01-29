//
// Created by Ethan Williams on 2019-01-28.
//

#ifndef NESEMU_GAMEPAK_H
#define NESEMU_GAMEPAK_H

#include <string>

struct iNES_headers {
    char magic_string[4]; //bytes 0-3
    uint8_t PRG_ROM_size; //byte 4
    uint8_t CHR_ROM_size; //byte 5
    uint8_t flags[2]; //bytes 6 and 7
    /* only flags in bytes 6 and 7 respected */

};

class Gamepak {
private:
    std::string filename;
    char *rom_data;
    char *trainer;
    char *PRG_rom_data;
    char *CHR_rom_data;
    size_t PRG_size;
		size_t CHR_size;
    struct iNES_headers * headers;
    int mapper;
    std::string nametable_mirroring_type;

    bool disable_mirroring;
public:
    explicit Gamepak(std::string filename);
    ~Gamepak();
    int initialize();
    int verifyHeaders();



};


#endif //NESEMU_GAMEPAK_H
