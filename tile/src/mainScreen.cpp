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
#include <array>

//								  	  		       TOP    LEFT   RIGHT  BOTTOM  TOPLEFT	  TOPRIGHT  BOTTOMLEFT  BOTTOM RIGHT
static std::array<int8_t, 16> neighborOffsets = { 0,-1,  -1,0,  1,0,   0,1,    -1, -1,     1, -1,    -1, 1,      1, 1};

void MainScreen::start()
{
	_isActive = true;
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

	_spritePreviewerButtons = {
		std::pair{std::move(backgroundLayerSpritePreviewer), std::move(backgroundButton)},
		std::pair{std::move(middleLayerSpritePreviewer), std::move(middlegroundButton)},
		std::pair{std::move(foregroundLayerSpritePreviewer), std::move(foregroundButton)}
	};

}

void closeOpenScreenAndEnableMainScreen()
{
	getSettingsScreen()._isActive = false;
	getAddSpritesScreen()._isActive = false;
	getPanelScreen()._isPanelActive = false;
	getMainScreen()._isActive = true;
}

void toggleSettingsScreen()
{
	getSettingsScreen()._isActive = !getSettingsScreen()._isActive;
	getMainScreen()._isActive = !getSettingsScreen()._isActive;
	getPanelScreen()._isPanelActive = getSettingsScreen()._isActive;
}

void toggleAddSpritesScreen()
{
	getAddSpritesScreen()._isActive = !getAddSpritesScreen()._isActive;
	getMainScreen()._isActive = !getAddSpritesScreen()._isActive;
	getPanelScreen()._isPanelActive = getAddSpritesScreen()._isActive;
}

void MainScreen::handleAddSpritesToLayersDebug()
{
#ifndef RELEASE_BUILD
	static std::array<Sprite, 14> debugSprites = {
		Sprite({256,128}, {16, 0}), // bg
		Sprite({80,15}, {0, 128}), // water
		Sprite({49,12}, {0, 144}), // clouds
		Sprite({256,83}, {0, 160}), // trees
		Sprite({34,43}, {272, 0}), // tree 
		Sprite({16,12}, {272, 48}), // frog
		Sprite({18,16}, {288, 48}), // fish
		Sprite({16,16}, {272, 64}), // sign
		Sprite({9,6}, {288, 64}), // leaf
		Sprite({6,4}, {288, 70}), // rock
		Sprite({16,16}, {304, 64}), // random1
		Sprite({16,16}, {320, 64}), // random2
		Sprite({16,16}, {336, 64}), // random3  
		Sprite({16, 16}, {384, 0}, true, {384, 0}) // tile
	};

	if (wasKeyPressedThisFrame(SDL_SCANCODE_1))
	{
		for (Sprite& sprite : debugSprites)
		{
			_spritePreviewerButtons[BACKGROUND].first._spritesToPreview.emplace_back(sprite);
		}
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_2))
	{
		for (Sprite& sprite : debugSprites)
		{
			_spritePreviewerButtons[MIDDLEGROUND].first._spritesToPreview.emplace_back(sprite);
		}
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_3))
	{
		for (Sprite& sprite : debugSprites)
		{
			_spritePreviewerButtons[FOREGROUND].first._spritesToPreview.emplace_back(sprite);
		}
	}
#endif
}

void MainScreen::update()
{
	if (wasKeyPressedThisFrame(SDL_SCANCODE_ESCAPE))
	{
		closeOpenScreenAndEnableMainScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_TAB) && !getAddSpritesScreen()._isActive)
	{
		toggleSettingsScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_P) && !getSettingsScreen()._isActive)
	{
		toggleAddSpritesScreen();
	}

	if (wasKeyPressedThisFrame(SDL_SCANCODE_Q))
	{
		s_debugCollidersEnabled = !s_debugCollidersEnabled;
	}

	if (!_isActive)
	{
		return;
	}

	handleAddSpritesToLayersDebug();

	for (SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
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
			_tilePlayground.undoLastPlacedSprite();
		}
		else if (wasKeyReleasedThisFrame(k_redoKey))
		{
			_tilePlayground.redoLastRemovedSprite();
		}
	}

	_undoButton.update();
	if (_undoButton.tryPress())
	{
		_tilePlayground.undoLastPlacedSprite();
	}

	_redoButton.update();
	if (_redoButton.tryPress())
	{
		_tilePlayground.redoLastRemovedSprite();
	}
}

void updateSpriteInHandPositionToFollowGrid(Sprite& spriteInHand)
{
	spriteInHand.position.x = (s_mousePositionThisFrame.x / spriteInHand.size.x) * spriteInHand.size.x;
	spriteInHand.position.y = (s_mousePositionThisFrame.y / spriteInHand.size.y) * spriteInHand.size.y;
}

