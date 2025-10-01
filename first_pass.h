/*header for first_pass.c*/

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include "data.h"


#define MAX_INSTRUCTIONS 1024
#define MAX_LINE_LENGTH 80
#define WORD_LEN 15
#define SYMBOL_NAME_LEN 50
#define SYMBOL_IDENTIFIER_LEN 10
#define DATA_ARRAY_SIZE 1024

/* Structure for holding parts of an instruction */
typedef struct {
    char *operation;
    char *operand1;
    char *operand2;
} InstructionParts;



/* Define Instruction structure */
typedef struct {
    char *mnemonic;
    char *machineCode;
} Instruction;




/* Global variables */
extern int *dataArray;
extern int DC;
extern char instructionTable[MAX_INSTRUCTIONS][WORD_LEN];
extern int IC;
extern Symbol variableTable[MAX_SYMBOL_COUNT];
extern int externUsageCount;
extern int isError;


/* Function declarations */

int findMacroDefine(char *symbolName);
void appendData(int value);
void addSymbolEntry(char *symbolName, int symbolValue, char *symbolIdentifier);
InstructionParts splitInstruction(char *instructionLine);
void intToBinaryString(int num, char *binary);
void addFirstInstructionWord(char *instructionLine);
void printAllData(void);
void scanAndAllocateWords(FILE *file, Symbol *variableTable);

#endif 
