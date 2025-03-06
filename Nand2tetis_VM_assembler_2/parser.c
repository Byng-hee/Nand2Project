#include "parser.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parser(FILE *open, FILE *write, segment_A* s1){

    char* argument[2];
    int i;  //char *argument의 순서를 나타내기 위함!
    int repeat = 0;

    if (open == NULL) {
        // 파일을 열 수 없을 경우
        printf("Error opening file\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), open) != NULL) {
            commandtype c_word;
            i = 0;

            if( (line[0] == '/' ) || ( strspn(line, " \t\r\n") == strlen(line) ) ){
                continue;
            }
            // 줄 끝에 있는 개행 문자 제거
            line[strcspn(line, "\n")] = '\0';
 
            // 문자열 분할
            char *token = strtok(line, " ");  // 첫 번째 토큰 얻기
            c_word = find_commandtype(token);

            if( c_word == C_FUNCTION ){
                token = strtok(NULL, " ");
                segment_B *s2 = createSegment_B(token, s1, value);
                addSegment_B(&s2);
                current_seg = s2;
                value = value + 50;
            }
            else{
                segment_B *s2 = createSegment_B("basic", s1, value);
                addSegment_B(&s2);
                current_seg = s2;
                value = value + 50;
            }

            switch (c_word) {
                case C_PUSH:
                    while (token != NULL) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    push(argument[0], argument[1], s1, current_seg);
                    fprintf(write, "@%d\n", atoi(argument[1]));
                    fprintf(write, "D=A\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "M=D\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M+1\n");
                    break;
                case C_POP :
                    while (token != NULL) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    pop(argument[0], argument[1], s1, current_seg);
                    fprintf(write, "@%d\n", atoi(argument[1]));
                    fprintf(write, "D=A\n");
                    fprintf(write, "@ARG\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "D=D+A\n");
                    fprintf(write, "@13\n");
                    fprintf(write, "M=D\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M-1\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "D=M\n");
                    fprintf(write, "@13\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "M=D\n");
                    break;
                case C_ARITHMEIC :
                    if ( strcmp(token, "add") == 0 ) {
                        add(s1, current_seg);
                        printf("ADD Result : %d\n", s1->RAM[(current_seg->sp)-1]);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=D+M\n");
                        fprintf(write, "M=D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "sub") == 0 ) {
                        sub(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=D-M\n");
                        fprintf(write, "M=D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "neg") == 0 ) {
                        neg(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "D=!D\n");
                        fprintf(write, "M=D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "and") == 0 ) {
                        and(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=M&D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "or") == 0 ) {
                        or(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=M|D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "not") == 0 ) {
                        not(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M-1\n");
                        fprintf(write, "M=!M\n");
                    }
                    else {
                        printf("NOT RUN!!\n");
                        exit(1);
                    }
                    break;
                case C_IF :
                    if( strcmp(token, "eq") == 0 ) {
                        eq(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=D-M\n");
                        fprintf(write, "@EQ_TRUE%d", repeat);
                        fprintf(write, "D;JEQ");
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=0");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d", repeat);
                        fprintf(write, "0;JMP");
                        fprintf(write, "(EQ_TRUE%d)", repeat);
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=-1"); // TRUE면 -1저장한다(모든 비트가 1인값)
                        fprintf(write, "(CONTINUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                        repeat++;
                        printf("I'm GOING LT\n");
                        
                    }
                    else if( strcmp(token, "gt") == 0 ){
                        gt(s1, current_seg);
                        eq(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M-D\n");
                        fprintf(write, "@GT_TRUE%d", repeat);
                        fprintf(write, "D;JGT");
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=0");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d", repeat);
                        fprintf(write, "0;JMP");
                        fprintf(write, "(GT_TRUE%d)", repeat);
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=-1"); // TRUE면 -1저장한다(모든 비트가 1인값)
                        fprintf(write, "(CONTINUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                        repeat++;
                        printf("I'm GOING LT\n");
                        
                    }
                    else if( strcmp(token, "lt") == 0 ){
                        lt(s1, current_seg);
                        eq(s1, current_seg);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M-D\n");
                        fprintf(write, "@LT_TRUE%d", repeat);
                        fprintf(write, "D;JLT");
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=0");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d", repeat);
                        fprintf(write, "0;JMP");
                        fprintf(write, "(LT_TRUE%d)", repeat);
                        fprintf(write, "@SP");
                        fprintf(write, "A=M");
                        fprintf(write, "M=-1"); // TRUE면 -1저장한다(모든 비트가 1인값)
                        fprintf(write, "(CONTINUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                        repeat++;
                        printf("I'm GOING LT\n");
                    }
                    else{
                        printf("NOT RUN!!\n");
                        exit(1);
                    }
                    break;
                case C_GOTO :
                    token = strtok(NULL, " ");
                    fprintf(write, "@%s", token);
                    fprintf(write, "0;JMP");
                    break;
                case C_LABEL :
                    fprintf(write, "(%s)", token);
                    break;
                case C_IFGOTO :
                    token = strtok(NULL, " ");
                    fprintf(write, "@SP");
                    fprintf(write, "M=M-1");
                    fprintf(write, "A=M");
                    fprintf(write, "D=M");
                    fprintf(write, "@%s", token);
                    fprintf(write, "D;JMP");
                    break;
                case C_CALL :
                    //시스템 내부적으로 컴파일만 가능함.
                    //실제 이 컴파일러 내부에서는 CALL 처리를 구현
                    //하지 않았음. 추후에 개발 할려면 해야됨!
                    //file을 읽고 CALL 호출시 LABEL이 최초로 
                    //선언된 라인으로 이동 할 수 있도록 라인들도
                    //따로 기록하는 데이터 구조 필요
                    token = strtok(NULL, " ");
                    if(!findfunc(token)){
                        current_seg = findfunc(token);
                    }
                    else {
                        printf("NO FUNCTION NAME!!");
                    }
                    break;
                case C_RETURN :
                    //return 시에 새롭게 매로리 할당하는 것을 구현할 필요가 있음

                default : 
                    break;
            }
    }
    free_segment(funcList_head);
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
    else if (strcmp(token, "if-goto") == 0) {
        c_word = C_IFGOTO;
    }
    else if (strcmp(token, "function") == 0){
        c_word = C_FUNCTION;
    }
    else {
        printf("Unknown command: %s\n", token);
        c_word = -1;
    }
    return c_word;
}
