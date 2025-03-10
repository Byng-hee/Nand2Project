//function.h
#ifndef function_H
#define function_H

// segment_A는 .vm파일에서 접근하는 메모리 할당공간
typedef struct {
    int RAM[16384];
    int SP;
    int LCL;
    int ARG;
    int THIS;
    int THAT;
    int STATIC;
    int TEMP;
    int POINTER;
    char *staticName;
}segment_A;

typedef struct allocStatic {
    int staticStart;
    int staticEnd;
}allocStatic;

typedef struct func {
    int func_argument;
}func;

char* make_outputfilename(char* argv);      // 작성할 파일의 파일명을 만든다. ex) "병희.vm" -> "병희.asm"으로 변환
void pop(char *arg1, char * arg2, segment_A* s1, allocStatic* sizeofstatic);     // stack에 arg1[arg2]위치의 값을 저장한다. sp++된다.
void push(char *arg1, char * arg2, segment_A* s1);    // stack에서 값을 빼서 arg1[arg2]위치에 저장한다.
void add(segment_A* s1);
void sub(segment_A* s1);
void neg(segment_A* s1);
void eq(segment_A* s1);
void gt(segment_A* s1);
void lt(segment_A* s1);
void and(segment_A* s1);
void or(segment_A* s1);
void not(segment_A* s1);
void func_call(segment_A* s1, int PC, int loCount, int localArraySize);
void func_return(segment_A* s1);
void func_IFGOTO(segment_A* s1);

#endif
