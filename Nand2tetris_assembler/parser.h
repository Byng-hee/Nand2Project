//parser.h
#ifndef parser_H
#define parser_H

typedef struct{
    char *address;
    int I;
}Reg;

typedef enum { A_COMMAND, C_COMMAND, L_COMMAND, IGNORE } CommandType;

CommandType get_command_type(const char* str);
char* symbol(char* str, CommandType AorL);
void C_COMPUTING(char* line, char* address);
void binaryAddress(int , char* address);

typedef struct{
    char symbol_name[256];
    int symbol_address;
}SymbolTable;

//bool has_symbol_contains();
//int get_symbol_address();
#endif
