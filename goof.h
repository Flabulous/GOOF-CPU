//Main variables
FILE *rom_file;

struct sbank {unsigned char *bank;};
struct sbank rom[256];

unsigned char *ram;
unsigned char a,b,c,d; //Registers: a: Accumulator | b: Logical Accumulator | c: RAM pointer | d: ROM pointer

//Back end functions:
int initTable();



//Front end functions:
int cpuRuntime();
int pcopenrom();
int pcsetuprom();

//GUI functions:
int initGUI();
int refreshGUI();

//Instructions:
//...
//I'll do this later
