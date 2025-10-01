#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "data.h"
#include "second_pass.h"
#include "first_pass.h"


CodeSymbol symbolRegistry[MAX_SYMBOL_ENTRIES];
void updateExternalUsage(const char *symbolName, int InstructionCounter){}
int InstructionCounter=0;


void addAdditionalWords(char *instructionLine) {
    InstructionParts parts = splitInstruction(instructionLine);
    char *operand1 = parts.operand1;
    char *operand2 = parts.operand2;
    int i;

    
    InstructionCounter++;

    if (operand2 == NULL && operand1 != NULL) {
        operand2 = operand1;
        operand1 = NULL;
    }

    
    while (operand1 && isspace((unsigned char)*operand1)) {
        operand1++;
    }
    while (operand2 && isspace((unsigned char)*operand2)) {
        operand2++;
    }


    if (operand1 && operand1[0] == 'r' && operand2 && operand2[0] == 'r') {
        
        int reg1 = operand1[1] - '0';
        int reg2 = operand2[1] - '0';
        char binRegisters[WORD_SIZE] = "00000000000000";
        char binReg1Num[4];
        char binReg2Num[4];

        
        for ( i = 2; i >= 0; i--) {
            binReg1Num[i] = (reg1 & 1) ? '1' : '0';
            reg1 >>= 1;
            binReg2Num[i] = (reg2 & 1) ? '1' : '0';
            reg2 >>= 1;
        }

        /* Fill the binary register numbers in the word*/
        binReg1Num[3] = '\0';
        binReg2Num[3] = '\0';
        strncpy(binRegisters + 6, binReg1Num, 3);
        strncpy(binRegisters + 9, binReg2Num, 3);

        /* Null-terminate the string*/
        binRegisters[WORD_SIZE - 1] = '\0';

        /* Add to instructionBuffer*/
        strcpy(instructionBuffer[InstructionCounter], binRegisters);
        printf("Added register operands, word: %s\n", binRegisters);
        InstructionCounter++;
        return; 
    }

    /* Handling the first operand*/
    if (operand1) {
       
        if (operand1[0] == '#') {
            char *valueStr = operand1 + 1;
            int value;
            char binary[WORD_SIZE];


            if (sscanf(valueStr, "%d", &value) != 1) {

                value = findMacroDefine(valueStr);
            }

            intToBinaryString(value, binary);


            strcpy(instructionBuffer[InstructionCounter], binary);
            printf("Added immediate operand, word: %s\n", binary);
        } else if (strstr(operand1, "[") && strstr(operand1, "]")) {

            char *arrayName;
            char *tempOperand1 = strdup(operand1);
            char *tempOperand2 = strdup(operand1);
            int symbolIndex, arrayAddress;
            char binAddress[WORD_SIZE];
            int value;
            char binIndex[WORD_SIZE];
	char *indexStr = strtok(tempOperand2, "[");
            

            /* Extract the array name*/
            arrayName = strtok(tempOperand1, "[");
            symbolIndex = locateSymbol(arrayName);
            arrayAddress = symbolRegistry[symbolIndex].value;


            intToBinaryString(arrayAddress, binAddress);


            if (strcmp(symbolRegistry[symbolIndex].identifier, "external") == 0) {

                updateExternalUsage(arrayName, InstructionCounter);
                binAddress[12] = '0';
                binAddress[13] = '1';
            } else {
                binAddress[12] = '1';
                binAddress[13] = '0';
            }
            binAddress[14] = '\0';
            strcpy(instructionBuffer[InstructionCounter], binAddress);
            printf("Added array index operand, word 1/2: %s\n", binAddress);

            /* Increment instruction counter*/
            InstructionCounter++;
            free(tempOperand1);



            indexStr = strtok(NULL, "]");

            /* Parse the index value and convert it to binary*/
            if (sscanf(indexStr, "%d", &value) != 1) {

                value = findMacroDefine(indexStr);
            }
            printf("Index value: %d\n", value);
            free(tempOperand2);

            /* Convert the index to binary*/
            intToBinaryString(value, binIndex);


            strcpy(instructionBuffer[InstructionCounter], binIndex);
            printf("Added array index operand, word 2/2: %s\n", binIndex);
        } else if (operand1[0] == 'r' && isdigit(operand1[1]) && operand1[1] >= '0' && operand1[1] <= '7') {
            /* Direct register addressing: r4*/
            int regNum = operand1[1] - '0';
            char binRegister[WORD_SIZE] = "00000000000000";
            char binRegNum[4];
            int i;

            /* Convert the register number to binary*/
            for (i = 2; i >= 0; i--) {
                binRegNum[i] = (regNum & 1) ? '1' : '0';
                regNum >>= 1;
            }

            binRegNum[3] = '\0';
            binRegister[6] = binRegNum[0];
            binRegister[7] = binRegNum[1];
            binRegister[8] = binRegNum[2];

            binRegister[14] = '\0'; 


            strcpy(instructionBuffer[InstructionCounter], binRegister);
            printf("Added direct register operand, word: %s\n", binRegister);
        } else {

            int symbolIndex = locateSymbol(operand1);
            int symbolAddress = symbolRegistry[symbolIndex].value;
            char binAddress[WORD_SIZE];


            intToBinaryString(symbolAddress, binAddress);


            if (strcmp(symbolRegistry[symbolIndex].identifier, "external") == 0) {
                updateExternalUsage(operand1, InstructionCounter);
                binAddress[12] = '0';
                binAddress[13] = '1';
            } else {
                binAddress[12] = '1';
                binAddress[13] = '0';
            }

            binAddress[14] = '\0'; 
            strcpy(instructionBuffer[InstructionCounter], binAddress);
            printf("Added direct operand, word: %s\n", binAddress);
        }

        /* Increment instruction counter*/
        InstructionCounter++;
    }

    /* Handling the second operand*/
    if (operand2) {

        if (operand2[0] == '#') {
            char *valueStr = operand2 + 1;
            int value;
            char binary[WORD_SIZE];


            if (sscanf(valueStr, "%d", &value) != 1) {

                value = findMacroDefine(valueStr);
            }

            intToBinaryString(value, binary);


            strcpy(instructionBuffer[InstructionCounter], binary);
            printf("Added immediate operand, word: %s\n", binary);
        } else if (strstr(operand2, "[") && strstr(operand2, "]")) {

            char *arrayName;
            char *tempOperand1 = strdup(operand2);
            char *tempOperand2 = strdup(operand2);
            int symbolIndex, arrayAddress;
            char binAddress[WORD_SIZE];
            int value;
            char binIndex[WORD_SIZE];
            char *indexStr = strtok(tempOperand2, "[");

            /* Extract the array name*/
            arrayName = strtok(tempOperand1, "[");
            symbolIndex = locateSymbol(arrayName);
            arrayAddress = symbolRegistry[symbolIndex].value;


            intToBinaryString(arrayAddress, binAddress);


            if (strcmp(symbolRegistry[symbolIndex].identifier, "external") == 0) {

                updateExternalUsage(arrayName, InstructionCounter);
                binAddress[12] = '0';
                binAddress[13] = '1';
            } else {
                binAddress[12] = '1';
                binAddress[13] = '0';
            }

            binAddress[14] = '\0';
            strcpy(instructionBuffer[InstructionCounter], binAddress);
            printf("Added array index operand, word 1/2: %s\n", binAddress);


            InstructionCounter++;
            free(tempOperand1);



            indexStr = strtok(NULL, "]");


            if (sscanf(indexStr, "%d", &value) != 1) {
                /* If not a number, resolve from symbol table*/
                value = findMacroDefine(indexStr);
            }
            printf("Index value: %d\n", value);
            free(tempOperand2);


            intToBinaryString(value, binIndex);


            strcpy(instructionBuffer[InstructionCounter], binIndex);
            printf("Added array index operand, word 2/2: %s\n", binIndex);
        } else if (operand2[0] == 'r' && isdigit(operand2[1]) && operand2[1] >= '0' && operand2[1] <= '7') {

            int regNum = operand2[1] - '0';
            char binRegister[WORD_SIZE] = "00000000000000";
            char binRegNum[4];

            /* Convert the register number to binary*/
            for (i = 2; i >= 0; i--) {
                binRegNum[i] = (regNum & 1) ? '1' : '0';
                regNum >>= 1;
            }

            binRegNum[3] = '\0';
            binRegister[9] = binRegNum[0];
            binRegister[10] = binRegNum[1];
            binRegister[11] = binRegNum[2];

            binRegister[14] = '\0'; 

            strcpy(instructionBuffer[InstructionCounter], binRegister);
            printf("Added direct register operand, word: %s\n", binRegister);
        } else {

            int symbolIndex = locateSymbol(operand2);
            int symbolAddress = symbolRegistry[symbolIndex].value;
            char binAddress[WORD_SIZE];


            intToBinaryString(symbolAddress, binAddress);


            if (strcmp(symbolRegistry[symbolIndex].identifier, "external") == 0) {
                updateExternalUsage(operand2, InstructionCounter);
                binAddress[12] = '0';
                binAddress[13] = '1';
            } else {
                binAddress[12] = '1';
                binAddress[13] = '0';
            }

            binAddress[14] = '\0';
            strcpy(instructionBuffer[InstructionCounter], binAddress);
            printf("Added direct operand, word: %s\n", binAddress);
        }


        InstructionCounter++;
    }
}

