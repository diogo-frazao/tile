#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"
#include "core/log.h"
#include <vector>

void SettingsScreen::start()
{
	title = createText("SETTINGS", { s_targetWidth / 2, 35 }, 100, s_gray);
	subtitle = createText("enter to apply or esc to leave", { s_targetWidth / 2, 130 }, 70, s_gray);
	testCheckBox = createCheckbox(false, { s_targetWidth / 2, 200 }, 50, s_white);
	const std::vector<const char*> options = {
		"<  320x180  >",
		"<  640x320  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >"
	};
	testOptionSelector = createOptionSelector(options, { s_targetWidth / 2, 300 }, 75, s_white);
}

void SettingsScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_DOWN))
	{
		title.onHovered();
		testCheckBox.onSelected();
		testCheckBox.onHovered();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_RIGHT))
	{
		testOptionSelector.onRightPressed();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_LEFT))
	{
		testOptionSelector.onLeftPressed();
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

	title.render(uiTexture, dest);
	subtitle.render(uiTexture, dest);
	testCheckBox.render(uiTexture, dest);
	testOptionSelector.render(uiTexture, dest);
}

void SettingsScreen::destroy()
{
	//title.destroy();
	//subtitle.destroy();
	//testCheckBox.destroy();
	//testOptionSelector.destroy();
}