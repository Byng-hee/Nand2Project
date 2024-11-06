#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "code.h"

void parser1(FILE* open, FILE* write);
//void parser2();


int main(int argc, char *argv[]) {
    
    char *dot = strrchr(argv[1], '.');
    char *outputFileName;
    if (dot != NULL) {
        // '.'이 있는 경우, 확장자 제외한 파일 이름의 길이 계산
        size_t length = dot - argv[1];
        // 동적으로 메모리 할당
        outputFileName = (char *)malloc(length + strlen(".txt") + 1); // +1은 널 종료 문자
        if (outputFileName == NULL) {
            perror("Memory Alloc Failure");
            exit(1);
        }
        strncpy(outputFileName, argv[1], length); // 원본 파일 이름 복사
        outputFileName[length] = '\0'; // 문자열 종료
    }
    strcat(outputFileName, ".txt");


    // 파일을 읽기 모드로 열기 (생성)
    FILE *assembely_file = fopen(argv[1], "r");
    // 파일을 쓰기 모드로 열기 (생성)
    FILE* machine_language_file = fopen(outputFileName, "w");
    if (machine_language_file == NULL || assembely_file == NULL) {
        perror("machine_language_file && assembely_file open failure");
        exit(1);
    }
    parser1(assembely_file, machine_language_file);
    //parser2();

    fclose(assembely_file);
    fclose(machine_language_file);
    free(outputFileName);
    return 0;
}



void parser1(FILE* open, FILE* write){
    Reg REG;
    char line[50];
    REG.address = (char *)malloc(16);

    while(fgets(line, 50, open) != NULL ){
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        // 디버깅을 위해 symbol 함수의 반환 값을 직접 출력해보기
        char *sym_result = symbol(line, get_command_type(line));


        if( symbol(line ,get_command_type(line)) != NULL){
            printf("line's value is : %s\n", line);

            REG.I = atoi(symbol(line ,get_command_type(line)));
            printf("atoi value is : %d \n", REG.I);

            binaryAddress(REG.I, REG.address);  //address에 I값이 저장됨
            printf("binaryAddress value is : %s\n", REG.address);
            fprintf(write, "%s\n", REG.address);
            
        }
        else{
            printf("%s\n", line);

            C_COMPUTING(line, REG.address);      //C_COMMAND일 경우를 처리하는 코드
            printf("C_COMPUTING value is : %s\n", REG.address);
            fprintf(write, "%s\n", REG.address);
            
        }

    }
    free(REG.address);
}


//void parser2(){}
