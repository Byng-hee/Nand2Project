//function.h
#include <stdio.h>
#ifndef parser_H
#define parser_H

typedef enum { C_ARITHMEIC, C_PUSH, C_POP, C_LABEL,
     C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL} commandtype;

void parser(FILE *open, FILE* write, segment *s1);
commandtype find_commandtype(char* token);

#endif