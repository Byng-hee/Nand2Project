#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "parser.h"
//READ FILE
//애초에 개발 방향 : 단순한 .vm(파일)을 .asm(파일)으로 변환하는 것이다.
//초반에 개발에 방향성에 대한 이해 부족으로 아예 VM 에뮬레이터를 개발해버렸다.... 하아,,,,,,
//그래서 개발 방향 수정함.....ㅆ...... 이렇게 된거 그냥 만들자!!!!!!!!!!!!!^^
// => RAM공간 구현!
// => 실제 .vm을 .asm으로 변환할 때 컴퓨터 내부에서 발생하는 그 로우단을 구현함!(가상 머신이다! 히익..)
// => 인터페이스까지 개발할것 (현재도 가능)
/*=================================================================================================*/
// => (미래) 가능하면 사이트 환경에서 인터페이스 구축하고 싶다.
// => Main호출 부터 function호출마다 갱신되는 RAM[0]-RAM[15]의 모습들 + RAM[256]부터 그 이후의 전역 STACK의 변화 보여줌
// =>                                                        *256번지는 임의로 초기한 시작 SP의 주소*


//VM가상환경 자체에는 temp, constant 공간이 이미 생성되어있다. ->
//각 실행파일(.vm)은 시작시에 staic 공간을 생성하고 ->
//실행파일 내의 함수는 선언된 후에 argument,local,this,that,pointer의 공간을 생성해야한다

int main(int argc, char *argv[]){
    segment_A *s1 = (segment_A*)malloc(sizeof(segment_A));
    // 나중에 Sys.init 함수를 호출해서 해결할것(아마도.. 책에서 이거부르라던데 나도 먼지 몰라.. 무서워)
    s1->RAM[0] = 256; // SP를 RAM[256]을 가리키게 만듦.
    s1->SP = 256;   // SP를 RAM[256]을 가리키게 만듦.
    s1->LCL = 256;
    s1->ARG = 256;
    s1->THAT = 0;
    s1->THIS = 0;
    s1->STATIC = 16;    // STATIC의 범위는 16 - 255 까지의 범위를 가진다.
    s1->TEMP = 5;
    s1->POINTER = 3;

    char* outputFileName = make_outputfilename(argv[1]);
    s1->staticName = outputFileName;
    strcat(outputFileName, ".asm");

    // 파일을 읽기 모드로 열기 (생성)
    FILE *assembely_file = fopen(argv[1], "r");

    // 파일을 쓰기 모드로 열기 (생성)
    FILE* machine_language_file = fopen(outputFileName, "w");

    if (machine_language_file == NULL || assembely_file == NULL) {
        perror("machine_language_file && assembely_file open failure");
        exit(1);
    }
    
    parser(assembely_file,machine_language_file, s1);

    free(s1);
    free(machine_language_file);
    free(assembely_file);
    return 0;
}
