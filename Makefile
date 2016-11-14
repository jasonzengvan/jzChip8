all: 
	g++ chip_8_emulator.cc chip_8.cc -Wall -framework OpenGL -framework GLUT -Wno-deprecated-declarations -o chip_8_emulator