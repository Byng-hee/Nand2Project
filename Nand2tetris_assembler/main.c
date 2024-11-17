#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "code.h"

void parser1(FILE* open, SymbolTable* recoder);
void parser2(FILE* open, FILE* write, SymbolTable* recoder);

int main(int argc, char *argv[]) {
    SymbolTable *recoder = (SymbolTable *)malloc(sizeof(SymbolTable)*20);
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
    parser1(assembely_file, recoder);
    parser2(assembely_file, machine_language_file, recoder);


    free(recoder);
    fclose(assembely_file);
    fclose(machine_language_file);
    free(outputFileName);
    return 0;
}

void parser1(FILE* open, SymbolTable *recoder){
    for(int i=0; i<=15; i++){
        sprintf(recoder[i].symbol_name,"R%d",i);
        recoder[i].symbol_address=i;
    }
    recoder[16].symbol_address = 16384;
    strcpy(recoder[16].symbol_name , "SCREEN");
    recoder[17].symbol_address = 24576;
    strcpy(recoder[17].symbol_name , "KBD");
    int index = 18;
    int line_number = 1;

    char line[100];
    char *temp;
    while(fgets(line, 100, open) != NULL ){
        if((get_command_type(line) == C_COMMAND) || (get_command_type(line) == A_COMMAND)){
            line_number++;
        }
        if(get_command_type(line)==L_COMMAND){
            temp = symbol(line, get_command_type(line));
            recoder[index].symbol_address = line_number;
            strcpy(recoder[index++].symbol_name , temp);
        }
    }

    SymbolTable *t = (SymbolTable *)realloc(recoder, index*sizeof(SymbolTable));
    recoder = t;

}

void parser2(FILE* open, FILE* write, SymbolTable* recoder){
    Reg REG;
    char line[100];
    REG.address = (char *)malloc(17);
    char *tmp;
    int size = sizeof(recoder) / sizeof(recoder[0]);
    while(fgets(line, 100, open) != NULL ){
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        tmp = symbol(line, get_command_type(line));

        if( *tmp == A_COMMAND){
            printf("line's value is : %s\n", line);
            if(tmp[0] >=48 && tmp[0] <=57 ){
                REG.I = atoi(tmp);
                printf("atoi value is : %d \n", REG.I);
            }
            else{
                for(int i=0; i<size; i++){
                    if(strcmp(recoder[i].symbol_name, tmp) == 0){ 
                        REG.I = recoder[i].symbol_address;
                    }
                }
            }

            binaryAddress(REG.I, REG.address);  //address에 I값이 저장됨
            printf("binaryAddress value is : %s\n", REG.address);
            fprintf(write, "%s\n", REG.address);

        }
        else if(*tmp==L_COMMAND){}
        else{
            printf("%s\n", line);
            C_COMPUTING(line, REG.address);      //C_COMMAND일 경우를 처리하는 코드
            printf("C_COMPUTING value is : %s\n", REG.address);
            fprintf(write, "%s\n", REG.address);
        }
    }

    free(REG.address);

}