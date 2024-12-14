#include "app.h"
#include <SDL.h>
#include "log.h"
#include "spriteHandler.h"
#include "input.h"
#include <SDL_ttf.h>

void App::runApp()
{
	initWindow();
	textures::loadAtlas();
	start();
	update();
	killWindow();
}

void App::initWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		D_ASSERT(false, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	if (TTF_Init() < 0)
	{
		D_ASSERT(false, "Failed to init TTF text lib. Error: &s\n", TTF_GetError());
	}

	s_window = SDL_CreateWindow("tile", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s_screenWidth, s_screenHeight, SDL_WINDOW_SHOWN);

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

	// Change window size after we set the render size to the original 320x180
	// Only the mouse input needs to be adjusted taking into account the current window size.
	// Everything else adapts to the window size as if the size was still 320x180
	SDL_SetWindowSize(s_window, 960, 540);
}

void App::start()
{
	_mainScreen.start();
}

void App::update()
{
	SDL_Event ev;
	while (true)
	{
		SDL_PollEvent(&ev);
		if (ev.type == SDL_QUIT)
		{
			return;
		}
		
		handleKeyboardInput(ev);
		handleMouseInput(ev);

		_mainScreen.update();

		resetKeyboardAndMouseInput();
		render();
	}
}

void App::render()
{
	SDL_SetRenderDrawColor(s_renderer, 32, 33, 63, 1);
	SDL_RenderClear(s_renderer);

	_mainScreen.render();

	/*// Render panel in front of main screen but behind everything else
	SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(s_renderer, nullptr);
	SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 1);*/

	SDL_RenderPresent(s_renderer);
}

void App::killWindow()
{
	SDL_DestroyRenderer(s_renderer);
	SDL_DestroyWindow(s_window);
	s_window = nullptr;
	s_renderer = nullptr;
	TTF_Quit();
	SDL_Quit();
}