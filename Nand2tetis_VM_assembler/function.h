//function.h
#ifndef function_H
#define function_H

// segment_A는 .vm파일에서 접근하는 메모리 할당공간
typedef struct {
    int static_memory[100];
    int RAM[16384];
}segment_A;

// segment_B는 .vm파일 내부의 함수들이 접근하는 메모리 할당공간. 
// => 하나의 .vm파일 내에는 여러개의 함수가 존재 각 함수마다의 별도의 공간 부여
typedef struct segment_B{
    char name[50];
    int local[100];
    int argumnet[100];
    int this[100];
    int that[100];
    int stack[100];
    int sp;
    int *temp;
    struct segment_B *next; 
}segment_B;

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
