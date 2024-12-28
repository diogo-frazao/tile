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
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE))
	{
		SettingsScreen::s_active = !SettingsScreen::s_active;
	}
}

void MainScreen::render()
{
	static SDL_Rect src{};
	static SDL_FRect dest{};

	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };

	src.x = mockup.offset.x;
	src.y = mockup.offset.y;
	src.w = mockup.size.x;
	src.h = mockup.size.y;

	dest.x = mockup.position.x;
	dest.y = mockup.position.y;
	dest.w = (float)mockup.size.x;
	dest.h = (float)mockup.size.y;

	SDL_RenderCopyExF(s_renderer, s_atlasTexture, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
}