#include "cpu.h"
#include "fontset.h"
#include <stdlib.h>
#include <time.h>

void printCPU(CPU* cpu, int registers, int stack)
{
	printf("pc is 0x%04X\n", cpu->pc );
	printf("sp is %i\n", cpu->sp);
	printf("I is %X\n", cpu->I);
	printf("current opcode is 0x%X\n", cpu->opcode );

	if(registers)
	{
		printf("Registers\n");
		for(int i =0; i< 16; i++)
		{
			printf("%X: 0x%X\n", i, cpu->V[i]);
		}
		printf("\n");
	}

	if(stack)
	{
		printf("Stack\n");
		for(int i =0; i< 16; i++)
		{
			printf("%X: 0x%X\n", i, cpu->stack[i]);
		}
		printf("\n");
	}
printf("\n");
}


void emulateCycle(CPU *cpu)
{
	cpu->opcode = cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1]; 
	unsigned short* pc = &cpu->pc;
	unsigned short* sp = &cpu->sp;
	unsigned short* opcode = &cpu->opcode;
	unsigned short* stack = cpu->stack;
	unsigned char* V = cpu->V;
	unsigned char* memory = cpu->memory;
	unsigned short* I = &cpu->I;
	unsigned char* delay_timer = &cpu->delay_timer;
	unsigned char* sound_timer = &cpu->sound_timer;
	unsigned char* gfx = cpu->gfx;
	int* drawFlag = &cpu->drawFlag;
	unsigned char* key = cpu->key;
	switch(*opcode &  0xF000)
	{
		case 0x0000:
			switch(*opcode)
			{
				case 0x00E0:
					for(int i = 0;i<64*32; i++ )
					{
						gfx[i] = 0;
					}
					*drawFlag = 1;
					(*pc)+=2;
					break;
				case 0x00EE:
					(*sp)--;
					(*pc) = stack[*sp];
					(*pc) += 2;
					break;
				default:
					printf("unknown 0x0000 opcode: %X\n", *opcode);
					exit(0);
			}
			break;
		case 0x1000:
			(*pc) = *opcode & 0x0fff;
			break;
		case 0x2000:
			stack[*sp] = *pc;
			(*sp)++;
			(*pc) = *opcode & 0x0fff;
			break;
		case 0x3000:
			if(V[(*opcode & 0x0f00)>>8] == (*opcode &0x00ff))
				(*pc) += 4;
			else
				(*pc) += 2;
			break;
		case 0x4000:
			if(V[(*opcode & 0x0f00)>>8] != (*opcode &0x00ff))
				(*pc) += 4;
			else
				(*pc) += 2;
			break;
		case 0x5000:
			if(V[(*opcode & 0x0f00)>>8] == V[(*opcode & 0x00f0)>>4])
				(*pc)+=4;
			else
				(*pc)+=2;
			break;
		case 0x6000:
			V[(*opcode & 0x0f00)>>8] = *opcode & 0x00ff;
			(*pc)+=2;
			break;
		case 0x7000:
			V[(*opcode & 0x0f00)>>8] += *opcode & 0x00ff; 
			(*pc) += 2;
			break;
		case 0x8000:
			switch(*opcode & 0x000f)
			{
				case 0x0000:
					V[(*opcode & 0x0f00)>>8] = V[(*opcode & 0x00f0)>>4];
					(*pc) += 2;
					break;
				case 0x0001:
					V[(*opcode & 0x0f00)>>8] |= V[(*opcode & 0x00f0)>>4];
					(*pc) += 2;
					break;
				case 0x0002:
					V[(*opcode & 0x0f00)>>8] &= V[(*opcode & 0x00f0)>>4];
					(*pc) += 2;
					break;
				case 0x0003:
					V[(*opcode & 0x0f00)>>8] ^= V[(*opcode & 0x00f0)>>4];
					(*pc) += 2;
					break;
				case 0x0004:
					if(V[(*opcode & 0x0f00)>>8] > (0xFF - V[(*opcode & 0x00f0)>>4]))
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(*opcode & 0x0f00)>>8] += V[(*opcode & 0x00f0)>>4];
					(*pc) +=2;
					break;
				case 0x0005:
					if(V[(*opcode & 0x0f00)>>8]<V[(*opcode & 0x00f0)>>4])
						V[0xF] = 0;
					else
						V[0xF] = 1;
					V[(*opcode & 0x0f00)>>8] -= V[(*opcode & 0x00f0)>>4];
					(*pc) +=2;
					break;
				case 0x0006:
					V[0xf] =V[(*opcode & 0x0f00)>>8]&0x1;
					V[(*opcode & 0x0f00)>>8]>>=1;
					(*pc) +=2;
					break;
				case 0x0007:
					if(V[(*opcode & 0x0f00)>>8]>V[(*opcode & 0x00f0)>>4])
						V[0xF] = 0;
					else
						V[0xF];
					V[(*opcode & 0x0f00)>>8] = V[(*opcode & 0x00f0)>>4] - V[(*opcode & 0x0f00)>>8] ;
					(*pc) +=2;
					break;
				case 0x000E:
					V[0xf] = V[(*opcode & 0x0f00)>>8]>>7;
					V[(*opcode & 0x0f00)>>8]<<=1;
					(*pc)+=2;
					break;
			}
			break;
		case 0x9000:
			if(V[(*opcode & 0x0f00) >> 8] != V[(*opcode & 0x00f0) >> 4])
				(*pc) += 4;
			else
				(*pc) += 2;
			break;
		case 0xA000:
			(*I) = *opcode & 0x0fff;
			(*pc) += 2;
			break;
		case 0xC000:
			V[(*opcode & 0x0f00)>>8] = (rand() % 0xff) & (*opcode & 0x00ff);
			(*pc) +=2;
			break;
		case 0xD000:
			V[0xf] = 0;
			unsigned short x = V[(*opcode & 0x0f00)>>8];
			unsigned short y = V[(*opcode & 0x00f0)>>4];
			unsigned short height = (*opcode & 0x000f);
			unsigned short pixel;
			for (int yline = 0; yline < height; yline++)
			  {
				pixel = memory[(*I) + yline];
				for(int xline = 0; xline < 8; xline++)
				{
				  if((pixel & (0x80 >> xline)) != 0)
				  {
					if(gfx[(x + xline + ((y + yline) * 64))] == 1)
					  V[0xF] = 1;                                 
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				  }
				}
			  }
			*drawFlag = 1;
			(*pc) +=2;
			break;
		case 0xE000:
			switch (*opcode & 0x00ff)
			{
				case 0x009E:
					if(key[V[(*opcode & 0x0f00)>>8]]!=0)
						(*pc)+=4;
					else
						(*pc)+=2;
					break;
				case 0x00A1:
					if(key[V[(*opcode & 0x0f00)>>8]]==0)
						(*pc)+=4;
					else
						(*pc)+=2;
					break;
			}
			break;
		case 0xF000:
			switch(*opcode & 0x00ff)
			{
				case 0x0007:
					V[(*opcode & 0x0f00)>>8]= (*delay_timer);
					(*pc)+=2;
					break;
				case 0x000A:
					for(int i =0; i<16; i++)
					{
						if(key[i])
						{
							V[(*opcode & 0x0f00)] = i;
							(*pc)+=2;
						}
					}
					break;
				case 0x0015:
					*delay_timer = V[(*opcode & 0x0f00)>>8];
					(*pc) +=2;
					break;
				case 0x0018:
					cpu->sound_timer = V[(*opcode & 0x0f00)>>8];
					(*pc) +=2;
					break;
				case 0x001E:
					(*I)+=V[(*opcode & 0x0f00)>>8];
					(*pc) +=2;
					break;
				case 0x0029:
					*I = V[(*opcode & 0x0f00)>>8]*0x5;
					(*pc)+=2;
					break;
				case 0x0033:
					memory[*I]       = V[(*opcode & 0x0F00) >> 8] / 100;
				  	memory[(*I) + 1] = (V[(*opcode & 0x0F00) >> 8] / 10) % 10;
				  	memory[(*I) + 2] = (V[(*opcode & 0x0F00) >> 8] % 100) % 10;
				  	(*pc) += 2;
					break;
				case 0x0055:
					for(int i = 0; i < ((*opcode & 0x0f00)>>8); i++)
					{
						memory[(*I)+i] = V[i];  
					}
					(*pc)+=2;
					break;
				case 0x0065:
					for(int i = 0; i < ((*opcode & 0x0f00)>>8); i++)
					{
						V[i] = memory[(*I)+i];
					}
					(*pc)+=2;
					break;
				default:
					printf("Unknown 0xF000 opcode: %X\n", *opcode);
					exit(0);
			}
			break;
		default:
			printf	("Unknown opcode: 0x%X\n", cpu->opcode);
			exit(0);
	}
	if(cpu->delay_timer >0)
		cpu->delay_timer--;
	
	if(cpu->sound_timer >0)
	{
		if(cpu->sound_timer == 1)
		{
			printf("\aBUZZ!\n");
		}
		cpu->sound_timer--;
	}

	//printf("0x%X\n", *opcode);
}

