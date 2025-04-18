#pragma once
#include "ui.h"
#include "lib.h"
#include <vector>
#include <functional>

struct SDL_Texture;

class WidgetLink
{
public:
	WidgetLink() = default;

	/// <summary>
	/// Must be called after _leftTexts and _rightWidgets are populated
	/// </summary>
	/// <param name="startingLeftPos"> Top left text in _leftTexts (index 0) will be positioned here </param>
	/// <param name="verticalDistance"> Vertical distance for different lines when we draw the next index (imagine two lines) </param>
	/// <param name="horizontalDistance"> Horizontal distance on the same line between _leftTexts and _rightWidgets for the current index </param>
	void setupRules(const Vec2& startingLeftPos, const float verticalDistance, const float horizontalDistance, 
		const DrawMode leftDrawMode, const DrawMode rightDrawMode);

	void update();
	void render(SDL_Texture* externaluiTexture);
	void destroy();

	std::vector<int16_t> getResults();

	std::vector<InteractableText> _leftTexts;
	std::vector<InteractableText*> _rightWidgets;
	std::vector<RectCollider> _highlightColliders;

	std::function<void(const std::vector<int16_t>&)> _resultsDelegate;
	std::function<void()> _discardDelegate;

private:
	Vec2 calculateHighlightColliderSize(uint16_t i);
};