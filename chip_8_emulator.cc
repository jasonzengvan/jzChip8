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
	printf("...\n");
}

void display() {
	chip_8.cycle();
	if (chip_8.renderFlag) {
		printf("Rendering...\n");

		glClear(GL_COLOR_BUFFER_BIT);
		//updateTexture(chip_8);
		updateScreen(chip_8);
		glutSwapBuffers();
		chip_8.renderFlag = 0;
	} 
}



int main(int argc, char *argv[]) {
	chip_8.initialize();
	chip_8.load("roms/MERLIN");

	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutIdleFunc(display);
	//setupTexture();	
	glutMainLoop();


	return 0;
}