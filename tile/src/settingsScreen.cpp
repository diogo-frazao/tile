#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"

void SettingsScreen::start()
{
	title = createText("SETTINGS", { s_targetWidth / 2, 35 }, 100, { 137, 128, 128 });
	subtitle = createText("enter to apply or esc to leave", { s_targetWidth / 2, 130 }, 70, { 137, 128, 128 });
}

void SettingsScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_DOWN))
	{
		onUISelected(title);
	}
}

void SettingsScreen::render()
{
	PanelScreen::isPanelActive = active;
	if (!active)
	{
		return;
	}

	static SDL_FRect dest{};
	static SDL_Texture* uiTexture = SDL_CreateTexture(
		s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, s_targetWidth, s_targetHeight);
	SDL_SetTextureBlendMode(uiTexture, SDL_BLENDMODE_BLEND);

	renderText(uiTexture, dest, title);
	renderText(uiTexture, dest, subtitle);
}

void SettingsScreen::destroy()
{
	destroyText(title);
	destroyText(subtitle);
}