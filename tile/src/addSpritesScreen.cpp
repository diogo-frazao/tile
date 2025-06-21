#include "screens.h"
#include <SDL.h>
#include "core/app.h"
#include "core/log.h"

void AddSpritesScreen::createSpriteFromResults(const std::vector<int16_t>& results)
{
	int16_t layerToAddSprite = results[0];
	int16_t startX = results[2];
	int16_t startY = results[3];
	int16_t endX = results[4];
	int16_t endY = results[5];
	D_ASSERT((endX >= startX) && (endY >= startY), "Trying to add invalid sprite. Size is wrong");

	// Ignore empty sprites results
	if (startX == 0 && startY == 0 && endX == 0 && endY ==0)
	{
		return;
	}

	Sprite sprite{{endX - startX, endY - startY}, {startX, startY}};
	MainScreen::s_spritePreviewerButtons[layerToAddSprite].first._spritesToPreview.emplace_back(std::move(sprite));

	D_LOG(MINI, "Added sprite with offset %i,%i size %i,%i for layer %i", 
		sprite.offset.x, sprite.offset.y, sprite.size.x, sprite.size.y, layerToAddSprite);

	s_active = false;
	PanelScreen::s_isPanelActive = false;
	MainScreen::s_active = true;
}

void AddSpritesScreen::start()
{
	_uiTexture = createUITexture();

	_title = InteractableText("ADD IMAGE", { k_screenWidth / 2, 5 }, 100, k_gray, CENTER);
	_subtitle = InteractableText("enter to add or esc to leave", { k_screenWidth / 2, 25 }, 60, k_gray, CENTER);

	_widgetLink._resultsDelegate = std::bind(&AddSpritesScreen::createSpriteFromResults, this, std::placeholders::_1);
	_widgetLink._leftTexts.reserve(9);
	_widgetLink._rightWidgets.reserve(9);

	_widgetLink._leftTexts.emplace_back(InteractableText("layer", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("pivot", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("start x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("start y", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("end x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("end y", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("isTile", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("tile size x", { 0,0 }, 65, k_white));
	_widgetLink._leftTexts.emplace_back(InteractableText("tile size y", { 0,0 }, 65, k_white));

	std::array<const char*, 3> layerOptions = { "< 0 >", "< 1 >", "< 2 >" };
	std::array<const char*, 3> pivotOptions = { "< top >", "< center >", "< bottom >" };
	_widgetLink._rightWidgets.emplace_back(new OptionSelector(std::vector<const char*>(layerOptions.begin(), layerOptions.end()), {0,0}, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new OptionSelector(std::vector<const char*>(pivotOptions.begin(), pivotOptions.end()), {0,0}, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new CheckBox(false, { 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));
	_widgetLink._rightWidgets.emplace_back(new InputWidget({ 0,0 }, 65, k_white));

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