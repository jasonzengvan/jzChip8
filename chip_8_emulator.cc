#include "chip_8.cc"

Chip_8 chip_8;

int main(int argc, char const *argv[]) {
	chip_8.initialize();
	chip_8.load("roms/maze");

	for(int i = 0; i < 10; i++) {
		chip_8.cycle();
		if (chip_8.renderFlag) chip_8.render(); 
	}

	return 0;
}