# GOOF-CPU
Literally me goofing off and making an entire virtual 8-bit CPU in C.

# Registers:
A - Basic Accumulator
B - Logical Accumulator
C - RAM Counter
D - ROM Counter

# Instructions:

1 Byte:
0x00 - Load ram pointed to by C into A register
0x01 - Load ram pointed to by C into B register
0x02 - Load ram pointed to by C into C register
0x03 - Load ram pointed to by C into D register

0x04 - Write A register into ram pointed to by C
0x05 - Write B register into ram pointed to by C
0x06 - Write C register into ram pointed to by C
0x07 - Write D register into ram pointed to by C

2 Bytes:
0x08 - Subtract immediate 8bit data from A register
0x09 - Subtract immediate 8bit data from B register
0x0A - Subtract immediate 8bit data from C register
0x0B - Subtract immediate 8bit data from D register

0x0C - Add immediate 8bit data to A register
0x0D - Add immediate 8bit data to B register
0x0E - Add immediate 8bit data to C register
0x0F - Add immediate 8bit data to D register

3 Bytes:
0x1A - If b = xx, Load ram pointed to by C into A register
0x1B - If b = xx, Write A register into ram pointed to by C
0x1C - If b = xx, Subtract immediate 8bit data from A register
0x1D - If b = xx, Add immediate 8bit data to A register

# Bank Switching:
GOOF supports up to 255 different 256-byte banks, for a total of 65280 bytes. 
Banks are automatically split and created from the .bin file. 
Editing RAM adress 0xFF will allow you to switch different banks. Writing 0xFF to this location will trigger an EoF and close the CPU.

