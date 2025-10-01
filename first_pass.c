/* This program is part of a two-pass assembler for a hypothetical assembly-like language.
   It implements the first pass of the assembler, which scans the source file, processes
   instructions, and generates the symbol table, data array, and instruction table.
   The program defines functions to split instructions, add data to the data array, convert
   integers to binary strings, handle first instruction words, parse instruction lines, scan
   and allocate words from the source file, and add symbol entries to the symbol table. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "first_pass.h"
#include "data.h"

/* Global variables */
int *dataArray = NULL;
int DC = 0;
int IC = 0;
int symbolCount = 0;
int dataArraySize = 100;
int isError = 0;

/* Function prototypes */
char instructionTable[MAX_INSTRUCTIONS][WORD_LEN];
Symbol variableTable[MAX_SYMBOL_COUNT];
int externUsageCount = 0;
InstructionParts splitInstruction(char *instructionLine);


/* Function to find symbol value from variable table by name */
int findMacroDefine(char *symbolName)
{
    int i;
    int length = strlen(symbolName);

    while (isspace((unsigned char)*symbolName))
    {
        symbolName++;
    }
    while (length > 0 && isspace((unsigned char)symbolName[length - 1]))
    {
        symbolName[length - 1] = '\0';
        length--;
    }

    /* Iterate over the variableTable */
    for (i = 0; i < symbolCount; i++)
    {
        if (strcmp(variableTable[i].name, symbolName) == 0 && strcmp(variableTable[i].identifier, "mdefine") == 0)
        {
            return variableTable[i].value;
        }
    }

    printf("Error: Symbol %s not found or not an mdefine.\n", symbolName);
    isError = 1;
    return -1;
}

/* Add a data to dataArray */
void appendData(int value)
{
    int *tempDataArray;

    /* Check if dataArray is full and reallocate more memory if needed */
    if (DC >= dataArraySize)
    {
        tempDataArray = realloc(dataArray, dataArraySize * 2 * sizeof(int));
        if (tempDataArray)
        {
            dataArray = tempDataArray;
            dataArraySize *= 2;
        }
        else
        {
            perror("Failed to reallocate memory for dataArray");
            exit(EXIT_FAILURE);
        }
    }

    dataArray[DC] = value;
    DC++;
    printf("ADDED DATA: %d | : %d\n", value, DC);
}

/* Convert integer number to 14-bit binary string */
void intToBinaryString(int num, char *binary)
{
    int i;
    for (i = 13; i >= 0; i--)
    {
        binary[13 - i] = ((num >> i) & 1) ? '1' : '0';
    }
    binary[14] = '\0'; /* Add null terminator */
}

