# Chip-8 Emulator
This is a Chip-8 emulator in C++.
### Platform
Only executable in Mac now. Should be easy to support other platforms in the future since there are only few platform-specific implementations.  
### CPU Cycle
A cycle consists of fetch(), decode_execute(), update_timer().  
The implementations are based on [Chip-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8).  
### Keyboard Input
Chip-8 uses hex inputs, 0x1 - 0xF.  
Keyboard mapping:  
|1|2|3|C|  ========> |1|2|3|4|  
|4|5|6|D|  ========> |Q|W|E|R|  
|7|8|9|E|  ========> |A|S|D|F|  
|A|0|B|F|  ========> |Z|X|C|V|  
### Graphics
Graphic display is achieved by using OpenGL.  
FPS is hardcored to ~60 by using sleep() although I couldn't find any sources about the refreshing rate of the original Chip-8.  
### Sound
Not supported yet.