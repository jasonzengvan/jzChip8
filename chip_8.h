#ifndef __CHIP_8
#define __CHIP_8

#include <stdint.h>
#include <string>



#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define CLOCK_SPEED 60

class Chip_8 {
	public:
		uint8_t renderFlag;
		uint8_t graphics[SCREEN_WIDTH][SCREEN_HEIGHT]; 
		uint8_t input[0x10];

		void initialize();
		void load(std::string);
		void cycle();
		

		

	private:
		uint8_t mem[0x1000]; // 0x1000 byte Memory
		uint8_t v[0x10]; // 16 Registers v0 - vf
		uint16_t opcode; // current Opcode
		uint16_t index; // a memory Index
		uint16_t pc; // program counter
		uint16_t stack[0x10]; // stack for return addresses
		uint16_t sp; // stack pointer
		uint8_t delay_timer;
		uint8_t sound_timer;
		

		void renderGraphics();
		void fetch();
		void decode_execute();
		void update_timers();
		void setInput();
		void printStates();
		void clearGraphics();
	
};

#endif