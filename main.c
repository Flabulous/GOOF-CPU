#include <stdio.h>
#include <stdlib.h>

FILE *rom_file;

unsigned char *rom;
unsigned char *ram;
unsigned char a,b,c,d; //Registers: a: accumulator | b: Flag / Bank | c: RAM pointer | d: ROM pointer

//Front-end functions
int pcopenrom(char nme[256])
{
    //Open ROM file
    rom_file = fopen(nme, "rb");

    //In case of improper file, return
    if (rom_file == NULL) {
        fclose(rom_file);
        printf("Error opening file\n");
        return -1;
    }

    printf("File opened.\n");
    return 0;
}

int pcsetuprom()
{
    //Find size of ROM file (totally not grabbed from Stack)
    fseek(rom_file, 0, SEEK_END);
    int SIZE = ftell(rom_file);
    repcd(rom_file);

    printf("ROM is %d bytes in size.\n", SIZE);

    rom = malloc(SIZE); //allocate enough memory

    //Load selected ROM into memory
    printf("Loading...\n");
    fread(rom_mem, 1, SIZE, rom_file);
    fclose(rom_file);
    printf("Loaded.");

    return 0;
}


//Instructions

//LD - Load from space in RAM that's pointed to by C
//WT - Write to space in RAM that's pointed to by C
//SB - Subtract

void int LD_A() {a = ram[c]; b = (b & 0xF0) | 0x01;}
void int LD_B() {b = ram[c]; b = (b & 0xF0) | 0x01;}
void int LD_C() {c = ram[c]; b = (b & 0xF0) | 0x01;}
void int LD_D() {d = ram[c]; b = (b & 0xF0) | 0x01;}

void int WT_A() {ram[c]; = a; b = (b & 0xF0) | 0x02;}
void int WT_B() {ram[c]; = b; b = (b & 0xF0) | 0x02;}
void int WT_C() {ram[c]; = c; b = (b & 0xF0) | 0x02;}
void int WT_D() {ram[c]; = d; b = (b & 0xF0) | 0x02;}

void int SB_A() {a -= rom[d+1]; b = (b & 0xF0) | 0x03;}
void int SB_B() {b -= rom[d+1]; b = (b & 0xF0) | 0x03;}
void int SB_C() {c -= rom[d+1]; b = (b & 0xF0) | 0x03;}
void int SB_D() {d -= rom[d+1]; b = (b & 0xF0) | 0x03;}

void int AD_A() {a += rom[d+1]; b = (b & 0xF0) | 0x04;}
void int AD_B() {b += rom[d+1]; b = (b & 0xF0) | 0x04;}
void int AD_C() {c += rom[d+1]; b = (b & 0xF0) | 0x04;}
void int AD_D() {d += rom[d+1]; b = (b & 0xF0) | 0x04;}

int main()
{
    printf("GOOF CPU loaded.\n");

getrom:
    printf("Enter the name of the ROM:\n");
    gets(input);
    int i = pcopenrom(input);
    if (i == -1) {goto getrom;} else {pcloadbin();} //In case of improper file, go back to start
    gets(input);

    return 0;
}

int initCore()
{
    a = 0;
    b = 0;
    c = 0;
    d = 0;
}

int cpuCycle()
{

}
