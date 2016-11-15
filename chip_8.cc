#include "chip_8.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>		// std::hex
#include <stdlib.h>     // srand, rand 
#include <time.h>       // time 
#include <unistd.h>

void Chip_8::initialize() {
	pc = 0x200;
	opcode = 0;
	index = 0;
	sp = 0;

	// TODO: clear everything

	uint8_t fontset[80] = { 
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	for (int i = 0; i < 80; i++) {
		mem[i] = fontset[i];
	}
}

void Chip_8::load(std::string path) {
	std::ifstream fin(path);
	if (fin.is_open()) {
		uint8_t byte;
		int i = 0x200;
		while(fin >> std::noskipws >> byte) {
			mem[i++] = byte;
		}
		fin.close();
	}
}

void Chip_8::cycle() {

	//printStates();

	fetch();
	decode_execute();
	update_timers();
}

void Chip_8::fetch() {
	opcode = mem[pc] << 8 | mem[pc+1];
}

void Chip_8::decode_execute() {

	const uint16_t N = opcode & 0x000f;
	const uint16_t NN = opcode & 0x00ff;
	const uint16_t NNN = opcode & 0x0fff;
	const uint16_t X = (opcode & 0x0f00) >> 8;
	const uint16_t Y = (opcode & 0x00f0) >> 4;
	int key_pressed = 0;

	switch(opcode & 0xf000) {
		case 0x0000:
			switch(opcode) {
				// 00E0: Clears the screen
				case 0x00e0: 
					clearGraphics();
					pc += 2;
				break;

				// 00EE: Returns from a subroutine
				case 0x00ee:
					pc = stack[--sp];
					pc += 2;
				break;

				default:
					printf ("Unknown opcode: 0x%X\n", opcode);
					exit(1);
			}
		break;

		// 1NNN: Jumps to address NNN
		case 0x1000: 
			pc = NNN;
		break;

		// 2NNN : Calls subroutine at NNN
		case 0x2000:
			stack[sp++] = pc;
			pc = NNN;
		break;

		// 3XNN: Skips the next instruction if VX equals NN
		case 0x3000: 
			if (v[X] == NN) pc += 2;
			pc += 2;
		break;

		// 3XNN: Skips the next instruction if VX doesn't equal NN
		case 0x4000: 
			if (v[X] != NN) pc += 2;
			pc += 2;
		break;

		// 5XY0: Skips the next instruction if VX equals VY
		case 0x5000: 
			if (v[X] == v[Y]) pc += 2;
			pc += 2;
		break;

		// 6XNN: Sets VX to NN
		case 0x6000: 
			v[X] = NN;
			pc += 2;
		break;

		// 7XNN: Adds NN to VX
		case 0x7000: 
			v[X] += NN;
			pc += 2;
		break;

		case 0x8000:
			switch(opcode & 0x000f) {

				// 8XY0: Sets VX to the value of VY
				case 0x0000: 
					v[X] = v[Y];
					pc += 2;
				break;

				// 8XY1: Sets VX to VX or VY
				case 0x0001: 
					v[X] = v[X] | v[Y];
					pc += 2;
				break;

				// 8XY2: Sets VX to VX and VY
				case 0x0002: 
					v[X] = v[X] & v[Y];
					pc += 2;
				break;

				// 8XY3: Sets VX to VX xor VY
				case 0x0003: 
					v[X] = v[X] ^ v[Y];
					pc += 2;
				break;

				// 8XY4: Adds VY to VX 
				// VF is set to 1 when there's a carry, and to 0 when there isn't
				case 0x0004: 
					if (v[X] > (0xff - v[Y])) v[0xf] = 1;
					else v[0xf] = 0;
					v[X] += v[Y];
					pc += 2;
				break;

				// 8XY5: VY is subtracted from VX 
				// VF is set to 0 when there's a borrow, and 1 when there isn't
				case 0x0005: 
					if (v[X] < v[Y]) v[0xf] = 0;
					else v[0xf] = 1;
					v[X] -= v[Y];
					pc += 2;
				break;

				// 8XY6: Shifts VX right by one 
				// VF is set to the value of the least significant bit of VX before the shift
				case 0x0006: 
					v[0xf] = v[X] & 0x01;
					v[X] >>= 1;
					pc += 2;
				break;

				// 8XY7: Sets VX to VY minus VX 
				// VF is set to 0 when there's a borrow, and 1 when there isn't
				case 0x0007: 
					if (v[Y] < v[X]) v[0xf] = 0;
					else v[0xf] = 1;
					v[X] = v[Y] - v[X];
					pc += 2;
				break;

				// 8XYE: Shifts VX left by one 
				// VF is set to the value of the most significant bit of VX before the shift
				case 0x000e: 
					v[0xf] = (v[X] & 0x80) >> 7;
					v[X] <<= 1;
					pc += 2;
				break;

				default:
					printf ("Unknown opcode: 0x%X\n", opcode);
					exit(1);

			}
		break;

		// 9XY0: Skips the next instruction if VX doesn't equal VY
		case 0x9000: 
			if (v[X] != v[Y]) pc += 2;
			pc += 2;
		break;

		// ANNN: Sets I to the address NNN
		case 0xa000: 
			index = NNN;
			pc += 2;
		break;

		// BNNN: Jumps to the address NNN plus V0
		case 0xb000: 
			pc = v[0] + NNN;
		break;

		// CXNN: Sets VX to a random number (Typically: 0 to 255) and NN
		case 0xc000: 
			srand(time(NULL));
			v[X] = (rand() % 0x100) & NN;
			pc += 2;
		break;

		// DXYN: Draws a sprite at coordinate (VX, VY)
		// that has a width of 8 pixels and a height of N pixels. 
		// Each row of 8 pixels is read as bit-coded starting from memory location I; 
		// I value doesn’t change after the execution of this instruction. 
		// As described above, VF is set to 
		// 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
		// and to 0 if that doesn’t happen

		case 0xd000: 
			v[0xf] = 0;
			for(int y = 0; y < N; y++) {
				uint8_t pixels = mem[index + y];
				for (int x = 0; x < 8; x++) {
					if((pixels & (0x80 >> x)) != 0) {
				        if(graphics[v[X] + x][v[Y] + y] == 1)
				        	v[0xf] = 1;                                 
				        graphics[v[X] + x][v[Y] + y] ^= 1;
				    }
				}
			}
			renderFlag = 1;
			pc += 2;
			// FPS ~= 60
			usleep(16667);
		break;

		case 0xe000:
			switch(opcode & 0x00ff) {
				// EX9E: Skips the next instruction if the key stored in VX is pressed
				case 0x009e: 	
					if (input[v[X]]) {
						pc += 2;
					}
					pc += 2;
				break;

				// EXA1: Skips the next instruction if the key stored in VX isn't pressed
				case 0x00a1: 
					if (!input[v[X]]) pc += 2;
					pc += 2;
				break;

				default:								
					printf ("Unknown opcode: 0x%X\n", opcode);
					exit(1);

			}
		break;

		case 0xf000:
			switch(opcode & 0x00ff) {
				// FX07: Sets VX to the value of the delay timer
				case 0x0007: 
					v[X] = delay_timer;
					pc += 2;
				break;

				// FX0A: A key press is awaited, and then stored in VX
				case 0x000a: 
					for (int i = 0; i < 0x10; i++) {
						if (input[i]) {
							v[X] = i;
							key_pressed = 1;
							input[i] = 0;
						}
					}
					if (key_pressed) {
						pc += 2;
					}
				break;

				// FX15: Sets the delay timer to VX
				case 0x0015: 
					delay_timer = v[X];
					pc += 2;
				break;

				// FX18: Sets the sound timer to VX
				case 0x0018: 
					sound_timer = v[X];
					pc += 2;
				break;

				// FX1E: Adds VX to I
				case 0x001e: 
					index += v[X];
					pc += 2;
				break;

				// FX29: Sets I to the location of the sprite for the character in VX. 
				// Characters 0-F (in hexadecimal) are represented by a 4x5 font
				case 0x0029: 
					index = v[X] * 5;
					pc += 2;
				break;

				// FX33: Stores the binary-coded decimal representation of VX, 
				// with the most significant of three digits at the address in I, 
				// the middle digit at I plus 1, 
				// and the least significant digit at I plus 2
				case 0x0033: 
					mem[index] = v[X] / 100;
					mem[index + 1] = (v[X] / 10) % 10;
					mem[index + 2] = v[X] % 10;
					pc += 2;
				break;

				// FX55: Stores V0 to VX (including VX) in memory starting at address I
				case 0x0055: 
					for (int i = 0; i <= X; i++) 
						mem[index + i] = v[i];
					pc += 2;
				break;

				// FX65: Fills V0 to VX (including VX) with values from memory starting at address I
				case 0x0065: 
					for (int i = 0; i <= X; i++) 
						v[i] = mem[index + i];
					pc += 2;
				break;

				default:
					printf ("Unknown opcode: 0x%X\n", opcode);
					exit(1);
			}
		break;

		default: 
			printf ("Unknown opcode: 0x%X\n", opcode);
			exit(1);
	}
}

void Chip_8::renderGraphics() {
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			if (graphics[i][j] == 1)
				std::cout << "*";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-----------------------------------------------------------------" << std::endl;
}

void Chip_8::clearGraphics() {
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			graphics[i][j] = 0;
		}
	}
	renderFlag = 1;
}

void Chip_8::update_timers() {
	if (delay_timer) {
		delay_timer--;
	}
	if(sound_timer) {
		// TODO: BEEP
		sound_timer--;
	}
}

void Chip_8::printStates() {
	std::cout << "pc = " << std::hex <<(int)pc << "  ";
	std::cout << "opcode = " << std::hex <<(int)opcode << "  ";
	std::cout << "index = " << std::hex <<(int)index << "  ";
	std::cout << "sp = " << std::hex <<(int)sp << "  " <<std::endl;
	for(int i = 0; i < 0x10; i++) {
		std::cout << "V" << std::hex << (int)i;
		std::cout << " = " << (int)v[i] << "    ";
		if (i % 4 == 3) std::cout << std::endl;
	}
	std::cout << "stack = ";
	for (int i = 0; i < 0x10; i++) {
		std::cout << std::hex << (int)stack[i];
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "----------------" << std::endl;
}