// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.
(LOOP2)	
@SCREEN	//SCREEN에 검은 선칠하기(키보드 입력이 있는동안)
D=A
@R0		//R0=16384 SCREEN의 메모리 주소값
M=D
(LOOP)
@KBD
D=M
@END
D;JEQ
@R0
D=M
A=D
M=-1
D=D+1		//R0 = 16384 + 1 SCREEN에서 한 칸씩 칠해나가기 위함
@R0
M=D
@LOOP
0;JMP
(END)
@LOOP		//SCREEN에 흰색칠하기 (키보드입력 X동안)
D;JNE
@R0
D=M
@SCREEN	//SCREEN주소인 16384이외에 다른 주소값 건드리지 못하기 위함
D=D-A
D=D+1
@LOOP2
D;JLE
@R0		//R0에 저장된 주소를 찾아가서 검은선을 지우기 위함
D=M
A=D
M=0
D=D-1		//R0 = 16384 + A - 1
@R0
M=D
@END
0;JMP