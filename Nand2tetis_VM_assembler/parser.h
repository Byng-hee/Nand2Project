//function.h
#include <stdio.h>
#include "function.h"
#ifndef parser_H
#define parser_H

segment_B* createSegment_B(const char*name, segment_A* s1, int value);
void addSegment_B(segment_B** s2);

typedef enum { C_ARITHMEIC, C_PUSH, C_POP, C_LABEL,
     C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, C_IFGOTO} commandtype;

void parser(FILE *open, FILE* write, segment_A* s1);
commandtype find_commandtype(char* token);

#endif