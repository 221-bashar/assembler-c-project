/* This program includes functions for generating entry and external files based on symbol information.
   It defines functions to create entry files containing symbol names and values, and external files
   containing external symbol names and their locations. These files are used in the context of a larger
   system to manage symbol data for an assembly-like language. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Include necessary header files*/
#include "data.h"
#include "encoding.h"
#include "define_variable.h"

Symbol variableTable[MAX_SYMBOL_COUNT];
int defineSymbolCount  = 0;
int externUsageSum  = 0;

/*Creates an entry file containing the 'entry' symbols from the variable table*/
void entryFileCreator(const char* outputFileName) {
    FILE *entryFile;
    int index;
    int foundEntries = 0;

    /* Check if there are any 'entry' symbols in the variableTable */
    for (index = 0; index < defineSymbolCount ; index++) {
        if (strcmp(variableTable[index].identifier, "entry") == 0) {
            foundEntries = 1;
            break;
        }
    }

    /* If there are no entries, return without creating a file */
    if (!foundEntries) return;

    entryFile = fopen(outputFileName, "w");
    if (entryFile == NULL) {
        fprintf(stderr, "Error: Unable to open output file %s.\n", outputFileName);
        exit(EXIT_FAILURE);
    }

    /* Write the entries to the file */
    for (index = 0; index < defineSymbolCount ; index++) {
        if (strcmp(variableTable[index].identifier, "entry") == 0) {
            fprintf(entryFile, "%s\t%04d\n", variableTable[index].name, variableTable[index].value);
        }
    }

    fclose(entryFile);
}


/*Creates an external file containing the 'external' symbols from the external usage table.*/
void externalFileCreator(const char* outputFileName) {
    FILE *externFile;
    int symbolIndex, usageIndex;
    int foundExterns = 0;

    /* Use new name ExternalSymbolUsage */
    ExternalSymbolUsage* externUsageTable = externUsageTable; 

    /* Check if there are any 'external' symbols in the variableTable */
    for (symbolIndex = 0; symbolIndex < defineSymbolCount ; symbolIndex++) {
        if (strcmp(variableTable[symbolIndex].identifier, "external") == 0) {
            foundExterns = 1;
            break;
        }
    }

    /* If there are no externs, return without creating a file */
    if (!foundExterns) return;

    externFile = fopen(outputFileName, "w");
    if (externFile == NULL) {
        fprintf(stderr, "Error: Unable to open output file %s.\n", outputFileName);
        exit(EXIT_FAILURE);
    }

    /* Write the externs to the file */
    for (symbolIndex = 0; symbolIndex < externUsageSum; symbolIndex++) {
        for (usageIndex = 0; usageIndex < MAX_USAGE; usageIndex++) {
            if (externUsageTable[symbolIndex].locations[usageIndex] != 0) {
                fprintf(externFile, "%s\t%04d\n", externUsageTable[symbolIndex].symbolName, externUsageTable[symbolIndex].locations[usageIndex] + 100);
            }
        }
    }

    fclose(externFile);
}

