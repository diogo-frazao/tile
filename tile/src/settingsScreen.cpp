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

	OptionSelector* optionSelector = new OptionSelector(std::vector<const char*>(_resolutionOptions.begin(), _resolutionOptions.end()), {k_screenWidth / 2, 55}, 65, k_white);

	_widgetLink._resultsDelegate = std::bind(&SettingsScreen::onSettingsSaved, this, std::placeholders::_1);
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
	if (!s_active)
	{
		return;
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

void SettingsScreen::onSettingsSaved(const std::vector<int8_t>& results)
{
	bool areResultsValid = _widgetLink._rightWidgets.size();
	D_ASSERT(areResultsValid, "Invalid results");
	D_LOG(LOG, "Settings saved");

	int8_t resolutionIndex = results[1];
	bool isResolutionIndexValid = resolutionIndex >= 0 && resolutionIndex < _resolutionOptions.size();
	D_ASSERT(isResolutionIndexValid, "Invlid resolution index");
	switch (resolutionIndex)
	{
		case 0:
			SDL_SetWindowSize(s_window, 320, 180);
			break;
		case 1:
			SDL_SetWindowSize(s_window, 960, 540);
			break;
		case 2:
			SDL_SetWindowSize(s_window, 1280, 720);
			break;
		case 3:
			SDL_SetWindowSize(s_window, 1920, 1080);
			break;
		case 4:
			D_LOG(ERROR, "This is 1080 because my monitor doesn't support 1440p");
			SDL_SetWindowSize(s_window, 1920, 1080);
			break;
	}

	bool enableFullscreen = results[0];
	if (enableFullscreen)
	{
		SDL_SetWindowFullscreen(s_window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(s_window, 0);
	}

	s_active = false;
}
