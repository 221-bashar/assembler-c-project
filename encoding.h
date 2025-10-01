/* encoding.h: Header file for encoding functionalities */

#ifndef ENCODING_H
#define ENCODING_H
#include "data.h"
#include <stdio.h>

#define OPCODE_COUNT 16

char* encode_opcode(int opcode);
char* encode_era(int eraValue);
void writeObject(FILE* ob_file);
#endif 
