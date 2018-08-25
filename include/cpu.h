#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include<string.h>
typedef struct 
{
	// stores the current opcode which is 2 bytes long
	unsigned short opcode;
	
	// emulates the 4mb memory array
	unsigned char memory[4096];
	
	// emulates the 15 + 1 8-bit registers present. Named V*
	unsigned char V[16];
	
	// the index register
	unsigned short I;

	// the program counter. Can have a value fro 0x000-0xfff
	unsigned short pc;
	
	// screen consists of b/w pixels in a 64 x 32 display
	unsigned char gfx[64 * 32];

	// ticks at 60hz refresh rate
	unsigned char delay_timer;
	
	// system comes with a single buzzer that will sound when the timer hits 0
	unsigned char sound_timer;

	// self implemented stack and stack pointer for jmp commands
	unsigned short stack[16];
	unsigned short sp;

	// HEX based keypad
	unsigned char key[16];

	int drawFlag;
} CPU;

void printTest(CPU* p);

void init(CPU* cpu);

void emulateCycle(CPU* cpu);

void debugRender();

int loadGame(CPU* cpu, char* romName);
#endif
