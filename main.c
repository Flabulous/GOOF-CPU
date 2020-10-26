#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
FILE *rom_file;

struct sbank {unsigned char *bank;};
struct sbank rom[256];

unsigned char *ram;
unsigned char a,b,c,d; //Registers: a: accumulator | b: Flag / Second Accu | c: RAM pointer | d: ROM pointer



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

char setlowbit(char orig, char nibble) {
    char res = orig;
    res &= 0xF0; // Clear out the lower nibble
    res |= (nibble & 0x0F); // OR in the desired mask
    return res;
}

char setupbit(char orig, char nibble) {
    char res = orig;
    res &= 0x0F; // Clear out the upper nibble
    res |= ((nibble << 4) & 0xF0); // OR in the desired mask
    return res;
}

//Back-end functions
int initTable();
int cpuRuntime();

//Instructions

//LD - Load from space in RAM that's pointed to by C
//WT - Write to space in RAM that's pointed to by C
//SB - Subtract immediate 8 bits from register
//AD - Add immediate 8 bits to register

void LD_A() {printf("debug"); a = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_B() {printf("debug"); b = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_C() {printf("debug"); c = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_D() {printf("debug"); d = ram[c]; b = (b & 0xF0) | 0x01;}

void WT_A() {printf("debug"); ram[c] = a; b = (b & 0xF0) | 0x02;}
void WT_B() {printf("debug"); ram[c] = b; b = (b & 0xF0) | 0x02;}
void WT_C() {printf("debug"); ram[c] = c; b = (b & 0xF0) | 0x02;}
void WT_D() {printf("debug"); ram[c] = d; b = (b & 0xF0) | 0x02;}

void SB_A() {printf("debug"); d++; a -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_B() {printf("debug"); d++; b -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_C() {printf("debug"); d++; c -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_D() {printf("debug"); d++; d -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}

void AD_A() {printf("debug"); d++; a += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_B() {printf("debug"); d++; b += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_C() {printf("debug"); d++; c += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_D() {printf("debug"); d++; d += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}

void (*inst_table[16])();

int main()
{
    printf("GOOF CPU loaded.\n");

    char input[256];

getrom:
    printf("Enter the name of the ROM:\n");
    gets(input);
    int i = pcopenrom(input);
    if (i == -1) {goto getrom;} else {pcsetuprom();} //In case of improper file, go back to start

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
        printf("Cycles: %d, Counter: %d\n, Instruction: %x\n", cycles, d, rom[ram[0xFF]].bank[d]);
        d++;
        inst_table[rom[ram[0xFF]].bank[d]]();
        sleep(1);
        cycles++;
        printf("%x",ram[0xFF]);
    }
    return 0;
}
