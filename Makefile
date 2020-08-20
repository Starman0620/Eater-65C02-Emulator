eater: clean
	g++ Source/*.cpp -o eater -std=c++17

ROM:
	vasm program.asm -Fbin -dotdir -o rom.bin

clean:
	rm -f eater rom.bin
