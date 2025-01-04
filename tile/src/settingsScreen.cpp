#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"
#include "core/log.h"
#include <vector>
#include <array>

void SettingsScreen::start()
{
	_uiTexture = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, k_uiResolutionWidth, k_uiResolutionHeight);
	SDL_SetTextureBlendMode(_uiTexture, SDL_BLENDMODE_BLEND);

	_title = Text("SETTINGS", { k_screenWidth / 2, 5 }, 100, k_gray, CENTER);
	_subtitle = Text("enter to apply or esc to leave", { k_screenWidth / 2, 25 }, 60, k_gray, CENTER);

	const std::array<const char*, 5> options {
		"<  320x180  >",
		"<  640x320  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >" 
	};
	OptionSelector* optionSelector = new OptionSelector(std::vector<const char*>(options.begin(), options.end()), {k_screenWidth / 2, 55}, 65, k_white);

	_widgetLink._leftTexts.reserve(2);
	_widgetLink._rightWidgets.reserve(2);

	_widgetLink._leftTexts.push_back(Text("fullscreen", {0,0}, 65, k_white));
	_widgetLink._leftTexts.push_back(Text("resolution", {0,0}, 65, k_white));

	_widgetLink._rightWidgets.push_back(new CheckBox(false, { 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(optionSelector);

	_widgetLink.setupRules(Vec2(80, 45), 7, 90, LEFT, CENTER);
}

void SettingsScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_X))
	{
		SDL_SetWindowSize(s_window, 1280, 720);
	}

	_widgetLink.update();

	_title.update();
	_subtitle.update();
}

void SettingsScreen::render()
{
	static SDL_FRect dest{};
	PanelScreen::s_isPanelActive = s_active;

	if (!s_active)
	{
		return;
	}

	_widgetLink.render(_uiTexture);
	_title.render(_uiTexture, dest);
	_subtitle.render(_uiTexture, dest);
}

void SettingsScreen::destroy()
{
	destroyWidget(_title);
	destroyWidget(_subtitle);
	_widgetLink.destroy();
	SDL_DestroyTexture(_uiTexture);
}