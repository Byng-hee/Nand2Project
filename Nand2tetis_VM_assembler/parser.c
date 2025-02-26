#include "parser.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parser(FILE *open, FILE *write, segment *s1){

    if (open == NULL) {
        // 파일을 열 수 없을 경우
        printf("Error opening file\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), open) != NULL) {
        commandtype c_word;

        if( (line[0] == '/' ) || (line[0] == ' ') ){
            continue;
        }
        // 줄 끝에 있는 개행 문자 제거
        line[strcspn(line, "\n")] = '\0';

        // 문자열 분할
        char *token = strtok(line, " ");  // 첫 번째 토큰 얻기
        c_word = find_commandtype(token);
        
        char* argument[2];
        int i = 0;

        if( c_word == C_FUNCTION ){
            token = strtok(NULL, " ");
            switch (c_word) {
                case C_PUSH:
                    while (token != NULL) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    push(argument[0], argument[1], *s1);
                    break;
                default : 
                    break;
            }

        }

    }
    
}

commandtype find_commandtype(char *token)
{
    commandtype c_word;
    if (strcmp(token, "push") == 0) {
        c_word = C_PUSH;
    }
    else if (strcmp(token, "pop") == 0) {
        c_word = C_POP;
    }
    else if (strcmp(token, "goto") == 0) {
        c_word = C_GOTO;
    }
    else if (strcmp(token, "return") == 0) {
        c_word = C_RETURN;
    }
    else if (strcmp(token, "call") == 0) {
        c_word = C_CALL;
    }
    else if (strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 || strcmp(token, "neg") == 0) {
        c_word = C_ARITHMEIC;
    }
    else if (strcmp(token, "eq") == 0 || strcmp(token, "gt") == 0 || strcmp(token, "lt") == 0) {
        c_word = C_IF;
    }
    else if (strcmp(token, "and") == 0 || strcmp(token, "or") == 0 || strcmp(token, "not") == 0) {
        c_word = C_ARITHMEIC;
    }
    else {
        printf("Unknown command: %s\n", token);
        c_word = -1;
    }
    return c_word;
}
