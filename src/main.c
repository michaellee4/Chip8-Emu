#include "cpu.h"
#include "graphics.h"
#include "keyHandler.h"
#include <unistd.h>
#include "fpsClock.h"
int main (int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Please attach a rom.\n");
		exit(0);
	}

	double FPS = 600;
	double MILLI_PER_FRAME = 1000/FPS;

	CPU cpu;
	init(&cpu);

	loadGame(&cpu, argv[1]);

	SDL_Window* window = createWindow();
    SDL_Renderer* renderer = createRenderer(window);
   
    // Clear winow
    SDL_RenderClear( renderer );
	fpsClock fps;
    while (1) 
    {   
	fpsStart(&fps);
      // Get the next event
      SDL_Event event;
      emulateCycle(&cpu);
      if(cpu.drawFlag)
      {
        SDL_RenderClear(renderer);
        drawPixels(renderer, &cpu);
      	SDL_RenderPresent(renderer);
        cpu.drawFlag = 0;
      }
	  
	  setKeys(&cpu);

      if (SDL_PollEvent(&event))
      {   
        if (event.type == SDL_QUIT)
        {
          // Break out of the loop on quit
          break;
        }   
      }
	double milliElapsed = fpsStop(&fps);
	if(milliElapsed < MILLI_PER_FRAME)
	{
		SDL_Delay( MILLI_PER_FRAME - milliElapsed);
	}
}
	
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

