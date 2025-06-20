#include "screens.h"
#include "core/spriteHandler.h"
#include <SDL.h>
#include "core/app.h"
#include "core/lib.h"
#include "core/input.h"
#include "core/log.h"
#include "core/ui.h"
#include <cmath>
#include "core/debugUtils.h"

void MainScreen::start()
{
	s_active = true;
	_uiTexture = createUITexture();
	_addSpriteButton = Button("+", BUTTON, Vec2(0, 5), 62);

	_undoButton = SpriteButton(textures::getSprite(UNDO_BUTTON), textures::getSprite(UNDO_BUTTON_HOVERED));
	_undoButton._sprite.position = {250 , 5};
	_redoButton = SpriteButton(textures::getSprite(UNDO_BUTTON), textures::getSprite(UNDO_BUTTON_HOVERED));
	_redoButton._sprite.position = {_undoButton._sprite.position.x + _undoButton._sprite.size.x + 5, 5};

	Button backgroundButton {"0", BUTTON, Vec2(0, _addSpriteButton._sprite.position.y + _addSpriteButton._sprite.size.y + 1.f)};
	Button middlegroundButton {"1", BUTTON, Vec2(0, backgroundButton._sprite.position.y + backgroundButton._sprite.size.y + 1.f)};
	Button foregroundButton {"2", BUTTON, Vec2(0, middlegroundButton._sprite.position.y + middlegroundButton._sprite.size.y + 1.f)};

	SpritePreviewer backgroundLayerSpritePreviewer {backgroundButton._sprite.position, BACKGROUND};
	SpritePreviewer middleLayerSpritePreviewer {middlegroundButton._sprite.position, MIDDLEGROUND};
	SpritePreviewer foregroundLayerSpritePreviewer {foregroundButton._sprite.position, FOREGROUND};

	s_spritePreviewerButtons = {
		std::pair{std::move(backgroundLayerSpritePreviewer), std::move(backgroundButton)},
		std::pair{std::move(middleLayerSpritePreviewer), std::move(middlegroundButton)},
		std::pair{std::move(foregroundLayerSpritePreviewer), std::move(foregroundButton)}
	};

}

void closeOpenScreenAndEnableMainScreen()
{
	SettingsScreen::s_active = false;
	AddSpritesScreen::s_active = false;
	PanelScreen::s_isPanelActive = false;
	MainScreen::s_active = true;
}

void toggleSettingsScreen()
{
	SettingsScreen::s_active = !SettingsScreen::s_active;
	MainScreen::s_active = !SettingsScreen::s_active;
	PanelScreen::s_isPanelActive = SettingsScreen::s_active;
}

void toggleAddSpritesScreen()
{
	AddSpritesScreen::s_active = !AddSpritesScreen::s_active;
	MainScreen::s_active = !AddSpritesScreen::s_active;
	PanelScreen::s_isPanelActive = AddSpritesScreen::s_active;
}

void MainScreen::handleAddSpritesToLayersDebug()
{
#if DEBUG_ENABLED == 1
	static std::array<Sprite, 6> debugSprites = {
		Sprite({10,16}, {328, 9}), // sign
		Sprite({8,5}, {347, 0}), // rock
		Sprite({24,13}, {338, 9}), // bush
		Sprite({35,48}, {328, 25}), // tree
		Sprite({7,3}, {355, 0}), // mini rock
		Sprite({16, 16}, {372, 0}, true) // tile
	};

	if (wasKeyPressedThisFrame(SDL_SCANCODE_1))
	{
		int randomSpriteIndex = rand() % debugSprites.size();
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		s_spritePreviewerButtons[BACKGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_2))
	{
		int randomSpriteIndex = rand() % debugSprites.size();
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		s_spritePreviewerButtons[MIDDLEGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_3))
	{
		int randomSpriteIndex = rand() % debugSprites.size();
		Sprite spriteToAdd = debugSprites[randomSpriteIndex];
		s_spritePreviewerButtons[FOREGROUND].first._spritesToPreview.emplace_back(spriteToAdd);
	}
#endif
}

void MainScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE))
	{
		closeOpenScreenAndEnableMainScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_TAB) && !AddSpritesScreen::s_active)
	{
		toggleSettingsScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_P) && !SettingsScreen::s_active)
	{
		toggleAddSpritesScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_Q))
	{
		s_debugCollidersEnabled = !s_debugCollidersEnabled;
	}

	if (!s_active)
	{
		return;
	}

	handleAddSpritesToLayersDebug();

	for (SpritePreviewerButtonPair& pair : s_spritePreviewerButtons)
	{
		// Only allow hovering text if the spritePreviewer isn't expanded/visible
		// When exapended, it's always hovered
		if (!pair.first._isVisible)
		{
			pair.second._text.tryHover();
		}

		if (pair.second.tryPress())
		{
			toggleSpritePreviewerAndDisableOthers(pair.first);
			pair.second._text.onHovered(true);
		}
	}

	_addSpriteButton._text.tryHover();
	if (_addSpriteButton.tryPress())
	{
		toggleAddSpritesScreen();
	}

	handleSpriteInHand();
	handleUndoAndRedoPlacedSprites();
}

