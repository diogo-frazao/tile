#include "spritePreviewer.h"
#include "core/log.h"
#include "core/debugUtils.h"
#include "core/input.h"
#include "screens.h"

SpritePreviewer::SpritePreviewer(const Vec2& startLocation, LayerType layer)
{
	_locationToStartGrid = startLocation;
	_spritesToPreview.reserve(10);
	_backgroundSprite = textures::getSprite(PREVIEWER_BG);
	_layer = layer;
}

bool didPressSpriteButton(const RectCollider& rect)
{
	if (wasMouseButtonPressedThisFrame(k_createSpriteOnSpritePreviewerClickMouseButton))
	{
		return pointInRect(s_mousePositionThisFrame, rect);
	}

	return false;
}

bool didPressDeleteSpriteButton(const RectCollider& rect)
{
	if (wasMouseButtonReleasedThisFrame(k_deleteSpriteOnSpritePreviewerClickMouseButton))
	{
		return pointInRect(s_mousePositionThisFrame, rect);
	}

	return false;
}

void centerSpriteInsideBackground(Sprite& sprite, const Vec2& targetRect) 
{
	sprite.position.x += (targetRect.x - sprite.size.x) / 2;
	sprite.position.y += (targetRect.y - sprite.size.y) / 2;
}

void SpritePreviewer::render()
{
	if (!_isVisible)
	{
		return;
	}

	static const uint8_t offsetBetweenSprites = _backgroundSprite.size.x;
	static Vec2 targetRect = { _backgroundSprite.size.x - 1.f, _backgroundSprite.size.y - 1.f };

	constexpr uint8_t k_maxSpritesPerRow = 4;
	uint8_t spritesThisRow = 0;
	uint8_t numColumns = 0;

	for (int16_t i = 0; i < _spritesToPreview.size(); ++i)
	{
		Sprite& sprite = _spritesToPreview[i].sprite;

		if (spritesThisRow < k_maxSpritesPerRow)
		{
			spritesThisRow++;
		}
		else
		{
			spritesThisRow = 1;
			numColumns++;
		}

		// Position sprite at top left of background sprite
		sprite.position = _locationToStartGrid;
		sprite.position.x += spritesThisRow * offsetBetweenSprites;
		sprite.position.y += numColumns * offsetBetweenSprites;

		_backgroundSprite.position = sprite.position;
		RectCollider backgroundSpriteRect{_backgroundSprite.position, _backgroundSprite.size};
		_spritesToPreview[i].rect = backgroundSpriteRect;

		if (didPressSpriteButton(backgroundSpriteRect))
		{
			// Since the sprite will be follwing the mouse, make it match already to avoid flickering
			sprite.position = s_mousePositionThisFrame;
			
			MainScreen::s_tilePlayground.addSpriteToRender({sprite, _layer});
		}
		else if (didPressDeleteSpriteButton(backgroundSpriteRect))
		{
			_spritesToPreview.erase(_spritesToPreview.begin() + i);
			return;
		}

		renderSprite(_backgroundSprite);
		debugDrawRect(backgroundSpriteRect);

		bool isSpriteBiggerThanBackground = ((float)sprite.size.x > targetRect.x) || ((float)sprite.size.y > targetRect.y);
		if (!isSpriteBiggerThanBackground)
		{
			centerSpriteInsideBackground(sprite, targetRect);
		}

		renderSpriteInsideRect(sprite, targetRect);
	}
}