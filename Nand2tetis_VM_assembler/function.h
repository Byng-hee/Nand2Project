//function.h
#ifndef function_H
#define function_H

typedef struct {
    int static[100];
    int RAM[16384];
}segment_A;

typedef struct {
    int local[100];
    int argumnet[100];
    int this[100];
    int that[100];
    int stack[100];
    int st;
    int temp[8];
}segment_B;

char* make_outputfilename(char* argv);
void pop(char *arg1, char * arg2, segment* s1);
void push(char *arg1, char * arg2);
void add();
void sub();
void neg();
void eq();
void gt();
void lt();
void and();
void or();
void not();

#endif
