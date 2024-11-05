#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int dest(const char* str, CommandType command){
	int dest_name;
	if(command==C_COMMAND){
		switch(str){
		case 'M':
			dest_name=0b001;
			break;
		case 'D':
			dest_name=0b010;
			break;
		case 'MD':
			dest_name=0b011;
			break;
		case 'A':
			dest_name=0b100;
			break;
		case 'AM':
			dest_name=0b101;
			break;
		case 'AD':
			dest_name=0b110;
			break;
		case 'AMD':
			dest_name=0b111;
			break;
		default:
			dest_name=0b000;
		}
	}
	else{
		dest_name=NULL;
	}
	return dest_name;
}

int comp(const char* str, CommandType command){
	int comp_name;
	if (command == C_COMMAND) {
	    switch (str) {
	        case '0':
	            comp_name = 0b0101010;
	            break;
	        case '1':
	            comp_name = 0b0111111;
	            break;
	        case '-1':
	            comp_name = 0b0111010;
	            break;
            case 'D':
	            comp_name = 0b0001100;
	            break;
	        case 'A':
	            comp_name = 0b0110000;
	            break;
	        case '!D':
	            comp_name = 0b0001101;
	            break;
	        case '!A':
	            comp_name = 0b0110001;
	            break;
	        case '-D':
	            comp_name = 0b0001111;
	            break;
	        case '-A':
	            comp_name = 0b0110011;
	            break;
	        case 'D+1':
	            comp_name = 0b0011111;
	            break;
	        case 'A+1':
	            comp_name = 0b0110111;
	            break;
	        case 'D-1':
	            comp_name = 0b0001110;
	            break;
	        case 'A-1':
	            comp_name = 0b0110010;
	            break;
	        case 'D+A':
	            comp_name = 0b0000010;
	            break;
	        case 'D-A':
	            comp_name = 0b0010011;
	            break;
	        case 'A-D':
	            comp_name = 0b0000111;
	            break;
	        case 'D&A':
	            comp_name = 0b0000000;
	            break;
	        case 'D|A':
	            comp_name = 0b0010101;
	            break;
	        case 'M':
	            comp_name = 0b1110000;
	            break;
	        case '!M':
	            comp_name = 0b1110001;
	            break;
	        case '-M':
	            comp_name = 0b1110011;
	            break;
	        case 'M+1':
	            comp_name = 0b1110111;
	            break;
	        case 'M-1':
	            comp_name = 0b1110010;
	            break;
	        case 'D+M':
	            comp_name = 0b1000010;
	            break;
	        case 'D-M':
	            comp_name = 0b1010011;
	            break;
	        case 'M-D':
	            comp_name = 0b1000111;
	            break;
	        case 'D&M':
	            comp_name = 0b1000000;
	            break;
	        case 'D|M':
	            comp_name = 0b1010101;
	            break;
	        default:
	            comp_name = NULL;
	    }
	}
	else {
	    comp_name = NULL;
	}
	return comp_name;
}


int jump(const char* str, CommandType c_command){
	int jump_name;
	if (c_command == C_COMMAND) {
	    switch (str) {
	        case "JGT":
	            jump_name = 0b001;
	            break;
	        case "JEQ":
	            jump_name = 0b010;
	            break;
	        case "JGE":
	            jump_name = 0b011;
	            break;
	        case "JLT":
	            jump_name = 0b100;
	            break;
	        case "JNE":
	            jump_name = 0b101;
	            break;
	        case "JLE":
	            jump_name = 0b110;
	            break;
	        case "JMP":
	            jump_name = 0b111;
	            break;
	        default:
	            jump_name = 0b000;
	    }
	} else {
	    jump_name = NULL;
	}
	return jump_name;

}