/* Add first word of instruction */
void addFirstInstructionWord(char *instructionLine)
{
    char *operation, *operand1 = NULL, *operand2 = NULL;
    int i;
    int operationFound = 0;
    char resultWord[WORD_LEN + 1] = "0000";
    int additionalWords = 0;
    InstructionParts parts = splitInstruction(instructionLine);

    operation = parts.operation;
    operand1 = parts.operand1;
    operand2 = parts.operand2;

    /* Iterate over instructionSet array checking operation validity */
    for (i = 0; i < sizeof(instructionSet) / sizeof(instructionSet[0]); i++)
    {
        if (strcmp(instructionSet[i].mnemonic, operation) == 0)
        {
            strcat(resultWord, instructionSet[i].machineCode);
            operationFound = 1;
            break;
        }
    }

    /* Calculate additional words based on operands */
    if (operand1 && operand1[0] == 'r' && operand2 && operand2[0] == 'r')
    {
        additionalWords = 1; /* Two register operands can be combined into one word */
    }
    else
    {
        additionalWords = (operand1 ? 1 : 0) + (operand2 ? 1 : 0);
        if (operand1 && strstr(operand1, "[") && strstr(operand1, "]"))
        {
            additionalWords++;
        }
        if (operand2 && strstr(operand2, "[") && strstr(operand2, "]"))
        {
            additionalWords++;
        }
    }

    /* Handle source operand bits 5-4 */
    if (operand1 != NULL)
    {
        if (operand1[0] == '#')
        {
            strcat(resultWord, "00");
        }
        else if (strstr(operand1, "[") && strstr(operand1, "]"))
        {
            strcat(resultWord, "10");
        }
        else if (operand1[0] == 'r' && isdigit(operand1[1]))
        {
            strcat(resultWord, "11");
        }
        else
        {
            strcat(resultWord, "01");
        }
    }
    else
    {
        strcat(resultWord, "00");
    }

    /* Handle target operand bits 3-2 */
    if (operand2 != NULL)
    {
        if (operand2[0] == '#')
        {
            strcat(resultWord, "00");
        }
        else if (strstr(operand2, "[") && strstr(operand2, "]"))
        {
            strcat(resultWord, "10");
        }
        else if (operand2[0] == 'r' && isdigit(operand2[1]))
        {
            strcat(resultWord, "11");
        }
        else
        {
            strcat(resultWord, "01");
        }
    }
    else
    {
        strcat(resultWord, "00");
    }

    /* Add A, R, E bits */
    strcat(resultWord, "00");

    printf("Additional words: %d\n", additionalWords);
    printf("Word: %s\n", resultWord);

    if (!operationFound)
    {
        printf("Error: Unknown operation - %s.\n", operation);
        isError = 1;
    }
    else
    {
        /* Check if there's enough space in the instructionTable */
        if (IC + additionalWords >= MAX_INSTRUCTIONS)
        {
            printf("Error: Too many instructions.\n");
            isError = 1;
            return;
        }
        /* Add the resultWord to the instructionTable */
        strcpy((char *)instructionTable[IC], resultWord);
        IC += additionalWords + 1;
    }
}

/* Function to parse the instruction line */
InstructionParts splitInstruction(char *instructionLine)
{
    InstructionParts parts;
    char *token = strtok(instructionLine, " ");
    if (token)
    {
        parts.operation = token;
        parts.operand1 = strtok(NULL, " ");
        parts.operand2 = strtok(NULL, " ");
    }
    else
    {
        parts.operation = NULL;
        parts.operand1 = NULL;
        parts.operand2 = NULL;
    }
    return parts;
}

void addSymbolEntry(char *symbolName, int symbolValue, char *symbolIdentifier)
{
    if (symbolCount >= MAX_SYMBOL_COUNT)
    {
        printf("Error: Symbol table overflow.\n");
        isError = 1;
        return;
    }

    /* Add the symbol entry */
    strcpy(variableTable[symbolCount].name, symbolName);
    variableTable[symbolCount].value = symbolValue;
    strcpy(variableTable[symbolCount].identifier, symbolIdentifier);
    symbolCount++;
}
/* Print all data for debugging purposes */
void printAllData(void)
{
    int i;
    printf("Data count: %d\nInstruction count: %d\n", DC, IC);
    printf("~~~~~~~ Symbols: ~~~~~~~\n");
    for (i = 0; i < symbolCount; i++)
    {
        printf("Name: %s, Value: %d, Identifier: %s\n", variableTable[i].name, variableTable[i].value, variableTable[i].identifier);
    }

    printf("\n~~~~~~~ Instruction Table: ~~~~~~~\n");
    for (i = 0; i < IC; i++)
    {
        printf("Instruction %d: %s\n", i, (char *)instructionTable[i]);
    }

    printf("\n~~~~~~~ Data Array: ~~~~~~~\n");
    for (i = 0; i < DC; i++)
    {
        printf("Data: %d\n", dataArray[i]);
    }
}