void drawGraphics(CPU* cpu)
{
	for(int i =0; i< 32; i++)
	{
		for(int j=0; j<64; j++)
		{
			printf("%c ", cpu->gfx[j+64*i]? '#' : ' ' );
		}
		printf("\n");
	}
}
void debugRender();

// method called when first starting up the system
// clears all memory and places initial values in registers
void init(CPU *cpu)
{
	// system expects application to be loaded at 0x200
	cpu->pc = 0x200;

	//reset some registers
	cpu->opcode = 0;
	cpu->I = 0;
	cpu->sp = 0;

	//clear display
	for(int i = 0; i < 2048; i++)
	{
		cpu->gfx[i] = 0;
	}

	//clear stack, keypad, registers
	for(int i =0; i < 16; i++)
	{
		cpu->stack[i] = 0;
		cpu->key[i] = 0;
		cpu->V[i] = 0;
	}

	//clear memory
	for(int i = 0; i < 4096; i++)
	{
		cpu->memory[i] = 0;
	}
	//Load fontset
	for(int i = 0; i < 80; i++)
	{
		cpu->memory[i] = chip8_fontset[i];
	}

	cpu->delay_timer = 0;
	cpu->sound_timer = 0;
	cpu->drawFlag = 1;
	srand(time(NULL));
}


int loadGame (CPU *cpu, char* romName)
{
	//FILE* game = fopen("/home/michael/Documents/Projects/C/Chip8/roms/tetris.c8","rb" );
	
	char* test = malloc(strlen("roms/")+strlen(romName)+strlen(".c8")+1);
	strcpy(test, "roms/");
	strcat(test, romName);
	strcat(test, ".c8");
	FILE* game = fopen(test,"rb" );
	free(test);
	
	// get file size to allocate a buffer
	fseek(game, 0l, SEEK_END);
	long size = ftell(game);
	rewind(game);
	printf("File size is %lu bytes\n" ,size);

	// create a buffer
	char* buffer = (char*) malloc(size*sizeof(char));
	
	// read the file into the buffer
	size_t read = fread(buffer, 1, size, game);

	// copy the buffer into our virtual memory
	for(int i = 0; i < size; i++)
	{
		cpu->memory[512+i] = buffer[i];
	}

	printf("%s\n", "Successfully loaded file");

	fclose(game);
	free(buffer);
	return 1;
}

