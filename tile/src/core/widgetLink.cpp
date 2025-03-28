#include "widgetLink.h"
#include "log.h"
#include <SDL.h>
#include "app.h"
#include "input.h"

void WidgetLink::setupRules(const Vec2& startingLeftPos, const float verticalDistance, const float horizontalDistance, 
	const DrawMode leftDrawMode, const DrawMode rightDrawMode)
{
	D_ASSERT(!_leftTexts.empty() && !_rightWidgets.empty(), "_leftTexts and/or _rightWidgets need to be populated before calling this function");

	// Setup left texts
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		if (i == 0)
		{
			_leftTexts[i]._worldPosition = startingLeftPos;
		}
		else
		{
			InteractableText& previousText = _leftTexts[i - 1];
			_leftTexts[i]._worldPosition.x = startingLeftPos.x;
			_leftTexts[i]._worldPosition.y = previousText._worldPosition.y + previousText._worldBounds.y + verticalDistance;
		}

		_leftTexts[i].setupDrawMode(leftDrawMode);
	}

	// Setup right widgets
	for (uint8_t i = 0; i < _rightWidgets.size(); ++i)
	{
		InteractableText* text = _rightWidgets[i];
		text->_worldPosition.x = startingLeftPos.x + _leftTexts[0]._worldBounds.x + horizontalDistance;

		if (i == 0)
		{
			text->_worldPosition.y = startingLeftPos.y;
		}
		else
		{
			InteractableText* previousText = _rightWidgets[i - 1];
			text->_worldPosition.y = previousText->_worldPosition.y + previousText->_worldBounds.y + verticalDistance;
		}

		_rightWidgets[i]->setupDrawMode(rightDrawMode);
	}

	// Add one big collider per row to highlight left and right links at once
	_highlightColliders.reserve(_leftTexts.size());
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		Vec2 size {calculateHighlightColliderSize(i)};
		RectCollider col(_leftTexts[i]._worldPosition, size);
		col.debugColor = k_red;
		_highlightColliders.emplace_back(std::move(col));
	}
}

void WidgetLink::update()
{
	for (InteractableText& text : _leftTexts)
	{
		text.update();
	}

	bool areConfirmResultsBlocked = false;
	for (InteractableText* text : _rightWidgets)
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
			case INPUT_WIDGET:
			{
				InputWidget* inputWidget = static_cast<InputWidget*>(text);
				// This needs to be done before tryEditText since it changes _isEditingText
				// We're essentially reading the last frame's state
				if(inputWidget->_isEditingText)
				{
					areConfirmResultsBlocked = true;
				}

				inputWidget->tryEditText();
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
			bool preventHover = false;
			if (_rightWidgets[i]->_widgetType == INPUT_WIDGET)
			{
				InputWidget* inputWidget = static_cast<InputWidget*>(_rightWidgets[i]);
				if (inputWidget->_isEditingText)
				{
					preventHover = true;
				}
			}

			if (!preventHover)
			{
				_leftTexts[i].onHovered(isHovering);
				_rightWidgets[i]->onHovered(isHovering);
			}
		}
	}

	if (wasKeyPressedThisFrame(k_confirmKey) && !areConfirmResultsBlocked)
	{
		if (_resultsDelegate != nullptr)
		{
			_resultsDelegate(getResults());
		}
	}

	if (!wasKeyPressedThisFrame(k_returnKey))
	{
		return;
	}

	// On return key pressed
	// Code below is only called on the same frame as return key is pressed

	if (_discardDelegate != nullptr)
	{
		_discardDelegate();
		return;
	}

	for (InteractableText* text : _rightWidgets)
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
				if (checkbox->_isSelected)
				{
					checkbox->onSelected();
				}
				break;
			}
			case OPTIONSELECTOR:
			{
				OptionSelector* optionSelector = static_cast<OptionSelector*>(text);
				if (optionSelector->_selectedIndex != 0)
				{
					optionSelector->selectOption(0);
				}
				break;
			}
		}
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

	for (InteractableText& text : _leftTexts)
	{
		text.render(externaluiTexture);
	}

	for (InteractableText* text : _rightWidgets)
	{
		text->render(externaluiTexture);
	}
}

void WidgetLink::destroy()
{
	for (uint8_t i = 0; i < _leftTexts.size(); ++i)
	{
		InteractableText& text = _leftTexts[i];
		destroyWidget(text);
	}

	for (uint8_t i = 0; i < _rightWidgets.size(); ++i)
	{
		InteractableText* text = _rightWidgets[i];
		destroyWidget(text);
	}
}

std::vector<int16_t> WidgetLink::getResults()
{
	std::vector<int16_t> results;
	results.reserve(_rightWidgets.size());

	for (InteractableText* text : _rightWidgets)
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
				results.emplace_back(checkbox->_isSelected);
				break;
			}
			case OPTIONSELECTOR:
			{
				OptionSelector* optionSelector = static_cast<OptionSelector*>(text);
				results.emplace_back(optionSelector->_selectedIndex);
				break;
			}
			case INPUT_WIDGET:
			{
				InputWidget* inputWidget = static_cast<InputWidget*>(text);				
				results.emplace_back(convertStringToInt(inputWidget->_currentText));
				break;
			}
			default:
			{
				D_ASSERT(false, "this type of widget is not supported by the widget link");
				break;
			}
		}
	}

	return results;
}

Vec2 WidgetLink::calculateHighlightColliderSize(uint16_t i)
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
