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
		symbol_name[end] = '\0';
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
void binaryAddress(int n, int *address) {
    for (int i = 16; i >= 0; i--) {
        int bit = (n >> i) & 1;  // n의 i번째 비트를 추출
        address[16-i] = bit;
    }
}

const int* C_COMPUTING(char* line){
	char *dot1 = strrchr(line, '=');
	char *dot2 = strrchr(line, ';');
	char *dest_part, *comp_part, *jump_part;
	int compute_code[16] = {1,1,1,0};
	unsigned int binary_value;

	if( (dot1 == NULL) and (dot2 == NULL) ){

		size_t length = strlen(line);
		comp_part = (char *)malloc(length);
		comp_part = line;
		binary_value = comp(comp_part, C_COMMAND);
		for (int i = 0; i < 7; i++) {
        	compute_code[3 + i] = (binary_value >> (6 - i)) & 1;
    	}
		return compute_code;

	}
	else if( (dot1 != NULL) and (dot2 == NULL) ){

		size_t length = dot1 - line;
		dest_part = (char *)malloc(length);
		comp_part = (char *)malloc(strlen(line) - length);
		strncpy(dest_part, line, length);
		comp_part = dot1 + 1;

		binary_value = comp(comp_part, C_COMMAND);
		for (int i = 0; i < 7; i++) {
        	compute_code[3 + i] = (binary_value >> (6 - i)) & 1;
    	}
		binary_value = dest(dest_part, C_COMMAND);
		for (int i = 0; i < 3; i++) {
        	compute_code[10 + i] = (binary_value >> (2 - i)) & 1;
    	}

		return compute_code;

	}
	else if( (dot1 == NULL) and (dot2 != NULL) ){

		size_t length = dot2 - line;
		comp_part = (char *)malloc(length);
		jump_part = (char *)malloc(strlen(line)- length);
		strncpy(comp_part, line, length);
		jump_part = dot2 + 1;

		binary_value = comp(comp_part, C_COMMAND);
		for (int i = 0; i < 7; i++) {
        	compute_code[3 + i] = (binary_value >> (6 - i)) & 1;
    	}
		binary_value = jump(jump_part, C_COMMAND);
		for (int i = 0; i < 3; i++) {
        	compute_code[13 + i] = (binary_value >> (2 - i)) & 1;
    	}
		return compute_code;

	}
	else{

		size_t length1 = dot1 - line;
		size_t length2 = dot2 - line;
		dest_part = (char *)malloc(length1);
		comp_part = (char *)malloc(dot2-dot1-1);
		jump_part = (char *)malloc(strlen(line) - length2);
		strncpy(dest_part, line, length1);
		strncpy(comp_part, dot1+1, dot2-dot1-1);
		jump_part = dot2 + 1;
		
		binary_value = comp(comp_part, C_COMMAND);
		for (int i = 0; i < 7; i++) {
        	compute_code[3 + i] = (binary_value >> (6 - i)) & 1;
    	}
		binary_value = dest(dest_part, C_COMMAND);
		for (int i = 0; i < 3; i++) {
        	compute_code[10 + i] = (binary_value >> (2 - i)) & 1;
    	}
		binary_value = jump(jump_part, C_COMMAND);
		for (int i = 0; i < 3; i++) {
        	compute_code[13 + i] = (binary_value >> (2 - i)) & 1;
    	}

		return compute_code;

	}
}
