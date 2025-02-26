#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* make_outputfilename(char* argv){
    char *dot = strrchr(argv, '.');
    char *outputFileName;

    if (dot != NULL) {
        // '.'이 있는 경우, 확장자 제외한 파일 이름의 길이 계산
        size_t length = dot - argv;
        // 동적으로 메모리 할당
        outputFileName = (char *)malloc(length + strlen(".asm") + 1); // +1은 널 종료 문자
        if (outputFileName == NULL) {
            perror("Memory Alloc Failure");
            exit(1);
        }
        strncpy(outputFileName, argv, length); // 원본 파일 이름 복사
        outputFileName[length] = '\0'; // 문자열 종료
    }
    strcat(outputFileName, ".asm");
    return outputFileName;
}


void push(char *arg1, char * arg2, segment* s1){
    segment_B s2 = (segment_B *)(malloc(sizeof(segment_B));
    s2.sp = 100;
    s1->RAM[0] = s2.sp; // stack의 최상위 위치를 가리킨다
    s1->RAM[1] = 200; // LOCAL세그먼트 기저 주소 가리킨다
    s1->RAM[2] = 300; // argument 세그먼트 기저 주소 가리킨다
    s1->RAM[3] = 400; // THIS세그먼트의 시작주소를 가리킨다
    s1->RAM[4] = 500; // THAT세그먼트의 시작주소를 가리킨다
    s2.temp = &RAM[5]; // 5 - 12 까지 할당
    int v = atoi(arg2);
    if ( (strcmp(arg1, "constant") == 0) ){
        s1->RAM[s2.sp++] = v;   
    }
    else if(strcmp(arg1, "local") == 0){
        s1->RAM[s2.sp++] = s2.local[v];
    }
    else if(strcmp(arg1, "argumnet") == 0){
        s1->RAM[s2.sp++] = s2.argumnet[v];
    }
    else if(strcmp(arg1, "that") == 0){
        s1->RAM[s2.sp++] = s2.that[v];
    }
    else if(strcmp(arg1, "this") == 0){
        s1->RAM[s2.sp++] = s2.this[v];
    }
    else if(strcmp(arg1, "temp") == 0){
        s1->RAM[s2.sp++] = s2.temp[v];
    }
    else if(strcmp(arg1, "static") == 0){
        s1->RAM[s2.sp++] = s1.static[v];
    }
    else{
        fprintf(stderr, "잘못된 인자값입니다.");
    }
    s1->RAM[0] = s2.sp;
    free(s2);
}
void pop(char *arg1, char * arg2, segment* s1){
    int v = atoi(arg2);
    if ( (strcmp(arg1, "constant") == 0) ){
        s1->RAM[s1->sp++] = v;
    }

}
void add(){

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