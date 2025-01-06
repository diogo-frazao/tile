#include "widgetLink.h"
#include "log.h"
#include <SDL.h>
#include "app.h"
#include "input.h"

void WidgetLink::setupRules(const Vec2& startingLeftPos, const float verticalDistance, const float horizontalDistance, 
	const DrawMode leftDrawMode, const DrawMode rightDrawMode)
{
	D_ASSERT(!_leftTexts.empty() && !_rightWidgets.empty(), "_leftTexts and/or _rightWidgets need to be populated before calling this function");

	if (_uiTexture == nullptr)
	{
		_uiTexture = SDL_CreateTexture(s_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, k_uiResolutionWidth, k_uiResolutionHeight);
		SDL_SetTextureBlendMode(_uiTexture, SDL_BLENDMODE_BLEND);
	}

	// Setup left texts
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		if (i == 0)
		{
			_leftTexts[i]._worldPosition = startingLeftPos;
		}
		else
		{
			Text& previousText = _leftTexts[i - 1];
			_leftTexts[i]._worldPosition.x = startingLeftPos.x;
			_leftTexts[i]._worldPosition.y = previousText._worldPosition.y + previousText._worldBounds.y + verticalDistance;
		}

		_leftTexts[i].setupDrawMode(leftDrawMode);
	}

	// Setup right widgets
	for (uint8_t i = 0; i < _rightWidgets.size(); ++i)
	{
		Text* text = _rightWidgets[i];
		text->_worldPosition.x = startingLeftPos.x + _leftTexts[0]._worldBounds.x + horizontalDistance;

		if (i == 0)
		{
			text->_worldPosition.y = startingLeftPos.y;
		}
		else
		{
			Text* previousText = _rightWidgets[i - 1];
			text->_worldPosition.y = previousText->_worldPosition.y + previousText->_worldBounds.y + verticalDistance;
		}

		_rightWidgets[i]->setupDrawMode(rightDrawMode);
	}

	// Add one big collider per row to highlight left and right links at once
	_highlightColliders.reserve(_leftTexts.size());
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		Vec2 size = calculateHighlightColliderSize(i);
		RectCollider col(_leftTexts[i]._worldPosition, size);
		col.debugColor = k_red;
		_highlightColliders.push_back(col);
	}
}

void WidgetLink::update()
{
	for (Text& text : _leftTexts)
	{
		text.update();
	}

	for (Text* text : _rightWidgets)
	{
		text->update();
		switch (text->_widgetType)
		{
			case TEXT:
			{
				break;
			}
			case CHECKBOX:
			{
				CheckBox* checkbox = static_cast<CheckBox*>(text);
				checkbox->trySelect();
				break;
			}
			case OPTIONSELECTOR:
			{
				OptionSelector* optionSelector = static_cast<OptionSelector*>(text);
				optionSelector->trySwapOption();
				break;
			}
		}
	}

	for (uint8_t i = 0; i < _highlightColliders.size(); ++i)
	{
		_highlightColliders[i].size = calculateHighlightColliderSize(i);

		IVec2 mousePos = getMousePosition();
		bool isHovering = pointInRect(mousePos, _highlightColliders[i]);
		bool isRowHighlighted = _leftTexts[i]._isHovered && _rightWidgets[i]->_isHovered;
		if (isHovering != isRowHighlighted)
		{
			_leftTexts[i].onHovered(isHovering);
			_rightWidgets[i]->onHovered(isHovering);
		}
	}

	if (wasKeyPressedThisFrame(k_confirmKey))
	{
		auto results = getResults();
	}

	if (wasKeyPressedThisFrame(k_returnKey))
	{
		D_LOG(LOG, "Discard settings");
	}
}

void WidgetLink::render(SDL_Texture* externaluiTexture)
{
	if (s_debugCollidersEnabled)
	{
		for (RectCollider& collider : _highlightColliders)
		{
			SDL_SetRenderDrawColor(s_renderer, collider.debugColor.r, collider.debugColor.g, collider.debugColor.b, collider.debugColor.a);
			SDL_FRect debugRect{ collider.centerPoint.x, collider.centerPoint.y, collider.size.x, collider.size.y };
			SDL_RenderDrawRectF(s_renderer, &debugRect);
			SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 1);
		}
	}

	static SDL_FRect dest;
	SDL_Texture* targetUITexture = externaluiTexture == nullptr ? _uiTexture : externaluiTexture;

	for (Text& text : _leftTexts)
	{
		text.render(targetUITexture, dest);
	}

	for (Text* text : _rightWidgets)
	{
		text->render(targetUITexture, dest);
	}
}

void WidgetLink::destroy()
{
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		Text& text = _leftTexts[i];
		destroyWidget(text);
	}

	for (uint8_t i = 0; i < _rightWidgets.size(); ++i)
	{
		Text* text = _rightWidgets[i];
		destroyWidget(text);
	}

	SDL_DestroyTexture(_uiTexture);
}

std::vector<int8_t> WidgetLink::getResults()
{
	std::vector<int8_t> results;
	results.reserve(_rightWidgets.size());

	for (Text* text : _rightWidgets)
	{
		switch (text->_widgetType)
		{
			case TEXT:
			{
				D_ASSERT(false, "We don't support right side widgets as text");
				break;
			}
			case CHECKBOX:
			{
				CheckBox* checkbox = static_cast<CheckBox*>(text);
				int8_t isSelected = checkbox->_isSelected;
				results.push_back(isSelected);
				break;
			}
			case OPTIONSELECTOR:
			{
				OptionSelector* optionSelector = static_cast<OptionSelector*>(text);
				int8_t selectedIndex = optionSelector->_selectedIndex;
				results.push_back(selectedIndex);
				break;
			}
		}
	}

	return results;
}

Vec2 WidgetLink::calculateHighlightColliderSize(uint8_t i)
{
	D_ASSERT(_rightWidgets[i], "Right widget doesn't exist at index: %i", i);
	Vec2 rightWidgetFurthestPoint;
	rightWidgetFurthestPoint.x = _rightWidgets[i]->_worldPosition.x + _rightWidgets[i]->_worldBounds.x;
	rightWidgetFurthestPoint.y = _rightWidgets[i]->_worldPosition.y;

	Vec2 size;
	size.x = rightWidgetFurthestPoint.x - _leftTexts[i]._worldPosition.x + 1;
	size.y = max(_leftTexts[i]._worldBounds.y, _rightWidgets[i]->_worldBounds.y);
	return size;
}
