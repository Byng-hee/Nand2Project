//parser.h
#ifndef parser_H
#define parser_H

typedef struct{
    int Memory[43256];
    char *address;
    int M; 
    int I, D;
}Reg;

typedef enum { A_COMMAND, C_COMMAND, L_COMMAND } CommandType;

CommandType get_command_type(const char* str);
char* symbol(char* str, CommandType AorL);
void C_COMPUTING(char* line, char* address);
void binaryAddress(int , char* address);

#endif
