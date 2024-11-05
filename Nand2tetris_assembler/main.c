#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <stdbool.h>
#include "parser.h"
#include "code.c"
// #include "symbol_table.h"

void parser1(FILE* open, FILE* write);
void parser2();



int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "HOW TO USE : %s <FILENAME>\n", argv[0]);
        return 1;
    }
    
    char *dot = strrchr(argv[1], '.');
    char *outputFileName;
    if (dot != NULL) {
        // '.'이 있는 경우, 확장자 제외한 파일 이름의 길이 계산
        size_t length = dot - argv[1];
        // 동적으로 메모리 할당
        outputFileName = (char *)malloc(length + strlen(".hack") + 1); // +1은 널 종료 문자
        if (outputFileName == NULL) {
            perror("Memory Alloc Failure");
            return 1;
        }
        strncpy(outputFileName, argv[1], length); // 원본 파일 이름 복사
        outputFileName[length] = '\0'; // 문자열 종료
    }
    strcat(outputFileName, ".hack");


    // 파일을 읽기 모드로 열기 (생성)
    FILE *assembely_file = fopen(argv[1], "r");
    // 파일을 쓰기 모드로 열기 (생성)
    FILE* machine_language_file = fopen(outputFileName, "w");
    if (machine_language_file == NULL || assembely_file == NULL) {
        perror("파일 열기 실패");
        return 1;
    }

    parser1(assembely_file, machine_language_file);
    parser2();

    fclose(assembely_file);
    fclose(machine_language_file);
    return 0;
}



void parser1(FILE* open, FILE* write){
    reg REG;
    char line[50];
    while(fgets(line, 50, open)){
        if( symbol(line ,get_command_type(line)) != NULL){
            REG.I = atoi(symbol(line ,get_command_type(line)))
            binaryAddress(REG.I, REG.address);
            size_t written = fwrite(&REG.address, sizeof(int), 16, write);
            if(wittern != 16){
                printf('file witer error');
            }
            else{
                printf('write good');
            }
        }
        else{
            REG.address = C_COMPUTING(line);      //C_COMMAND일 경우를 처리하는 코드
            size_t written = fwrite(&REG.address, sizeof(int), 16, write);
            if(wittern != 16){
                printf('file witer error');
            }
            else{
                printf('write good');
            }
        }
    }
}

void parser2(){
}