void MainScreen::handleUndoAndRedoPlacedSprites()
{
	if (isKeyDown(SDL_SCANCODE_LCTRL))
	{
		if (wasKeyPressedThisFrame(k_undoKey))
		{
			s_tilePlayground.undoLastPlacedSprite();
		}
		else if (wasKeyReleasedThisFrame(k_redoKey))
		{
			s_tilePlayground.redoLastRemovedSprite();
		}
	}

	_undoButton.update();
	if (_undoButton.tryPress())
	{
		s_tilePlayground.undoLastPlacedSprite();
	}

	_redoButton.update();
	if (_redoButton.tryPress())
	{
		s_tilePlayground.redoLastRemovedSprite();
	}
}

void MainScreen::handleSpriteInHand()
{
	if (!s_tilePlayground.hasSpriteInHand())
	{
		MouseScreen::instance().setMouseState(MouseScreen::MouseSpriteState::NORMAL);
		return;
	}

	MouseScreen::instance().setMouseState(MouseScreen::MouseSpriteState::DRAGGING);
	s_tilePlayground.getSpriteInHand().position = s_mousePositionThisFrame;

	// TODO: Improve
	if (wasMouseButtonPressedThisFrame(SDL_BUTTON_RIGHT))
	{
		s_tilePlayground.clearSpriteInHand();
		s_tilePlayground.undoLastPlacedSprite();
		return;
	}

	if(!wasMouseButtonPressedThisFrame(SDL_BUTTON_LEFT))
	{
		return;
	}

	std::optional<TilePlayground::PlaceableSprite> spriteToReplace = shouldReplaceSpriteInHand();
	if (spriteToReplace.has_value())
	{
		s_tilePlayground.replaceSpriteInHand(spriteToReplace.value());
	}
	else if (shouldReleaseSpriteInHand())
	{
		if (s_tilePlayground.getSpriteInHand().isTile)
		{
			// To paint tiles, as soon as we place one sprite, we create another
			TilePlayground::PlaceableSprite& placeableSpriteInHand = s_tilePlayground.getPlaceableSpriteInHand();
			s_tilePlayground.clearSpriteInHand();
			s_tilePlayground.addSpriteToRenderAndPutSpriteInHand(placeableSpriteInHand);
		}
		else 
		{
			MouseScreen::instance().setMouseState(MouseScreen::MouseSpriteState::NORMAL);
			s_tilePlayground.clearSpriteInHand();
		}
	}
}

std::optional<TilePlayground::PlaceableSprite> MainScreen::shouldReplaceSpriteInHand()
{
	for (const SpritePreviewerButtonPair& pair : s_spritePreviewerButtons)
	{
		if (!pair.first._isVisible)
		{
			continue;
		}

		for (const SpriteWithExternalRect& sprite : pair.first._spritesToPreview)
		{
			if (pointInRect(s_mousePositionThisFrame, sprite.rect))
			{
				return TilePlayground::PlaceableSprite {std::move(sprite.sprite), pair.first._layer};
			}
		}
	}

	return std::nullopt;
}

bool MainScreen::shouldReleaseSpriteInHand()
{
	// Prevent missplacing a sprite if we're hovering a sprite previewer.
	for (const SpritePreviewerButtonPair& pair : s_spritePreviewerButtons)
	{
		if (pointInRect(s_mousePositionThisFrame, pair.second._text._mainCollider))
		{
			return false;
		}
	}

	return true;
}

void MainScreen::toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewer)
{
	for (uint8_t i = 0; i < s_spritePreviewerButtons.size(); ++i)
	{
		if (s_spritePreviewerButtons[i].first == spritePreviewer)
		{
			s_spritePreviewerButtons[i].first._isVisible = !s_spritePreviewerButtons[i].first._isVisible;
		}
		else
		{
			s_spritePreviewerButtons[i].first._isVisible = false;
		}
	}
}

void MainScreen::render()
{
	// TODO remove later
	static Sprite mockup = textures::getSprite(MOCKUP);
	mockup.position = { 0,0 };
	renderSprite(mockup);

	_addSpriteButton.render(_uiTexture);

	if (s_tilePlayground.hasSpriteInHand())
	{
		renderSprite(s_tilePlayground.getSpriteInHand());
	}

	for (SpritePreviewerButtonPair& pair : s_spritePreviewerButtons)
	{
		pair.first.render();
		pair.second.render(_uiTexture);
	}

	for (TilePlayground::PlaceableSprite& placeableSprite : s_tilePlayground._placedSprites)
	{
		renderSprite(placeableSprite.sprite);
	}

	_undoButton.render();
	_redoButton.render(true);
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}
