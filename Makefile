all: chip_8_emulator.o chip_8.o
	g++ chip_8_emulator.o chip_8.o -framework OpenGL -framework GLUT -Wno-deprecated-declarations -o chip_8_emulator

chip_8_emulator.o: chip_8_emulator.cc
	g++ chip_8_emulator.cc -c

chip_8.o: chip_8.cc
	g++ chip_8.cc -c

clean: 
	rm -f chip_8_emulator chip_8_emulator.o chip_8 chip_8.o