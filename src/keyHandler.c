#include "keyHandler.h"
#include <stdio.h>
void setKeys(CPU* cpu)
{
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	
	SDL_Scancode hexPad[] = 
	{	SDL_SCANCODE_1,
		SDL_SCANCODE_2,
		SDL_SCANCODE_3,
		SDL_SCANCODE_4,
		SDL_SCANCODE_Q,
		SDL_SCANCODE_W,
		SDL_SCANCODE_E,
		SDL_SCANCODE_R,
		SDL_SCANCODE_A,
		SDL_SCANCODE_S,
		SDL_SCANCODE_D,
		SDL_SCANCODE_F,
		SDL_SCANCODE_Z,
		SDL_SCANCODE_X,
		SDL_SCANCODE_C,
		SDL_SCANCODE_V
	};
	
	for(int i =0; i<16;i++)
	{
		cpu->key[i] = keyboard[hexPad[i]];
	}
}
