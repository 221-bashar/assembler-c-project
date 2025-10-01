/* This program contains functions for handling assembly-like instructions.
   It provides functionalities to locate symbols in a symbol registry, parse
   instruction lines into components (command and operands), and check if a
   line contains only whitespace characters. These functions are designed to
   facilitate the manipulation and interpretation of assembly-like code within
   a larger system. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "data.h"

/* Find the index of a symbol in the symbol registry by its identifier */
int locateSymbol(char* identifier) {
    int i;
    int len = strlen(identifier);
CodeSymbol symbolRegistry[MAX_SYMBOL_ENTRIES] = {0};  
int symbolRegistryCount = 0;  


    /* Remove newline at the end of identifier */
    if (len > 0 && identifier[len - 1] == '\n') {
        identifier[len - 1] = '\0';
    }

  
    for (i = 0; i < symbolRegistryCount; i++) {
        if (strcmp(symbolRegistry[i].identifier, identifier) == 0) {
            return i;  
        }
    }
    
    return -1;
}

/* Split an instruction line into its components (command and operands) */
InstructionComponent parseInstructionLine(char* instructionLine) {
    InstructionComponent component;
    char* tokens;
    int len;

    
    component.argument1 = NULL;
    component.argument2 = NULL;

    /* Split the instruction line into the operation and operands */
    tokens = strtok(instructionLine, " ");
    if (tokens != NULL) {
        strncpy(component.command, tokens, MAX_LINE_LENGTH);
        component.command[MAX_LINE_LENGTH - 1] = '\0';
    } else {
        component.command[0] = '\0';
    }

    /* Split operands if available */
    tokens = strtok(NULL, ",");
    if (tokens != NULL) {
        component.argument1 = tokens;

        tokens = strtok(NULL, ",");
        if (tokens != NULL) {
            component.argument2 = tokens;
        }
    }

    /* remove whitespace from the command */
     len = strlen(component.command);
    while (len > 0 && isspace((unsigned char)component.command[len - 1])) {
        component.command[len - 1] = '\0';
        len--;
    }

    return component;
}

/* Check if a line contains only whitespace */
int isWhitespaceLine(char* line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

