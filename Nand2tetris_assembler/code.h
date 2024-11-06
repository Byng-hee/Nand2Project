#ifndef CODE_H
#define CODE_H
#include "parser.h"

int dest(const char* str, CommandType command);
int comp(const char* str, CommandType command);
int jump(const char* str, CommandType c_command);

#endif