//parser.h
#ifndef parser_H
#define parser_H
#include <stdlib.h>
#include <stdbool.h>
#include "code.c"

typedef struct REGISTER{
    static int Memory[43256];
    static int address[16];
    static int M; 
    static int I, D;
}reg;

typedef enum { A_COMMAND, C_COMMAND, L_COMMAND } CommandType;

CommandType get_command_type(const char* str);
char* symbol(char* str, CommandType AorL);
int* C_COMPUTING(char* line);
void binaryAddress(int n, int *address);

#endif
