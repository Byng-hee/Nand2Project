#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char* dest(const char* str, CommandType command){
	const char dest_name[10];
	if(command==C_COMMAND){
		switch(str){
		case 'M':
			dest_name="001";
			break;
		case 'D':
			dest_name="010";
			break;
		case 'MD':
			dest_name="011";
			break;
		case 'A':
			dest_name="100";
			break;
		case 'AM':
			dest_name="101";
			break;
		case 'AD':
			dest_name="110";
			break;
		case 'AMD':
			dest_name="111";
			break;
		default:
			dest_name="000";
		}
	}
	else{
		dest_name='\n';
	}
	return dest_name;
}

const char* comp(const char* str, CommandType command){
	const char comp_name[10]; // 'dest'를 'comp'로 변경
	if (command == C_COMMAND) {
	    switch (str) {
	        case '0':
	            comp_name = "0.101010"; // 첫 번째 요소에 문자열 할당
	            break;
	        case '1':
	            comp_name = "0.111111";
	            break;
	        case '-1':
	            comp_name = "0.111010";
	            break;
            case 'D':
	            comp_name = "0.001100";
	            break;
	        case 'A':
	            comp_name = "0.110000";
	            break;
	        case '!D':
	            comp_name = "0.001101";
	            break;
	        case '!A':
	            comp_name = "0.110001";
	            break;
	        case '-D':
	            comp_name = "0.001111";
	            break;
	        case '-A':
	            comp_name = "0.110011";
	            break;
	        case 'D+1':
	            comp_name = "0.011111";
	            break;
	        case 'A+1':
	            comp_name = "0.110111";
	            break;
	        case 'D-1':
	            comp_name = "0.001110";
	            break;
	        case 'A-1':
	            comp_name = "0.110010";
	            break;
	        case 'D+A':
	            comp_name = "0.000010";
	            break;
	        case 'D-A':
	            comp_name = "0.010011";
	            break;
	        case 'A-D':
	            comp_name = "0.000111";
	            break;
	        case 'D&A':
	            comp_name = "0.000000";
	            break;
	        case 'D|A':
	            comp_name = "0.010101";
	            break;
	        case 'M':
	            comp_name = "1.110000";
	            break;
	        case '!M':
	            comp_name = "1.110001";
	            break;
	        case '-M':
	            comp_name = "1.110011";
	            break;
	        case 'M+1':
	            comp_name = "1.110111";
	            break;
	        case 'M-1':
	            comp_name = "1.110010";
	            break;
	        case 'D+M':
	            comp_name = "1.000010";
	            break;
	        case 'D-M':
	            comp_name = "1.010011";
	            break;
	        case 'M-D':
	            comp_name = "1.000111";
	            break;
	        case 'D&M':
	            comp_name = "1.000000";
	            break;
	        case 'D|M':
	            comp_name = "1.010101";
	            break;
	        default:
	            comp_name = '\0';
	    }
	}
	else {
	    comp_name = "\0";
	}
	return comp_name;
}


const char* jump(const char* str, CommandType c_command){
	const char jump_name[10]; // 'dest'를 'jump'로 변경
	if (c_command == C_COMMAND) {
	    switch (str) {
	        case "JGT":
	            jump_name = "001"; // 배열의 인덱스를 사용해야 함
	            break;
	        case "JEQ":
	            jump_name = "010";
	            break;
	        case "JGE":
	            jump_name = "011";
	            break;
	        case "JLT":
	            jump_name = "100";
	            break;
	        case "JNE":
	            jump_name = "101";
	            break;
	        case "JLE":
	            jump_name = "110";
	            break;
	        case "JMP":
	            jump_name = "111";
	            break;
	        default:
	            jump_name = "000";
	    }
	} else {
	    jump_name = '\0';
	}
	return jump_name; // 반환값도 배열의 첫 번째 요소로 수정

}
