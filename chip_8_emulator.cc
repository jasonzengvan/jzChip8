#include "chip_8.h"
#include <stdint.h>
#include <string>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>

#define PIXEL_SIZE 10

Chip_8 chip_8;

void renderPixel(int x, int y) {
	float x1_coordinate = ((float) x) * 2 / ((float) SCREEN_WIDTH) - 1; 
	float y1_coordinate = ((float) -y) * 2 / ((float) SCREEN_HEIGHT) + 1;
	float x2_coordinate = ((float) x + 1) * 2 / ((float) SCREEN_WIDTH) - 1; 
	float y2_coordinate = ((float) -y - 1) * 2 / ((float) SCREEN_HEIGHT) + 1;

	glBegin(GL_QUADS);
		glVertex3f(x1_coordinate, y1_coordinate, 0.0);
		glVertex3f(x1_coordinate, y2_coordinate, 0.0);
		glVertex3f(x2_coordinate, y2_coordinate, 0.0);
		glVertex3f(x2_coordinate, y1_coordinate, 0.0);
	glEnd();
}

void updateScreen(Chip_8 chip_8) {
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			if (!chip_8.graphics[x][y]) {
				
			} else {
				renderPixel(x, y);
			}
			
		}
	}
}

void display() {
	chip_8.cycle();
	if (chip_8.renderFlag) {
		glClear(GL_COLOR_BUFFER_BIT);
		updateScreen(chip_8);
		glutSwapBuffers();
		chip_8.renderFlag = 0;
	}
}


void pressKey(unsigned char input, int x, int y) {
	if(input == '1')      chip_8.input[0x1] = 1;
	else if(input == '2') chip_8.input[0x2] = 1;
	else if(input == '3') chip_8.input[0x3] = 1;
	else if(input == '4') chip_8.input[0xc] = 1;
	else if(input == 'q') chip_8.input[0x4] = 1;
	else if(input == 'w') chip_8.input[0x5] = 1;
	else if(input == 'e') chip_8.input[0x6] = 1;
	else if(input == 'r') chip_8.input[0xd] = 1;
	else if(input == 'a') chip_8.input[0x7] = 1;
	else if(input == 's') chip_8.input[0x8] = 1;
	else if(input == 'd') chip_8.input[0x9] = 1;
	else if(input == 'f') chip_8.input[0xe] = 1;
	else if(input == 'z') chip_8.input[0xa] = 1;
	else if(input == 'x') chip_8.input[0x0] = 1;
	else if(input == 'c') chip_8.input[0xb] = 1;
	else if(input == 'v') chip_8.input[0xf] = 1;
	else if(input == 27) exit(0);
}

void releaseKey(unsigned char input, int x, int y) {
	if(input == '1')      chip_8.input[0x1] = 0;
	else if(input == '2') chip_8.input[0x2] = 0;
	else if(input == '3') chip_8.input[0x3] = 0;
	else if(input == '4') chip_8.input[0xc] = 0;
	else if(input == 'q') chip_8.input[0x4] = 0;
	else if(input == 'w') chip_8.input[0x5] = 0;
	else if(input == 'e') chip_8.input[0x6] = 0;
	else if(input == 'r') chip_8.input[0xd] = 0;
	else if(input == 'a') chip_8.input[0x7] = 0;
	else if(input == 's') chip_8.input[0x8] = 0;
	else if(input == 'd') chip_8.input[0x9] = 0;
	else if(input == 'f') chip_8.input[0xe] = 0;
	else if(input == 'z') chip_8.input[0xa] = 0;
	else if(input == 'x') chip_8.input[0x0] = 0;
	else if(input == 'c') chip_8.input[0xb] = 0;
	else if(input == 'v') chip_8.input[0xf] = 0;
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
	  std::cerr << "Usage: chip_8_emulator rom_file" << std::endl;
	  throw std::exception();
	}



	chip_8.initialize();
	chip_8.load(argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Chip-8 Emulator by JsnZng");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(pressKey);
	glutKeyboardUpFunc(releaseKey);
	glutMainLoop();


	return 0;
}