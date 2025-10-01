#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include <string.h>
#include <stdio.h>
#include "data.h"


typedef struct Macro {
    char name[MAX_LINE_LENGTH];  
    char **lines;                
    int num_lines;               
} Macro;

/* External variables */
extern Macro macros[MAX_MACROS_COUNT]; 
extern int num_macros;           


void free_macros(void);          
void trim_whitespace(char *str);   
void replace_macros(char *line, FILE *output_file); 
void expand_macros(FILE *input_file, FILE *output_file); 

#endif 