void scanAgainAndAddWords(FILE *file, CodeSymbol *symbolRegistry) {
    char line[MAX_LINE_LENGTH];  
    char *colonPosition;

    printf("~~~~~~~~ SECOND SCAN! ~~~~~~~~\n");

    while (fgets(line, sizeof(line), file)) {
        printf("\n------------------------\n");
        printf("%s\n", line);
	if (line[0] == ';' || isWhitespaceLine(line)) {
            continue;
        }

        if (strncmp(line, ".extern", 7) == 0){
            printf("extern line, ignoring\n");
            continue;
        }
        else if (strncmp(line, ".entry", 6) == 0) {
            char symbolName[MAX_LINE_LENGTH];
            int symbolIndex;
            sscanf(line, ".entry %s", symbolName);
            symbolIndex = locateSymbol(symbolName);
            /* changing the symbol identifier to entry */
            if (symbolIndex != -1) {
                strcpy(symbolRegistry[symbolIndex].identifier, "entry");
                printf("Changing symbol %s identifier to entry\n", symbolName);
            }
            continue;
        } else if (strncmp(line, ".define", 7) == 0) {
            printf("define line, ignoring\n");
            continue;
        }

        /* if its a symbol defenition like "SYMBOL: ____" */
        /* saving the pointer for the ":" in the line if exists */
        colonPosition = strchr(line, ':');
        if (colonPosition) {
            char *directiveStart;

            /* move the pointer to the start of the directive */
            directiveStart = colonPosition + 1;
            /* Remove leading white spaces */
            while(isspace((unsigned char) *directiveStart)) {
                directiveStart++;
            }
            
            /* skipping .data .string */
            /* .data */
            if (strlen(directiveStart) >= 6 && strncmp(directiveStart, ".data ", 6) == 0) {
                printf("data directive, ignoring\n");
            }
            /* .string */
            else if (strlen(directiveStart) >= 8 && strncmp(directiveStart, ".string ", 8) == 0) {
                printf("string directive, ignoring\n");
            }
            /* code */
            else {
                char *instructionStart;
                /* Extract the part of the line after the ":" */
                instructionStart = colonPosition + 1;
                /* Skip any leading spaces or tabs */
                instructionStart += strspn(instructionStart, " \t");
                addAdditionalWords(instructionStart);
            }
        }
        /* independant operation */
        else {
            printf("independant operation, adding words...\n");
            addAdditionalWords(line);
        }
    }
    printAllData();
}
 

