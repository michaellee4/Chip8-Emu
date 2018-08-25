#ifndef GRAPHICS_H

#define GRAPHICS_H
#include "cpu.h"
#include<SDL2/SDL.h>

SDL_Window* createWindow();
SDL_Renderer* createRenderer(SDL_Window* window);
void drawPixels(SDL_Renderer* renderer, CPU* cpu);


#endif
