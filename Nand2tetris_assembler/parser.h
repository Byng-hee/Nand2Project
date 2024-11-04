//parser.h
#ifndef parser_H
#define parser_H
#include <stdlib.h>
#include <stdbool.h>
#include "code.c"

typedef struct REGISTER{
    int Memory[43256];
    int M; 
    int I, D;
}reg;

typedef enum { A_COMMAND, C_COMMAND, L_COMMAND } CommandType;

CommandType get_command_type(const char* str);
char* symbol(char* str, CommandType AorL);
const char* C_COMPUTING(char* line, CommandType ALC);

#endif
