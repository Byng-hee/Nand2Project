#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "code.h"

void parser1(FILE *open, SymbolTable **recoder_ref);
void parser2(FILE* open, FILE* write, SymbolTable* recoder);

int main(int argc, char *argv[]) {
    SymbolTable *recoder = NULL;
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
    parser1(assembely_file, &recoder);
    rewind(assembely_file);
    parser2(assembely_file, machine_language_file, recoder);
    // while(recoder->next != NULL){
    //     printf("symbol_address : %d, symbol_name : %s \n", recoder->symbol_address, recoder->symbol_name);
    //     recoder = recoder->next;
    // }
    // printf("symbol_address : %d, symbol_name : %s \n", recoder->symbol_address, recoder->symbol_name);
    SymbolTable *current = recoder;
    while(current != NULL){
        SymbolTable *temp = current;
        current = current->next;
        free(temp);
    }
    fclose(assembely_file);
    fclose(machine_language_file);
    free(outputFileName);
    return 0;
}

void parser1(FILE *open, SymbolTable **recoder_ref){

    SymbolTable *head = NULL;
    SymbolTable *tail = NULL;
    int line_number = 0;
    int index = 0;

    for (int i = 0; i <= 15; i++) {
        SymbolTable *node = (SymbolTable *)malloc(sizeof(SymbolTable));
        sprintf(node->symbol_name, "R%d", i);
        node->symbol_address = i;
        node->next = NULL;

        if (*recoder_ref == NULL) {
            *recoder_ref = node;
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }
    
    SymbolTable *screen = (SymbolTable *)malloc(sizeof(SymbolTable));
    strcpy(screen->symbol_name, "SCREEN");
    screen->symbol_address = 16384;
    screen->next = NULL;
    tail->next = screen;
    tail = screen;

    SymbolTable *kbd = (SymbolTable *)malloc(sizeof(SymbolTable));
    strcpy(kbd->symbol_name, "KBD");
    kbd->symbol_address = 24576;
    kbd->next = NULL;
    tail->next = kbd;
    tail = kbd;

    char line[100];
    while(fgets(line, 100, open) != NULL ){
        //개행문자 제거
        line[strcspn(line, "\r\n")] = 0;
        CommandType type = get_command_type(line);

        if((type == A_COMMAND) || (type == C_COMMAND)){
            line_number++;
        }
        else if(type == L_COMMAND){
            char *temp = symbol(line, type);
            // 새로운 심볼 추가
            SymbolTable *node = (SymbolTable *)malloc(sizeof(SymbolTable));
            strcpy(node->symbol_name, temp);
            node->symbol_address = line_number;
            node->next = NULL;

            tail->next = node;
            tail = node;

            free(temp);
        }
    }
}

void parser2(FILE* open, FILE* write, SymbolTable* recoder){

    Reg REG;
    char line[100];
    REG.address = (char *)malloc(17);
    while(fgets(line, 100, open) != NULL ){
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        char *tmp = symbol(line, get_command_type(line));
        if(get_command_type(line) == A_COMMAND){
            if(tmp[0] >=48 && tmp[0] <=57 ){
                REG.I = atoi(tmp);
                //printf("line : %s, atoi value is : %d \n", line,  REG.I);
            }
            else{
                SymbolTable *current = recoder;
                while(current != NULL){
                    if(strcmp(current->symbol_name, tmp) == 0){ 
                        REG.I = current->symbol_address;
                        //printf("line : %s, L_COMPUTING value is : %s\n",line, REG.address);
                        break;
                    }
                    current = current->next;
                }
            }

            binaryAddress(REG.I, REG.address);  //address에 I값이 저장됨
            printf("binaryAddress value is : %s\n", REG.address);
            printf("A line is : %s\n", line);
            fprintf(write, "%s\n", REG.address);

        }
        else if(get_command_type(line) == L_COMMAND){}
        else if(get_command_type(line) == C_COMMAND){
            C_COMPUTING(line, REG.address);      //C_COMMAND일 경우를 처리하는 코드
            //printf("line : %s, C_COMPUTING value is : %s\n", line, REG.address);
            fprintf(write, "%s\n", REG.address);
        }
    }

    free(REG.address);

}