/*
File: main.cpp
Author: Starman0620
Description: The main code for the Ben Eater 6502 emulator
*/

#include "mos6502.h"
#include "eater.h"

#define DEBUGMODE

std::map<uint16_t, uint8_t> RAM; // 32K RAM
std::map<uint16_t, uint8_t> VIA; // 4K Addressable VIA space. As far as I'm aware, the real VIA doesn't actually store data like this but we do it just to make some things a bit easier.
char* ROM; // 32K ROM
char* OUT; // The 16x2 LCD
int LCDINDEX = 0;

int main() {
    #ifdef __linux__
    system("clear");
    #elif defined _WIN32
    system("cls");
    #endif

    // Create and read ROM
    ROM = new char[32768];
    OUT = new char[32];
    if(std::filesystem::exists("rom.bin"))
        ReadROM(); // Read ROM from "rom.bin"
    else {
        // Abort
        std::cerr << "No ROM file was found! Make sure there is a binary named rom.bin in the same directory as this executable." << std::endl;
        return -1;
    }

    // Create the CPU
    mos6502 cpu(MemoryRead, MemoryWrite);

    // Main loop
    while(true){
        cpu.Run(1);
        std::cout << ">" << OUT << std::endl;
        #ifdef DEBUGMODE
        std::cout << "Press enter to step";
        getchar();
        #endif
        this_thread::sleep_for(chrono::milliseconds(1));
        #ifdef __linux__
        system("clear");
        #elif defined _WIN32
        system("cls");
        #endif
    }
}

void ReadROM()
{
    std::ifstream file("rom.bin");
    // Determine the length of the ROM
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    // Only read as much as can fit in ROM
    if(length > 32768) {
        std::cout << "ROM file is over 32K! Reading first 32K anyways...";
        length = 32768;
    }
    
    // Read the ROM into the ROM char*
    file.read(ROM, length);
}

uint8_t MemoryRead(uint16_t address) {
    uint8_t val;

    // Read ROM or RAM
    if(address >= 0x8000) // ROM (0x8000 and greater)
        val = ROM[address - 0x8000];
    else // RAM (lower than 0x8000)
        val = RAM[address];
    
    #ifdef DEBUGMODE
    // Outputs
    std::bitset<8>  x(val);
    std::bitset<16> y(address);
    std::cout << y << " " << x;
    printf(" | %04x %02x | READ\n", address, val);
    #endif

    return val;
}

// Write to RAM
void MemoryWrite(uint16_t address, uint8_t value) {
    if(address >= 0x6000 && address <= 0x7FFF) // 6522 VIA addresses
    {
        switch(address){
            case 0x6001: // PORTA; LCD enable
            if(value == 0xa0) { // 0xa0 == enable signal when printing a character
                OUT[LCDINDEX] = (char)VIA[0];
                LCDINDEX++;
            }
            break;
            case 0x6000: // PORTB; write to VIA
            VIA[address - 0x6000] = value;
            break;
        }
    } 
    else
        RAM[address] = value;

    #ifdef DEBUGMODE
    // Outputs
    std::bitset<8>  x(value);
    std::bitset<16> y(address);
    std::cout << y << " " << x;
    printf(" | %04x %02x | WRITE\n", address, value);
    #endif
}