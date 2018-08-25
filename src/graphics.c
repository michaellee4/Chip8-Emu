#include "graphics.h"
SDL_Window* createWindow()
{
	SDL_Window* window = NULL;
    window = SDL_CreateWindow
    (   
        "Chip 8", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        64 * 16, 
        32 * 16, 
        SDL_WINDOW_SHOWN
    );

	return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1,SDL_RENDERER_ACCELERATED);

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear(renderer);
	return renderer;
}

void drawPixels(SDL_Renderer* renderer, CPU* cpu)
{
	for(int i =0; i< 32; i++)
		{
			for(int j=0; j<64; j++)
			{
				SDL_Rect r;
				r.x = j*16; 
				r.y = i*16; 
				r.w = 16*8;
				r.h = 16*8;

				if(cpu->gfx[i*64 + j])
					SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
				else
					SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
			// Render rect
				SDL_RenderFillRect( renderer, &r );

			}
		}

			SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
}

