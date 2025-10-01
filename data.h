#ifndef data_h
#define data_h

#define MAX_LINE_LENGTH 80          
#define MAX_MACROS_COUNT 100        
#define MAX_SYMBOL_ENTRIES 4096     
#define MAX_INSTRUCTION_COUNT 3996  
#define WORD_SIZE 15                
#define MAX_SYMBOL_USAGE 4096 
#define MAX_SYMBOL_COUNT 1024     

/* Data array to hold values for .string and .data directives */
extern int* dataMemory;
extern int dataMemorySize;
/* Error flag to indicate if there was an error during processing */
extern int hasErrorOccurred;
/* External symbol table array to hold symbols */

extern int symbolRegistryCount; 

extern int externalUsageCount;
extern int instructionBufferCount; 
extern int symbolCount;


/* Structure to hold information about symbols in the assembly code */
typedef struct {
    char identifier[MAX_LINE_LENGTH]; 
    char category[MAX_LINE_LENGTH];   
    int value;                        
} CodeSymbol;

typedef struct {
    char name[50];             
    char identifier[10];      
    int value;                
} Symbol;

typedef struct {
    char symbolName[50];
    int locations[MAX_SYMBOL_COUNT];
} ExternalSymbolUsage;


/* Structure to hold information about operations */
typedef struct {
    const char* mnemonic; 
    const char* machineCode; 
} AssemblerOperation;

/* Predefined array of operations and their machine codes */
static const AssemblerOperation instructionSet[] = {
    {"mov", "0000"},
    {"cmp", "0001"},
    {"add", "0010"},
    {"sub", "0011"},
    {"not", "0100"},
    {"clr", "0101"},
    {"lea", "0110"},
    {"inc", "0111"},
    {"dec", "1000"},
    {"jmp", "1001"},
    {"bne", "1010"},
    {"red", "1011"},
    {"prn", "1100"},
    {"jsr", "1101"},
    {"rts", "1110"},
    {"hlt", "1111"},
};

/* Structure to hold parts of an instruction line */
typedef struct {
    char command[MAX_LINE_LENGTH];  
    char* argument1;                
    char* argument2;                
} InstructionComponent;

/* Structure to hold information about external usage of symbols */
typedef struct {
    char symbolLabel[MAX_LINE_LENGTH]; 
    int positions[MAX_SYMBOL_USAGE];   
} ExternalUsage;

ExternalUsage externalUsageList[MAX_SYMBOL_ENTRIES];
extern CodeSymbol symbolRegistry[MAX_SYMBOL_ENTRIES];
extern ExternalSymbolUsage externUsageTable[MAX_SYMBOL_COUNT];

int locateSymbol(char* identifier);
InstructionComponent parseInstructionLine(char* instructionLine);
int isWhitespaceLine(char* line);
/* Array to hold instructions in the assembly code */
char instructionBuffer[MAX_INSTRUCTION_COUNT][WORD_SIZE];
#endif 
