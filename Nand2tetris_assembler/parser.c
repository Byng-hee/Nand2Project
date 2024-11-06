//parser.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "code.h"

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
void binaryAddress(int n, char *address) {
    for (int i = 15; i >= 0; i--) {
        int bit = (n >> i) & 1;      // n의 i번째 비트를 추출
        address[15 - i] = bit + '0'; // int 값을 문자로 변환하여 저장
    }
    address[16] = '\0';  // 문자열 끝에 NULL 문자 추가
	printf("Binary address: %s\n", address);
}

void C_COMPUTING(char* line, char *address){
    char *dot1 = strrchr(line, '=');
    char *dot2 = strrchr(line, ';');
    char *dest_part, *comp_part, *jump_part;
    char compute_code[17] = {'1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    compute_code[16] = '\0';  // 문자열 끝에 NULL 문자 추가

    unsigned int binary_value;

    if( (dot1 == NULL) && (dot2 == NULL) ){
        size_t length = strlen(line);
        comp_part = (char *)malloc(length + 1);
        strcpy(comp_part, line);
        binary_value = comp(comp_part, C_COMMAND);
        
        for (int i = 0; i < 7; i++) {
            compute_code[3 + i] = ((binary_value >> (6 - i)) & 1) + '0';
        }
        
        free(comp_part);
    }
    else if( (dot1 != NULL) && (dot2 == NULL) ){
        size_t length = dot1 - line;
        dest_part = (char *)malloc(length + 1);
        strncpy(dest_part, line, length);
        dest_part[length] = '\0';

        comp_part = dot1 + 1;
        binary_value = comp(comp_part, C_COMMAND);
        
        for (int i = 0; i < 7; i++) {
            compute_code[3 + i] = ((binary_value >> (6 - i)) & 1) + '0';
        }
        
        binary_value = dest(dest_part, C_COMMAND);
        
        for (int i = 0; i < 3; i++) {
            compute_code[10 + i] = ((binary_value >> (2 - i)) & 1) + '0';
        }
        
        free(dest_part);
    }
    else if( (dot1 == NULL) && (dot2 != NULL) ){
        size_t length = dot2 - line;
        comp_part = (char *)malloc(length + 1);
        strncpy(comp_part, line, length);
        comp_part[length] = '\0';
        
        jump_part = dot2 + 1;
        binary_value = comp(comp_part, C_COMMAND);
        
        for (int i = 0; i < 7; i++) {
            compute_code[3 + i] = ((binary_value >> (6 - i)) & 1) + '0';
        }
        
        binary_value = jump(jump_part, C_COMMAND);
        
        for (int i = 0; i < 3; i++) {
            compute_code[13 + i] = ((binary_value >> (2 - i)) & 1) + '0';
        }
        
        free(comp_part);
    }
    else{
        size_t length1 = dot1 - line;
        dest_part = (char *)malloc(length1 + 1);
        strncpy(dest_part, line, length1);
        dest_part[length1] = '\0';

        size_t length2 = dot2 - dot1 - 1;
        comp_part = (char *)malloc(length2 + 1);
        strncpy(comp_part, dot1 + 1, length2);
        comp_part[length2] = '\0';

        jump_part = dot2 + 1;
        binary_value = comp(comp_part, C_COMMAND);
        
        for (int i = 0; i < 7; i++) {
            compute_code[3 + i] = ((binary_value >> (6 - i)) & 1) + '0';
        }
        
        binary_value = dest(dest_part, C_COMMAND);
        
        for (int i = 0; i < 3; i++) {
            compute_code[10 + i] = ((binary_value >> (2 - i)) & 1) + '0';
        }
        
        binary_value = jump(jump_part, C_COMMAND);
        
        for (int i = 0; i < 3; i++) {
            compute_code[13 + i] = ((binary_value >> (2 - i)) & 1) + '0';
        }
        
        free(dest_part);
        free(comp_part);
    }

    // 최종 결과를 address로 복사
    strcpy(address, compute_code);
}
