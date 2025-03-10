#include "parser.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

const char *collection[] = {"LCL", "ARG", "THIS", "THAT"};

typedef struct labelOrFucntionName {
    char name[50];
    int localArraySize;
    int count;
    struct labelOrFucntionName* next;
}labelOrFucntionName;

labelOrFucntionName* head = NULL;
labelOrFucntionName* tail = NULL;

labelOrFucntionName* createLabelOrFunctionName(const char* name, int localArraySize) {
    labelOrFucntionName* new = (labelOrFucntionName *)malloc(sizeof(labelOrFucntionName));
    strcpy(new->name, name);
    new->count = 0;
    new->localArraySize = localArraySize;
    new->next = NULL;
    return new;
}

void addLabelOrFunction(labelOrFucntionName** node) {
    if(head == NULL){
        head = *node;
        tail = *node;
    }
    else {
        tail->next = *node;
        tail = *node;
        tail->next = NULL;
    }
}

labelOrFucntionName* findLabelOrFunctionName(const char* name) {
    labelOrFucntionName* current = head;
    while(!current) {
        if( strcmp(name, current->name) == 0 ) {
            return current;
        }
        else{
            current = current->next;
        }
    }
    return NULL;
}

char* translate(const char* name) {
    if(strcmp(name, "local") == 0){
        return "LCL";
    }
    else if(strcmp(name, "argument") == 0) {
        return "ARG";
    }
    else if(strcmp(name, "this") == 0) {
        return "THIS";
    }
    else if(strcmp(name, "that") == 0) {
        return "THAT";
    }
    else if(strcmp(name, "pointer") == 0) {
        return "POINTER";
    }
    else if(strcmp(name, "temp") == 0) {
        return "TEMP";
    }
    else{
        return NULL;
    }

}


