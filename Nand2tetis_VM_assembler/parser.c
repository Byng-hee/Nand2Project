#include "parser.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

segment_B *funcList = NULL;

segment_B* createSegment_B(const char* name, segment_A* s1, int value){
    segment_B *newSeg_B = (segment_B*)(malloc(sizeof(segment_B)));
    if(!newSeg_B){
        perror("메모리할당 실패!");
        exit(1);
    }
    strcpy(newSeg_B->name, name);
    newSeg_B->sp = value;        // stack의 최상위 위치를 가리킨다.     
    s1->RAM[0] = newSeg_B->sp; // stack의 최상위 위치를 가리킨다.     SP
    s1->RAM[1] = value+10; // LOCAL세그먼트 기저 주소 가리킨다.      LCL
    s1->RAM[2] = value+20; // argument 세그먼트 기저 주소 가리킨다.  ARG
    s1->RAM[3] = value+30; // THIS세그먼트의 시작주소를 가리킨다.    THIS
    s1->RAM[4] = value+40; // THAT세그먼트의 시작주소를 가리킨다.    THAT
    newSeg_B->temp = &(s1->RAM[5]); // RAM의 [5] - [12] 까지 할당     
    newSeg_B->next = NULL;
    return newSeg_B;
}
void addSegment_B(segment_B* s2){
    s2->next = funcList;
    funcList = s2;
}
void free_segment(segment_B *funcList){
    segment_B* current = funcList;
    segment_B* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next;  // 다음 노드 저장
        free(current);  // 현재 노드 메모리 해제
        current = nextNode;  // 다음 노드로 이동
    }
}


void parser(FILE *open, FILE *write, segment_A *s1){
    int value = 10;

    char* argument[2];
    int i;
    int repeat = 0;

    if (open == NULL) {
        // 파일을 열 수 없을 경우
        printf("Error opening file\n");
        return;
    }

    char line[256];
    segment_B *s2 = createSegment_B("basic", s1, value);
    addSegment_B(s2);

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

            if( (c_word == C_FUNCTION)  && 0 ){
                segment_B *s2 = createSegment_B(token, s1, value);
                addSegment_B(s2);
                value = value + 50;
            }

            switch (c_word) {
                case C_PUSH:
                    while (token != NULL) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    push(argument[0], argument[1], s1, funcList);
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
                    pop(argument[0], argument[1], s1, funcList);
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
                    if ( strcmp(token, "add") == 0 ){
                        segment_B *current = funcList;
                        add(s1, current);
                        printf("ADD Result : %d\n", s1->RAM[(current->sp)-1]);
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
                    else if( strcmp(token, "sub") == 0 ){
                        segment_B *current = funcList;
                        sub(s1, current);
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
                    else if( strcmp(token, "neg") == 0 ){
                        segment_B *current = funcList;
                        neg(s1, current);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "D=!D\n");
                        fprintf(write, "M=D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    else if( strcmp(token, "and") == 0 ){
                        segment_B *current = funcList;
                        and(s1, current);
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
                    else if( strcmp(token, "or") == 0 ){
                        segment_B *current = funcList;
                        or(s1, current);
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
                    else if( strcmp(token, "not") == 0 ){
                        segment_B *current = funcList;
                        not(s1, current);
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M-1\n");
                        fprintf(write, "M=!M\n");
                    }
                    else{
                        printf("NOT RUN!!\n");
                        exit(1);
                    }
                    break;
                case C_IF :
                    if( strcmp(token, "eq") == 0 ){
                        segment_B *current = funcList;
                        eq(s1, current);
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
                        segment_B *current = funcList;
                        gt(s1, current);
                        eq(s1, current);
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
                        segment_B *current = funcList;
                        lt(s1, current);
                        eq(s1, current);
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
                default : 
                    break;
            }
    }
    free_segment(funcList);
    
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
