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

typedef std::pair<SpritePreviewer, Button> SpritePreviewerButtonPair;

struct SDL_Texture;

class TilePlayground
{
public:
	TilePlayground()
	{
		_placedSprites.reserve(50);
	}

	struct PlaceableSprite
	{
		Sprite sprite;
		LayerType layer;
	};

	inline Sprite& getSpriteInHand() { return _spriteInHand.sprite; }
	inline LayerType getSpriteInHandLayer() { return _spriteInHand.layer; }
	inline void setSpriteInHand(const Sprite& other) { _spriteInHand.sprite = other; }
	inline void setSpriteInHandLayer(LayerType layer) { _spriteInHand.layer = layer; }

	inline void addSpriteInHandToRnder()
	{
		if (_placedSprites.size() == 0)
		{
			_placedSprites.emplace_back(_spriteInHand);
			_spriteInHand.sprite.invalidate();
			return;
		}

		uint16_t indexToPlaceSpriteInHand = -1;
		for (uint16_t i = 0; i < _placedSprites.size(); ++i)
		{
			if (_placedSprites[i].layer <= _spriteInHand.layer)
			{
				indexToPlaceSpriteInHand++;
				continue;
			}
			else
			{
				_placedSprites.insert(_placedSprites.begin() + indexToPlaceSpriteInHand, _spriteInHand);
				_spriteInHand.sprite.invalidate();
				return;
			}
		}

		if (indexToPlaceSpriteInHand == (_placedSprites.size() - 1))
		{
			_placedSprites.emplace_back(_spriteInHand);
			_spriteInHand.sprite.invalidate();
			return;
		}
	}

	PlaceableSprite _spriteInHand;
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