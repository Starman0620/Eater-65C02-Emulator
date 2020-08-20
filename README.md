# Ben Eater 65C02 Emulator

## Usage
Place the ROM file you would like the emulator to use next to the executable with ``rom.bin`` as the name then run ``./eater``

## Building
### Linux
Run ``make`` to build the emulator, this requires g++ as you might expect.
``make ROM`` will use vasm (6502, oldstyle syntax) to build ``program.asm`` into a ``.bin`` file for the emulator to run. This assumes you have vasm in a location like /usr/bin

### Windows
I don't have specific build instructions for Windows platforms but any old C++ compiler should work in theory so long as it supports C++17 

## ToDo
- Implement I/O
- Implement a GUI (??)