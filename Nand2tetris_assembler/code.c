#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"


int dest(const char* str, CommandType command){
	int dest_name;
	if(command==C_COMMAND){
		if (strcmp(str, "M") == 0) {
    		dest_name = 0b001;
		} else if (strcmp(str, "D") == 0) {
    		dest_name = 0b010;
		} else if (strcmp(str, "MD") == 0) {
    		dest_name = 0b011;
		} else if (strcmp(str, "A") == 0) {
    		dest_name = 0b100;
		} else if (strcmp(str, "AM") == 0) {
    		dest_name = 0b101;
		} else if (strcmp(str, "AD") == 0) {
    		dest_name = 0b110;
		} else if (strcmp(str, "AMD") == 0) {
    		dest_name = 0b111;
		} else {
    		dest_name = 0b000;  // 기본값
		}
	}
	else{
		dest_name=0;
	}
	return dest_name;
}

int comp(const char* str, CommandType command){
	int comp_name;
	if (command == C_COMMAND) {
		if (strcmp(str, "0") == 0) {
			comp_name = 0b0101010;
		} else if (strcmp(str, "1") == 0) {
			comp_name = 0b0111111;
		} else if (strcmp(str, "-1") == 0) {
			comp_name = 0b0111010;
		} else if (strcmp(str, "D") == 0) {
			comp_name = 0b0001100;
		} else if (strcmp(str, "A") == 0) {
			comp_name = 0b0110000;
		} else if (strcmp(str, "!D") == 0) {
			comp_name = 0b0001101;
		} else if (strcmp(str, "!A") == 0) {
			comp_name = 0b0110001;
		} else if (strcmp(str, "-D") == 0) {
			comp_name = 0b0001111;
		} else if (strcmp(str, "-A") == 0) {
			comp_name = 0b0110011;
		} else if (strcmp(str, "D+1") == 0) {
			comp_name = 0b0011111;
		} else if (strcmp(str, "A+1") == 0) {
			comp_name = 0b0110111;
		} else if (strcmp(str, "D-1") == 0) {
			comp_name = 0b0001110;
		} else if (strcmp(str, "A-1") == 0) {
			comp_name = 0b0110010;
		} else if (strcmp(str, "D+A") == 0) {
			comp_name = 0b0000010;
		} else if (strcmp(str, "D-A") == 0) {
			comp_name = 0b0010011;
		} else if (strcmp(str, "A-D") == 0) {
			comp_name = 0b0000111;
		} else if (strcmp(str, "D&A") == 0) {
			comp_name = 0b0000000;
		} else if (strcmp(str, "D|A") == 0) {
			comp_name = 0b0010101;
		} else if (strcmp(str, "M") == 0) {
			comp_name = 0b1110000;
		} else if (strcmp(str, "!M") == 0) {
			comp_name = 0b1110001;
		} else if (strcmp(str, "-M") == 0) {
			comp_name = 0b1110011;
		} else if (strcmp(str, "M+1") == 0) {
			comp_name = 0b1110111;
		} else if (strcmp(str, "M-1") == 0) {
			comp_name = 0b1110010;
		} else if (strcmp(str, "D+M") == 0) {
			comp_name = 0b1000010;
		} else if (strcmp(str, "D-M") == 0) {
			comp_name = 0b1010011;
		} else if (strcmp(str, "M-D") == 0) {
			comp_name = 0b1000111;
		} else if (strcmp(str, "D&M") == 0) {
			comp_name = 0b1000000;
		} else if (strcmp(str, "D|M") == 0) {
			comp_name = 0b1010101;
		} else {
			comp_name = 0;
		}
	}
	else {
	    comp_name = 0;
	}
	return comp_name;
}


int jump(const char* str, CommandType c_command){
	int jump_name;
	if (c_command == C_COMMAND) {
		if (strcmp(str, "JGT") == 0) {
			jump_name = 0b001;
		} else if (strcmp(str, "JEQ") == 0) {
			jump_name = 0b010;
		} else if (strcmp(str, "JGE") == 0) {
			jump_name = 0b011;
		} else if (strcmp(str, "JLT") == 0) {
			jump_name = 0b100;
		} else if (strcmp(str, "JNE") == 0) {
			jump_name = 0b101;
		} else if (strcmp(str, "JLE") == 0) {
			jump_name = 0b110;
		} else if (strcmp(str, "JMP") == 0) {
			jump_name = 0b111;
		} else {
			jump_name = 0b000;
		}
	} else {
	    jump_name = 0;
	}
	return jump_name;

}