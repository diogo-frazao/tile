#pragma once
#include "core/spriteHandler.h"
#include "core/lib.h"
#include "core/ui.h"
#include "core/widgetLink.h"
#include <vector>
#include <stdint.h>
#include <array>
#include "spritePreviewer.h"

struct SDL_Texture;

class MainScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	inline static bool s_active = false;
	inline static std::array<SpritePreviewer, 3> _spritePreviewers;
private:
	void toggleSpritePreviewerAtIndex(uint8_t spritePreviewerIndex);
	SDL_Texture* _uiTexture;
	Button _addSpriteButton;
	Button _backgroundButton;
	Button _middlegroundButton;
	Button _foregroundButton;
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
	void start();
	void update();
	void render();
private:
	Sprite _mouseSprite;
	IVec2 _lastMousePosition;
};