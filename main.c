#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
FILE *rom_file;

struct sbank {unsigned char *bank;};
struct sbank rom[256];

unsigned char *ram;
unsigned char a,b,c,d; //Registers: a: Accumulator | b: Logical Accumulator | c: RAM pointer | d: ROM pointer

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
    fseek(rom_file, 0, SEEK_SET);

    printf("ROM is %d bytes in size.\n", SIZE);

    int banks = round(SIZE/256);
    //Load selected ROM into memory
    printf("Loading...\n");
    for (int i = banks; i >= 0; i--) {
        rom[i].bank = malloc(256);
        fread(rom[i].bank, 1, 256, rom_file);
    }

    fclose(rom_file);
    printf("Loaded.\n");

    return 0;
}

//Back-end functions
int initTable();
int cpuRuntime();

//Instructions

//LD - Load from space in RAM that's pointed to by C
//WT - Write to space in RAM that's pointed to by C
//SB - Subtract immediate 8 bits from register
//AD - Add immediate 8 bits to register

//I - Logical. Only affects the A register

void LD_A() { a = ram[c];}
void LD_B() { b = ram[c];}
void LD_C() { c = ram[c];}
void LD_D() { d = ram[c];}

void WT_A() { ram[c] = a;}
void WT_B() { ram[c] = a;}
void WT_C() { ram[c] = c;}
void WT_D() { ram[c] = d;}

void SB_A() { d++; a -= rom[ram[0xFF]].bank[d];}
void SB_B() { d++; b -= rom[ram[0xFF]].bank[d];}
void SB_C() { d++; c -= rom[ram[0xFF]].bank[d];}
void SB_D() { d++; d -= rom[ram[0xFF]].bank[d];}

void AD_A() { d++; a += rom[ram[0xFF]].bank[d];}
void AD_B() { d++; a += rom[ram[0xFF]].bank[d];}
void AD_C() { d++; c += rom[ram[0xFF]].bank[d];}
void AD_D() { d++; d += rom[ram[0xFF]].bank[d];}

void I_LD_A() { d++; if(b == rom[ram[0xFF]].bank[d]) {a = ram[c];}}
void I_LD_B() { d++; if(b == rom[ram[0xFF]].bank[d]) {b = ram[c];}}
void I_LD_C() { d++; if(b == rom[ram[0xFF]].bank[d]) {c = ram[c];}}
void I_LD_D() { d++; if(b == rom[ram[0xFF]].bank[d]) {d = ram[c];}}

void I_WT_A() { d++; if(b == rom[ram[0xFF]].bank[d]) {ram[c] = a;}}
void I_WT_B() { d++; if(b == rom[ram[0xFF]].bank[d]) {ram[c] = b;}}
void I_WT_C() { d++; if(b == rom[ram[0xFF]].bank[d]) {ram[c] = c;}}
void I_WT_D() { d++; if(b == rom[ram[0xFF]].bank[d]) {ram[c] = d;}}

void I_SB_A() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; a -= rom[ram[0xFF]].bank[d];} else {d++;}}
void I_SB_B() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; b -= rom[ram[0xFF]].bank[d];} else {d++;}}
void I_SB_C() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; c -= rom[ram[0xFF]].bank[d];} else {d++;}}
void I_SB_D() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; d -= rom[ram[0xFF]].bank[d];} else {d++;}}

void I_AD_A() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; a += rom[ram[0xFF]].bank[d];} else {d++;}}
void I_AD_B() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; b += rom[ram[0xFF]].bank[d];} else {d++;}}
void I_AD_C() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; c += rom[ram[0xFF]].bank[d];} else {d++;}}
void I_AD_D() { d++; if(b == rom[ram[0xFF]].bank[d]) {d++; d += rom[ram[0xFF]].bank[d];} else {d++;}}

void (*inst_table[256])();

int main()
{
    printf("GOOF CPU loaded.\n");

    char input[0xFF];

    do{
        printf("Enter the name of the ROM:\n");
        gets(input);
    }while(pcopenrom(input) == -1);

    pcsetuprom();

    printf("Press Enter to run.\n");
    gets(input);

    initTable();
    cpuRuntime();
    return 0;
}

int initTable()
{
    inst_table[0x00] = &LD_A;
    inst_table[0x01] = &LD_B;
    inst_table[0x02] = &LD_C;
    inst_table[0x03] = &LD_D;

    inst_table[0x04] = &WT_A;
    inst_table[0x05] = &WT_B;
    inst_table[0x06] = &WT_C;
    inst_table[0x07] = &WT_D;

    inst_table[0x08] = &SB_A;
    inst_table[0x09] = &SB_B;
    inst_table[0x0A] = &SB_C;
    inst_table[0x0B] = &SB_D;

    inst_table[0x0C] = &AD_A;
    inst_table[0x0D] = &AD_B;
    inst_table[0x0E] = &AD_C;
    inst_table[0x0F] = &AD_D;

    inst_table[0x10] = &I_LD_A;
    inst_table[0x11] = &I_LD_B;
    inst_table[0x12] = &I_LD_C;
    inst_table[0x13] = &I_LD_D;

    inst_table[0x14] = &I_WT_A;
    inst_table[0x15] = &I_WT_B;
    inst_table[0x16] = &I_WT_C;
    inst_table[0x17] = &I_WT_D;

    inst_table[0x18] = &I_SB_A;
    inst_table[0x19] = &I_SB_B;
    inst_table[0x1A] = &I_SB_C;
    inst_table[0x1B] = &I_SB_D;

    inst_table[0x1C] = &I_AD_A;
    inst_table[0x1D] = &I_AD_B;
    inst_table[0x1E] = &I_AD_C;
    inst_table[0x1F] = &I_AD_D;

    return 0;
}
int cpuRuntime()
{
    int cycles =0;
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    ram = malloc(256);
    ram[0xFF] = 0x00;

   // for (int i = 16; i >= 0; i--) {
   //     inst_table[i];
   //     printf("%x\n",inst_table[i]);
   // }

    while (ram[0xFF] != 0xFF)
    {
        inst_table[rom[ram[0xFF]].bank[d]]();
        d++;
        printf("%x %x %x %x - %d \r",a, b, c, d, ram[0xAA]);
        sleep(1);
    }

    printf("CPU read EoF.");
    char input[1];
    gets(input);
    return 0;
}
