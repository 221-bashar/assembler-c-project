/**
 * Assembler for assembly language programs.
 *
 * This program reads an assembly language file, processes it through multiple stages including
 * macro expansion, symbol table creation, and binary encoding, and outputs the result into
 * several files including object code and symbol tables.
 *
 * Usage:
 *   assembler input_file.as
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "encoding.h"
#include "define_variable.h"
#include "data.h"




Symbol symbolTable[MAX_SYMBOL_COUNT]; /* Symbol table array */
int *dataArray;                       /* Array to hold data segment values */
int dataArraySize;                    /* Size of the data array */

int main(int argc, char *argv[])
{
    FILE *input_file, *am_file, *ob_file;
      char *input_filename;
    char *amFileName;
    char *obFileName;
    char *entFileName;
    char *extFileName;
    char *copy_input_filename = strdup(argv[1]); /* Duplicate the input filename for manipulation */

    /* Allocate memory for the data array and initialize its size */
    dataArray = (int *)malloc(80 * sizeof(int));
    dataArraySize = 80;

    if (argc != 2)
    {
        printf("Usage: %s input_file.as\n", argv[0]); 
        return 1;
    }

    /* Create strings for output file names based on the input file name */
    input_filename = strtok(copy_input_filename, ".");
    amFileName = malloc(strlen(input_filename) + strlen(".am") + 1);
    obFileName = malloc(strlen(input_filename) + strlen(".ob") + 1);
    entFileName = malloc(strlen(input_filename) + strlen(".ent") + 1);
    extFileName = malloc(strlen(input_filename) + strlen(".ext") + 1); 
    strcpy(amFileName, input_filename);
    strcpy(obFileName, input_filename);
    strcpy(entFileName, input_filename);
    strcpy(extFileName, input_filename);
    strcat(amFileName, ".am");
    strcat(obFileName, ".ob");
    strcat(entFileName, ".ent");
    strcat(extFileName, ".ext");

    input_file = fopen(argv[1], "r");
    am_file = fopen(amFileName, "w");

    /* Check if files were opened successfully */
    if (input_file == NULL)
    {
        printf("Error: Unable to open input file %s.\n", argv[1]);
        free(copy_input_filename);
        free(amFileName);
        exit(1);
    }
    if (am_file == NULL)
    {
        printf("Error: Unable to open output file %s.\n", amFileName);
        free(copy_input_filename);
        free(amFileName);
        exit(1);
    }
    free(copy_input_filename);

    /* Expand macros in the input file and write the result to the am_file */
    expand_macros(input_file, am_file);

    free(dataArray);

    /* Close files */
    fclose(input_file);
    fclose(am_file);

    /* Reopen the .am file in read mode */
    am_file = fopen(amFileName, "r");
    if (am_file == NULL)
    {
        printf("Error: Unable to open output file %s.\n", amFileName);
        exit(1);
    }

    free(amFileName);
    scanAndAllocateWords(am_file, symbolTable); /* Scan symbols and allocate words */

    /* Reset the file pointer to the start of the file */
    rewind(am_file);

    scanAgainAndAddWords(am_file, symbolTable); /* Scan again and add words */

    ob_file = fopen(obFileName, "w");
    if (ob_file == NULL)
    {
        printf("Error: Unable to open output file %s.\n", obFileName);
        exit(1);
    }

    writeObject(ob_file); /* Write the object file */
    /* It will create the file only if there are entries */
    entryFileCreator(entFileName);
    externalFileCreator(extFileName);

    return 0;
}
