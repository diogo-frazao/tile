#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include "core/ui.h"
#include "core/widgetLink.h"
#include <vector>
#include <stdint.h>
#include <array>
#include "spritePreviewer.h"
#include <utility>
#include <optional>

typedef std::pair<SpritePreviewer, Button> SpritePreviewerButtonPair;

struct SDL_Texture;

class TilePlayground
{
public:
	TilePlayground()
	{
		_placedSprites.reserve(50);
		_spriteInHandIndex = -1;
	}

	struct PlaceableSprite
	{
		Sprite sprite;
		LayerType layer;

		PlaceableSprite(const Sprite& sprite, const LayerType layer) : sprite(sprite), layer(layer) {};
	};

	inline Sprite& getSpriteInHand()
	{
		D_ASSERT((_spriteInHandIndex > -1), "Can't access invalid sprite in hand");
		return _placedSprites[_spriteInHandIndex].sprite;
	}

	inline bool hasSpriteInHand()
	{
		return _spriteInHandIndex > -1;
	}

	inline void clearSpriteInHand()
	{
		_spriteInHandIndex = -1;
	}

	inline void replaceSpriteInHand(const PlaceableSprite& sprite)
	{
		D_ASSERT((_spriteInHandIndex > -1), "Can't access invalid sprite in hand");

		if (sprite.layer == _placedSprites[_spriteInHandIndex].layer)
		{
			_placedSprites[_spriteInHandIndex] = sprite;
		}
		else
		{
			// If the layers are different, we have to delete the old item and create a new one
			// Since the rendering order will change
			_placedSprites.erase(_placedSprites.begin() + _spriteInHandIndex);
			clearSpriteInHand();
			addSpriteToRender(sprite);
		}
		
	}

	inline void addSpriteToRender(const PlaceableSprite& placeableSprite)
	{
		if (hasSpriteInHand())
		{
			return;
		}

		if (_placedSprites.size() == 0)
		{
			_placedSprites.emplace_back(placeableSprite);
			_spriteInHandIndex = 0;
			return;
		}

		int16_t indexToPlaceSprite = -1;
		for (uint16_t i = 0; i < _placedSprites.size(); ++i)
		{
			if (_placedSprites[i].layer <= placeableSprite.layer)
			{
				indexToPlaceSprite++;
				continue;
			}
			else
			{
				indexToPlaceSprite = (indexToPlaceSprite == -1) ? 0 : indexToPlaceSprite;
				_placedSprites.insert(_placedSprites.begin() + indexToPlaceSprite, placeableSprite);
				_spriteInHandIndex = indexToPlaceSprite;
				return;
			}
		}

		if (indexToPlaceSprite == (_placedSprites.size() - 1))
		{
			_placedSprites.emplace_back(placeableSprite);
			_spriteInHandIndex = _placedSprites.size() - 1;
			return;
		}
	}

	int16_t _spriteInHandIndex;
	std::vector<PlaceableSprite> _placedSprites;
};

class MainScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
	inline static std::array<SpritePreviewerButtonPair, 3> s_spritePreviewerButtons;
	inline static TilePlayground s_tilePlayground;
private:
	void handleAddSpritesToLayersDebug();
	void toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewer);
	std::optional<TilePlayground::PlaceableSprite> shouldReplaceSpriteInHand();
	bool shouldReleaseSpriteInHand();
	SDL_Texture* _uiTexture;
	Button _addSpriteButton;
};

class SettingsScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
	void onSettingsSaved(const std::vector<int16_t>& results);
	void onSettingsDiscarded();
private:
	SDL_Texture* _uiTexture;
	InteractableText _title;
	InteractableText _subtitle;
	WidgetLink _widgetLink;
};

class AddSpritesScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
private:
	void createSpriteFromResults(const std::vector<int16_t>& results);
	SDL_Texture* _uiTexture;
	InteractableText _title;
	InteractableText _subtitle;
	WidgetLink _widgetLink;
};

class PanelScreen
{
public:
	void render();
	inline static bool s_isPanelActive = false;
};

class MouseScreen
{
public:
	enum class MouseSpriteState
	{
		INVALID,
		NORMAL,
		DRAGGING
	};

	static MouseScreen& instance()
	{
		static MouseScreen mouseScreen;
		return mouseScreen;
	}

	void start();
	void update();
	void render();
	void setMouseState(MouseSpriteState state);
private:
	MouseSpriteState _mouseSpriteState = MouseSpriteState::INVALID;
	Sprite _mouseSprite;
	IVec2 _lastMousePosition;
};