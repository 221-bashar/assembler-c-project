/* encoding.c: Implementation of encoding functionalities */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoding.h"
#include "data.h"
#include "first_pass.h" 

/* Base-4 symbols mapping */
static const char base4_symbols[] = {'*', '#', '%', '!'};

/* Function to encode opcode into special base 4 with symbols */
char* encode_opcode(int opcode) {
    static char encoding[3]; /* Two symbols for base-4 encoding + null terminator */

    /* Validate opcode range */
    if(opcode < 0 || opcode >= OPCODE_COUNT) {
        encoding[0] = '\0'; 
        return encoding;
    }

    encoding[0] = base4_symbols[opcode / 4]; 
    encoding[1] = base4_symbols[opcode % 4]; 
    encoding[2] = '\0';                      

    return encoding;
}

/* Function to encode the 'A,R,E' field into special base 4 with symbols */
char* encode_era(int eraValue) {
    static char eraEncoding[3]; 

    /* Validate 'A,R,E' value range */
    if(eraValue < 0 || eraValue > 3) {
        eraEncoding[0] = '\0';
        return eraEncoding;
    }

    eraEncoding[0] = base4_symbols[eraValue / 2]; 
    eraEncoding[1] = base4_symbols[eraValue % 2]; 
    eraEncoding[2] = '\0';                        

    return eraEncoding;
}
void writeObject(FILE* ob_file) {
    /* Check if the file is open */
    if (ob_file == NULL) {
        printf("Error: File is not open.\n");
        return;
    }
}

