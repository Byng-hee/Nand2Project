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
    strcat(outputFileName, ".asm");
    return outputFileName;
}


void push(char *arg1, char * arg2, segment_A* s1, segment_B* s2){
    int v = atoi(arg2);
    if ( (strcmp(arg1, "constant") == 0) ){
        s1->RAM[s2->sp++] = v;
    }
    else if(strcmp(arg1, "local") == 0){
        s1->RAM[s2->sp++] = s2->local[v];
    }
    else if(strcmp(arg1, "argumnet") == 0){
        s1->RAM[s2->sp++] = s2->argumnet[v];
    }
    else if(strcmp(arg1, "that") == 0){
        s1->RAM[s2->sp++] = s2->that[v];
    }
    else if(strcmp(arg1, "this") == 0){
        s1->RAM[s2->sp++] = s2->this[v];
    }
    else if(strcmp(arg1, "temp") == 0){
        s1->RAM[s2->sp++] = s2->temp[v];
    }
    else if(strcmp(arg1, "static") == 0){
        s1->RAM[s2->sp++] = s1->static_memory[v];
    }
    else{
        fprintf(stderr, "잘못된 인자값입니다.");
    }
    s1->RAM[0] = s2->sp;
}

void pop(char *arg1, char * arg2, segment_A* s1, segment_B* s2){
 /*   int v = atoi(arg2);
    if ( (strcmp(arg1, "constant") == 0) ){
        s1->RAM[s1->sp++] = v;
    }
*/
}
void add(segment_A* s1, segment_B* s2){
    int temp = s1->RAM[(s2->sp--)-1];
    int sum = s1->RAM[(s2->sp--)-1];
    sum = sum + temp;
    s1->RAM[s2->sp++] = sum;
}
void sub(){

}
void neg(){

}
void eq(){

}
void gt(){

}
void lt(){

}
void and(){

}
void or(){

}
void not(){

}