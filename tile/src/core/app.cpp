#include "app.h"
#include <SDL.h>
#include "log.h"
#include "spriteHandler.h"
#include "input.h"
#include <SDL_ttf.h>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

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

	s_window = SDL_CreateWindow("tile", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, k_screenWidth, k_screenHeight, SDL_WINDOW_SHOWN);

	if (s_window == nullptr)
	{
		D_ASSERT(false, "Failed to create window. Error: %s\n", SDL_GetError());
		return;
	}

	s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED);

	if (s_renderer == nullptr)
	{
		D_ASSERT(false, "Failed to create renderer. Error: %s\n", SDL_GetError());
		return;
	}

	SDL_RenderSetLogicalSize(s_renderer, k_screenWidth, k_screenHeight);
	SDL_SetRenderDrawBlendMode(s_renderer, SDL_BLENDMODE_BLEND);

	// Change window size after we set the render size to the original 320x180
	// Only the mouse input needs to be adjusted taking into account the current window size.
	// Everything else adapts to the window size as if the size was still 320x180
	SDL_SetWindowSize(s_window, 960, 540);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(s_window, s_renderer);
	ImGui_ImplSDLRenderer2_Init(s_renderer);
}

void App::start()
{
	_mainScreen.start();
	_settingsScreen.start();
	_addSpritesScreen.start();
	MouseScreen::instance().start();
}

void App::update()
{
	SDL_Event ev;
	uint64_t lastFrameTimeStamp = SDL_GetTicks64();
	bool showDemoWindow = true;
	while (true)
	{
		static constexpr uint8_t s_targetFrameRate = 60;
		static const uint8_t s_millisecondsBetweenFrames = static_cast<uint8_t>(std::floorf(1000.f / s_targetFrameRate));

		const uint64_t millisecondsSinceLastFrame = SDL_GetTicks64() - lastFrameTimeStamp;
		if (millisecondsSinceLastFrame >= s_millisecondsBetweenFrames)
		{
			lastFrameTimeStamp = SDL_GetTicks64();
			//D_LOG(MINI, "FPS: %i", 1000 / millisecondsSinceLastFrame);
			while (SDL_PollEvent(&ev))
			{
				ImGui_ImplSDL2_ProcessEvent(&ev);
				if (ev.type == SDL_QUIT)
				{
					return;
				}

				handleKeyboardInput(ev);
				handleMouseInput(ev);
			}

			ImGui_ImplSDLRenderer2_NewFrame();
    		ImGui_ImplSDL2_NewFrame();
    		ImGui::NewFrame();

			ImGui::ShowDemoWindow(&showDemoWindow);

			_settingsScreen.update();
			_addSpritesScreen.update();
			_mainScreen.update();
			MouseScreen::instance().update();

			render();
			resetKeyboardAndMouseInput();
		}
	}
}

void App::render()
{
	ImGui::Render();
	SDL_SetRenderDrawColor(s_renderer, 32, 33, 63, 1);
	SDL_RenderClear(s_renderer);

	_mainScreen.render();
	// Update panel between main screen and ui screens
	_panelScreen.render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), s_renderer);
	_settingsScreen.render();
	_addSpritesScreen.render();
	MouseScreen::instance().render();

	SDL_RenderPresent(s_renderer);
}

void App::killWindow()
{
	_mainScreen.destroy();
	_settingsScreen.destroy();
	_addSpritesScreen.destroy();

	ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	SDL_DestroyRenderer(s_renderer);
	SDL_DestroyWindow(s_window);
	s_window = nullptr;
	s_renderer = nullptr;
	TTF_Quit();
	SDL_Quit();
}