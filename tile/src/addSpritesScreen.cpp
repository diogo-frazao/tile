#include "screens.h"
#include <SDL.h>
#include "core/app.h"
#include "core/log.h"

void createSpriteFromResults(const std::vector<int8_t>& results)
{
	uint8_t layerToAddSprite = results[0];
	Sprite sign{{10,16}, {328, 9}};
	MainScreen::_spritePreviewers[layerToAddSprite]._spritesToPreview.emplace_back(std::move(sign));
}

void AddSpritesScreen::start()
{
	_uiTexture = createUITexture();

	_title = InteractableText("ADD IMAGE", { k_screenWidth / 2, 5 }, 100, k_gray, CENTER);
	_subtitle = InteractableText("enter to add or esc to leave", { k_screenWidth / 2, 25 }, 60, k_gray, CENTER);

	_widgetLink._resultsDelegate = createSpriteFromResults;
	_widgetLink._leftTexts.reserve(9);
	_widgetLink._rightWidgets.reserve(9);

	_widgetLink._leftTexts.push_back(InteractableText("layer", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("pivot", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("start x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("start y", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("end x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("end y", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("isTile", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("tile size x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.push_back(InteractableText("tile size y", { 0,0 }, 65, k_white));

	std::array<const char*, 3> layerOptions = { "< 0 >", "< 1 >", "< 2 >" };
	std::array<const char*, 3> pivotOptions = { "< top >", "< center >", "< bottom >" };
	_widgetLink._rightWidgets.push_back(new OptionSelector(std::vector<const char*>(layerOptions.begin(), layerOptions.end()), {0,0}, 65, k_white));
	_widgetLink._rightWidgets.push_back(new OptionSelector(std::vector<const char*>(pivotOptions.begin(), pivotOptions.end()), {0,0}, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new CheckBox(false, { 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.push_back(new InputWidget({ 0,0 }, 65, k_white));

	_widgetLink.setupRules(Vec2(100, 45), 1, 90, LEFT, CENTER);
}

void AddSpritesScreen::update()
{
	if (!s_active)
	{
		return;
	}

	_widgetLink.update();
}

void AddSpritesScreen::render()
{
	if (!s_active)
	{
		return;
	}

	_title.render(_uiTexture);
	_subtitle.render(_uiTexture);
	_widgetLink.render(_uiTexture);
}

void AddSpritesScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}