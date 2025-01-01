#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"
#include "core/log.h"
#include <vector>

void SettingsScreen::start()
{
	_uiTexture = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, k_uiResolutionWidth, k_uiResolutionHeight);
	SDL_SetTextureBlendMode(_uiTexture, SDL_BLENDMODE_BLEND);

	_title = Text("SETTINGS", { k_screenWidth / 2, 5 }, 100, k_gray);
	_subtitle = Text("enter to apply or esc to leave", { k_screenWidth / 2, 25 }, 60, k_gray);
	_checkbox = CheckBox(false, { k_screenWidth / 2, 40 }, 70, k_white);
	const std::vector<const char*> options = {
		"<  320x180  >",
		"<  640x320  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >" 
	};
	_optionSelector = OptionSelector(options, { k_screenWidth / 2, 55 }, 75, k_white);
}

void SettingsScreen::update()
{
	_title.update();
	_subtitle.update();
	_checkbox.update();
	_optionSelector.update();

	_checkbox.tryHover();
	_checkbox.trySelect();

	_optionSelector.tryHover();
	_optionSelector.trySwapOption();
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