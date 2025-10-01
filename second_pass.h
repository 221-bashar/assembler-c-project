#ifndef SECOND_SCAN_H
#define SECOND_SCAN_H
#include <stdio.h>
#include "data.h"
#include "first_pass.h" 

void updateExternalUsage(const char *symbolName, int InstructionCounter);


void addAdditionalWords(char* instructionLine);


void scanAgainAndAddWords(FILE *file, CodeSymbol *symbolRegistry);



#endif 

