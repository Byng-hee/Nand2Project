//parser.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "code.h"

CommandType get_command_type(const char* str) {
    // 1. 빈 줄 처리: 줄의 첫 문자가 '\0' (빈 줄)일 경우
    if (str[0] == '\n' || str[0] == '\0') {
        return IGNORE;
    }

    // 2. 주석 처리: 문자열에서 '//'가 첫 번째 문자로 나타나면 주석
    if (strchr(str,'/') != NULL) {
        return IGNORE;
    }

    // 3. A_COMMAND: '@'로 시작
    if (strchr(str,'@')) {
        return A_COMMAND;
    }

    // 4. L_COMMAND: '('로 시작
    if (strchr(str,'(')) {
        return L_COMMAND;
    }

    // 5. C_COMMAND: 나머지 모든 경우
    return C_COMMAND;
}

char* symbol(char* str, CommandType AorL){
    // 문자열 앞의 공백을 제거
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }

    // 문자열 뒤의 공백을 제거
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // 끝을 null 문자로 설정하여 문자열 크기를 조정
    *(end + 1) = '\0';


	static char symbol_name[30];
	size_t end_str = strlen(str);
	if((AorL==A_COMMAND) && (str != NULL)){
		for(size_t i=1; i<end_str; i++){
			symbol_name[i-1] = str[i];
		}
		symbol_name[end_str-1] = '\0';
		return symbol_name;
	}
	else if((AorL==L_COMMAND) && (str != NULL)){
		for(size_t i=1; i<end_str-1; i++){
			symbol_name[i-1] = str[i];
		}
		symbol_name[end_str-2] = '\0';
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
	//printf("Binary address: %s\n", address);
}

void C_COMPUTING(char* line, char *address){
    // 문자열 앞의 공백을 제거
    while (*line && isspace((unsigned char)*line)) {
        line++;
    }

    // 문자열 뒤의 공백을 제거
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        end--;
    }

    // 끝을 null 문자로 설정하여 문자열 크기를 조정
    *(end + 1) = '\0';

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