void MainScreen::handleSpriteInHand()
{
	if (!_tilePlayground.hasSpriteInHand())
	{
		getMouseScreen().setMouseState(MouseScreen::MouseSpriteState::NORMAL);
		return;
	}

	getMouseScreen().setMouseState(MouseScreen::MouseSpriteState::DRAGGING);

	Sprite& spriteInHand = _tilePlayground.getSpriteInHand();
	if (isKeyDown(k_snapKey) || spriteInHand.isTile)
	{
		updateSpriteInHandPositionToFollowGrid(spriteInHand);
	}
	else
	{
		spriteInHand.position = s_mousePositionThisFrame;
	}

	if (wasMouseButtonPressedThisFrame(SDL_BUTTON_RIGHT) || wasKeyPressedThisFrame(k_returnKey))
	{
		_tilePlayground.deleteSpriteInHand();
		return;
	}

	if(!wasMouseButtonPressedThisFrame(SDL_BUTTON_LEFT))
	{
		return;
	}

	std::optional<TilePlayground::PlaceableSprite> spriteToReplace = shouldReplaceSpriteInHand();
	if (spriteToReplace.has_value())
	{
		_tilePlayground.replaceSpriteInHand(spriteToReplace.value());
	}
	else if (shouldReleaseSpriteInHand())
	{
		if (_tilePlayground.getSpriteInHand().isTile)
		{
			// To paint tiles, as soon as we place one sprite, we create another.
			TilePlayground::PlaceableSprite& placeableSpriteInHand = _tilePlayground.getPlaceableSpriteInHand();
			_tilePlayground.releaseSpriteInHand();
			_tilePlayground.addSpriteToRenderAndPutSpriteInHand(placeableSpriteInHand);
		}
		else 
		{
			_tilePlayground.releaseSpriteInHand();
		}

		// Even for tiles change the mouse sprite to normal to give a small feedback that the tile was placed.
		// On the next frame it will change automatically to grabbing since we have a sprite in hand.
		getMouseScreen().setMouseState(MouseScreen::MouseSpriteState::NORMAL);
	}
}

std::optional<TilePlayground::PlaceableSprite> MainScreen::shouldReplaceSpriteInHand()
{
	for (const SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
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
	for (const SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
	{
		if (pointInRect(s_mousePositionThisFrame, pair.second._text._mainCollider))
		{
			return false;
		}
	}

	// Only allow releasing a tile if a tile doesn't exist on the same layer at the same position
	TilePlayground::PlaceableSprite& placeableSriteInHand = _tilePlayground.getPlaceableSpriteInHand();
	if (_tilePlayground.getSpriteInHand().isTile)
	{
		int32_t placedSpritesSize = static_cast<int32_t>(_tilePlayground._placedSprites.size());
		for (int32_t i = 0; i < placedSpritesSize; ++i)
		{
			TilePlayground::PlaceableSprite& placeableSprite = _tilePlayground._placedSprites[i];

			bool isFromDifferentLayer = placeableSprite.layer != placeableSriteInHand.layer;
			bool isSpriteInHand = (i == _tilePlayground._spriteInHandIndex);

			if (!placeableSprite.sprite.isTile || isFromDifferentLayer || isSpriteInHand)
			{
				continue;
			}

			if (placeableSprite.sprite.position == placeableSriteInHand.sprite.position)
			{
				return false;
			}
		}
	}

	return true;
}

void MainScreen::toggleSpritePreviewerAndDisableOthers(SpritePreviewer& spritePreviewerToToggle)
{
	for (size_t i = 0; i < _spritePreviewerButtons.size(); ++i)
	{
		SpritePreviewer& spritePreviewer = _spritePreviewerButtons[i].first;
		if (spritePreviewer == spritePreviewerToToggle)
		{
			spritePreviewer._isVisible = !spritePreviewer._isVisible;
		}
		else
		{
			spritePreviewer._isVisible = false;
		}
	}
}

void MainScreen::render()
{
	for (TilePlayground::PlaceableSprite& placeableSprite : _tilePlayground._placedSprites)
	{
		Sprite& sprite = placeableSprite.sprite;
		if(!sprite.isVisible)
		{
			continue;
		}
		
		if (!sprite.isTile)
		{
			renderSprite(sprite);
			continue;
		}

		renderTile(placeableSprite);
	}

	debugDrawGrid(16);

	_addSpriteButton.render(_uiTexture);

	for (SpritePreviewerButtonPair& pair : _spritePreviewerButtons)
	{
		pair.first.render();
		pair.second.render(_uiTexture);
	}

	_undoButton.render();
	_redoButton.render(true);
}

void MainScreen::renderTile(TilePlayground::PlaceableSprite& tile)
{
	int8_t neighborMask = 0;
	int8_t neighborsCount = 0;
	int8_t emptyNeighborIndex = -1;

	// For each neighbor
	for (uint8_t j = 0; j < 8; ++j)
	{
		bool foundNeighbor = doesNeighborExistAtIndex(j, tile);

		if (foundNeighbor)
		{
			bool isTopDownLeftRightIndex = j < 4;
			if (isTopDownLeftRightIndex)
			{
				neighborMask |= 1 << j;
			}

			neighborsCount++;
			foundNeighbor = true;
		}
		else
		{
			emptyNeighborIndex = j;
		}
	}

	bool isDiagonalNeighborEmpty = emptyNeighborIndex >= 4;
	bool isCornerTilePiece = (neighborsCount == 7) && isDiagonalNeighborEmpty;
	if (isCornerTilePiece)
	{
		tile.sprite.handleTileCornerFromEmptyNeighborIndex(emptyNeighborIndex);
	}
	else
	{
		tile.sprite.setTileOffsetFromMask(neighborMask);
	}
	
	renderSprite(tile.sprite);
}

bool MainScreen::doesNeighborExistAtIndex(int8_t index, TilePlayground::PlaceableSprite& placeableSprite)
{
	Sprite& sprite = placeableSprite.sprite;

	for (TilePlayground::PlaceableSprite& potentialTile : _tilePlayground._placedSprites)
	{
		bool isTileFromSameLayer = potentialTile.sprite.isTile && potentialTile.layer == placeableSprite.layer;
		if (!isTileFromSameLayer || !potentialTile.sprite.isVisible)
		{
			continue;
		}

		float neighborX = sprite.position.x + neighborOffsets[index * 2] * sprite.size.x;
		float neighborY = sprite.position.y + neighborOffsets[index * 2 + 1] * sprite.size.y;

		if (potentialTile.sprite.position == Vec2(neighborX, neighborY))
		{
			return true;
		}
	}

	return false;
}

void MainScreen::destroy()
{
	SDL_DestroyTexture(_uiTexture);
}
