/*this code implements a pre-assembler in ANSI C, which is a program that processes source code before actual assembly. Specifically, it defines functions to handle macros in assembly code. Macros are sequences of instructions or directives that are given a name and can be reused throughout the code. The functions in this code handle the expansion of macros in assembly files, allowing for more efficient and organized code writing*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *strdup(const char *s);

#include "pre_assembler.h"

Macro macros[MAX_MACROS_COUNT];
int num_macros = 0;

/* Function to free all macros from the macro array */
void free_macros(void)
{
    int i, j;
    for (i = 0; i < num_macros; i++)
    {
        for (j = 0; j < macros[i].num_lines; j++)
        {
            free(macros[i].lines[j]); /* Free each line in the macro */
        }
        free(macros[i].lines);
    }
    num_macros = 0;
}

/* Function to remove any leading and trailing whitespace from a string */
void trim_whitespace(char *str)
{
    char *start = str;
    char *end = str + strlen(str) - 1;

    /* Trim leading whitespace */
    while (start <= end && (*start == ' ' || *start == '\t'))
    {
        start++;
    }

    while (end >= start && (*end == ' ' || *end == '\t' || *end == '\n'))
    {
        *end = '\0';
        end--;
    }

    memmove(str, start, end - start + 2);
}

/* Function to replace macros in a line and write to the output file */
void replace_macros(char *line, FILE *output_file)
{
    int i, j;
    for (i = 0; i < num_macros; i++)
    {
        if (strncmp(line, macros[i].name, strlen(macros[i].name)) == 0)
        {

            for (j = 0; j < macros[i].num_lines; j++)
            {
                fprintf(output_file, "%s\n", macros[i].lines[j]);
            }
            return;
        }
    }

    /* Line is not a macro call; write it to the output file */
    if (fprintf(output_file, "%s\n", line) < 0)
    {
        fprintf(stderr, "ERROR: Failed to write to output file\n");
        exit(1);
    }
}

/* Function to expand macros from input file and write to output file */
void expand_macros(FILE *input_file, FILE *output_file)
{
    char line[MAX_LINE_LENGTH];
    int in_macro = 0;
    Macro *current_macro = NULL; /* Pointer to the current macro */

    while (fgets(line, sizeof(line), input_file) != NULL)
    {

        line[strcspn(line, "\n")] = '\0';

        trim_whitespace(line);

        /* Check for macro definition start */
        if (strncmp(line, "mcr", 3) == 0)
        {
            char keyword[4];
            in_macro = 1;
            current_macro = &macros[num_macros++];

            sscanf(line, "%3s %s", keyword, current_macro->name);

            current_macro->name[MAX_LINE_LENGTH - 1] = '\0';
            current_macro->lines = NULL;
            current_macro->num_lines = 0;

            /* Check for macro definition end */
        }
        else if (strncmp(line, "endmcr", 6) == 0)
        {
            in_macro = 0; /* Exit macro definition */
            current_macro = NULL;

            /* Inside a macro definition */
        }
        else if (in_macro)
        {
            /* Reallocate memory for a new line */
            char **new_lines = realloc(current_macro->lines, (current_macro->num_lines + 1) * sizeof(char *));
            if (new_lines == NULL)
            {
                fprintf(stderr, "ERROR: Memory allocation failed\n");
                exit(1);
            }
            current_macro->lines = new_lines;

            /* Add the line to the current macro */
            current_macro->lines[current_macro->num_lines] = strdup(line);
            if (current_macro->lines[current_macro->num_lines] == NULL)
            {
                fprintf(stderr, "ERROR: Memory allocation failed\n");
                exit(1);
            }
            current_macro->num_lines++;
        }
        else
        {
            replace_macros(line, output_file);
        }
    }
}

