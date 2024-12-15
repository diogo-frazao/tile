#include "screens.h"
#include "core/spriteHandler.h"
#include <SDL.h>
#include "core/app.h"
#include "core/lib.h"
#include "core/input.h"
#include "core/log.h"
#include "core/ui.h"

void MainScreen::start()
{

}

void MainScreen::update()
{

}

void MainScreen::render()
{
	static SDL_Rect src{};
	static SDL_FRect dest{};

	static Sprite mainTexture = textures::getSprite(MAIN);
	mainTexture.position = { 0,0 };

	src.x = mainTexture.offset.x;
	src.y = mainTexture.offset.y;
	src.w = mainTexture.size.x;
	src.h = mainTexture.size.y;

	dest.x = mainTexture.position.x;
	dest.y = mainTexture.position.y;
	dest.w = (float)mainTexture.size.x;
	dest.h = (float)mainTexture.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);

	static SDL_Texture* nativeTextRenderTarget = SDL_CreateTexture(
		s_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		1920, // Replace with your window's width
		1080 // Replace with your window's height
	);
	SDL_SetTextureBlendMode(nativeTextRenderTarget, SDL_BLENDMODE_BLEND);

	static SDL_Texture* secondTexture = SDL_CreateTexture(
		s_renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		1920, // Replace with your window's width
		1080 // Replace with your window's height
	);
	SDL_SetTextureBlendMode(secondTexture, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(s_renderer, nativeTextRenderTarget);

	using namespace ui;
	static Text text = createText("the quick brown fox", { 50, 50 }, 80, { 255, 255, 255 });

	dest.x = text.position.x;
	dest.y = text.position.y;
	dest.w = text.bounds.x;
	dest.h = text.bounds.y;
	SDL_RenderCopyExF(s_renderer, text.texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
	SDL_SetRenderTarget(s_renderer, nullptr);
	SDL_RenderCopy(s_renderer, nativeTextRenderTarget, nullptr, nullptr);

	if (true)
	{
		// Render panel in front of main screen but behind everything else
		SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(s_renderer, nullptr);
		SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 1);

		SDL_SetRenderTarget(s_renderer, secondTexture);

		using namespace ui;
		static Text text = createText("SETTINGS", { 960, 80 }, 150, { 255, 255, 255 });

		dest.x = text.position.x;
		dest.y = text.position.y;
		dest.w = text.bounds.x;
		dest.h = text.bounds.y;
		SDL_RenderCopyExF(s_renderer, text.texture, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
		SDL_SetRenderTarget(s_renderer, nullptr);
		SDL_RenderCopy(s_renderer, secondTexture, nullptr, nullptr);
	}
}