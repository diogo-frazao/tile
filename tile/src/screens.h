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

enum LayerType
{
	BACKGROUND,
	MIDDLEGROUND,
	FOREGROUND
};

class MainScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
	inline static std::array<SpritePreviewerButtonPair, 3> _spritePreviewerButtons;
	inline static Sprite _spriteInHand;
private:
	void handleAddSpritesToLayersDebug();
	void toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewer);
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