/* Scan and allocate words from the file */
void scanAndAllocateWords(FILE *file, Symbol *variableTable)
{
    char line[MAX_LINE_LENGTH];
    int i;

    while (fgets(line, sizeof(line), file))
    {
        char symbolName[MAX_LINE_LENGTH];
        char symbolIdentifier[10];
        int symbolValue;
        char *colonPosition;

        printf("\n--------------------------\n");
        printf("%s\tIC=%d\n", line, IC);

        /* Skip comments and whitespace lines */
        if (line[0] == ';' || isWhitespaceLine(line))
        {
            continue;
        }

        /* Parse the line according to a .define line */
        if (strncmp(line, ".define", 7) == 0)
        {
            strcpy(symbolIdentifier, "mdefine");
            sscanf(line, ".define %s = %d", symbolName, &symbolValue);
            addSymbolEntry(symbolName, symbolValue, symbolIdentifier);
            continue;
        }

        /* Parse the line according to a .extern line */
        else if (strncmp(line, ".extern", 7) == 0)
        {
            strcpy(symbolIdentifier, "external");
            sscanf(line, ".extern %s", symbolName);
            addSymbolEntry(symbolName, -1, symbolIdentifier);
            continue;
        }

        /* Parse the line according to a .entry line */
        else if (strncmp(line, ".entry", 6) == 0)
        {
            continue;
        }

        /* Handle code with a colon in the line */
        colonPosition = strchr(line, ':');
        if (colonPosition)
        {
            char *directiveStart;
            int length = colonPosition - line;

            printf("Symbol found!\n");

            /* Copy the symbol name from the line */
            strncpy(symbolName, line, length);
            symbolName[length] = '\0';

            directiveStart = colonPosition + 1;
            while (isspace((unsigned char)*directiveStart))
            {
                directiveStart++;
            }

            printf("SymbolName: %s | directiveStart: %s\n", symbolName, directiveStart);

            /* Handle .data directive */
            if (strncmp(directiveStart, ".data ", 6) == 0)
            {
                char *pointer = strtok(directiveStart + 6, ",");
                strcpy(symbolIdentifier, "data");
                symbolValue = DC;
                addSymbolEntry(symbolName, symbolValue, symbolIdentifier);

                /* Parse each data value and add it to the array */
                while (pointer != NULL)
                {
                    int value;
                    char binary[15];

                    if (sscanf(pointer, "%d", &value) != 1)
                    {
                        value = findMacroDefine(pointer);
                    }

                    appendData(value);
                    intToBinaryString(value, binary);
                    strcpy((char *)instructionTable[IC++], binary);

                    pointer = strtok(NULL, ",");
                }
            }

            /* handle .string directive */
            else if (strncmp(directiveStart, ".string ", 8) == 0)
            {
                char *pointer = strchr(directiveStart, '\"');
                char zero[15] = "00000000000000";

                strcpy(symbolIdentifier, "data");
                symbolValue = DC;
                addSymbolEntry(symbolName, symbolValue, symbolIdentifier);

                if (pointer == NULL)
                {
                    printf("Error: Missing starting quote in .string directive\n");
                    isError = 1;
                    continue;
                }

                pointer++;

                /* Parse each character in the string */
                while (*pointer != '\"')
                {
                    char binary[15];
                    appendData((int)*pointer);

                    intToBinaryString((int)*pointer, binary);
                    strcpy((char *)instructionTable[IC++], binary);

                    pointer++;
                    if (*pointer == '\0')
                    {
                        printf("Error: Missing closing quote in .string directive\n");
                        isError = 1;
                        break;
                    }
                }

                /* Add a null character to the array */
                appendData(0);
                strcpy((char *)instructionTable[IC++], zero);
            }

            else
            {
                char *instructionStart;
                printf("Code symbol\n");

                strcpy(symbolIdentifier, "code");
                symbolValue = IC + 100;
                addSymbolEntry(symbolName, symbolValue, symbolIdentifier);

                instructionStart = colonPosition + 1;
                instructionStart += strspn(instructionStart, " \t");

                addFirstInstructionWord(instructionStart);
            }
        }
        else
        {
            addFirstInstructionWord(line);
        }
    }

    /* Update symbol values for data symbols */
    for (i = 0; i < symbolCount; i++)
    {
        if (strcmp(variableTable[i].identifier, "data") == 0)
        {
            variableTable[i].value += IC + 100 - DC;
        }
    }

    printAllData();
}


