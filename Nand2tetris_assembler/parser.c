//parser.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "code.c"

CommandType get_command_type(const char* str) {

    // A_COMMAND는 '@'로 시작
    if (str[0] == '@') {
        return A_COMMAND;
    }
    // L_COMMAND는 '('로 시작
    else if (str[0] == '(') {
        return L_COMMAND;
    }
    // 나머지는 C_COMMAND로 간주
    else {
        return C_COMMAND;
    }
}

char* symbol(char* str, CommandType AorL){
	static char symbol_name[30];
	size_t end = strlen(str);
	if(AorL==A_COMMAND){
		for(size_t i=1; i<end; i++){
			symbol_name[i-1] = str[i];
		}
		symbol_name[end-1] = '\0';
		return symbol_name;
	}
	else if(AorL==L_COMMAND){
		for(size_t i=1; i<end-1; i++){
			symbol_name[i-1] = str[i];
		}
		symbol_name[end-1] = '\0';
		return symbol_name;
	}
	else{
		return NULL;
	}
}

const char* C_COMPUTING(char* line, CommandType ALC){

	if(ALC == A_COMMAND){
		
	}
}
