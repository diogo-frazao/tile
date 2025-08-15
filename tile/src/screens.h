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
#include "tilePlayground.h"

typedef std::pair<SpritePreviewer, Button> SpritePreviewerButtonPair;

struct SDL_Texture;

class MainScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	bool _isActive = false;
	std::array<SpritePreviewerButtonPair, 3> _spritePreviewerButtons;
	TilePlayground _tilePlayground;
private:
	void handleAddSpritesToLayersDebug();
	void toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewer);

	void handleSpriteInHand();
	void handleUndoAndRedoPlacedSprites();

	std::optional<TilePlayground::PlaceableSprite> shouldReplaceSpriteInHand();
	bool shouldReleaseSpriteInHand();
	
	void renderTile(TilePlayground::PlaceableSprite& tile);
	bool doesNeighborExistAtIndex(int8_t index, TilePlayground::PlaceableSprite& tileToCheck);

	SDL_Texture* _uiTexture;
	Button _addSpriteButton;
	SpriteButton _undoButton;
	SpriteButton _redoButton;
};

class LayerInspectorScreen
{
public:
	void start();
	void update();
	void render();
	void onUndoLastPlacedSprite();
};

class SettingsScreen
{
public:
	void start();
	void update();
	void render();
	void destroy();
	bool _isActive = false;
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
	bool _isActive = false;
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
	bool _isPanelActive = false;
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

	void start();
	void update();
	void render();
	void setMouseState(MouseSpriteState state);
private:
	MouseSpriteState _mouseSpriteState = MouseSpriteState::INVALID;
	Sprite _mouseSprite;
	IVec2 _lastMousePosition;
};

inline MainScreen& getMainScreen()
{
	static MainScreen mainScreen;
	return mainScreen;
}

inline LayerInspectorScreen& getLayerInspectorScreen()
{
	static LayerInspectorScreen layerInspectorScreen;
	return layerInspectorScreen;
}

inline SettingsScreen& getSettingsScreen()
{
	static SettingsScreen settingsScreen;
	return settingsScreen;
}

inline AddSpritesScreen& getAddSpritesScreen()
{
	static AddSpritesScreen addSpritesScreen;
	return addSpritesScreen;
}

inline PanelScreen& getPanelScreen()
{
	static PanelScreen panelScreen;
	return panelScreen;
}

inline MouseScreen& getMouseScreen()
{
	static MouseScreen mouseScreen;
	return mouseScreen;
}