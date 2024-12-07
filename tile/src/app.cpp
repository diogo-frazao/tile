#include "app.h"
#include <SDL.h>
#include "log.h"

void runApp()
{
	_initWindow();
	_update();
	_killWindow();
}

void _initWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		D_ASSERT(false, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	s_window = SDL_CreateWindow("tile", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);

	if (s_window == nullptr)
	{
		D_ASSERT(false, "Failed to create window. Error: %s\n", SDL_GetError());
		return;
	}

	s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (s_renderer == nullptr)
	{
		D_ASSERT(false, "Failed to create renderer. Error: %s\n", SDL_GetError());
		return;
	}

	SDL_RenderSetLogicalSize(s_renderer, s_screenWidth, s_screenHeight);
	SDL_SetRenderDrawBlendMode(s_renderer, SDL_BLENDMODE_BLEND);
}

void _update()
{
	SDL_Event ev;
	while (true)
	{
		SDL_PollEvent(&ev);
		if (ev.type == SDL_QUIT)
		{
			return;
		}

		_render();
	}
}

void _render()
{
	SDL_SetRenderDrawColor(s_renderer, 32, 33, 63, 1);
	SDL_RenderClear(s_renderer);

	SDL_RenderPresent(s_renderer);
}

void _killWindow()
{
	SDL_DestroyRenderer(s_renderer);
	SDL_DestroyWindow(s_window);
	s_window = nullptr;
	s_renderer = nullptr;
	SDL_Quit();
}