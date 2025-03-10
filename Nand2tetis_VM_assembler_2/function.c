#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* make_outputfilename(char* argv){
    char *dot = strrchr(argv, '.');
    char *outputFileName = (char *)malloc(100);

    if (dot != NULL) {
        // '.'이 있는 경우, 확장자 제외한 파일 이름의 길이 계산 
        size_t length = dot - argv;
        char *dot1 = strrchr(argv, '\\');
        size_t length2 = dot1 - argv + 1;
        length = length - length2;
        // 동적으로 메모리 할당
        outputFileName = (char *)malloc(length + strlen(".asm") + 1); // +1은 널 종료 문자
        if (outputFileName == NULL) {
            perror("Memory Alloc Failure");
            printf("error! \n");
            exit(1);
        }

        strncpy(outputFileName, argv+length2, length); // 원본 파일 이름 복사
        outputFileName[length] = '\0'; // 문자열 종료
    }
    return outputFileName;
}


void push(char *arg1, char *arg2, segment_A* s1){
    int v = atoi(arg2);
    if ( (strcmp(arg1, "constant") == 0) ){
        s1->RAM[s1->SP++] = v;
    }
    else if(strcmp(arg1, "local") == 0){
        s1->RAM[s1->SP++] = s1->RAM[s1->LCL + v];
    }
    else if(strcmp(arg1, "argument") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->ARG + v];
    }
    else if(strcmp(arg1, "that") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->THAT + v];
    }
    else if(strcmp(arg1, "this") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->THIS + v];
    }
    else if(strcmp(arg1, "pointer") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->POINTER + v];
    }
    else if(strcmp(arg1, "temp") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->TEMP + v];
    }
    else if(strcmp(arg1, "static") == 0) {
        s1->RAM[s1->SP++] = s1->RAM[s1->STATIC + v];
    }
    else{
        perror("HOLLY SHIT_PUSH_ERROR!! \n");
        exit(1);
    }
    s1->RAM[0] = s1->SP;
}
void pop(char *arg1, char * arg2, segment_A* s1, allocStatic* sizeofstatic){
    int v = atoi(arg2);
    if(strcmp(arg1, "local") == 0){
        s1->SP--;
        s1->RAM[s1->LCL + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "argument") == 0) {
        s1->SP--;
        s1->RAM[s1->ARG + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "that") == 0) {
        s1->SP--;
        s1->RAM[s1->THAT + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "this") == 0) {
        s1->SP--;
        s1->RAM[s1->THIS + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "pointer") == 0) {
        s1->SP--;
        s1->RAM[s1->POINTER + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "temp") == 0) {
        s1->SP--;
        s1->RAM[s1->TEMP + v] = s1->RAM[s1->SP];
    }
    else if(strcmp(arg1, "static") == 0) {
        s1->SP--;
        s1->RAM[s1->STATIC + v] = s1->RAM[s1->SP];
        sizeofstatic->staticEnd++;
    }
    else{
        perror("HOLLY SHIT_POP_ERROR!! \n");
        exit(1);
    }
    s1->RAM[0] = s1->SP;
}
void add(segment_A* s1){
    int temp = s1->RAM[(s1->SP--)-1];
    int sum = s1->RAM[(s1->SP--)-1];
    sum = sum + temp;
    s1->RAM[s1->SP++] = sum;
}
void sub(segment_A* s1){
    int temp = s1->RAM[(s1->SP--)-1];
    int sub = s1->RAM[(s1->SP--)-1];
    sub = sub - temp;
    s1->RAM[s1->SP++] = sub;
}
void neg(segment_A* s1){
    int temp = s1->RAM[(s1->SP)-1];
    temp = ~temp + 1;
    s1->RAM[(s1->SP)-1] = temp;
}
void eq(segment_A* s1){
    int temp1 = s1->RAM[(s1->SP--)-1];
    int temp2 = s1->RAM[(s1->SP--)-1];
    if(temp1 == temp2){
        s1->RAM[s1->SP++] = 1;
    }
    else{
        s1->RAM[s1->SP++] = 0;
    }
}
void gt(segment_A* s1){
    int temp1 = s1->RAM[(s1->SP--)-1];
    int temp2 = s1->RAM[(s1->SP--)-1];
    if(temp1 >= temp2){
        s1->RAM[s1->SP++] = 1;
    }
    else{
        s1->RAM[s1->SP++] = 0;
    }
}
void lt(segment_A* s1){
    int temp1 = s1->RAM[(s1->SP--)-1];
    int temp2 = s1->RAM[(s1->SP--)-1];
    if(temp1 <= temp2){
        s1->RAM[s1->SP++] = 1;
    }
    else{
        s1->RAM[s1->SP++] = 0;
    }
}
void and(segment_A* s1){
    int temp = s1->RAM[(s1->SP--)-1];
    int sum = s1->RAM[(s1->SP--)-1];
    sum = sum & temp;
    s1->RAM[s1->SP++] = sum;
}
void or(segment_A* s1){
    int temp = s1->RAM[(s1->SP--)-1];
    int sum = s1->RAM[(s1->SP--)-1];
    sum = sum | temp;
    s1->RAM[s1->SP++] = sum;
}
void not(segment_A* s1){
    int temp = s1->RAM[(s1->SP--)-1];
    temp =  ~temp;
    s1->RAM[s1->SP++] = temp;
}
void func_call(segment_A *s1, int PC, int localCount, int localArraySize) {
    s1->RAM[s1->SP++] = PC; // return address를 표현하기 위함!
    s1->RAM[s1->SP++] = s1->LCL;
    s1->RAM[s1->SP++] = s1->ARG;
    s1->RAM[s1->SP++] = s1->THIS;
    s1->RAM[s1->SP++] = s1->THAT;
    s1->LCL = s1->SP;
    s1->ARG = s1->LCL - (5 + localCount);
    s1->SP = s1->LCL + localArraySize; // X는 호출된 function이 가지고 있는 local변수의 갯수.
    s1->THIS = 0;
    s1->THAT = 0;
}
void func_return(segment_A *s1) {
    s1->RAM[13] = s1->LCL;  // FRAME 저장
    s1->RAM[14] = s1->RAM[s1->LCL - 5]; // return address 저장
    s1->RAM[s1->ARG] = s1->RAM[s1->SP - 1]; // return 값을 RMA[ARG]위치에 저장함
    s1->SP = s1->ARG + 1;                   // SP의 위치는 return값 바로 위
    s1->THAT = s1->RAM[ s1->RAM[13] - 1 ];  // 호출한 함수의 THAT 반환!
    s1->THIS = s1->RAM[ s1->RAM[13] - 2 ];
    s1->ARG = s1->RAM[ s1->RAM[13] - 3 ];
    s1->LCL = s1->RAM[ s1->RAM[13] - 4 ];
}

void func_IFGOTO(segment_A* s1) {
    s1->SP -= 1;
}