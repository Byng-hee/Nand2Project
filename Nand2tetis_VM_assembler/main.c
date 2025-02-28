#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include "parser.h"

//VM가상환경 자체에는 temp, constant 공간이 이미 생성되어있다. ->
//각 실행파일(.vm)은 시작시에 staic 공간을 생성하고 ->
//실행파일 내의 함수는 선언된 후에 argument,local,this,that,pointer의 공간을 생성해야한다

int main(int argc, char *argv[]){

    char* outputFileName = make_outputfilename(argv[1]);

    // 파일을 읽기 모드로 열기 (생성)
    FILE *assembely_file = fopen(argv[1], "r");

    // 파일을 쓰기 모드로 열기 (생성)
    FILE* machine_language_file = fopen(outputFileName, "w");

    if (machine_language_file == NULL || assembely_file == NULL) {
        perror("machine_language_file && assembely_file open failure");
        exit(1);
    }

    segment_A *s1 = (segment_A*)malloc(sizeof(segment_A));

    parser(assembely_file,machine_language_file, s1);

    free(s1);
    free(machine_language_file);
    free(assembely_file);
    return 0;
}
