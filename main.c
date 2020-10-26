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
initCore();
cpuRuntime();

//Instructions

//LD - Load from space in RAM that's pointed to by C
//WT - Write to space in RAM that's pointed to by C
//SB - Subtract immediate 8 bits from register
//AD - Add immediate 8 bits to register

void LD_A() {a = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_B() {b = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_C() {c = ram[c]; b = (b & 0xF0) | 0x01;}
void LD_D() {d = ram[c]; b = (b & 0xF0) | 0x01;}

void WT_A() {ram[c] = a; b = (b & 0xF0) | 0x02;}
void WT_B() {ram[c] = b; b = (b & 0xF0) | 0x02;}
void WT_C() {ram[c] = c; b = (b & 0xF0) | 0x02;}
void WT_D() {ram[c] = d; b = (b & 0xF0) | 0x02;}

void SB_A() {d++; a -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_B() {d++; b -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_C() {d++; c -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}
void SB_D() {d++; d -= rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x03;}

void AD_A() {d++; a += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_B() {d++; b += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_C() {d++; c += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}
void AD_D() {d++; d += rom[ram[0xFF]].bank[d]; b = (b & 0xF0) | 0x04;}

void (*insttable[0xF])() =
{LD_A,
 LD_B,
 LD_C,
 LD_D,
 WT_A,
 WT_B,
 WT_C,
 WT_D,
 SB_A,
 SB_B,
 SB_C,
 SB_D,
 AD_A,
 AD_B,
 AD_C,
 AD_D,};

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

    cpuRuntime();
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
    while (ram[0xFF] != 0xFF)
    {
        printf("Cycles: %d, Counter: %d\n, Instruction: %x\n", cycles, d, rom[ram[0xFF]].bank[d]);
        d++;
        (*insttable[rom[ram[0xFF]].bank[d]])();
        sleep(1);
        cycles++;
    }
    return 0;
}
