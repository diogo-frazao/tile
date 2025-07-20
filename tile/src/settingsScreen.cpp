#include "screens.h"
#include "core/ui.h"
#include <SDL.h>
#include "core/app.h"
#include "core/input.h"
#include "core/log.h"
#include <vector>
#include <array>
#include "userDefaults.h"
#include <imgui.h>

void SettingsScreen::start()
{
	_uiTexture = createUITexture();

	_title = InteractableText("SETTINGS", { k_screenWidth / 2, 5 }, 100, k_gray, CENTER);
	_subtitle = InteractableText("enter to apply or esc to leave", { k_screenWidth / 2, 25 }, 60, k_gray, CENTER);

	OptionSelector* optionSelector = new OptionSelector(std::vector<const char*>(user_defaults::_resolutions.begin(), 
		user_defaults::_resolutions.end()), {k_screenWidth / 2, 55}, 65, k_white, LEFT, true, 1);

	_widgetLink._resultsDelegate = std::bind(&SettingsScreen::onSettingsSaved, this, std::placeholders::_1);
	_widgetLink._discardDelegate = std::bind(&SettingsScreen::onSettingsDiscarded, this);

	_widgetLink._leftTexts.reserve(2);
	_widgetLink._rightWidgets.reserve(2);

	_widgetLink._leftTexts.emplace_back(InteractableText("fullscreen", {0,0}, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("resolution", {0,0}, 65, k_white));

	_widgetLink._rightWidgets.emplace_back(new CheckBox(false, { 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(optionSelector);

	_widgetLink.setupRules(Vec2(80, 45), 7, 90, LEFT, CENTER);
}

void SettingsScreen::update()
{
	if (!s_active)
	{
		return;
	}

	_widgetLink.update();
}

void SettingsScreen::render()
{
	if (!s_active)
	{
		return;
	}

	_widgetLink.render(_uiTexture);
	_title.render(_uiTexture);
	_subtitle.render(_uiTexture);
}

void SettingsScreen::destroy()
{
	destroyWidget(_title);
	destroyWidget(_subtitle);
	_widgetLink.destroy();
	SDL_DestroyTexture(_uiTexture);
}

void SettingsScreen::onSettingsDiscarded()
{
	CheckBox* checkbox = static_cast<CheckBox*>(_widgetLink._rightWidgets[0]);
	if (checkbox->_isSelected != user_defaults::_currentSettings.isFullscreen)
	{
		checkbox->onSelected();
	}

	OptionSelector* resolutionSelector = static_cast<OptionSelector*>(_widgetLink._rightWidgets[1]);
	if (resolutionSelector->_selectedIndex != user_defaults::_currentSettings.resolutionIndex)
	{
		resolutionSelector->selectOption(user_defaults::_currentSettings.resolutionIndex);
	}
}

static void setImGuiDisplaySizeToWindowSize()
{
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(s_window, &windowWidth, &windowHeight);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
}

void SettingsScreen::onSettingsSaved(const std::vector<int16_t>& results)
{
	D_ASSERT((_widgetLink._rightWidgets.size() == results.size()), "Invalid results");
	D_LOG(LOG, "Settings saved");

	int8_t resolutionIndex = results[1];
	D_ASSERT((resolutionIndex >= 0 && resolutionIndex < static_cast<int8_t>(user_defaults::_resolutions.size())), "Invlid resolution index");
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

	setImGuiDisplaySizeToWindowSize();

	user_defaults::applySettings(user_defaults::Settings(resolutionIndex, enableFullscreen));
	s_active = false;
	PanelScreen::s_isPanelActive = false;
	MainScreen::s_active = true;
}
