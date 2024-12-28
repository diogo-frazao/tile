#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"
#include "core/log.h"
#include <vector>

void SettingsScreen::start()
{
	_uiTexture = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, s_uiWidth, s_uiHeight);
	SDL_SetTextureBlendMode(_uiTexture, SDL_BLENDMODE_BLEND);

	_title = Text("SETTINGS", { s_uiWidth / 2, 35 }, 100, k_gray);
	_subtitle = Text("enter to apply or esc to leave", { s_uiWidth / 2, 130 }, 70, k_gray);
	_checkbox = CheckBox(false, { s_uiWidth / 2, 200 }, 70, k_white);
	const std::vector<const char*> options = {
		"<  320x180  >",
		"<  640x320  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >" 
	};
	_optionSelector = OptionSelector(options, { s_uiWidth / 2, 300 }, 75, k_white);
}

void SettingsScreen::update()
{
	_title.update();
	_subtitle.update();
	_checkbox.update();

	if (wasKeyPressedThisFrame(SDL_SCANCODE_DOWN))
	{
		_checkbox.onSelected();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_RIGHT))
	{
		_optionSelector.onRightPressed();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_LEFT))
	{
		_optionSelector.onLeftPressed();
	}
}

void SettingsScreen::render()
{
	static SDL_FRect dest{};
	PanelScreen::s_isPanelActive = s_active;

	if (!s_active)
	{
		return;
	}

	_title.render(_uiTexture, dest);
	_subtitle.render(_uiTexture, dest);
	_checkbox.render(_uiTexture, dest);
	_optionSelector.render(_uiTexture, dest);
}

void SettingsScreen::destroy()
{
	destroyWidget(_title);
	destroyWidget(_subtitle);
	destroyWidget(_checkbox);
	destroyWidget(_optionSelector);
}