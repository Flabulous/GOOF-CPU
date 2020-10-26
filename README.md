# GOOF-CPU
Literally me goofing off and making an entire virtual 8-bit CPU in C.

# Registers:
A - Accumulator
B - Flag
C - RAM Counter
D - ROM Counter

# 16 Instructions:
0x0 - Load ram pointed to by C into A register
0x1 - Load ram pointed to by C into B register
0x2 - Load ram pointed to by C into C register
0x3 - Load ram pointed to by C into D register

0x4 - Write A register into ram pointed to by C
0x5 - Write B register into ram pointed to by C
0x6 - Write C register into ram pointed to by C
0x7 - Write D register into ram pointed to by C

0x8 - Subtract immediate 8bit data from A register
0x9 - Subtract immediate 8bit data from B register
0xA - Subtract immediate 8bit data from C register
0xB - Subtract immediate 8bit data from D register

0xC - Add immediate 8bit data to A register
0xD - Add immediate 8bit data to B register
0xE - Add immediate 8bit data to C register
0xF - Add immediate 8bit data to D register

# Bank Switching:
GOOF supports up to 255 different 256-byte banks. Banks are automatically split and created from the .bin file. Editing RAM adress 0xFF will allow you to switch different banks. Writing 0xFF to this location will trigger an EoF and close the CPU.