void parser(FILE *open, FILE *write, segment_A* s1){
    allocStatic *sizeofstatic = (allocStatic*)(malloc(sizeof(allocStatic)));
    sizeofstatic->staticStart = s1->STATIC;
    sizeofstatic->staticStart = s1->STATIC;

    char* argument[2];
    int i;  //char *argument의 순서를 나타내기 위함!
    int repeat = 0;
    int PC = 0;
    if (open == NULL) {
        // 파일을 열 수 없을 경우
        printf("Error opening file\n");
        return;
    }

    char *line = (char *)malloc(500);

    while (fgets(line, 500, open) != NULL) {

            commandtype c_word;
            i = 0;

            printf("%d : %s", PC, line);

            if(strspn(line, " \t\r\n") > 0) {
                int len = strspn(line, " \t\r\n");  // 처음의 공백 문자 길이
                memmove(line, line + len, strlen(line) - len + 1);
            }
            
            // 줄 끝에 있는 개행 문자 제거
            line[strcspn(line, "\n")] = '\0';

            if( ( line[0] == '/' ) || ( strspn(line, " \t\r\n") == strlen(line) ) ) {
                PC++;
                continue;
            }

            // 문자열 분할
            char *token = strtok(line, " ");  // 첫 번째 토큰 얻기

            c_word = find_commandtype(token);
            
            switch (c_word) {
                case C_PUSH:
                    for(int j =0; j<2; j++) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    push(argument[0], argument[1], s1);

                    if( strcmp(argument[0], "constant") == 0 ) {
                        fprintf(write, "@%s\n", argument[1]);
                        fprintf(write, "D=A\n");
                    }
                    else{
                        fprintf(write, "@%s\n", argument[1]);
                        fprintf(write, "D=A\n");
                        if(!translate(argument[0])){    //push static n인 경우
                            fprintf(write, "@%s.%d\n", s1->staticName, argument[1]);
                        }
                        else{
                            fprintf(write, "@%s\n", translate(argument[0]));
                        }
                        fprintf(write, "A=M+D\n");
                        fprintf(write, "D=M\n");
                    }
                    fprintf(write, "@SP\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "M=D\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M+1\n");
                    break;
                case C_POP :
                    for(int j =0; j<2; j++) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    pop(argument[0], argument[1], s1, sizeofstatic);
                    fprintf(write, "@%s\n", argument[1]);
                    fprintf(write, "D=A\n");
                    fprintf(write, "@%s\n", translate(argument[0]));
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
                        add(s1);
                        printf("ADD Result : %d\n", s1->RAM[(s1->SP)-1]);
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
                        sub(s1);
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
                        neg(s1);
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
                        and(s1);
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
                        or(s1);
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
                        not(s1);
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
                        eq(s1);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=D-M\n");
                        fprintf(write, "@EQ_TRUE%d\n", repeat);
                        fprintf(write, "D;JEQ\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=0\n");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d\n", repeat);
                        fprintf(write, "0;JMP\n");
                        fprintf(write, "(EQ_TRUE%d\n)", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=-1\n"); // TRUE면 -1저장한다(모든 비트가 1인값)
                        fprintf(write, "(CONTINUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                        repeat++;
                        printf("I'm GOING LT\n");
                        
                    }
                    else if( strcmp(token, "gt") == 0 ){
                        gt(s1);
                        eq(s1);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M-D\n");
                        fprintf(write, "@GT_TRUE%d\n", repeat);
                        fprintf(write, "D;JGT\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=0\n");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d\n", repeat);
                        fprintf(write, "0;JMP\n");
                        fprintf(write, "(GT_TRUE%d\n)", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=-1\n"); // TRUE면 -1저장한다(모든 비트가 1인값)
                        fprintf(write, "(CONTINUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                        repeat++;
                        printf("I'm GOING LT\n");
                        
                    }
                    else if( strcmp(token, "lt") == 0 ){
                        lt(s1);
                        eq(s1);
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M-1\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "D=M-D\n");
                        fprintf(write, "@LT_TRUE%d\n", repeat);
                        fprintf(write, "D;JLT\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=0\n");  // FALSE를 stack의 최상단에 저장! FALSE == 0
                        fprintf(write, "@CONTINUE%d\n", repeat);
                        fprintf(write, "0;JMP\n");
                        fprintf(write, "(LT_TRUE%d)\n", repeat);
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=-1\n"); // TRUE면 -1저장한다(모든 비트가 1인값)
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
                    fprintf(write, "@%s\n", token);
                    fprintf(write, "0;JMP\n");
                    break;
                case C_LABEL :
                    fprintf(write, "(%s)\n", token);
                    break;
                case C_IFGOTO :
                    token = strtok(NULL, " ");

                    func_IFGOTO(s1);

                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M-1\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "D=M\n");
                    fprintf(write, "@%s\n", token);
                    fprintf(write, "D;JLT\n");
                    break;
                case C_CALL :
                    i = 0;
                    for(int j =0; j<2; j++) {
                        token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                        argument[i++] = token;
                    }
                    labelOrFucntionName* current = findLabelOrFunctionName(argument[0]);
                    if(current == NULL){
                        perror("It's wrong name!");
                        exit(1);
                    }
                    current->count++;

                    func_call(s1, PC, atoi(argument[1]), current->localArraySize);

                    fprintf(write, "%s@ret%d\n", argument[0],argument[1] ); // return address를 위한 작업
                    fprintf(write, "D=A\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "M=D\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M+1\n");
                    for(int j=0; j<4; j++){
                        fprintf(write, "@%s\n", collection[j]); // 호출프로시저의 LCL, ARG, THIS, THAT의 반환주소 저장!
                        fprintf(write, "D=A\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "A=M\n");
                        fprintf(write, "M=D\n");
                        fprintf(write, "@SP\n");
                        fprintf(write, "M=M+1\n");
                    }
                    fprintf(write, "@SP\n");    // 새로운 FRAME의 시작인 LOCAL의 기저 주소를 새롭게 R1에 저장
                    fprintf(write, "D=M\n");
                    fprintf(write, "@LCL\n");
                    fprintf(write, "M=D\n");

                    fprintf(write, "@SP\n");    // 새로운 FRAME의 ARG의 기저 주소를 새롭게 저장
                    fprintf(write, "D=M\n");
                    fprintf(write, "@%d\n", 5+atoi(argument[1]) );  // SP - (5 + N)이 ARG의 기저 주소 N은 매개변수 갯수!
                    fprintf(write, "D=D-A\n");  
                    fprintf(write, "@ARG\n");
                    fprintf(write, "M=D\n");
                    break;
                case C_FUNCTION :
                    token = strtok(NULL, " ");  // 공백 기준으로 다음 토큰 얻기
                    fprintf(write, "(%s)\n", token);
                    break;
                case C_RETURN :

                    func_return(s1);
            

                    fprintf(write, "@LCL\n");
                    fprintf(write, "D=M\n");
                    fprintf(write, "@R13\n");
                    fprintf(write, "M=D\n");          // FRAME = LCL
                    
                    fprintf(write, "@5\n");
                    fprintf(write, "A=D-A\n");
                    fprintf(write, "D=M\n");
                    fprintf(write, "@R14\n");
                    fprintf(write, "M=D\n");          // return address = *(FRAME-5)
                    
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=M-1\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "D=M\n");
                    fprintf(write, "@ARG\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "M=D\n");          // *ARG = pop()
                    
                    fprintf(write, "@ARG\n");
                    fprintf(write, "D=M+1\n");
                    fprintf(write, "@SP\n");
                    fprintf(write, "M=D\n");          // SP = ARG + 1
                    
                    for(int j = 4; j>0; j--){
                        fprintf(write, "@R13\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@%d\n", j);
                        fprintf(write, "A=D-A\n");
                        fprintf(write, "D=M\n");
                        fprintf(write, "@%s\n", collection[j-1]); // THAT = *(FRAME-1) THIS = *(FRAME-2) ... LCL = *(FRAME-4)
                        fprintf(write, "M=D\n");
                    }
                    
                    fprintf(write, "@R14\n");
                    fprintf(write, "A=M\n");
                    fprintf(write, "0;JMP\n");      // goto return address
                    break;
                default : 
                    break;
            }
            PC++;
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
    else if (strcmp(token, "if-goto") == 0) {
        c_word = C_IFGOTO;
    }
    else if (strcmp(token, "function") == 0 || strcmp(token, "label") == 0){
        c_word = C_FUNCTION;
    }
    else {
        printf("Unknown command: %s\n", token);
        c_word = -1;
    }
    return c_word;
}
