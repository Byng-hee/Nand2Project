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
}segment_A;

char* make_outputfilename(char* argv);      // 작성할 파일의 파일명을 만든다. ex) "병희.vm" -> "병희.asm"으로 변환
void pop(char *arg1, char * arg2, segment_A* s1, segment_B* s2);     // stack에 arg1[arg2]위치의 값을 저장한다. sp++된다.
void push(char *arg1, char * arg2, segment_A* s1, segment_B* s2);                 // stack에서 값을 빼서 arg1[arg2]위치에 저장한다.
void add(segment_A* s1, segment_B* s2);
void sub(segment_A* s1, segment_B* s2);
void neg(segment_A* s1, segment_B* s2);
void eq(segment_A* s1, segment_B* s2);
void gt(segment_A* s1, segment_B* s2);
void lt(segment_A* s1, segment_B* s2);
void and(segment_A* s1, segment_B* s2);
void or(segment_A* s1, segment_B* s2);
void not(segment_A* s1, segment_B* s2);

#endif
