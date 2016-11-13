#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>		// std::hex
#include <stdlib.h>     // srand, rand 
#include <time.h>       // time 


class Chip_8 {
	public:
		void initialize();
		void load(std::string);
		void cycle();
		void fetch();
		void decode_execute();
		void render();
		void setInput();
		void printStates();

		uint8_t renderFlag;

	private:
		uint8_t mem[0x1000];
		uint8_t v[0x10];
		uint16_t opcode;
		uint16_t index;
		uint16_t pc;
		uint8_t stack[0x10];
		uint8_t sp;
		uint8_t graphics[64 * 32];
		uint8_t delay_timer;
		uint8_t sound_timer;
		uint8_t input[0x10];
	
};