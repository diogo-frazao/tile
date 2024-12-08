#include "app.h"
#include <SDL.h>
#include "log.h"
#include "spriteHandler.h"

void runApp()
{
	_initWindow();
	textures::loadAtlas();
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

	SDL_SetWindowSize(s_window, 960, 540);
	SDL_ShowCursor(SDL_DISABLE);
}

void _update()
{
	SDL_Event ev;

	int32_t lastMouseX = 0;
	int32_t lastMouseY = 0;

	Sprite mouseSprite = textures::getSprite(CURSOR);

	while (true)
	{
		SDL_PollEvent(&ev);
		if (ev.type == SDL_QUIT)
		{
			return;
		}

		if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_1)
		{
			SDL_SetWindowSize(s_window, 1280, 720);
		}

		int32_t w = 0;

		SDL_GetWindowSize(s_window, &w, nullptr);
		uint8_t screenfactor = w / s_screenWidth;

		int32_t mx = 0;
		int32_t my = 0;
		SDL_GetMouseState(&mx, &my);

		int32_t mouseX = mx / screenfactor;
		int32_t mouseY = my / screenfactor;

		mouseSprite.position.x = lerp(lastMouseX, mouseX, 0.6f);
		mouseSprite.position.y = lerp(lastMouseY, mouseY, 0.6f);

		lastMouseX = mouseX;
		lastMouseY = mouseY;

		D_LOG(MINI, "Mouse X: %f Y: %f", mouseSprite.position.x, mouseSprite.position.y);

		SDL_SetRenderDrawColor(s_renderer, 32, 33, 63, 1);
		SDL_RenderClear(s_renderer);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = mouseSprite.size.x;
		src.h = mouseSprite.size.y;

		SDL_FRect dest;
		dest.x = mouseSprite.position.x;
		dest.y = mouseSprite.position.y;
		dest.w = (float)mouseSprite.size.x;
		dest.h = (float)mouseSprite.size.y;

		SDL_RenderCopyExF(s_renderer, s_atlasTexture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);

		SDL_RenderPresent(s_renderer);

		//_render();
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